
#pragma once

#include "../Utilities/Memory.h"
#include <cell/pad.h>

class Input
{
public:
	enum Buttons
	{
		BUTTON_L2,
		BUTTON_R2,
		BUTTON_L1,
		BUTTON_R1,
		BUTTON_TRIANGLE,
		BUTTON_CIRCLE,
		BUTTON_CROSS,
		BUTTON_SQUARE,
		BUTTON_SELECT,
		BUTTON_L3,
		BUTTON_R3,
		BUTTON_START,
		BUTTON_PAD_UP,
		BUTTON_PAD_RIGHT,
		BUTTON_PAD_DOWN,
		BUTTON_PAD_LEFT,
		BUTTON_PS
	};

	enum Analogs
	{
		ANALOG_RIGHT_X,
		ANALOG_RIGHT_Y,
		ANALOG_LEFT_X,
		ANALOG_LEFT_Y
	};

public:
	explicit Input() = default;
	~Input() noexcept = default;

	CellPadData* GetInputData();
	void SetInputData(CellPadData* data);
	void ClearInputData();

	bool IsButtonDown(Buttons button, unsigned int delay = 0);
	bool IsButtonBinds(Buttons button1, Buttons button2);

	double GetAnalogInput(Analogs analog);
	double GetRightAnalogX();
	double GetRightAnalogY();
	double GetLeftAnalogX();
	double GetLeftAnalogY();

	void SetAnalogDeadzone(short deadzone);

private:
	CellPadData m_InputData{};
	unsigned int m_InputButtonLastTime[17]{};
	short m_InputAnalogDeadzone = 20;
};

extern Input g_Input;