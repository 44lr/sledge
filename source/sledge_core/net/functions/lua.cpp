#include "sledge/luahelpers.h"
#include "net/sledgelib.h"
#include "teardown/functions/lua.h"
#include "teardown/classes/script.h"
#include "teardown/classes/scene.h"

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void _RegisterLuaFunctionInternal(const char* cFunctionName) { Sledge::LuaHelpers::AddToRegisteredLuaFunctions(cFunctionName, SledgeLib::Interface->LuaFunctionWrapper); }
sledgelib_func void _UnregisterLuaFunctionInternal(const char* cFunctionName) { Sledge::LuaHelpers::RemoveRegisteredLuaFunction(cFunctionName); }

sledgelib_func char* _GetSCPath(ScriptCore* pSC) {
	if (pSC == 0) {
		char* pStringBuilder = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(1));
		pStringBuilder[1] = '\0';
		return pStringBuilder;
	}

	size_t lRetLen = pSC->m_ScriptPath.len();

	char* pStringBuilder = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(lRetLen + 1));
	memcpy(pStringBuilder, pSC->m_ScriptPath.c_str(), lRetLen);
	pStringBuilder[lRetLen] = '\0';

	return pStringBuilder;
}

sledgelib_func int _lua_tointeger(lua_State* L, int iIndex) { return Teardown::lua_tointeger(L, iIndex); }
sledgelib_func bool _lua_toboolean(lua_State* L, int iIndex) { return Teardown::lua_toboolean(L, iIndex); }
sledgelib_func double _lua_tonumber(lua_State* L, int iIndex) { return Teardown::lua_tonumber(L, iIndex); }
sledgelib_func void* _lua_tolstring(lua_State* L, int iIndex) {
	size_t lLength = 0;
	const char* cLuaString = Teardown::lua_tolstring(L, iIndex, &lLength);
	char* pStringBuilder = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(lLength + 1));
	
	if (cLuaString)
		memcpy(pStringBuilder, cLuaString, lLength);
	
	pStringBuilder[lLength] = '\0';

	return pStringBuilder;
}

sledgelib_func void _lua_pushnil(lua_State* L) { Teardown::lua_pushnil(L); }
sledgelib_func void _lua_pushnumber(lua_State* L, double dValue) { Teardown::lua_pushnumber(L, dValue); }
sledgelib_func void _lua_pushinteger(lua_State* L, int iValue) { Teardown::lua_pushinteger(L, iValue); }
sledgelib_func void _lua_pushboolean(lua_State* L, bool bValue) { Teardown::lua_pushboolean(L, bValue); }
sledgelib_func void _lua_pushlstring(lua_State* L, const char* cValue) { Teardown::lua_pushlstring(L, cValue, strlen(cValue)); }

sledgelib_func lua_State* _GetLuaStateFromSC(ScriptCore* pSC) { return *pSC->m_SCLS.m_LuaState; }
sledgelib_func unsigned int _GetScriptCount() { return g_Scene->m_Scripts.size(); }
sledgelib_func void _WriteSCArray(void* pArray) {
	int iOffset = 0;
	unsigned __int64 iArrayPtr = reinterpret_cast<unsigned __int64>(pArray);
	for (unsigned int i = 0; i < g_Scene->m_Scripts.size(); i++) {
		ScriptCore* pSC = &g_Scene->m_Scripts.get_at(i)->m_SC;
		memcpy(reinterpret_cast<void*>(iArrayPtr + iOffset), &pSC, sizeof(void*));
		iOffset += sizeof(void*);
	}
}

sledgelib_func void _lua_getfield(lua_State* L, int iIdx, const char* cKey) { Teardown::lua_getfield(L, iIdx, cKey); }
sledgelib_func int _lua_gettop(lua_State* L) { return Teardown::lua_gettop(L); }

sledgelib_func int _lua_pcall(lua_State* L, int iArgs, int iResults, int iErr) { return Teardown::lua_pcall(L, iArgs, iResults, iErr); }
sledgelib_func void _lua_call(lua_State* L, int iArgs, int iResults) { Teardown::lua_call(L, iArgs, iResults); }

sledgelib_func void _lua_pop(lua_State* L, int iElements) { Teardown::lua_pop(L, iElements); }

sledgelib_func int _lua_type(lua_State* L, int iIdx) { return Teardown::lua_type(L, iIdx); }