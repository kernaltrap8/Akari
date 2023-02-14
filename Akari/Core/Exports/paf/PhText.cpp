
#include "PhText.h"
#include <algorithm>
#include <vsh/paf.h>

namespace paf
{
	PhText::PhText(PhWidget* parent, void* PhAppear)
	{
		paf_7F0930C6(this, parent, PhAppear);
	}

	PhText::~PhText()
	{
		if (IsAttached())
			paf_738BAAC0(this);
	}

	PhText& PhText::SetText(const std::wstring& text)
	{
		CallVirtualMethod<void>(70, this, text, 0);
		return *this;
	}

	PhText& PhText::SetText(const std::string& text)
	{
		std::wstring ws(text.begin(), text.end());
		return SetText(ws);
	}

	PhText& PhText::SetTextStyle(int style, float value)
	{
		paf_165AD4A6(m_Data.sRender->PhSText, style, value);
		return *this;
	}

	PhText& PhText::SetTextHeight(float height)
	{
		ViewportScaleVertical(height);

		SetTextStyle(0x28, height); // line height
		//SetTextStyle(0x2D, 0); // line spacing
		return *this;
	}

	int PhText::GetLineCount()
	{
		std::wstring text = m_Data.sRender->PhSText->text;
		size_t occurrences = std::count(text.begin(), text.end(), '\n');
		return 1 + occurrences;
	}

	float PhText::GetLineHeight()
	{
		float textHeight = paf_20477524(this);
		RevertViewportScaleVertical(textHeight);
		return textHeight;
	}

	float PhText::GetTextHeight()
	{
		return static_cast<float>(GetLineCount()) * GetLineHeight();
	}

	float PhText::GetTextWidth()
	{
		// Idk, find it yourself
		return 0;
	}
}