﻿public class SledgeLib
{
    internal enum ELogType
    {
        General = 4,
        Error = 5
    }

    internal struct CSledgeInternal
    {
        internal delegate void LogDelegate(ELogType LogType, string sMsg);
        internal LogDelegate Log;

        internal delegate IntPtr CreateBindDelegate(EBindType eType, EKeyCode iKeyCode, BindCallback pCallback, bool bActive = true);
        internal CreateBindDelegate CreateBind;
    }

    internal static CSledgeInternal m_Internal;

    internal static bool _SetInternal(IntPtr pInternal)
    {
        if (pInternal == IntPtr.Zero)
            return false;

        object? InternalObj = System.Runtime.InteropServices.Marshal.PtrToStructure(pInternal, typeof(CSledgeInternal));
        if (InternalObj == null)
            return false;

        m_Internal = (CSledgeInternal)InternalObj;
        WriteLog("sledgelib internal api loaded");

        return true;
    }

    public static void WriteLog(string sLog)
    {
        m_Internal.Log(ELogType.General, sLog);
    }

    public static void RegisterBind()
    {

    }

    public static void WriteError(string sLog)
    {
        m_Internal.Log(ELogType.Error, sLog);
    }
}