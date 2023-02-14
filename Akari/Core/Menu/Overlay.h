
#pragma once

#include "../Rendering.h"
#include "../../Utilities/Math.h"
#include "../../Utilities/System/ConsoleInfo.h"
#include <sys/ppu_thread.h>

class Overlay
{
public:
	Overlay();

	void OnUpdate();
	void OnShutdown();

	std::wstring GetTemperatureType();

private:
	void DrawOverlay();

	void UpdatePosition();
	void CalculateFps();
	static void UpdateInfoThread(uint64_t arg);

public:
	sys_ppu_thread_t UpdateInfoThreadId = SYS_PPU_THREAD_ID_INVALID;

	bool m_StateRunning{};

	float m_CPUTemp{};
	float m_GPUTemp{};
	float m_FanSpeed{};
	std::string m_LocalIp{};
	ConsoleInfo::memUsage_s m_MemoryUsage{};

private:
	vec2 m_Position{};
	Render::Align m_HorizontalAlignment{};
	Render::Align m_VerticalAlignment{};

	float m_FPS = 100.0;
	double m_FpsLastTime = 0.00;
	int m_FpsFrames = 0;
	int m_FpsFramesLastReport = 0;
	double m_FpsTimeElapsed = 0.00;
	double m_FpsTimeReport = 0.00;
	double m_FpsTimeLastReport = 0.00;
	float m_FpsREPORT_TIME = 1.0f;
};

extern Overlay g_Overlay;