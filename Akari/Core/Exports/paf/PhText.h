
#pragma once

#include "PhWidget.h"

namespace paf
{
	class PhText : public PhWidget
	{
	public:
		PhText(){}
		PhText(PhWidget* parent, void* PhAppear = nullptr);
		~PhText();

		PhText& SetText(const std::wstring& text);
		PhText& SetText(const std::string& text);

		PhText& SetTextStyle(int style, float value);
		PhText& SetTextHeight(float height);

		int GetLineCount();
		float GetLineHeight();
		float GetTextHeight();
		float GetTextWidth();

	private:
		std::uint8_t __padding_0x290[0x14];
	};
}