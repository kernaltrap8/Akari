
#include "Configuration.h"
#include "../Utilities/System/File.h"
#include "../Utilities/System/SimpleIni.h"

Config g_Config;

Config::Config()
{
	Load();
}

void Config::Load()
{
	std::string configPath = File::GetCurrentDir() + "Akari.ini";

	if (File::FileExist(configPath))
		LoadIniFile(configPath);
}

void Config::Save()
{
	std::string configPath = File::GetCurrentDir() + "Akari.ini";
	SaveIniFile(configPath);
}

void Config::Reset()
{
	std::string configPath = File::GetCurrentDir() + "Akari.ini";

	if (File::FileExist(configPath))
		File::DeleteFile(configPath);
	*this = Config();
	SaveIniFile(configPath);
}

void Config::LoadIniFile(const std::string& iniPath)
{
	CSimpleIniA ini;
	ini.SetUnicode();
	ini.LoadFile(iniPath.c_str());

	// [screenshots]
	screenshots.enableBinds = ini.GetBoolValue("screenshots", "bEnableBinds");
	screenshots.binds = ini.GetLongValue("screenshots", "iBinds");

	// [overlay]
	overlay.enable = ini.GetBoolValue("overlay", "bEnable");
	overlay.drawFps = ini.GetBoolValue("overlay", "bDrawFps");
	overlay.drawScreenRes = ini.GetBoolValue("overlay", "bDrawScreenRes");
	overlay.drawCPUTemp = ini.GetBoolValue("overlay", "bDrawCPUTemp");
	overlay.drawGPUTemp = ini.GetBoolValue("overlay", "bDrawGPUTemp");
	overlay.drawFanSpeed = ini.GetBoolValue("overlay", "bDrawFanSpeed");
	overlay.drawRAMUsage = ini.GetBoolValue("overlay", "bDrawRAMUsage");
	overlay.drawFreeRAM = ini.GetBoolValue("overlay", "bDrawFreeRAM");
	overlay.drawLocalIp = ini.GetBoolValue("overlay", "bDrawLocalIp");

	overlay.refreshDelay = ini.GetLongValue("overlay", "iRefreshDelay", 1L);
	overlay.fpsPrecision = ini.GetLongValue("overlay", "iFpsPrecision");
	overlay.tempType = ini.GetLongValue("overlay", "iTempType");
	overlay.memoryType = ini.GetLongValue("overlay", "iMemoryType");
	overlay.displayMode = ini.GetLongValue("overlay", "iDisplayMode");
	overlay.sizeText = ini.GetDoubleValue("overlay", "fSizeText", 20.0);
	overlay.horizontalPos = ini.GetLongValue("overlay", "iHorizontalPos");
	overlay.verticalPos = ini.GetLongValue("overlay", "iVerticalPos");

	// [menu]
	menu.animationType = ini.GetLongValue("menu", "iAnimationType");
	menu.position.x = ini.GetDoubleValue("menu", "fPositionX");
	menu.position.y = ini.GetDoubleValue("menu", "fPositionY");
	menu.sizeWidth = ini.GetDoubleValue("menu", "fSizeWidth", 300.0);
	menu.sizeBorder = ini.GetDoubleValue("menu", "fSizeBorder", 2.0);
	menu.sizeText = ini.GetDoubleValue("menu", "fSizeText", 20.0);
	menu.colorLeft.r = ini.GetDoubleValue("menu", "fColorLeftR", 1.0);
	menu.colorLeft.g = ini.GetDoubleValue("menu", "fColorLeftG", 1.0);
	menu.colorLeft.b = ini.GetDoubleValue("menu", "fColorLeftB", 0.0);
	menu.colorRight.r = ini.GetDoubleValue("menu", "fColorRightR", 0.0);
	menu.colorRight.g = ini.GetDoubleValue("menu", "fColorRightG", 1.0);
	menu.colorRight.b = ini.GetDoubleValue("menu", "fColorRightB", 1.0);
	menu.colorBackground.a = ini.GetDoubleValue("menu", "fBackgroundOpacity", 0.75);
	menu.useSound = ini.GetBoolValue("menu", "bSounds", true);
	menu.useDualColor = ini.GetBoolValue("menu", "bDualColor", true);
	menu.showControlsHelp = ini.GetBoolValue("menu", "bShowControlsHelp", true);
}

void Config::SaveIniFile(const std::string& iniPath)
{
	CSimpleIniA ini;
	ini.SetUnicode();

	// [screenshots]
	ini.SetBoolValue("screenshots", "bEnable", screenshots.enableBinds);
	ini.SetLongValue("screenshots", "iBinds", screenshots.binds);

	// [overlay]
	ini.SetBoolValue("overlay", "bEnable", overlay.enable);
	ini.SetBoolValue("overlay", "bDrawFps", overlay.drawFps);
	ini.SetBoolValue("overlay", "bDrawScreenRes", overlay.drawScreenRes);
	ini.SetBoolValue("overlay", "bDrawCPUTemp", overlay.drawCPUTemp);
	ini.SetBoolValue("overlay", "bDrawGPUTemp", overlay.drawGPUTemp);
	ini.SetBoolValue("overlay", "bDrawFanSpeed", overlay.drawFanSpeed);
	ini.SetBoolValue("overlay", "bDrawRAMUsage", overlay.drawRAMUsage);
	ini.SetBoolValue("overlay", "bDrawFreeRAM", overlay.drawFreeRAM);
	ini.SetBoolValue("overlay", "bDrawLocalIp", overlay.drawLocalIp);

	ini.SetLongValue("overlay", "iRefreshDelay", overlay.refreshDelay);
	ini.SetLongValue("overlay", "iFpsPrecision", overlay.fpsPrecision);
	ini.SetLongValue("overlay", "iTempType", overlay.tempType);
	ini.SetLongValue("overlay", "iMemoryType", overlay.memoryType);
	ini.SetLongValue("overlay", "iDisplayMode", overlay.displayMode);
	ini.SetDoubleValue("overlay", "fSizeText", overlay.sizeText);
	ini.SetLongValue("overlay", "iHorizontalPos", overlay.horizontalPos);
	ini.SetLongValue("overlay", "iVerticalPos", overlay.verticalPos);

	// [menu]
	ini.SetLongValue("menu", "iAnimationType", menu.animationType);
	ini.SetDoubleValue("menu", "fPositionX", menu.position.x);
	ini.SetDoubleValue("menu", "fPositionY", menu.position.y);
	ini.SetDoubleValue("menu", "fSizeWidth", menu.sizeWidth);
	ini.SetDoubleValue("menu", "fSizeBorder", menu.sizeBorder);
	ini.SetDoubleValue("menu", "fSizeText", menu.sizeText);
	ini.SetDoubleValue("menu", "fColorLeftR", menu.colorLeft.r);
	ini.SetDoubleValue("menu", "fColorLeftG", menu.colorLeft.g);
	ini.SetDoubleValue("menu", "fColorLeftB", menu.colorLeft.b);
	ini.SetDoubleValue("menu", "fColorRightR", menu.colorRight.r);
	ini.SetDoubleValue("menu", "fColorRightG", menu.colorRight.g);
	ini.SetDoubleValue("menu", "fColorRightB", menu.colorRight.b);
	ini.SetDoubleValue("menu", "fBackgroundOpacity", menu.colorBackground.a);
	ini.SetBoolValue("menu", "bSounds", menu.useSound);
	ini.SetBoolValue("menu", "bDualColor", menu.useDualColor);
	ini.SetBoolValue("menu", "bShowControlsHelp", menu.showControlsHelp);

	ini.SaveFile(iniPath.c_str());
}