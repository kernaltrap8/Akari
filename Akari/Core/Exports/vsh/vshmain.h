
#pragma once

#include <stdint.h>

namespace vsh
{
	enum class CooperationMode
	{
		XMB,
		Game,
		VideoPlayer,
		Emulator
	};

	CooperationMode GetCooperationMode();
	void SetCooperationMode(CooperationMode mode);
	void ApplyCooperationMode();

	uint32_t GetGameProcessID();
}