
#pragma once

#include "../Utilities/Math.h"
#include <string>

class Config
{
public:
	Config();

	void Load();
	void Save();
	void Reset();

	void LoadIniFile(const std::string& iniPath);
	void SaveIniFile(const std::string& iniPath);

public:
	struct {
		bool enableBinds{};
		int binds{};
	} screenshots;
	struct {
		bool enable{};
		bool drawFps{};
		bool drawScreenRes{};
		bool drawCPUTemp{};
		bool drawGPUTemp{};
		bool drawFanSpeed{};
		bool drawRAMUsage{};
		bool drawFreeRAM{};
		bool drawLocalIp{};

		int refreshDelay = 1;
		int fpsPrecision{};
		int tempType{};
		int memoryType{};
		int displayMode{};
		float sizeText = 20.0;
		int horizontalPos{};
		int verticalPos{};
	} overlay;
	struct {
		int animationType{};
		vec2 position{ 0.0, 0.0 };
		vec2 safeArea{ 31.0, 18.0 };
		float sizeWidth = 300.0;
		float sizeBorder = 2.0;
		float sizeText = 20.0;
		Color colorLeft{ 1.0, 1.0, 0.0, 1.0 };
		Color colorRight{ 0.0, 1.0, 1.0, 1.0 };
		Color colorBackground{ 0.0, 0.0, 0.0, 0.75 };
		Color colorText{ 1.0, 1.0, 1.0, 1.0 };
		bool useSound = true;
		bool useDualColor = true;
		bool showControlsHelp = true;
	} menu;
};

extern Config g_Config;
