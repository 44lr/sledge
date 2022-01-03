﻿using System.Runtime.InteropServices;
using System.Numerics;

namespace SledgeLib
{
    public class Body
    {
        [DllImport("sledge.dll")] private static extern uint CreateBody();
        public static dGetUInt Create = CreateBody;

        [DllImport("sledge.dll")] private static extern void DestroyBody(uint iBodyHandle);
        public delegate void dDestroyBody(uint iBodyHandle);
        public static dDestroyBody Destroy = DestroyBody;

        [DllImport("sledge.dll")] private static extern Transform GetBodyTransform(uint iHandle);
        public static dGetTransformEntity GetTransform = GetBodyTransform;
        [DllImport("sledge.dll")] private static extern void SetBodyTransform(uint iHandle, Transform tTransform);
        public static dSetTransformEntity SetTransform = SetBodyTransform;

        [DllImport("sledge.dll")] private static extern bool GetBodyDynamic(uint iHandle);
        public static dGetBoolEntity IsDynamic = GetBodyDynamic;
        [DllImport("sledge.dll")] private static extern void SetBodyDynamic(uint iHandle, bool bDynamic);
        public static dSetBoolEntity SetDynamic = SetBodyDynamic;

        [DllImport("sledge.dll")] private static extern void SetBodyVelocity(uint iHandle, Vector3 vVelocity);
        public static dSetVector3Entity SetVelocity = SetBodyVelocity;
        [DllImport("sledge.dll")] private static extern Vector3 GetBodyVelocity(uint iHandle);
        public static dGetVector3Entity GetVelocity = GetBodyVelocity;

        [DllImport("sledge.dll")] private static extern Vector3 GetBodyAngularVelocity(uint iHandle);
        public static dGetVector3Entity GetAngularVelocity = GetBodyAngularVelocity;
        [DllImport("sledge.dll")] private static extern void SetBodyAngularVelocity(uint iHandle, Vector3 vVelocity);
        public static dSetVector3Entity SetAngularVelocity = SetBodyAngularVelocity;

        [DllImport("sledge.dll")] private static extern Vector3 GetBodyPosition(uint iHandle);
        public static dGetVector3Entity GetPosition = GetBodyPosition;
        [DllImport("sledge.dll")] private static extern void SetBodyPosition(uint iHandle, Vector3 vVelocity);
        public static dSetVector3Entity SetPosition = SetBodyPosition;

        [DllImport("sledge.dll")] private static extern Quaternion GetBodyRotation(uint iHandle);
        public static dGetQuaternionEntity GetRotation = GetBodyRotation;
        [DllImport("sledge.dll")] private static extern void SetBodyRotation(uint iHandle, Quaternion vRotation);
        public static dSetQuaternionEntity SetRotation = SetBodyRotation;

        [DllImport("sledge.dll")] private static extern float GetBodyMass(uint iHandle);
        public static dGetFloatEntity GetMass = GetBodyMass;

        [DllImport("sledge.dll")] private static extern uint GetBodySibling(uint iHandle);
        public static dGetUIntEntity GetSibling = GetBodySibling;
        [DllImport("sledge.dll")] private static extern uint GetBodyChild(uint iHandle);
        public static dGetUIntEntity GetChild = GetBodyChild;

        public static List<uint> GetChildren(uint iHandle)
        {
            uint iLastHandle = Body.GetChild(iHandle);

            List<uint> ChildrenList = new List<uint>();
            while (iLastHandle != 0)
            {
                ChildrenList.Add(iLastHandle);
                iLastHandle = Shape.GetSibling(iLastHandle);
            }

            return ChildrenList;
        }
    }
}