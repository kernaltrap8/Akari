
#include "vshmain.h"
#include <vsh/vshmain.h>

namespace vsh
{
	CooperationMode GetCooperationMode()
	{
		return (CooperationMode)vshmain_EB757101();
	}

	void SetCooperationMode(CooperationMode mode)
	{
		vshmain_45D85C54((uint32_t)mode);
	}

	void ApplyCooperationMode()
	{
		vshmain_5046CFAB();
	}

	uint32_t GetGameProcessID()
	{
		return vshmain_0624D3AE();
	}
};