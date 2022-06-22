#include "teardown/functions.h"
#include "teardown/offsets.h"

#include "teardown/functions/misc.h"
#include "teardown/functions/memory.h"
#include "teardown/functions/tags.h"
#include "teardown/functions/registry.h"
#include "teardown/functions/lua.h"
#include "teardown/functions/constructors.h"
#include "teardown/functions/scene.h"
#include "teardown/functions/entity.h"

#include "util/timer.h"

#include "globals.h"

struct SFunctionInfo {
	unsigned __int64 m_Offset;
	void* m_Function;
	const char* m_Name;
};

static const SFunctionInfo aFunctions[] = {
	/*
		memory functions
	*/
	{ g_Offsets["free"], &Teardown::free, "free" },
	{ g_Offsets["malloc"], &Teardown::malloc, "malloc" },
	{ g_Offsets["calloc"], &Teardown::calloc, "calloc" },
	{ g_Offsets["realloc"], &Teardown::realloc, "realloc" },
	{ g_Offsets["memmove"], &Teardown::memmove, "memmove" },

	/*
		tag functions
	*/
	{ g_Offsets["SetTag"], &Teardown::SetTag, "SetTag" },
	{ g_Offsets["HasTag"], &Teardown::HasTag, "HasTag" },
	{ g_Offsets["RemoveTag"], &Teardown::RemoveTag, "RemoveTag" },
	{ g_Offsets["GetTag"], &Teardown::GetTag, "GetTag" },

	/*
		registry functions
	*/
	{ g_Offsets["SetBool"], &Teardown::SetBool, "SetBool" },
	{ g_Offsets["SetInt"], &Teardown::SetInt, "SetInt" },
	{ g_Offsets["SetFloat"], &Teardown::SetFloat, "SetFloat" },
	{ g_Offsets["SetString"], &Teardown::SetString, "SetString" },

	{ g_Offsets["GetBool"], &Teardown::GetBool, "GetBool" },
	{ g_Offsets["GetInt"], &Teardown::GetInt, "GetInt" },
	{ g_Offsets["GetFloat"], &Teardown::GetFloat, "GetFloat" },
	{ g_Offsets["GetString"], &Teardown::GetString, "GetString" },

	/*
		lua functions
	*/
	{ g_Offsets["lua_gettop"], &Teardown::lua_gettop, "lua_gettop" },
	{ g_Offsets["lua_getfield"], &Teardown::lua_getfield, "lua_getfield" },

	{ g_Offsets["lua_pushlightuserdata"], &Teardown::lua_pushlightuserdata, "lua_pushlightuserdata" },
	{ g_Offsets["lua_pushcclosure"], &Teardown::lua_pushcclosure, "lua_pushcclosure" },
	{ g_Offsets["lua_pushnil"], &Teardown::lua_pushnil, "lua_pushnil" },
	{ g_Offsets["lua_pushnumber"], &Teardown::lua_pushnumber, "lua_pushnumber" },
	{ g_Offsets["lua_pushinteger"], &Teardown::lua_pushinteger, "lua_pushinteger" },
	{ g_Offsets["lua_pushlstring"], &Teardown::lua_pushlstring, "lua_pushlstring" },
	{ g_Offsets["lua_pushboolean"], &Teardown::lua_pushboolean, "lua_pushboolean" },

	{ g_Offsets["lua_setfield"], &Teardown::lua_setfield, "lua_setfield" },

	{ g_Offsets["lua_topointer"], &Teardown::lua_topointer, "lua_topointer" },
	{ g_Offsets["lua_tointeger"], &Teardown::lua_tointeger, "lua_tointeger" },
	{ g_Offsets["lua_toboolean"], &Teardown::lua_toboolean, "lua_toboolean" },
	{ g_Offsets["lua_tolstring"], &Teardown::lua_tolstring, "lua_tolstring" },
	{ g_Offsets["lua_tonumber"], &Teardown::lua_tonumber, "lua_tonumber" },

	{ g_Offsets["lua_createtable"], &Teardown::lua_createtable, "lua_createtable" },

	{ g_Offsets["lua_pcall"], &Teardown::lua_pcall, "lua_pcall" },
	{ g_Offsets["lua_call"], &Teardown::lua_call, "lua_call" },

	{ g_Offsets["lua_type"], &Teardown::lua_type, "lua_type" },

	{ g_Offsets["lua_pop"], &Teardown::lua_pop, "lua_pop" },

	/*
		constructor functions
	*/
	{ g_Offsets["Body::Body"], &Teardown::BodyConstructor, "Body::Body", },
	{ g_Offsets["Shape::Shape"], &Teardown::ShapeConstructor, "Shape::Shape", },
	{ g_Offsets["Joint::Joint"], &Teardown::JointConstructor, "Joint::Joint", },
	{ g_Offsets["Light::Light"], &Teardown::LightConstructor, "Light::Light", },
	{ g_Offsets["Screen::Screen"], &Teardown::ScreenConstructor, "Screen::Screen", },
	{ g_Offsets["Script::Script"], &Teardown::ScriptConstructor, "Script::Script", },

	/*
		entity functions
	*/
	{ g_Offsets["LoadVox"], &Teardown::LoadVox, "LoadVox" },
	{ g_Offsets["GenVoxTexture"], &Teardown::GenVoxTexture, "GenVoxTexture" },
	{ g_Offsets["GenVoxBuffers"], &Teardown::GenVoxBuffers, "GenVoxBuffers" },
	{ g_Offsets["InitializeBody"], &Teardown::InitializeBody, "InitializeBody" },
	{ g_Offsets["UpdateBodyActive"], &Teardown::UpdateBodyActive, "UpdateBodyActive" },
	{ g_Offsets["InitializeBodyDynamic"], &Teardown::InitializeBodyDynamic, "InitializeBodyDynamic" },
	{ g_Offsets["AttachJoint"], &Teardown::AttachJoint, "AttachJoint" },
	{ g_Offsets["DetachJoint"], &Teardown::DetachJoint, "DetachJoint" },

	/*
		scene functions
	*/
	{ g_Offsets["QueryClosestPoint"], &Teardown::QueryClosestPoint, "QueryClosestPoint" },
	{ g_Offsets["QueryRaycast"], &Teardown::QueryRaycast, "QueryRaycast" },
	{ g_Offsets["QuerySpherecast"], &Teardown::QuerySpherecast, "QuerySpherecast" },
	{ g_Offsets["MakeHole"], &Teardown::MakeHole, "MakeHole" },
	{ g_Offsets["DrawLine"], &Teardown::DrawLine, "DrawLine" },
	{ g_Offsets["DrawCross"], &Teardown::DrawCross, "DrawCross" },
	{ g_Offsets["Paint"], &Teardown::Paint, "Paint" },

	/*
		misc functions
	*/
	{ g_Offsets["DebugPrint"], &Teardown::DebugPrint, "DebugPrint" },
	{ g_Offsets["Shutdown"], &Teardown::Shutdown, "Shutdown" }
};

void Teardown::GetFunctionAddresses() {
	MONITOR();
	for (const SFunctionInfo& Func : aFunctions) {
		unsigned __int64 iFunctionAddress = g_BaseAddress + Func.m_Offset;
		memcpy(Func.m_Function, &iFunctionAddress, sizeof(void*));
		LogVerbose("{} - {}", Func.m_Name, reinterpret_cast<void*>(iFunctionAddress));
	}
}