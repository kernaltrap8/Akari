
#pragma once

#include "System/ConsoleInfo.h"
#include "System/File.h"
#include "System/SystemCalls.h"
#include "System/Timers.h"
#include <vsh/stdc.h>

extern "C"
{
	void* _sys_malloc(size_t size);
	void* _sys_memalign(size_t boundary, size_t size);
	void _sys_free(void* ptr);
}