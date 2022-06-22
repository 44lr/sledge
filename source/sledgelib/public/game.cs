using System;
using System.Runtime.InteropServices;
using System.Text;

namespace SledgeLib
{
    public class Game
    {
        [DllImport("sledge_core.dll")] private static extern void _GetLevelPath(StringBuilder ReturnString, uint ReturnLength);
        [DllImport("sledge_core.dll")] private static extern bool _IsPlaying();
        [DllImport("sledge_core.dll")] private static extern void _GetLevelLayers(StringBuilder ReturnString, uint ReturnLength);
        [DllImport("sledge_core.dll")] private static extern void _GetLevelId(StringBuilder ReturnString, uint ReturnLength);
        [DllImport("sledge_core.dll")] private static extern void _LoadLevel(string LevelPath, string LevelLayers, string LevelId, string LevelModId);
        [DllImport("sledge_core.dll")] private static extern void _DebugPrint(string sMessage);
        [DllImport("sledge_core.dll")] private static extern void _Shutdown();
        [DllImport("sledge_core.dll")] private static extern EGameState _GetState();
        [DllImport("sledge_core.dll")] private static extern void _SetState(EGameState State);
        [DllImport("sledge_core.dll")] private static extern StringBuilder _GetCMDLine();

        public static string CommandLine
        {
            get { return _GetCMDLine().ToString(); }
        }

        public static EGameState State
        {
            get { return _GetState(); }
            set { _SetState(value); }
        }

        public static string LevelPath
        {
            get {
                StringBuilder ReturnString = new StringBuilder(255);
                _GetLevelPath(ReturnString, 255);
                return ReturnString.ToString();
            }
        }

        public static string LevelLayers
        {
            get {
                StringBuilder ReturnString = new StringBuilder(100);
                _GetLevelLayers(ReturnString, 100);
                return ReturnString.ToString();
            }
        }

        public static string LevelId
        {
            get {
                StringBuilder ReturnString = new StringBuilder(100);
                _GetLevelId(ReturnString, 100);
                return ReturnString.ToString();
            }
        }

        public static void LoadLevel(string? LevelPath, string? LevelLayers, string? LevelId, string? LevelModId)
        {
            if (LevelPath == null)
                LevelPath = "";

            if (LevelId == null)
                LevelId = "";

            if (LevelLayers == null)
                LevelLayers = "";

            if (LevelModId == null)
                LevelModId = "";

            _LoadLevel(LevelPath, LevelLayers, LevelId, LevelModId);
        }

        public static void DebugPrint(string sFormat, params object[] oArgs)
        {
            try { _DebugPrint(String.Format(sFormat, oArgs)); }
            catch (Exception ex) { Log.Error("Error ocurred while formatting string for DebugPrint: {0}", ex); }
        }

        public static void Shutdown() {
            _Shutdown();
        }

        public static bool Playing
        {
            get { return _IsPlaying(); }
        }
    }
}