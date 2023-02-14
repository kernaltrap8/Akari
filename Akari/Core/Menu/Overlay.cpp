
#include "Overlay.h"
#include "../Configuration.h"
#include "../Helpers.h"
#include "../Exports/paf/View.h"
#include "../Exports/vsh/vshmain.h"
#include "../../Utilities/System/Timers.h"
#include <sys/sys_time.h>
#include <vsh/netctl_main.h>
#include <vsh/paf.h>
#include <vsh/stdc.h>

Overlay g_Overlay;

Overlay::Overlay()
{
	sys_ppu_thread_create(&UpdateInfoThreadId, UpdateInfoThread, 0, 0xB01, 512, SYS_PPU_THREAD_CREATE_JOINABLE, "Overlay::UpdateInfoThread()");
}

void Overlay::OnUpdate()
{
	UpdatePosition();
	CalculateFps();

	if (g_Config.overlay.enable)
		DrawOverlay();
}

void Overlay::OnShutdown()
{
	if (UpdateInfoThreadId != SYS_PPU_THREAD_ID_INVALID)
	{
		m_StateRunning = false;

		sys_ppu_thread_yield();
		Timers::Sleep(g_Config.overlay.refreshDelay * 1000 + 500);

		uint64_t exitCode;
		sys_ppu_thread_join(UpdateInfoThreadId, &exitCode);
	}
}

void Overlay::DrawOverlay()
{
	if (g_Config.overlay.displayMode == 1 && vsh::GetCooperationMode() != vsh::CooperationMode::XMB)
		return;

	if (g_Config.overlay.displayMode == 2 && vsh::GetCooperationMode() == vsh::CooperationMode::XMB)
		return;

	std::wstring overlayText = L"";

	if (g_Config.overlay.drawFps)
	{
		wchar_t format[16];
		stdc::swprintf(format, 16, L"FPS: %%.%if\n", g_Config.overlay.fpsPrecision);

		overlayText += stdc::wva(format, m_FPS);
	}

	if (g_Config.overlay.drawScreenRes)
		overlayText += stdc::wva(L"Screen resolution: %ix%i\n", paf_F476E8AA(), paf_AC984A12());

	if (g_Config.overlay.drawCPUTemp)
		overlayText += stdc::wva(L"CPU: %.0f", m_CPUTemp) + GetTemperatureType() + (g_Config.overlay.drawGPUTemp ? L" / " : L"\n");

	if (g_Config.overlay.drawGPUTemp)
		overlayText += stdc::wva(L"GPU: %.0f", m_GPUTemp) + GetTemperatureType() + L"\n";

	if (g_Config.overlay.drawFanSpeed)
		overlayText += stdc::wva(L"Fan speed: %.0f%%\n", m_FanSpeed);

	if (g_Config.overlay.drawRAMUsage)
	{
		if (g_Config.overlay.memoryType == 1)
		{
			overlayText += stdc::wva(L"RAM: %.1f%% %.1f / %.1f MB", m_MemoryUsage.percent, m_MemoryUsage.used, m_MemoryUsage.total);
			overlayText += g_Config.overlay.drawFreeRAM ? stdc::wva(L" (Free: %.1f MB)\n", m_MemoryUsage.free) : L"\n";
		}
		else
		{
			overlayText += stdc::wva(L"RAM: %.1f%% %.0f / %.0f KB", m_MemoryUsage.percent, m_MemoryUsage.used, m_MemoryUsage.total);
			overlayText += g_Config.overlay.drawFreeRAM ? stdc::wva(L" (Free: %.0f KB)\n", m_MemoryUsage.free) : L"\n";
		}
	}

	if (g_Config.overlay.drawLocalIp)
		overlayText += stdc::wva(L"Local IP: %s\n", m_LocalIp.c_str());

	g_Render.Text(
		overlayText,
		m_Position,
		g_Config.overlay.sizeText,
		m_HorizontalAlignment,
		m_VerticalAlignment,
		g_Config.menu.colorText);
}

void Overlay::UpdatePosition()
{
	switch (g_Config.overlay.horizontalPos)
	{
	case 0:
	{
		m_Position.x = -VIEWPORT_WIDTH / 2 + g_Config.menu.safeArea.x + 5;
		m_HorizontalAlignment = Render::Left;
		break;
	}
	case 1:
	{
		m_Position.x = 0;
		m_HorizontalAlignment = Render::Centered;
		break;
	}
	case 2:
	{
		m_Position.x = VIEWPORT_WIDTH / 2 - g_Config.menu.safeArea.x - 5;
		m_HorizontalAlignment = Render::Right;
		break;
	}
	}

	switch (g_Config.overlay.verticalPos)
	{
	case 0:
	{
		m_Position.y = VIEWPORT_HEIGHT / 2 - g_Config.menu.safeArea.y - 5;
		m_VerticalAlignment = Render::Top;
		break;
	}
	case 1:
	{
		m_Position.y = 0;
		m_VerticalAlignment = Render::Centered;
		break;
	}
	case 2:
	{
		m_Position.y = -VIEWPORT_HEIGHT / 2 + g_Config.menu.safeArea.y + 5;
		m_VerticalAlignment = Render::Bottom;
		break;
	}
	}
}

void Overlay::CalculateFps()
{
	double timeNow = (double)sys_time_get_system_time() * .000001f;
	double timeElapsedInFrame = (double)(timeNow - m_FpsLastTime);

	m_FpsLastTime = timeNow;
	++m_FpsFrames;
	m_FpsTimeElapsed += timeElapsedInFrame;

	if (m_FpsTimeElapsed >= m_FpsTimeReport)
	{
		m_FPS = (m_FpsFrames - m_FpsFramesLastReport) * 1.0 / (float)(m_FpsTimeElapsed - m_FpsTimeLastReport);

		m_FpsTimeReport += m_FpsREPORT_TIME;
		m_FpsTimeLastReport = m_FpsTimeElapsed;
		m_FpsFramesLastReport = m_FpsFrames;
	}
}

std::wstring Overlay::GetTemperatureType()
{
	switch (g_Config.overlay.tempType)
	{
	case 0: return L"°C";
	case 1: return L"°F";
	case 2: return L"K";
	default: return L"";
	}
}

void Overlay::UpdateInfoThread(uint64_t arg)
{
	g_Overlay.m_StateRunning = true;

	while (g_Overlay.m_StateRunning)
	{
		Timers::Sleep(g_Config.overlay.refreshDelay * 1000);

		if (g_Helpers.m_IsHen && g_Helpers.m_StateGameJustLaunched)
			continue;
		
		g_Overlay.m_MemoryUsage = ConsoleInfo::GetMemoryUsage();

		if (g_Config.overlay.memoryType == 1)
		{
			g_Overlay.m_MemoryUsage.total /= 1024;
			g_Overlay.m_MemoryUsage.free /= 1024;
			g_Overlay.m_MemoryUsage.used /= 1024;
		}

		g_Overlay.m_FanSpeed = ConsoleInfo::GetFanSpeed();

		switch (g_Config.overlay.tempType)
		{
		case 0:
			g_Overlay.m_CPUTemp = ConsoleInfo::GetTemperatureCelsius(0);
			g_Overlay.m_GPUTemp = ConsoleInfo::GetTemperatureCelsius(1);
			break;
		case 1:
			g_Overlay.m_CPUTemp = ConsoleInfo::GetTemperatureFahrenheit(0);
			g_Overlay.m_GPUTemp = ConsoleInfo::GetTemperatureFahrenheit(1);
			break;
		case 2:
			g_Overlay.m_CPUTemp = ConsoleInfo::GetTemperatureKelvin(0);
			g_Overlay.m_GPUTemp = ConsoleInfo::GetTemperatureKelvin(1);
			break;
		}

		char ip[16];
		netctl_main_9A528B81(16, ip);
		g_Overlay.m_LocalIp = ip[0] ? ip : "0.0.0.0";
		
	}

	sys_ppu_thread_exit(0);
}