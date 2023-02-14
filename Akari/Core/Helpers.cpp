
#include "Helpers.h"
#include "Configuration.h"
#include "Input.h"
#include "Exports/vsh/vshcommon.h"
#include "Exports/vsh/vshmain.h"
#include "../Utilities/System.h"
#include <vsh/stdc.h>
#include <vsh/interfaces/system_plugin.h>

Helpers g_Helpers;

Helpers::Helpers()
{
	m_IsHen = Syscall::sys_mapi_is_hen();
}

void Helpers::OnUpdate()
{
	game_ext_plugin = paf::View::Find("game_ext_plugin");
	game_plugin = paf::View::Find("game_plugin");
	system_plugin = paf::View::Find("system_plugin");
	xmb_plugin = paf::View::Find("xmb_plugin");

	page_autooff_guide = system_plugin ? system_plugin->FindWidget("page_autooff_guide") : nullptr;


	MonitorGameState();

	if (g_Config.screenshots.enableBinds && ScreenshotsBinds())
		TakeScreenshot();
}

void Helpers::MonitorGameState()
{
	unsigned int timeNow = Timers::GetTimeNow();

	if (vsh::GetCooperationMode() != vsh::CooperationMode::XMB && !m_StateGameRunning)
	{
		m_StateGameRunning = true;
		m_StateGameJustLaunched = true;
		m_GameLaunchTime = timeNow;
	}
	else if (vsh::GetCooperationMode() == vsh::CooperationMode::XMB)
		m_StateGameRunning = false;

	if (timeNow - m_GameLaunchTime > 15 * 1000)
		m_StateGameJustLaunched = false;

}

void Helpers::TakeScreenshot()
{
	unsigned int timeNow = Timers::GetTimeNow();

	if ((timeNow - m_ScreenshotLastTime) < 500)
		return;

	m_ScreenshotLastTime = timeNow;

	if (vsh::GetCooperationMode() == vsh::CooperationMode::XMB)
	{
		if (!system_plugin)
			return;

		system_plugin_interface* system_interface = system_plugin->GetInterface<system_plugin_interface*>(1);
		if (!system_interface)
			return;

		std::string screenshotPath = File::GetCurrentDir() + "Screenshots";
		stdc::mkdir(screenshotPath.c_str(), 40777);

		time_t rawtime;
		stdc::time(&rawtime);
		tm* timeinfo = stdc::localtime(&rawtime);

		char datestr[100];
		stdc::strftime(datestr, 100, "%Y-%m-%d_%H-%M-%S", timeinfo);

		std::string screenshotName = screenshotPath + "/" + datestr + ".bmp";

		system_interface->saveBMP(screenshotName.c_str());

		wchar_t buffer[130];
		stdc::swprintf(buffer, 0xA0, L"Screenshot saved as: %s", screenshotName.c_str());
		vsh::ShowButtonNavigationMessage(buffer);
	}
	else
		vsh::ShowButtonNavigationMessage(L"Screenshots aren't supported in game");
}

bool Helpers::ScreenshotsBinds()
{
	switch (g_Config.screenshots.binds)
	{
	case 0: return g_Input.IsButtonBinds(Input::BUTTON_L1, Input::BUTTON_PAD_UP);
	case 1: return g_Input.IsButtonBinds(Input::BUTTON_L1, Input::BUTTON_PAD_DOWN);
	case 2: return g_Input.IsButtonBinds(Input::BUTTON_L1, Input::BUTTON_PAD_LEFT);
	case 3: return g_Input.IsButtonBinds(Input::BUTTON_L1, Input::BUTTON_PAD_RIGHT);
	default: return false;
	}
}