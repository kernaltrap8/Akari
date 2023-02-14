
#include "PhHandler.h"
#include <vsh/paf.h>

namespace paf
{
	int PhHandler::DoCallBack(int callbackId, void* PhEvent)
	{
		return paf_FE5A7950(this, callbackId, PhEvent);
	}

	int PhHandler::HandlerProc(void* PhEvent)
	{
		return paf_B60C4316(this, PhEvent);
	}

	int PhHandler::KillTimerCB(int callbackId)
	{
		return paf_2CBA5A33(this, callbackId);
	}

	int PhHandler::SetCallBack(int callbackId, void(*callback)(paf::PhWidget* widget, void* PhEvent, void* unknown), void* unknown)
	{
		return paf_986427A7(this, callbackId, (void(*)(void*, void*, void*))callback, unknown);
	}

	int PhHandler::SetDeleteCB(void(*callback)(paf::PhWidget *, void* PhEvent, void* unknown), void* unknown)
	{
		return paf_818162F2(this, (void(*)(void*, void*, void*))callback, unknown);
	}

	int PhHandler::HandleFocusEvent(void* PhEvent)
	{
		return paf_F7D65DC1(this, PhEvent);
	}

	int PhHandler::HandlePointEvent(void* PhEvent)
	{
		return paf_DBFE46EA(this, PhEvent);
	}

	int PhHandler::HandleStateEvent(void* PhEvent)
	{
		return paf_C051D361(this, PhEvent);
	}

	int PhHandler::HandleSystemEvent(void* PhEvent)
	{
		return paf_C96D94F(this, PhEvent);
	}

	int PhHandler::HandleCommandEvent(void* PhEvent)
	{
		return paf_CE14748C(this, PhEvent);
	}

	int PhHandler::HandleKeycodeEvent(void* PhEvent)
	{
		return paf_A4D0FDCB(this, PhEvent);
	}

	bool PhHandler::IsTimerCB(int callbackId)
	{
		return paf_4D0CAA13(this, callbackId);
	}
};