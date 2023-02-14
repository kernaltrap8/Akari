
#include "ConsoleInfo.h"
#include "SystemCalls.h"
#include "../Math.h"
#include <cell/cell_fs.h>
#include <vsh/stdc.h>

using namespace Syscall;

namespace ConsoleInfo
{
	float GetTemperatureCelsius(int dev_id)
	{
		uint32_t temperature = 0;
		sys_game_get_temperature(dev_id, &temperature);
		return static_cast<float>(temperature);
	}

	float GetTemperatureFahrenheit(int dev_id)
	{
		return (GetTemperatureCelsius(dev_id) * 1.8) + 32.0;
	}

	float GetTemperatureKelvin(int dev_id)
	{
		return GetTemperatureCelsius(dev_id) + 273.15;
	}

	float GetFanSpeed()
	{
		uint8_t st, mode, fan_speed, duty;
		sys_sm_get_fan_policy(0, &st, &mode, &fan_speed, &duty);
		return (static_cast<float>(fan_speed)* 100.0) / 255.0; // Fan speed is between 0x0 and 0xFF
	}

	memUsage_s GetMemoryUsage()
	{
		memInfo_s mem_info;
		{ system_call_1(352, (uint64_t)&mem_info); } // sys_memory_get_user_memory_size

		memUsage_s mem_usage;
		mem_usage.total = static_cast<float>(mem_info.total) / 1024.0;
		mem_usage.free = static_cast<float>(mem_info.available) / 1024.0;
		mem_usage.used = mem_usage.total - mem_usage.free;
		mem_usage.percent = (mem_usage.used * 100.0) / mem_usage.total;

		return mem_usage;
	}

	float GetFirmwareVersion()
	{
		float firmware = 0.0f;
		platform_info_s platform_info;

		memset(&platform_info, 0, sizeof(platform_info_s));
		sys_sm_get_system_info(&platform_info);

		// for exemple:
		// 4.84 : 04 08 40 00
		firmware = (float)(platform_info.firmware_version[0] & 0xF);
		firmware += (float)(platform_info.firmware_version[1] & 0xF) * 0.1f;
		firmware += (float)((platform_info.firmware_version[2] & 0xF0) >> 4) * 0.01f;

		return firmware;
	}

	std::string GetFirmwareType()
	{
		switch (sys_dbg_get_console_type())
		{
		case ConsoleType::CEX:
			return "CEX";
		case ConsoleType::DEX:
			return "DEX";
		case ConsoleType::DEH:
			return "DEH";
		default:
			return "[Unknown]";
		}
	}

	std::string GetCurrentIdps()
	{
		uint64_t idps[2];
		sys_mapi_get_idps(idps);

		return std::string(stdc::va("%016llX%016llX", idps[0], idps[1]));
	}

	std::string GetCurrentPsid()
	{
		uint64_t psid[2];
		sys_mapi_get_psid(psid);

		return std::string(stdc::va("%016llX%016llX", psid[0], psid[1]));
	}

	void RandomizeIdps()
	{
		idps_s idps;

		sys_mapi_get_idps((uint64_t*)&idps);

		for (int i = 0; i < 6; i++)
			idps.identifier[i] = (uint8_t)Math::irand(0x00, 0xFF);

		sys_mapi_set_idps((uint64_t*)&idps);
	}

	void RandomizePsid()
	{
		uint8_t psid[16];

		sys_mapi_get_psid((uint64_t*)psid);

		for (int i = 0; i < 16; i++)
			psid[i] = (uint8_t)Math::irand(0x00, 0xFF);

		sys_mapi_set_psid((uint64_t*)psid);
	}
}