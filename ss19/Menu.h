#pragma once
#include "gui.h"
#include "MyInput.h"

#define MENU_BG_COLOR 0x20202000
#define MENU_FG_COLOR C_RED

#define BTN_BG_COLOR C_dGRAY

class SrsColorPicker : public SrsWindow
{
	COLOR* pColor = nullptr;
	COLOR currColor;
public:
	SrsColorPicker( int x, int y, int w, int h, COLOR bgColor, COLOR fgColor );
	void BindProperty( COLOR* pColor );
	void ConfirmSelection();
};

class Menu
{
	SrsWindow* pWindow = nullptr;
	SrsColorPicker* pColorPicker = nullptr;

	bool bShow = true;
	bool bShowColorPicker = false;
public:
	static Menu* pMenu;
public:
	Menu();
	~Menu();
	void Toggle();
	bool isShown();
	void HandleInput();
	void Draw();
	void CreateControls();
	void ColorPickerOk();
	void ColorPickerClose();
	void PickEnemyColor();
};

