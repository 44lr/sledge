#pragma once

#include "teardown/types/small_string.h"

namespace Teardown {
	inline void (*DebugPrint) (void* m_Debug, small_string* ssMessage);
	inline void (*Shutdown)();
}