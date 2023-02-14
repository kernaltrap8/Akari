
#pragma once

#include "Exports/paf/PhPlane.h"
#include "Exports/paf/PhText.h"
#undef vector
#include <vector>

constexpr int MAX_PLANES = 50;
constexpr int MAX_TEXTS = 30;
class Render
{
public:
	enum Align
	{
		Centered,
		Left = (1 << 0),
		Right = (1 << 1),
		Top = (1 << 4),
		Bottom = (1 << 5)
	};

public:
	void OnUpdate();
	void DestroyWidgets();

	void Rectangle(vec2 position, vec2 size, Align horizontalAlign, Align verticalAlign, Color color, const std::string& texture = "", float angle = 0);
	void RectangleGradient(vec2 position, vec2 size, Align horizontalAlign, Align verticalAlign, Color colorLeft, Color colorRight, float angle = 0);
	void Line(vec2 from, vec2 to, float thickness, Color color);

	void Text(const std::wstring& text, vec2 position, float height, Align horizontalAlign, Align verticalAlign, Color color, float angle = 0);
	void Text(const std::string& text, vec2 position, float height, Align horizontalAlign, Align verticalAlign, Color color, float angle = 0);

private:
	paf::PhPlane* CreatePlane(const std::string& widgetName);
	paf::PhText* CreateText(const std::string& widgetName);

	paf::PhPlane* GetPlane(int index);
	paf::PhText* GetText(int index);

	void CreateWidgets();

	void ClearPlanes();
	void ClearTexts();

private:
	std::vector<paf::PhPlane*> m_PlaneList{};
	int m_CurrentPlane = 0;

	std::vector<paf::PhText*> m_TextList{};
	int m_CurrentText = 0;
};

extern Render g_Render;