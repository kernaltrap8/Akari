
#include "Submenus.h"
#include "Base.h"
#include "../Configuration.h"
#include "../Helpers.h"
#include "../Version.h"
#include "../Exports/vsh/vshcommon.h"
#include "../../Utilities/System.h"
#include <vsh/stdc.h>

void MainSubmenu()
{
	g_Menu.Title("Main menu");
	g_Menu.Option("Console manager").Submenu(ConsoleManagerSubmenu);
	g_Menu.Option("Screenshots").Submenu(ScreenshotsSubmenu);
	g_Menu.Option("Overlay").Submenu(OverlaySubmenu);
	g_Menu.Option("Settings").Submenu(SettingsSubmenu);
}

void ConsoleManagerSubmenu()
{
	g_Menu.Title("Console manager menu");
	g_Menu.Option("Informations").Submenu(ConsoleManagerInfoSubmenu);
	g_Menu.Option("Power options").Submenu(ConsoleManagerPowerSubmenu);
	g_Menu.Option("Console ID").Submenu(ConsoleManagerConsoleIdSubmenu);
	g_Menu.Option("Leds").Submenu(ConsoleManagerLedsSubmenu);
	g_Menu.Option("Buzzer").Submenu(ConsoleManagerBuzzerSubmenu);
	g_Menu.Option("Bluray drive").Submenu(ConsoleManagerBDSubmenu);
}

void ConsoleManagerInfoSubmenu()
{
	g_Menu.Title("Informations menu");
	g_Menu.Option("Show menu version").Action([]()
	{
		vsh::ShowButtonNavigationMessage(BUILD_STRING);
	});
	g_Menu.Option("Show firmware").Action([]()
	{
		vsh::ShowButtonNavigationMessage(stdc::va("Firmware: %1.2f %s%s", ConsoleInfo::GetFirmwareVersion(), ConsoleInfo::GetFirmwareType().c_str(), g_Helpers.m_IsHen ? " HEN" : ""));
	});
	g_Menu.Option("Show console type").Action([]()
	{
		vsh::ShowButtonNavigationMessage(stdc::va("Console: %s", ConsoleInfo::GetFirmwareType().c_str()));
	});
	g_Menu.Option("Show temperature").Action([]()
	{
		wchar_t buffer[0x20];

		switch (g_Config.overlay.tempType)
		{
		case 0:
			stdc::swprintf(buffer, 0x20, L"CPU: %.0f\u00B0C, GPU: %.0f\u00B0C", ConsoleInfo::GetTemperatureCelsius(0), ConsoleInfo::GetTemperatureCelsius(1));
			break;
		case 1:
			stdc::swprintf(buffer, 0x20, L"CPU: %.0f\u00B0F, GPU: %.0f\u00B0F", ConsoleInfo::GetTemperatureFahrenheit(0), ConsoleInfo::GetTemperatureFahrenheit(1));
			break;
		case 2:
			stdc::swprintf(buffer, 0x20, L"CPU: %.0fK, GPU: %.0fK", ConsoleInfo::GetTemperatureKelvin(0), ConsoleInfo::GetTemperatureKelvin(1));
			break;
		}

		vsh::ShowButtonNavigationMessage(buffer);
	});
	g_Menu.Option("Show fan speed").Action([]()
	{
		vsh::ShowButtonNavigationMessage(stdc::va("Fan speed: %.0f%%", ConsoleInfo::GetFanSpeed()));
	});
}

void ConsoleManagerPowerSubmenu()
{
	g_Menu.Title("Power menu");
	g_Menu.Option("Shutdown").Action([]()
	{
		File::DeleteFile("/dev_hdd0/tmp/turnoff");
		g_Config.Save();
		Syscall::sys_sm_shutdown(Syscall::sys_shutdown);
	});
	g_Menu.Option("Soft reboot").Action([]()
	{
		File::DeleteFile("/dev_hdd0/tmp/turnoff");
		g_Config.Save();
		Syscall::sys_sm_shutdown(Syscall::sys_reboot);
	});
	g_Menu.Option("Hard reboot").Action([]()
	{
		File::DeleteFile("/dev_hdd0/tmp/turnoff");
		g_Config.Save();
		Syscall::sys_sm_shutdown(Syscall::sys_reboot | Syscall::sys_hard_shutdown);
	});
	g_Menu.Option("LV2 reboot").Action([]()
	{
		File::DeleteFile("/dev_hdd0/tmp/turnoff");
		g_Config.Save();
		Syscall::sys_sm_shutdown(Syscall::sys_load_lpar | Syscall::lpar_id_1 | Syscall::sys_reboot);
	});
}

void ConsoleManagerConsoleIdSubmenu()
{
	g_Menu.Title("Console ID menu");
	g_Menu.Option("Show current idps").Action([]()
	{
		if (Syscall::sys_mapi_is_cobra_based())
			vsh::ShowButtonNavigationMessage(stdc::va("Current idps: %s", ConsoleInfo::GetCurrentIdps().c_str()));
		else
			vsh::ShowButtonNavigationMessage(L"Cobra need to be enabled in order to use this option");
	});
	g_Menu.Option("Show current psid").Action([]()
	{
		if (Syscall::sys_mapi_is_cobra_based())
			vsh::ShowButtonNavigationMessage(stdc::va("Current psid: %s", ConsoleInfo::GetCurrentPsid().c_str()));
		else
			vsh::ShowButtonNavigationMessage(L"Cobra need to be enabled in order to use this option");
	});
	g_Menu.Option("Randomize current idps").Action([]()
	{
		if (Syscall::sys_mapi_is_cobra_based())
		{
			ConsoleInfo::RandomizeIdps();
			vsh::ShowButtonNavigationMessage(stdc::va("New idps: %s", ConsoleInfo::GetCurrentIdps().c_str()));
		}
		else
			vsh::ShowButtonNavigationMessage(L"Cobra need to be enabled in order to use this option");
	});
	g_Menu.Option("Randomize current psid").Action([]()
	{
		if (Syscall::sys_mapi_is_cobra_based())
		{
			ConsoleInfo::RandomizePsid();
			vsh::ShowButtonNavigationMessage(stdc::va("New psid: %s", ConsoleInfo::GetCurrentPsid().c_str()));
		}
		else
			vsh::ShowButtonNavigationMessage(L"Cobra need to be enabled in order to use this option");
	});
}

void ConsoleManagerLedsSubmenu()
{
	const char* leds[] = { "Green", "Red" };
	static int currentLed = 0;

	g_Menu.Title("Leds menu");
	g_Menu.Option("Led").Strings(leds, currentLed, 2);
	g_Menu.Option("Turn off").Action([]()
	{
		Syscall::sys_sm_control_led(currentLed + 1, Syscall::led_action_off);
	});
	g_Menu.Option("Turn on").Action([]()
	{
		Syscall::sys_sm_control_led(currentLed + 1, Syscall::led_action_on);
	});
	g_Menu.Option("Blink slow").Action([]()
	{
		Syscall::sys_sm_control_led(currentLed + 1, Syscall::led_action_blink_slow);
	});
	g_Menu.Option("Blink fast").Action([]()
	{
		Syscall::sys_sm_control_led(currentLed + 1, Syscall::led_action_blink_fast);
	});
}

void ConsoleManagerBuzzerSubmenu()
{
	g_Menu.Title("Buzzer menu");
	g_Menu.Option("Single").Action([]() 
	{ 
		Syscall::sys_sm_ring_buzzer(Syscall::ring_buzzer_single);
		vsh::ShowButtonNavigationMessage(L"Ring buzzer 'Single'"); 
	});
	g_Menu.Option("Double").Action([]() 
	{ 
		Syscall::sys_sm_ring_buzzer(Syscall::ring_buzzer_double); 
		vsh::ShowButtonNavigationMessage(L"Ring buzzer 'Double'"); 
	});
	g_Menu.Option("Triple").Action([]() 
	{ 
		Syscall::sys_sm_ring_buzzer(Syscall::ring_buzzer_triple); 
		vsh::ShowButtonNavigationMessage(L"Ring buzzer 'Triple'"); 
	});
	g_Menu.Option("Continuous").Action([]() 
	{ 
		Syscall::sys_sm_ring_buzzer(Syscall::ring_buzzer_continuous); 
		vsh::ShowButtonNavigationMessage(L"Ring buzzer 'Continuous'"); 
	});
}

void ConsoleManagerBDSubmenu()
{
	g_Menu.Title("Bluray drive menu");
	g_Menu.Option("Eject disc").Action([]() 
	{ 
		Syscall::sys_storage_eject_insert(false); 
		vsh::ShowButtonNavigationMessage(L"Ejecting disc"); 
	});
	g_Menu.Option("Insert disc").Action([]() 
	{ 
		Syscall::sys_storage_eject_insert(true);
		vsh::ShowButtonNavigationMessage(L"Inserting disc"); 
	});
}

void ScreenshotsSubmenu()
{
	const wchar_t* screenshotsBinds[] = { L"\uF888 & \uF884", L"\uF888 & \uF885", L"\uF888 & \uF886", L"\uF888 & \uF887" };

	g_Menu.Title("Screenshots menu");
	g_Menu.Option("Take screenshot").Action([]() { g_Helpers.TakeScreenshot(); });
	g_Menu.Option("Bind screenshots").Toggle(g_Config.screenshots.enableBinds);
	g_Menu.Option("Binds").Strings(screenshotsBinds, g_Config.screenshots.binds, 4);
}

void OverlaySubmenu()
{

	g_Menu.Title("Overlay menu");
	g_Menu.Option("Enable").Toggle(g_Config.overlay.enable);
	g_Menu.Option("Draw fps").Toggle(g_Config.overlay.drawFps);
	g_Menu.Option("Draw screen resolution").Toggle(g_Config.overlay.drawScreenRes);
	g_Menu.Option("Draw CPU temperature").Toggle(g_Config.overlay.drawCPUTemp);
	g_Menu.Option("Draw GPU temperature").Toggle(g_Config.overlay.drawGPUTemp);
	g_Menu.Option("Draw fan speed").Toggle(g_Config.overlay.drawFanSpeed);
	g_Menu.Option("Draw VSH RAM usage").Toggle(g_Config.overlay.drawRAMUsage);
	g_Menu.Option("Draw free RAM").Toggle(g_Config.overlay.drawFreeRAM);
	g_Menu.Option("Draw local IP address").Toggle(g_Config.overlay.drawLocalIp);
	g_Menu.Option("Settings").Submenu(OverlaySettingsSubmenu);
}

void OverlaySettingsSubmenu()
{
	const char* displayModes[] = { "XMB & Game", "XMB only", "Game only" };
	const char* temperatureTypes[] = { "Celsius", "Fahrenheit", "Kelvin" };
	const char* memoryTypes[] = { "KB", "MB" };
	const char* overlayHorizontalLocations[] = { "Left", "Center", "Right" };
	const char* overlayVerticalLocations[] = { "Top", "Center", "Bottom" };

	g_Menu.Title("Settings menu");
	g_Menu.Option("Display mode").Strings(displayModes, g_Config.overlay.displayMode, 3);
	g_Menu.Option("Refresh delay").Slider(g_Config.overlay.refreshDelay, 1, 10, 1, 100, "%is");
	g_Menu.Option("Fps precision").Slider(g_Config.overlay.fpsPrecision, 0, 5, 1);
	g_Menu.Option("Temperature type").Strings(temperatureTypes, g_Config.overlay.tempType, 3);
	g_Menu.Option("RAM type").Strings(memoryTypes, g_Config.overlay.memoryType, 2);
	g_Menu.Option("Text size").Slider(g_Config.overlay.sizeText, 10, 40, 0.25, 50);
	g_Menu.Option("Horizontal location").Strings(overlayHorizontalLocations, g_Config.overlay.horizontalPos, 3);
	g_Menu.Option("Vertical location").Strings(overlayVerticalLocations, g_Config.overlay.verticalPos, 3);
}

void SettingsSubmenu()
{
	const char* animationTypes[] =
	{
		"Linear",
		"In (Sine)", "Out (Sine)", "In-out (Sine)",
		"In (Quad)", "Out (Quad)", "In-out (Quad)",
		"In (Cubic)", "Out (Cubic)", "In-out (Cubic)",
		"In (Quart)", "Out (Quart)", "In-out (Quart)",
		"In (Quint)", "Out (Quint)", "In-out (Quint)",
		"In (Expo)", "Out (Expo)", "In-out (Expo)",
		"In (Circ)", "Out (Circ)", "In-out (Circ)",
		"In (Back)", "Out (Back)", "In-out (Back)",
		// it brokey
		//"In (Circ)", "Out (Circ)", "In-out (Circ)",
		//"In (Elastic)", "Out (Elastic)", "In-out (Elastic)",
		//"In (Bounce)", "Out (Bounce)", "In-out (Bounce)"
	};

	g_Menu.Title("Settings menu");
	g_Menu.Option("Menu sounds").Toggle(g_Config.menu.useSound);
	g_Menu.Option("Show menu controls").Toggle(g_Config.menu.showControlsHelp);
	g_Menu.Option("Animation type").Strings(animationTypes, g_Config.menu.animationType, 25);
	g_Menu.Option("Edit position").Toggle(g_Menu.m_StateMoving);
	g_Menu.Option("Border thickness").Slider(g_Config.menu.sizeBorder, 0, 10, 1, 50, "%.0f");
	g_Menu.Option("Text size").Slider(g_Config.menu.sizeText, 10, 40, 0.25, 50);
	g_Menu.Option("Menu width").Slider(g_Config.menu.sizeWidth, 200, 500, 2, 50, "%.0f");
	g_Menu.Option("Dual color").Toggle(g_Config.menu.useDualColor);
	if (g_Config.menu.useDualColor)
	{
		g_Menu.Option("Color left R").Slider(g_Config.menu.colorLeft.r, 0, 1, 0.01, 50);
		g_Menu.Option("Color left G").Slider(g_Config.menu.colorLeft.g, 0, 1, 0.01, 50);
		g_Menu.Option("Color left B").Slider(g_Config.menu.colorLeft.b, 0, 1, 0.01, 50);
		g_Menu.Option("Color right R").Slider(g_Config.menu.colorRight.r, 0, 1, 0.01, 50);
		g_Menu.Option("Color right G").Slider(g_Config.menu.colorRight.g, 0, 1, 0.01, 50);
		g_Menu.Option("Color right B").Slider(g_Config.menu.colorRight.b, 0, 1, 0.01, 50);
	}
	else
	{
		g_Menu.Option("Color R").Slider(g_Config.menu.colorRight.r, 0, 1, 0.01, 50);
		g_Menu.Option("Color G").Slider(g_Config.menu.colorRight.g, 0, 1, 0.01, 50);
		g_Menu.Option("Color B").Slider(g_Config.menu.colorRight.b, 0, 1, 0.01, 50);
	}
	g_Menu.Option("Background opacity").Slider(g_Config.menu.colorBackground.a, 0, 1, 0.01, 50);
	g_Menu.Option("Save configuration").Action([]
	{
		g_Config.Save(); 
		vsh::ShowButtonNavigationMessage(L"Configuration saved");
	});
	g_Menu.Option("Load configuration").Action([]
	{ 
		g_Config.Load(); 
		vsh::ShowButtonNavigationMessage(L"Configuration loaded");
	});
	g_Menu.Option("Reset configuration").Action([]
	{
		g_Config.Reset();
		vsh::ShowButtonNavigationMessage(L"Configuration reset");
	});
}