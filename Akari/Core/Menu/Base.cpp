
#include "Base.h"
#include "../Configuration.h"
#include "../Helpers.h"
#include "../Input.h"
#include "../Rendering.h"
#include "../Exports/paf/View.h"
#include "../../Utilities/System.h"
#include <vsh/stdc.h>

Menu g_Menu;

Menu::Menu(Function mainSubmenu)
	: m_MainMenu(mainSubmenu) {}

bool Menu::IsOpened()
{
	return m_OpacityGlobal > 0.0 || m_StateOpened;
}

bool Menu::IsMoving()
{
	return m_StateMoving;
}

void Menu::SetMovingState(bool state)
{
	m_StateMoving = state;
}

bool Menu::IsInitialized()
{
	return m_StateInitialized;
}

void Menu::Initialize()
{
	m_StateInitialized = true;
}

bool Menu::IsOpenPressed()
{
	if (g_Input.IsButtonBinds(Input::BUTTON_L1, Input::BUTTON_L3))
	{
		PlaySound("snd_system_ok");
		return true;
	}

	return false;
}

bool Menu::IsEnterPressed()
{
	if (g_Input.IsButtonDown(Input::BUTTON_CROSS, 400))
	{
		PlaySound("snd_system_ok");
		return true;
	}

	return false;
}

bool Menu::IsBackPressed()
{
	if (g_Input.IsButtonDown(Input::BUTTON_CIRCLE, 400))
	{
		PlaySound("snd_cancel");
		return true;
	}

	return false;
}

bool Menu::IsUpPressed()
{
	if (g_Input.IsButtonDown(Input::BUTTON_PAD_UP, 250))
	{
		PlaySound("snd_cursor");
		return true;
	}

	return false;
}

bool Menu::IsDownPressed()
{
	if (g_Input.IsButtonDown(Input::BUTTON_PAD_DOWN, 250))
	{
		PlaySound("snd_cursor");
		return true;
	}

	return false;
}

bool Menu::IsLeftPressed()
{
	return g_Input.IsButtonDown(Input::BUTTON_PAD_LEFT, m_SliderDelay);
}

bool Menu::IsRightPressed()
{
	return g_Input.IsButtonDown(Input::BUTTON_PAD_RIGHT, m_SliderDelay);
}

bool Menu::IsLeftOrRightPressed()
{
	return g_Input.IsButtonDown(Input::BUTTON_PAD_LEFT, m_SliderDelay) || g_Input.IsButtonDown(Input::BUTTON_PAD_RIGHT, m_SliderDelay);
}

bool Menu::IsHovered()
{
	return IsOpened() && (m_CurrentOption == m_PrintingOption);
}

bool Menu::IsPressed()
{
	if (IsHovered())
		return IsEnterPressed();

	return false;
}

void Menu::OnOpen()
{
	m_StateOpened = true;
	m_CurrentMenu = (m_SavedMenu == nullptr) ? m_MainMenu : m_SavedMenu;
	m_SubmenuLevel = (m_SavedMenu == nullptr) ? 0 : m_SavedSubmenuLevel;
	m_CurrentOption = (m_SavedMenu == nullptr) ? 1 : m_SavedCurrentOption;
	m_CurrentIndex = (m_SavedMenu == nullptr) ? 1 : m_SavedCurrentIndex;

	g_ChangeOvertime.Add(&m_OpacityBackground, g_Config.menu.colorBackground.a, 200, 0, false, g_Config.menu.animationType);
	g_ChangeOvertime.Add(&m_OpacityGlobal, 1, 200, 0, false, g_Config.menu.animationType);
	g_ChangeOvertime.Add(&m_OpacityHighlightBar, 0.4, 200, 200, false, g_Config.menu.animationType);
	g_ChangeOvertime.Add(&m_OpacityText, 1, 200, 200, false, g_Config.menu.animationType);
	g_ChangeOvertime.Add(&m_HighlightBarPosY, (m_CurrentOption - m_CurrentIndex) * g_Config.menu.sizeText, 250, 0, true, g_Config.menu.animationType);
}

void Menu::OnClose()
{
	m_StateOpened = false;
	m_StateMoving = false;
	m_SavedMenu = m_CurrentMenu;
	m_SavedSubmenuLevel = m_SubmenuLevel;
	m_SavedCurrentOption = m_CurrentOption;
	m_SavedCurrentIndex = m_CurrentIndex;

	g_ChangeOvertime.Add(&m_SizeBackground, 0, 200, 200, true, g_Config.menu.animationType);
	g_ChangeOvertime.Add(&m_OpacityBackground, 0, 200, 200, false, g_Config.menu.animationType);
	g_ChangeOvertime.Add(&m_OpacityGlobal, 0, 200, 200, false, g_Config.menu.animationType);
	g_ChangeOvertime.Add(&m_OpacityHighlightBar, 0, 200, 0, false, g_Config.menu.animationType);
	g_ChangeOvertime.Add(&m_OpacityText, 0, 200, 0, false, g_Config.menu.animationType);
}

void Menu::OnCancel()
{
	m_SubmenuLevel--;
	m_CurrentMenu = m_LastSubmenu[m_SubmenuLevel];
	m_CurrentOption = m_LastOption[m_SubmenuLevel];
	m_CurrentIndex = m_LastIndex[m_SubmenuLevel];

	g_ChangeOvertime.Add(&m_HighlightBarPosY, (m_CurrentOption - m_CurrentIndex) * g_Config.menu.sizeText, 250, 0, true, g_Config.menu.animationType);
}

void Menu::OnScrollUp()
{
	m_CurrentOption--;

	if (m_CurrentOption == m_CurrentIndex + 1 && m_CurrentIndex > 1)
		m_CurrentIndex--;

	if (m_CurrentOption < 1)
	{
		m_CurrentOption = m_TotalOptions;
		m_CurrentIndex = (m_TotalOptions > maxOptionsPerPage) ? m_CurrentOption - maxOptionsPerPage + 1 : 1;
	}

	g_ChangeOvertime.Add(&m_HighlightBarPosY, (m_CurrentOption - m_CurrentIndex) * g_Config.menu.sizeText, 250, 0, true, g_Config.menu.animationType);
}

void Menu::OnScrollDown()
{
	m_CurrentOption++;

	if (m_CurrentOption == m_CurrentIndex + maxOptionsPerPage - 2 && m_CurrentIndex < m_TotalOptions - maxOptionsPerPage + 1)
		m_CurrentIndex++;

	if (m_CurrentOption > m_TotalOptions)
	{
		m_CurrentOption = 1;
		m_CurrentIndex = 1;
	}

	g_ChangeOvertime.Add(&m_HighlightBarPosY, (m_CurrentOption - m_CurrentIndex) * g_Config.menu.sizeText, 250, 0, true, g_Config.menu.animationType);
}

void Menu::EnterSubmenu(Function submenu)
{
	if (submenu == nullptr)
		return;

	m_LastSubmenu[m_SubmenuLevel] = m_CurrentMenu;
	m_LastOption[m_SubmenuLevel] = m_CurrentOption;
	m_LastIndex[m_SubmenuLevel] = m_CurrentIndex;
	m_CurrentMenu = submenu;
	m_CurrentOption = 1;
	m_CurrentIndex = 1;
	m_SubmenuLevel++;

	g_ChangeOvertime.Add(&m_HighlightBarPosY, (m_CurrentOption - m_CurrentIndex) * g_Config.menu.sizeText, 200, 0, true, g_Config.menu.animationType);
}

void Menu::ChangeSubmenu(Function submenu)
{
	if (submenu != nullptr)
		m_SubmenuDelay = submenu;
}

void Menu::PlaySound(const char* sound)
{
	if (g_Config.menu.useSound && g_Helpers.system_plugin)
		g_Helpers.system_plugin->PlaySound(sound);
}

void Menu::UpdateButtons()
{
	if (m_StateMoving)
	{
		if (IsBackPressed() || IsEnterPressed())
			m_StateMoving = false;
	}
	else
	{
		if (!m_StateOpened && (m_OpacityGlobal == 0.0))
		{
			if (IsOpenPressed())
				OnOpen();
		}
		else if (m_StateOpened && (m_OpacityText == 1.0))
		{
			if (IsOpenPressed())
				OnClose();
			else if (IsBackPressed())
			{
				if (m_CurrentMenu == m_MainMenu)
					OnClose();
				else
					OnCancel();
			}
			else if (IsUpPressed())
				OnScrollUp();
			else if (IsDownPressed())
				OnScrollDown();
		}
	}
}

void Menu::UpdatePosition()
{
	if (m_StateMoving)
	{
		double stickX = g_Input.GetRightAnalogX();
		double stickY = -g_Input.GetRightAnalogY();

		if (stickX != 0.0)
		{
			float xAdvance = g_Config.menu.position.x + stickX * 5.0;
			float xSafeLimit = VIEWPORT_WIDTH / 2 - g_Config.menu.sizeWidth / 2 - g_Config.menu.sizeBorder;

			if (xAdvance > -xSafeLimit && xAdvance < xSafeLimit)
				g_Config.menu.position.x += stickX * 5.0;

			if (g_Config.menu.position.x > xSafeLimit)
				g_Config.menu.position.x = xSafeLimit;

			if (g_Config.menu.position.x < -xSafeLimit)
				g_Config.menu.position.x = -xSafeLimit;
		}

		if (stickY != 0.0)
		{
			float yAdvance = g_Config.menu.position.y + stickY * 5.0;
			float ySafeLimit = VIEWPORT_HEIGHT / 2 - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 4 - g_Config.menu.sizeText;

			if (yAdvance > -ySafeLimit && yAdvance < ySafeLimit)
				g_Config.menu.position.y += stickY * 5.0;

			if (g_Config.menu.position.y > ySafeLimit)
				g_Config.menu.position.y = ySafeLimit;

			if (g_Config.menu.position.y < -ySafeLimit)
				g_Config.menu.position.y = -ySafeLimit;
		}
	}
}

void Menu::UpdateBackgroundSize()
{
	if (m_StateOpened)
	{
		static bool done = false;
		static unsigned int lastTime = 0;


		int optionsCount = (m_TotalOptions > maxOptionsPerPage) ? maxOptionsPerPage : m_TotalOptions;
		if (!optionsCount)
			return;

		float size = g_Config.menu.sizeText * optionsCount + m_SizeBackgroundSpace * 2;
		unsigned int timeNow = Timers::GetTimeNow();

		if (done && (timeNow - lastTime >= 200))
			done = false;

		if ((m_SizeBackground != size) && !done)
		{
			done = g_ChangeOvertime.Add(&m_SizeBackground, size, 200, 0, false, g_Config.menu.animationType);
			lastTime = timeNow;
		}

	}
}

void Menu::UpdateHighlightBarPos()
{
	if (m_StateOpened)
	{
		static bool done = false;
		static unsigned int lastTime = 0;

		float pos = (m_CurrentOption - m_CurrentIndex) * g_Config.menu.sizeText;
		unsigned int timeNow = Timers::GetTimeNow();

		if (done && (timeNow - lastTime >= 200))
			done = false;

		if ((m_HighlightBarPosY != pos) && !done)
		{
			done = g_ChangeOvertime.Add(&m_HighlightBarPosY, pos, 200, 0, false, g_Config.menu.animationType);
			lastTime = timeNow;
		}
	}
}

void Menu::UpdateUI()
{
	DrawControlsHelp();

	if (m_OpacityGlobal > 0.0)
	{
		m_TotalOptions = m_PrintingOption;
		m_PrintingOption = 0;

		UpdatePosition();
		UpdateBackgroundSize();
		UpdateHighlightBarPos();

		if (m_OpacityGlobal == 1.0)
			m_OpacityBackground = g_Config.menu.colorBackground.a;

		DrawHeader();
		DrawFooter();
		DrawBody();
		DrawHighlightBar();

		if (m_CurrentMenu != nullptr)
			m_CurrentMenu();

		if (m_SubmenuDelay != nullptr)
		{
			EnterSubmenu(m_SubmenuDelay);
			m_SubmenuDelay = nullptr;
		}
	}

	DrawMovingHelp();
}

void Menu::OnUpdate()
{
	if (!m_StateInitialized)
		Initialize();

	g_ChangeOvertime.Update();

	if (!paf::View::Find("xmb_plugin"))
	{
		if (m_StateOpened)
			OnClose();
		return;
	}

	UpdateButtons();
	UpdateUI();
}

void Menu::DrawBody()
{
	g_Render.Rectangle(
		g_Config.menu.position,
		vec2(g_Config.menu.sizeWidth, m_SizeBackground),
		Render::Centered,
		Render::Centered,
		Color(g_Config.menu.colorBackground.r, g_Config.menu.colorBackground.g, g_Config.menu.colorBackground.b, m_OpacityBackground));

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x, g_Config.menu.position.y + m_SizeBackground / 2),
		vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
		Render::Centered,
		Render::Bottom,
		Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x, g_Config.menu.position.y - m_SizeBackground / 2),
		vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
		Render::Centered,
		Render::Top,
		Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));

	if (g_Config.menu.useDualColor)
	{
		g_Render.Rectangle(
			vec2(g_Config.menu.position.x, g_Config.menu.position.y + m_SizeBackground / 2),
			vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
			Render::Centered,
			Render::Bottom,
			Color(g_Config.menu.colorLeft.r, g_Config.menu.colorLeft.g, g_Config.menu.colorLeft.b, m_OpacityGlobal),
			"tex_optionmenu_game");

		g_Render.Rectangle(
			vec2(g_Config.menu.position.x, g_Config.menu.position.y - m_SizeBackground / 2),
			vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
			Render::Centered,
			Render::Top,
			Color(g_Config.menu.colorLeft.r, g_Config.menu.colorLeft.g, g_Config.menu.colorLeft.b, m_OpacityGlobal),
			"tex_optionmenu_game");
	}

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x - g_Config.menu.sizeWidth / 2, g_Config.menu.position.y),
		vec2(g_Config.menu.sizeBorder, m_SizeBackground),
		Render::Right,
		Render::Centered,
		g_Config.menu.useDualColor ? Color(g_Config.menu.colorLeft.r, g_Config.menu.colorLeft.g, g_Config.menu.colorLeft.b, m_OpacityGlobal)
		: Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2, g_Config.menu.position.y),
		vec2(g_Config.menu.sizeBorder, m_SizeBackground),
		Render::Left,
		Render::Centered,
		Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));
}

void Menu::DrawHeader()
{
	g_Render.Rectangle(
		vec2(g_Config.menu.position.x, g_Config.menu.position.y + m_SizeBackground / 2 + g_Config.menu.sizeBorder + g_Config.menu.sizeText / 2),
		vec2(g_Config.menu.sizeWidth, g_Config.menu.sizeText),
		Render::Centered,
		Render::Centered,
		Color(g_Config.menu.colorBackground.r, g_Config.menu.colorBackground.g, g_Config.menu.colorBackground.b, m_OpacityBackground));

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x, g_Config.menu.position.y + m_SizeBackground / 2 + g_Config.menu.sizeBorder + g_Config.menu.sizeText),
		vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
		Render::Centered,
		Render::Bottom,
		Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));

	if (g_Config.menu.useDualColor)
	{
		g_Render.Rectangle(
			vec2(g_Config.menu.position.x, g_Config.menu.position.y + m_SizeBackground / 2 + g_Config.menu.sizeBorder + g_Config.menu.sizeText),
			vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
			Render::Centered,
			Render::Bottom,
			Color(g_Config.menu.colorLeft.r, g_Config.menu.colorLeft.g, g_Config.menu.colorLeft.b, m_OpacityGlobal),
			"tex_optionmenu_game");
	}

	g_Render.Text(
		"Akari",
		vec2(g_Config.menu.position.x, g_Config.menu.position.y + m_SizeBackground / 2 + g_Config.menu.sizeBorder + g_Config.menu.sizeText / 2),
		g_Config.menu.sizeText,
		Render::Centered,
		Render::Centered,
		Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));
}

void Menu::DrawFooter()
{
	g_Render.Rectangle(
		vec2(g_Config.menu.position.x, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3 - g_Config.menu.sizeText / 2),
		vec2(g_Config.menu.sizeWidth, g_Config.menu.sizeText),
		Render::Centered,
		Render::Centered,
		Color(g_Config.menu.colorBackground.r, g_Config.menu.colorBackground.g, g_Config.menu.colorBackground.b, m_OpacityBackground));

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3),
		vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
		Render::Centered,
		Render::Bottom,
		Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3 - g_Config.menu.sizeText),
		vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
		Render::Centered,
		Render::Top,
		Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));
	
	if (g_Config.menu.useDualColor)
	{
		g_Render.Rectangle(
			vec2(g_Config.menu.position.x, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3),
			vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
			Render::Centered,
			Render::Bottom,
			Color(g_Config.menu.colorLeft.r, g_Config.menu.colorLeft.g, g_Config.menu.colorLeft.b, m_OpacityGlobal),
			"tex_optionmenu_game");

		g_Render.Rectangle(
			vec2(g_Config.menu.position.x, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3 - g_Config.menu.sizeText),
			vec2(g_Config.menu.sizeWidth + g_Config.menu.sizeBorder * 2, g_Config.menu.sizeBorder),
			Render::Centered,
			Render::Top,
			Color(g_Config.menu.colorLeft.r, g_Config.menu.colorLeft.g, g_Config.menu.colorLeft.b, m_OpacityGlobal),
			"tex_optionmenu_game");
	}

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x - g_Config.menu.sizeWidth / 2, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3 - g_Config.menu.sizeText / 2),
		vec2(g_Config.menu.sizeBorder, g_Config.menu.sizeText),
		Render::Right,
		Render::Centered,
		g_Config.menu.useDualColor ? Color(g_Config.menu.colorLeft.r, g_Config.menu.colorLeft.g, g_Config.menu.colorLeft.b, m_OpacityGlobal)
		: Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));

	g_Render.Rectangle(
		vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3 - g_Config.menu.sizeText / 2),
		vec2(g_Config.menu.sizeBorder, g_Config.menu.sizeText),
		Render::Left,
		Render::Centered,
		Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityGlobal));

	g_Render.Text(
		stdc::va("[%i/%i]", m_CurrentOption, m_TotalOptions),
		vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 4, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3 - g_Config.menu.sizeText / 2),
		g_Config.menu.sizeText,
		Render::Right,
		Render::Centered,
		Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));
}

void Menu::DrawHighlightBar()
{
	vec2 pos = vec2(g_Config.menu.position.x, g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - m_HighlightBarPosY - g_Config.menu.sizeText / 2);

	if (g_Config.menu.useDualColor)
	{
		g_Render.RectangleGradient(
			pos,
			vec2(g_Config.menu.sizeWidth, g_Config.menu.sizeText),
			Render::Centered,
			Render::Centered,
			Color(g_Config.menu.colorLeft.r, g_Config.menu.colorLeft.g, g_Config.menu.colorLeft.b, m_OpacityHighlightBar),
			Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityHighlightBar));
	}
	else
	{
		g_Render.Rectangle(
			pos,
			vec2(g_Config.menu.sizeWidth, g_Config.menu.sizeText),
			Render::Centered,
			Render::Centered,
			Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityHighlightBar));
	}
}

void Menu::DrawControlsHelp()
{
	if (!g_Config.menu.showControlsHelp)
		return;

	vec2 pos = vec2(-VIEWPORT_WIDTH / 2 + g_Config.menu.safeArea.x + 5, -VIEWPORT_HEIGHT / 2 + g_Config.menu.safeArea.y + 5);
	
	std::wstring controlsHelpString = m_StateOpened ? L"Press \uF888 & \uF88A to close the menu | \uF884/\uF885 to scroll | \uF881 to select | \uF880 to go back"
		: L"Press \uF888 & \uF88A to open Akari menu";

	g_Render.Text(
		controlsHelpString,
		pos,
		g_Config.menu.sizeText,
		Render::Left,
		Render::Bottom,
		g_Config.menu.colorText);
}

void Menu::DrawMovingHelp()
{
	if (!m_StateMoving)
		return;

	g_Render.Text(
		L"Use \uF88D to move the menu",
		vec2(),
		g_Config.menu.sizeText,
		Render::Centered,
		Render::Centered,
		g_Config.menu.colorText);

	g_Render.Text(
		stdc::va("X: %.2f\nY: %.2f", g_Config.menu.position.x, g_Config.menu.position.y),
		vec2(0, -g_Config.menu.sizeText * 2),
		g_Config.menu.sizeText,
		Render::Centered,
		Render::Centered,
		g_Config.menu.colorText);
}

void Menu::DrawMenuTitle(const std::string& text)
{
	g_Render.Text(
		text,
		vec2(g_Config.menu.position.x - g_Config.menu.sizeWidth / 2 + m_SizeBackgroundSpace / 4, g_Config.menu.position.y - m_SizeBackground / 2 - g_Config.menu.sizeBorder * 3 - g_Config.menu.sizeText / 2),
		g_Config.menu.sizeText,
		Render::Left,
		Render::Centered,
		Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));
}

void Menu::DrawMenuOption(const std::string& text)
{
	if ((m_PrintingOption >= m_CurrentIndex) && (m_PrintingOption < (m_CurrentIndex + maxOptionsPerPage)))
	{
		g_Render.Text(
			text,
			vec2(g_Config.menu.position.x - g_Config.menu.sizeWidth / 2 + m_SizeBackgroundSpace / 2, 
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			g_Config.menu.sizeText,
			Render::Left,
			Render::Centered,
			Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));
	}
}

void Menu::DrawMenuSubmenu()
{
	if ((m_PrintingOption >= m_CurrentIndex) && (m_PrintingOption < (m_CurrentIndex + maxOptionsPerPage)))
	{
		g_Render.Text(
			">",
			vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 2,
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			g_Config.menu.sizeText,
			Render::Right,
			Render::Centered,
			Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));
	}
}

void Menu::DrawMenuToggle(bool var)
{
	if ((m_PrintingOption >= m_CurrentIndex) && (m_PrintingOption < (m_CurrentIndex + maxOptionsPerPage)))
	{
		g_Render.Rectangle(
			vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 2,
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			vec2(g_Config.menu.sizeText * 0.7, g_Config.menu.sizeText * 0.7),
			Render::Right,
			Render::Centered,
			var ? Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityText)
			: Color(0.15, 0.15, 0.15, m_OpacityText));
	}
}

void Menu::DrawMenuSlider(const std::string& text, float progress)
{
	if ((m_PrintingOption >= m_CurrentIndex) && (m_PrintingOption < (m_CurrentIndex + maxOptionsPerPage)))
	{
		float sliderWidth = g_Config.menu.sizeWidth / 4;

		g_Render.Rectangle(
			vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 2 - sliderWidth,
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			vec2(sliderWidth, 2),
			Render::Left,
			Render::Centered,
			Color(0.15, 0.15, 0.15, m_OpacityText));

		g_Render.Rectangle(
			vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 2 - sliderWidth,
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			vec2((sliderWidth * progress), 2),
			Render::Left,
			Render::Centered,
			Color(g_Config.menu.colorRight.r, g_Config.menu.colorRight.g, g_Config.menu.colorRight.b, m_OpacityText));

		g_Render.Rectangle(
			vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 2 - sliderWidth + (sliderWidth * progress),
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			vec2(2, 8),
			Render::Centered,
			Render::Centered,
			Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));

		g_Render.Text(
			text,
			vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 2 - sliderWidth - 5,
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			g_Config.menu.sizeText,
			Render::Right,
			Render::Centered,
			Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));
	}
}

void Menu::DrawMenuStringOption(std::string item)
{
	if ((m_PrintingOption >= m_CurrentIndex) && (m_PrintingOption < (m_CurrentIndex + maxOptionsPerPage)))
	{
		g_Render.Text(
			item,
			vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 2,
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			g_Config.menu.sizeText,
			Render::Right,
			Render::Centered,
			Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));
	}
}

void Menu::DrawMenuStringOption(std::wstring item)
{
	if ((m_PrintingOption >= m_CurrentIndex) && (m_PrintingOption < (m_CurrentIndex + maxOptionsPerPage)))
	{
		g_Render.Text(
			item,
			vec2(g_Config.menu.position.x + g_Config.menu.sizeWidth / 2 - m_SizeBackgroundSpace / 2,
			g_Config.menu.position.y + m_SizeBackground / 2 - m_SizeBackgroundSpace - (m_PrintingOption - m_CurrentIndex) * g_Config.menu.sizeText - g_Config.menu.sizeText / 2),
			g_Config.menu.sizeText,
			Render::Right,
			Render::Centered,
			Color(g_Config.menu.colorText.r, g_Config.menu.colorText.g, g_Config.menu.colorText.b, m_OpacityText));
	}
}

void Menu::Title(const std::string& text)
{
	DrawMenuTitle(text);
}

Menu& Menu::Option(const std::string& text)
{
	m_PrintingOption++;
	DrawMenuOption(text);
	return *this;
}
Menu& Menu::Submenu(Function sub)
{
	DrawMenuSubmenu();
	if (IsPressed())
		ChangeSubmenu(sub);
	return *this;
}

Menu& Menu::Toggle(bool& var)
{
	DrawMenuToggle(var);
	if (IsPressed())
		var ^= 1;
	return *this;
}

Menu& Menu::Toggle(bool& var, Function onEnable, Function onDisable)
{
	DrawMenuToggle(var);
	if (IsPressed())
	{
		var ^= 1;
		if (var)
		{
			if (onEnable != nullptr)
				onEnable();
		}
		else
		{
			if (onDisable != nullptr)
				onDisable();
		}
	}
	return *this;
}

Menu& Menu::Action(Function fn)
{
	if (fn != nullptr)
		if (IsPressed())
			fn();
	return *this;
}

Menu& Menu::Hover(Function fn)
{
	if (fn != nullptr)
		if (IsHovered())
			fn();
	return *this;
}

Menu& Menu::Slider(float& var, float min, float max, float step, unsigned int delay, const std::string& text)
{
	processOptionItemControls<float>(var, min, max, step);
	DrawMenuSlider(stdc::va(text.c_str(), var), (var - min) / (max - min));

	if (IsHovered())
		m_SliderDelay = delay;

	return *this;
}

Menu& Menu::Slider(int& var, int min, int max, int step, unsigned int delay, const std::string& text)
{
	processOptionItemControls<int>(var, min, max, step);
	DrawMenuSlider(stdc::va(text.c_str(), var), static_cast<float>(var - min) / static_cast<float>(max - min));

	if (IsHovered())
		m_SliderDelay = delay;

	return *this;
}

Menu& Menu::Strings(const char** items, int& index, int count)
{
	processOptionItemControls<int>(index, 0, count - 1, 1);

	if (IsHovered())
	{
		std::string selectedItem = items[index];
		std::string item = ((index > 0) ? "< " : " ") + selectedItem + ((index < count - 1) ? " >" : " ");

		DrawMenuStringOption(item);
		m_SliderDelay = 200;
	}
	else
		DrawMenuStringOption(items[index]);

	return *this;
}

Menu& Menu::Strings(const wchar_t** items, int& index, int count)
{
	processOptionItemControls<int>(index, 0, count - 1, 1);

	if (IsHovered())
	{
		std::wstring selectedItem = items[index];
		std::wstring item = ((index > 0) ? L"< " : L" ") + selectedItem + ((index < count - 1) ? L" >" : L" ");

		DrawMenuStringOption(item);
		m_SliderDelay = 200;
	}
	else
		DrawMenuStringOption(items[index]);

	return *this;
}