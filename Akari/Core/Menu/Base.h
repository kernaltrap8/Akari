
#pragma once

#include <string>

using Function = void(*)();

class Menu
{
public:
	Menu() = default;
	Menu(Function mainSubmenu);

	bool IsOpened();
	bool IsMoving();
	void SetMovingState(bool state);

	void OnUpdate();

	void Title(const std::string& text);
	Menu& Option(const std::string& text);
	Menu& Submenu(Function sub);
	Menu& Toggle(bool& var);
	Menu& Toggle(bool& var, Function onEnable, Function onDisable);
	Menu& Action(Function fn);
	Menu& Hover(Function fn);
	Menu& Slider(float& var, float min, float max, float step, unsigned int delay = 100, const std::string& text = "%.2f");
	Menu& Slider(int& var, int min, int max, int step, unsigned int delay = 100, const std::string& text = "%i");
	Menu& Strings(const char** items, int& index, int count);
	Menu& Strings(const wchar_t** items, int& index, int count);

private:
	bool IsInitialized();
	void Initialize();

	bool IsOpenPressed();
	bool IsEnterPressed();
	bool IsBackPressed();
	bool IsUpPressed();
	bool IsDownPressed();
	bool IsLeftPressed();
	bool IsRightPressed();
	bool IsLeftOrRightPressed();
	bool IsHovered();
	bool IsPressed();

	void OnOpen();
	void OnClose();
	void OnCancel();
	void OnScrollUp();
	void OnScrollDown();

	void EnterSubmenu(Function submenu);
	void ChangeSubmenu(Function submenu);
	void PlaySound(const char* sound);

	void UpdateButtons();
	void UpdatePosition();
	void UpdateBackgroundSize();
	void UpdateHighlightBarPos();
	void UpdateUI();

	void DrawBody();
	void DrawHeader();
	void DrawFooter();
	void DrawHighlightBar();
	void DrawControlsHelp();
	void DrawMovingHelp();

	void DrawMenuTitle(const std::string& text);
	void DrawMenuOption(const std::string& text);
	void DrawMenuSubmenu();
	void DrawMenuToggle(bool var);
	void DrawMenuSlider(const std::string& text, float progress);
	void DrawMenuStringOption(std::string item);
	void DrawMenuStringOption(std::wstring item);

	template <typename T>
	void processOptionItemControls(T& var, T min, T max, T step)
	{
		if (IsHovered())
		{
			if (IsLeftPressed())
			{
				if (var <= min)
					var = min;
				else
					var -= step;
			}

			if (var < min)
				var = min;

			if (IsRightPressed())
			{
				if (var >= max)
					var = max;
				else
					var += step;
			}

			if (var > max)
				var = max;
		}
	}

public:
	bool m_StateMoving{};

private:
	bool m_StateInitialized{};
	bool m_StateOpened{};

	Function m_OnMain{};
	Function m_MainMenu{};
	Function m_CurrentMenu{};
	Function m_SavedMenu{};
	Function m_SubmenuDelay{};

	static const unsigned maxSubmenuLevels = 20;
	static const unsigned maxOptionsPerPage = 10;
	Function m_LastSubmenu[maxSubmenuLevels]{};
	int m_LastOption[maxSubmenuLevels]{};
	int m_LastIndex[maxSubmenuLevels]{};

	int m_SavedSubmenuLevel = 0;
	int m_SavedCurrentOption = 1;
	int m_SavedCurrentIndex = 1;

	int m_SubmenuLevel = 0;
	int m_PrintingOption = 0;
	int m_TotalOptions = 0;
	int m_CurrentOption = 0;
	int m_CurrentIndex = 0;

	float m_OpacityGlobal = 0;
	float m_OpacityBackground = 0;
	float m_OpacityHighlightBar = 0;
	float m_OpacityText = 0;

	float m_SizeBackground = 0;
	float m_SizeBackgroundSpace = 10;

	float m_HighlightBarPosY = 0;

	unsigned int m_SliderDelay = 200;
};

extern Menu g_Menu;