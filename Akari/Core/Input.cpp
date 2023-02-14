
#include "Input.h"
#include "Menu/Base.h"
#include "../Utilities/System/Timers.h"
#include <string.h>
#include <vsh/stdc.h>

Input g_Input;

CellPadData* Input::GetInputData()
{
	return &m_InputData;
}

void Input::SetInputData(CellPadData* data)
{
	std::memcpy(&m_InputData, data, sizeof(CellPadData));
}

void Input::ClearInputData()
{
	std::memset(&m_InputData, 0, sizeof(CellPadData));
	m_InputData.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X] =
		m_InputData.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y] =
		m_InputData.button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X] =
		m_InputData.button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y] = 128;
}

bool Input::IsButtonDown(Buttons button, unsigned int delay)
{
	unsigned int timeNow = Timers::GetTimeNow();
	if (timeNow - m_InputButtonLastTime[button] > delay)
	{
		unsigned int buttonData = ((m_InputData.button[CELL_PAD_BTN_OFFSET_DIGITAL1] << 8) | m_InputData.button[CELL_PAD_BTN_OFFSET_DIGITAL2]);
		if (buttonData & (1 << button))
		{
			m_InputButtonLastTime[button] = timeNow;
			return true;
		}
	}
	return false;
}

bool Input::IsButtonBinds(Buttons button1, Buttons button2)
{
	return IsButtonDown(button1) && IsButtonDown(button2);
}

double Input::GetAnalogInput(Analogs analog)
{
	int16_t input = m_InputData.button[4 + analog] - 128;
	if ((input > -m_InputAnalogDeadzone) && (input < m_InputAnalogDeadzone))
		return 0.0;

	return static_cast<double>(input) / 128.0;
}

double Input::GetRightAnalogX()
{
	return GetAnalogInput(ANALOG_RIGHT_X);
}

double Input::GetRightAnalogY()
{
	return GetAnalogInput(ANALOG_RIGHT_Y);
}

double Input::GetLeftAnalogX()
{
	return GetAnalogInput(ANALOG_LEFT_X);
}

double Input::GetLeftAnalogY()
{
	return GetAnalogInput(ANALOG_LEFT_Y);
}

void Input::SetAnalogDeadzone(short deadzone)
{
	m_InputAnalogDeadzone = deadzone;
}