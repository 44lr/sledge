#include "teardown/classes/game.h"
#include "teardown/functions/misc.h"

#include "net/sledgelib.h"

#include <processenv.h>

#define sledgelib_func extern "C" __declspec(dllexport)

sledgelib_func void _GetLevelPath(char* cReturn, unsigned int iReturnBufferSize) {
	size_t LevelPathLen = g_Game->m_LevelPath.len();

	if (LevelPathLen > iReturnBufferSize) {
		memcpy(cReturn, g_Game->m_LevelPath.c_str(), iReturnBufferSize);
		cReturn[iReturnBufferSize] = '\0';
	}

	memcpy(cReturn, g_Game->m_LevelPath.c_str(), LevelPathLen);
	cReturn[LevelPathLen] = '\0';
}

sledgelib_func void _GetLevelLayers(char* cReturn, unsigned int iReturnBufferSize) {
	size_t LevelLayersLen = g_Game->m_LevelLayers.len();

	if (LevelLayersLen > iReturnBufferSize) {
		memcpy(cReturn, g_Game->m_LevelLayers.c_str(), iReturnBufferSize);
		cReturn[iReturnBufferSize] = '\0';
	}

	memcpy(cReturn, g_Game->m_LevelLayers.c_str(), LevelLayersLen);
	cReturn[LevelLayersLen] = '\0';
}

sledgelib_func void _GetLevelId(char* cReturn, unsigned int iReturnBufferSize) {
	size_t LevelIdLen = g_Game->m_LevelId.len();

	if (LevelIdLen > iReturnBufferSize) {
		memcpy(cReturn, g_Game->m_LevelId.c_str(), iReturnBufferSize);
		cReturn[iReturnBufferSize] = '\0';
	}

	memcpy(cReturn, g_Game->m_LevelId.c_str(), LevelIdLen);
	cReturn[LevelIdLen] = '\0';
}

sledgelib_func void _LoadLevel(char* cLevelPath, char* cLevelLayers, char* cLevelId, char* cLevelModId) {
	small_string* ssLevelPath = new small_string(cLevelPath);
	small_string* ssLevelLayers = new small_string(cLevelLayers);
	small_string* ssLevelId = new small_string(cLevelId);
	small_string* ssLevelModId = new small_string(cLevelModId);

	g_Game->m_LevelPath = *ssLevelPath;
	g_Game->m_LevelLayers = *ssLevelLayers;
	g_Game->m_LevelId = *ssLevelId;
	g_Game->m_LevelModId = *ssLevelModId;

	g_Game->m_NextState = EGameState::Playing;

	delete ssLevelPath;
	delete ssLevelLayers;
	delete ssLevelId;
	delete ssLevelModId;
}

sledgelib_func void _DebugPrint(char* cMessage) {
	small_string* ssMessage = new small_string(cMessage);
	Teardown::DebugPrint(g_Game->m_Debug, ssMessage);
	delete ssMessage;
}

sledgelib_func void _Shutdown() {
	Teardown::Shutdown();
}

sledgelib_func bool _IsPlaying() { return (g_Game->m_State == EGameState::Playing) && (!g_Game->m_Pause); }

sledgelib_func void _SetState(EGameState iState) { g_Game->m_NextState = iState; }
sledgelib_func EGameState _GetState() { return g_Game->m_State; }

sledgelib_func char* _GetCMDLine() {
	char* cCMDLine = GetCommandLineA();
	size_t lCMDLineLength = strlen(cCMDLine);
	char* pStringBuilder = reinterpret_cast<char*>(SledgeLib::Interface->AllocateString(lCMDLineLength + 1));
	memcpy(pStringBuilder, cCMDLine, lCMDLineLength);
	pStringBuilder[lCMDLineLength] = '\0';
	return pStringBuilder;
}