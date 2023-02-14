
#pragma once

namespace paf
{
	class PhWidget;

	class PhHandler
	{
	public:
		enum Handler
		{
			PositionHandler = 0x1000001,
			ColorHandler = 0x1000002,
			SizeHandler = 0x1000004,
			ScaleHandler = 0x1000005,
			RotationHandler = 0x1000006
		};

		int DoCallBack(int callbackId, void* PhEvent);
		int HandlerProc(void* PhEvent);
		int KillTimerCB(int callbackId);
		int SetCallBack(int callbackId, void(*callback)(paf::PhWidget* widget, void* PhEvent, void* unknown), void* unknown);
		int SetDeleteCB(void(*callback)(paf::PhWidget *, void* PhEvent, void* unknown), void* unknown);

		int HandleFocusEvent(void* PhEvent);
		int HandlePointEvent(void* PhEvent);
		int HandleStateEvent(void* PhEvent);
		int HandleSystemEvent(void* PhEvent);
		int HandleCommandEvent(void* PhEvent);
		int HandleKeycodeEvent(void* PhEvent);

		bool IsTimerCB(int callbackId);
	};
};