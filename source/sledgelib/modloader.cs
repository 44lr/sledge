using System.Reflection;
using System.Runtime.Loader;
using System.Text.Json;

using SledgeLib;

internal class CModLoader
{
    internal struct SModInfo
    {
        public string sModName { get; set; }
        public string sModAuthor { get; set; }
        public string sTypeName { get; set; }
        public string sLoadMethodName { get; set; }
        public string sUnloadMethodName { get; set; }
        public string sReloadMethodName { get; set; }
    }

    internal struct SRegisteredModInfo
    {
        public Assembly m_ModAsembly;
        public DateTime m_AssemblyLastWrite;
        public string m_Path;
        public string m_Name;
        public object m_Instance;
        public MethodInfo m_Load;
        public MethodInfo m_Unload;
        public MethodInfo? m_Reload;
        public AssemblyLoadContext m_LoadContext;
    }

    internal static List<SRegisteredModInfo> RegisteredMods = new List<SRegisteredModInfo>();
    internal static string? ModsPath = null;
    internal static FileSystemWatcher? Watcher = null;
    internal static Assembly ThisAssembly = typeof(CModLoader).Assembly;

    internal static void RegisterLoadedMod(SRegisteredModInfo Mod) 
    {
        lock (RegisteredMods) { RegisteredMods.Add(Mod); }
        Log.General("Registered mod: {0}", Mod.m_Name);
    }

    internal static void UnregisterLoadedMod(SRegisteredModInfo Mod)
    {
        lock (RegisteredMods) { RegisteredMods.Remove(Mod); }
        
        try
        {
            Mod.m_Unload.Invoke(Mod.m_Instance, null);
        } catch (Exception ex)
        {
            Log.Error("Error ocurred while invoking unload method for mod {0}: {1}", Mod.m_Name, ex.Message);
        }

        try
        {
            Mod.m_LoadContext.Unload();
        } catch (Exception ex)
        {
            Log.Error("Error ocurred while unloading AssemblyLoadContext for mod {0}: {1}", Mod.m_Name, ex.Message);
        }

        System.GC.Collect();
        System.GC.WaitForPendingFinalizers();
        System.GC.Collect();

        Log.General("Unregistered and unloaded mod: {0}", Mod.m_Name);
    }

    private static Assembly? _ModDependencyResolver(AssemblyLoadContext LoadContext, AssemblyName Name)
    {
        if (Name.ToString() == ThisAssembly.GetName().ToString())
            return ThisAssembly;

        foreach(SRegisteredModInfo ModInfo in RegisteredMods)
        {
            if (LoadContext.Name != ModInfo.m_LoadContext.Name)
                continue;

            Log.General("Mod {0} loaded an external dependency: {1}", ModInfo.m_Name, Name);
            break;
        }

        Log.Error("Unable to load assembly dependency: {0}", Name.ToString());
        return null;
    }

    internal static bool Init()
    {
        ModsPath = Path.GetDirectoryName(ThisAssembly.Location);
        if (ModsPath == null)
            return false;

        return true;
    }

    internal static bool RegisterMod(string sModPath, string sModName)
    {
        foreach (SRegisteredModInfo Mod in RegisteredMods)
        {
            if (Mod.m_Name == sModName)
            {
                Log.Error("Attempted to load mod twice. ({0})", sModName);
                return false;
            }
        }

        SRegisteredModInfo RegModInfo = new SRegisteredModInfo();
        RegModInfo.m_Name = sModName;
        RegModInfo.m_Path = sModPath;
        RegModInfo.m_LoadContext = new AssemblyLoadContext(sModName, true);
        RegModInfo.m_LoadContext.Resolving += _ModDependencyResolver;

        string sAssemblyPath = sModPath + "\\" + sModName + ".dll";

        FileStream AssemblyStream;
        try
        {
            AssemblyStream = File.OpenRead(sAssemblyPath);
        } catch (Exception ex)
        {
            Log.Error("Error ocurred while reading assembly for mod {0}: {1}", sModName, ex.Message);
            return false;
        }

        RegModInfo.m_AssemblyLastWrite = File.GetLastWriteTime(sAssemblyPath);

        try
        {
            RegModInfo.m_ModAsembly = RegModInfo.m_LoadContext.LoadFromStream(AssemblyStream);
        }
        catch (Exception ex)
        {
            Log.Error("Error ocurred while loading assembly for mod {0}: {1}", sModName, ex.Message);
            return false;
        }

        AssemblyStream.Close();

        string sModInfoPath = sModPath + "\\" + sModName + ".info.json";
        if (!File.Exists(sModInfoPath))
        {
            Log.Warning("Mod {0} has no info, attempting to generate one", sModPath);
            if (!CModInfoGenerator.GenerateModInfo(RegModInfo.m_ModAsembly, sModInfoPath, sModName))
                return false;
        }

        string sConfig;
        try
        {
            sConfig = File.ReadAllText(sModInfoPath);
        } catch (Exception ex)
        {
            Log.Error("Error ocurred while reading config for mod {0}: {1}", sModName, ex.Message);
            return false;
        }

        SModInfo ModInfo;
        try
        {
            ModInfo = JsonSerializer.Deserialize<SModInfo>(sConfig);
            if (ModInfo.sTypeName == null || ModInfo.sLoadMethodName == null || ModInfo.sUnloadMethodName == null)
            {
                Log.Error("Error ocurred while parsing config for mod {0}: {1}", sModName, "sTypeName, sLoadMethodName and/or sUnloadMethodName were null");
                return false;
            }
        } catch (Exception ex)
        {
            Log.Error("Error ocurred while parsing config for mod {0}: {1}", sModName, ex);
            return false;
        }


        Type? ModType;
        try
        {
            ModType = RegModInfo.m_ModAsembly.GetType(ModInfo.sTypeName, true);
            if (ModType == null)
            {
                Log.Error("Error ocurred while getting loader type for mod {0}: {1}", sModName, "GetType returned null");
                return false;
            }
        } catch (Exception ex)
        {
            Log.Error("Error ocurred while getting loader type for mod {0}: {1}", sModName, ex);
            return false;
        }


        MethodInfo? ModLoadMethod = ModType.GetMethod(ModInfo.sLoadMethodName);
        if (ModLoadMethod == null)
        {
            Log.Error("Error ocurred while getting load method for mod {0}: {1}", sModName, "GetMethod returned null");
            return false;
        }

        MethodInfo? ModUnloadMethod = ModType.GetMethod(ModInfo.sUnloadMethodName);
        if (ModUnloadMethod == null)
        {
            Log.Error("Error ocurred while getting unload method for mod {0}: {1}", sModName, "GetMethod returned null");
            return false;
        }

        MethodInfo? ModReloadMethod = null;
        if (!string.IsNullOrEmpty(ModInfo.sReloadMethodName))
            ModReloadMethod = ModType.GetMethod(ModInfo.sReloadMethodName);

        try
        {
            object? ModInstance = Activator.CreateInstance(ModType);
            if (ModInstance != null)
                RegModInfo.m_Instance = ModInstance;
            else
            {
                Log.Error("Error ocurred while getting type instance for mod {0}: {1}", sModName, "CreateInstance returned null");
            }
        } catch (Exception ex)
        {
            Log.Error("Error ocurred while creating type instance for mod {0}: {1}", sModName, ex.Message);
            return false;
        }


        RegModInfo.m_Load = ModLoadMethod;
        RegModInfo.m_Unload = ModUnloadMethod;
        RegModInfo.m_Reload = ModReloadMethod;

        RegisterLoadedMod(RegModInfo);
        return true;
    }

    private static void InitMods()
    {
        foreach (SRegisteredModInfo RegisteredMod in RegisteredMods)
        {
            try
            {
                RegisteredMod.m_Load.Invoke(RegisteredMod.m_Instance, null);
            } catch(Exception ex)
            {
                Log.Error("Error ocurred while invoking loader function for mod {0}: {1}", RegisteredMod.m_Name, ex.Message);
                UnregisterLoadedMod(RegisteredMod);
            }

            Log.General("Successfully started mod: {0}", RegisteredMod.m_Name);
        }
    }

    internal static void ReloadMod(SRegisteredModInfo ModInfo)
    {
        UnregisterLoadedMod(ModInfo);

        RegisterMod(ModInfo.m_Path, ModInfo.m_Name);

        foreach (SRegisteredModInfo RegisteredMod in RegisteredMods)
        {
            if (RegisteredMod.m_Path != ModInfo.m_Path || RegisteredMod.m_Name != ModInfo.m_Name)
                continue;

            MethodInfo MethodToInvoke;
            if (RegisteredMod.m_Reload != null)
                MethodToInvoke = RegisteredMod.m_Reload;
            else
                MethodToInvoke = RegisteredMod.m_Load;

            try
            {
                MethodToInvoke.Invoke(RegisteredMod.m_Instance, null);
            }
            catch (Exception ex)
            {
                Log.Error("Error ocurred while invoking loader function for mod {0}: {1}", RegisteredMod.m_Name, ex.Message);
                UnregisterLoadedMod(RegisteredMod);
            }

            break;
        }
    }

    internal static void OnModChanged(object Sender, FileSystemEventArgs Args)
    {
        foreach (SRegisteredModInfo RegisteredMod in RegisteredMods)
        {
            string sAssemblyPath = RegisteredMod.m_Path + "\\" + RegisteredMod.m_Name + ".dll";

            if (Args.FullPath == sAssemblyPath)
            {
                if (File.GetLastWriteTime(sAssemblyPath) == RegisteredMod.m_AssemblyLastWrite)
                    return;

                ReloadMod(RegisteredMod);
                return;
            }
        }
    }

    internal static void OnModCreated(object Sender, FileSystemEventArgs Args)
    {
        string sModName = Path.GetFileNameWithoutExtension(Args.FullPath);

        string? sModPath = Path.GetDirectoryName(Args.FullPath);
        if (sModPath == null)
            return;

        Log.General("Detected new mod: {0}", sModName);
        bool bRegisteredMod = RegisterMod(sModPath, sModName);

        if (!bRegisteredMod)
            return;

        foreach (SRegisteredModInfo RegisteredMod in RegisteredMods)
        {
            if (RegisteredMod.m_Path != sModPath || RegisteredMod.m_Name != sModName)
                continue;

            try
            {
                RegisteredMod.m_Load.Invoke(RegisteredMod.m_Instance, null);
            }
            catch (Exception ex)
            {
                Log.Error("Error ocurred while invoking loader function for mod {0}: {1}", RegisteredMod.m_Name, ex.Message);
                UnregisterLoadedMod(RegisteredMod);
            }

            break;
        }
    }

    internal static void OnModDeleted(object Sender, FileSystemEventArgs Args)
    {
        foreach (SRegisteredModInfo RegisteredMod in RegisteredMods)
        {
            string sAssemblyPath = RegisteredMod.m_Path + "\\" + RegisteredMod.m_Name + ".dll";

            if (Args.FullPath.ToString() == sAssemblyPath)
            {
                UnregisterLoadedMod(RegisteredMod);
                break;
            }
        }
    }

    internal static void LoadMods()
    {
        // ModsPath can't be 1, because Init already checks for it
        #pragma warning disable CS8604
        string[] ModDirectories = Directory.GetDirectories(ModsPath, "sledge*", SearchOption.TopDirectoryOnly);
        #pragma warning restore CS8604

        foreach (string sModFileFolder in ModDirectories)
        {
            // This will get the name of the folder directory, the mod .dll must match the folder name (e.g sledge_MOD) 
            string sModName = Path.GetFileName(sModFileFolder); 
            string sModDepsPath = sModFileFolder + "\\dependencies";

            if (Directory.Exists(sModDepsPath))
            {
                Log.General("[{0}] - Dependencies folder found", sModName);
                string[] sModDependencies = Directory.GetFiles(sModDepsPath, "*.dll", SearchOption.TopDirectoryOnly);

                foreach(string sModDepPath in sModDependencies)
                {
                    try
                    {
                        Assembly.LoadFrom(sModDepPath);
                        Log.General("[{0}] - Dependency {1} loaded", sModName, Path.GetFileName(sModDepPath));
                    }
                    catch (Exception ex)
                    {
                        Log.Error("Error ocurred while trying to load dependency {0}: {1}", sModDepPath, ex.Message);
                    }
                }
            }

            RegisterMod(sModFileFolder, sModName);
        }

        InitMods();

        Watcher = new FileSystemWatcher(ModsPath, "*.dll");
        Watcher.IncludeSubdirectories = true;
        Watcher.EnableRaisingEvents = true;

        Watcher.NotifyFilter = NotifyFilters.Attributes 
                                | NotifyFilters.CreationTime
                                | NotifyFilters.DirectoryName
                                | NotifyFilters.FileName
                                | NotifyFilters.LastAccess
                                | NotifyFilters.LastWrite
                                | NotifyFilters.Security
                                | NotifyFilters.Size;

        Watcher.Deleted += OnModDeleted;
        Watcher.Changed += OnModChanged;
        Watcher.Created += OnModCreated;
    }
}