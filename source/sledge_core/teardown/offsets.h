#pragma once
#include <unordered_map>
#include <string>

inline std::unordered_map<std::string, unsigned __int64> g_Offsets = {
	 {"SetBool", 0x000000000013A9B0 },
	 {"memmove", 0x0000000000290300 },
	 {"realloc", 0x00000000002F7470 },
	 {"free", 0x00000000002EDC54 },
	 {"SetInt", 0x000000000013AAC0 },
	 {"lua_topointer", 0x0000000000243E00 },
	 {"IsWindowForegroundWindow", 0x000000000016CD80 },
	 {"ChangeLevel", 0x000000000003E8C0 },
	 {"UpdatePlayer", 0x00000000000BFC70 },
	 {"calloc", 0x00000000002F1134 },
	 {"malloc", 0x00000000002EDC94 },
	 {"GetFloat", 0x0000000000139B20 },
	 {"SetString", 0x000000000013AB30 },
	 {"SetFloat", 0x000000000013AA50 },
	 {"GetBool", 0x0000000000139A70 },
	 {"lua_pushlightuserdata", 0x00000000002430F0 },
	 {"GetInt", 0x0000000000139B70 },
	 {"GetString", 0x0000000000139BC0 },
	 {"SetTag", 0x0000000000110630 },
	 {"HasTag", 0x00000000001102A0 },
	 {"RemoveTag", 0x00000000001103C0 },
	 {"GetTag", 0x0000000000110110 },
	 {"Game::Game", 0x0000000000038CC0 },
	 {"Body::Body", 0x00000000000BAC70 },
	 {"Shape::Shape", 0x000000000015DFA0 },
	 {"Update", 0x000000000003B6C0 },
	 {"SwitchGameState", 0x000000000003E090 },
	 {"lua_pushnil", 0x0000000000243180 },
	 {"lua_gettop", 0x00000000002427A0 },
	 {"lua_pushcclosure", 0x0000000000242FC0 },
	 {"lua_setfield", 0x0000000000243880 },
	 {"DebugPrint", 0x000000000010EF70 },
	 {"lua_tointeger", 0x0000000000243CA0 },
	 {"ScriptCore::RegisterLuaFunctions", 0x000000000014EE80 },
	 {"lua_toboolean", 0x0000000000243C40 },
	 {"lua_tolstring", 0x0000000000243D00 },
	 {"Log", 0x000000000016F330 },
};