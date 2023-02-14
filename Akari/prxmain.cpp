
#include "Core/Configuration.h"
#include "Core/Helpers.h"
#include "Core/Hooks.h"
#include "Core/Input.h"
#include "Core/Rendering.h"
#include "Core/Version.h"
#include "Core/Menu/Base.h"
#include "Core/Menu/Submenus.h"
#include "Core/Menu/Overlay.h"
#include "Utilities/System/SystemCalls.h"
#include "Utilities/System/Timers.h"
#include <sys/prx.h>
#include <sys/ppu_thread.h>
#include <vsh/stdc.h>

SYS_MODULE_INFO(Akari, 0, BUILD_VERSION_MAJOR, BUILD_VERSION_MINOR);
SYS_MODULE_START(module_start);
SYS_MODULE_STOP(module_stop);

sys_ppu_thread_t gMainPpuThreadId = SYS_PPU_THREAD_ID_INVALID;

CDECL_BEGIN
int module_start(size_t args, const void* argp)
{
	sys_ppu_thread_create(&gMainPpuThreadId, [](uint64_t arg) -> void
	{
		do Timers::Sleep(1000); while (!paf::View::Find("explore_plugin"));

		stdc::printf(BUILD_STRING "\n");

		g_Input = Input();
		g_Render = Render();
		g_Helpers = Helpers();

		g_Config = Config();
		g_Menu = Menu(MainSubmenu);
		g_Overlay = Overlay();

		InstallHooks();

		sys_ppu_thread_exit(0);
	},
		0, 1069, 1536, SYS_PPU_THREAD_CREATE_JOINABLE, "Akari::module_start");

	Syscall::_sys_ppu_thread_exit(0);
	return 0;
}

int module_stop(size_t args, const void* argp)
{
	sys_ppu_thread_t stopPpuThreadId;
	int ret = sys_ppu_thread_create(&stopPpuThreadId, [](uint64_t arg) -> void
	{
		RemoveHooks();

		g_Overlay.OnShutdown();
		g_Config.Save();
		g_Render.DestroyWidgets();

		sys_ppu_thread_yield();
		Timers::Sleep(1337);

		if (gMainPpuThreadId != SYS_PPU_THREAD_ID_INVALID)
		{
			uint64_t exitCode;
			sys_ppu_thread_join(gMainPpuThreadId, &exitCode);
		}

		sys_ppu_thread_exit(0);
	},
		0, 2814, 1024, SYS_PPU_THREAD_CREATE_JOINABLE, "Akari::module_stop");

	if (ret == 0)
	{
		uint64_t exitCode;
		sys_ppu_thread_join(stopPpuThreadId, &exitCode);
	}

	Timers::Sleep(5);

	Syscall::finalize_module();
	Syscall::_sys_ppu_thread_exit(0);
	return 0;
}
CDECL_END