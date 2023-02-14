
#include "Hooks.h"
#include "Helpers.h"
#include "Input.h"
#include "Rendering.h"
#include "Menu/Base.h"
#include "Menu/Overlay.h"
#include "../Utilities/Memory/ImportsExports.h"
#include "../Utilities/System/Timers.h"
#include "../Utilities/System/SystemCalls.h"
#include <vsh/stdc.h>

using namespace Memory;

Detour* cellPadGetDataExtraInternal_Detour;
ImportExportDetour* pafFrameworkBegin_Detour;

int cellPadGetDataExtraInternal_Hook(void* r3, unsigned int port, int* deviceType, CellPadData* data, bool returnWithoutClearing)
{
	int returnValue = cellPadGetDataExtraInternal_Detour->CallOriginal<int>(r3, port, deviceType, data, returnWithoutClearing);
	if (port)
		return returnValue;

	g_Input.SetInputData(data);

	if (g_Menu.IsOpened())
	{
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL1] &= ~(CELL_PAD_CTRL_LEFT | CELL_PAD_CTRL_DOWN | CELL_PAD_CTRL_RIGHT | CELL_PAD_CTRL_UP);
		data->button[CELL_PAD_BTN_OFFSET_DIGITAL2] &= ~(CELL_PAD_CTRL_CROSS | CELL_PAD_CTRL_CIRCLE);
	
		if (g_Menu.IsMoving())
		{
			data->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X] = 128;
			data->button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y] = 128;
		}
	}

	return returnValue;
}

void pafFrameworkBegin_Hook(void* framework, float frameTime)
{
	pafFrameworkBegin_Detour->CallOriginal<void>(framework, frameTime);

	g_Helpers.OnUpdate();
	g_Render.OnUpdate();
	g_Overlay.OnUpdate();
	g_Menu.OnUpdate();
}

void InstallHooks()
{
	cellPadGetDataExtraInternal_Detour = new Detour(ResolveBranch(FindExportByName("sys_io", 0x3733EA3C)->sub + 0x6C), (uint32_t)cellPadGetDataExtraInternal_Hook);
	pafFrameworkBegin_Detour = new ImportExportDetour(ImportExportDetour::Export, "paf", 0x59BDA198, (uint32_t)pafFrameworkBegin_Hook);
}

void RemoveHooks()
{
	delete cellPadGetDataExtraInternal_Detour;
	delete pafFrameworkBegin_Detour;
}