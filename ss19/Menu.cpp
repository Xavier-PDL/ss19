#include "Menu.h"
#include "Hack.h"
#include "Options.h"

Menu* Menu::pMenu = nullptr;

void PickEnemyColor()
{
	Menu::pMenu->PickEnemyColor();
}

void CloseColorPicker()
{
	Menu::pMenu->ColorPickerClose();
}

void OkColorPicker()
{
	Menu::pMenu->ColorPickerOk();
}

SrsColorPicker::SrsColorPicker( int x, int y, int w, int h, COLOR bgColor, COLOR fgColor )
	:
	SrsWindow( x, y, w, h, bgColor, fgColor )
{
}

void SrsColorPicker::BindProperty( COLOR * pColor )
{
	this->pColor = pColor;
}

void SrsColorPicker::ConfirmSelection()
{
	*pColor = currColor;
}

Menu::Menu()
{
	pMenu = this;
}


Menu::~Menu()
{
}

void Menu::Toggle()
{
	bShow = !bShow;
	if ( bShow )
		_pInput->DisableInput();
	else
		_pInput->EnableInput( _pGfx->gl_pvpActive );

}

void Menu::HandleInput()
{
	HWND hWnd = FindWindow( "SeriousSam", nullptr );
	RECT rect;
	GetWindowRect( hWnd, &rect );
	RECT cRect;
	GetClientRect( hWnd, &cRect );
	POINT p;
	GetCursorPos( &p );
	int x = p.x - rect.left - 1;
	int topHeight = (rect.bottom - rect.top) - cRect.bottom - 3;
	int y = p.y - rect.top - topHeight;
	auto state = GetAsyncKeyState( VK_LBUTTON );
	if ( state & 1 )
	{
		if ( !bShowColorPicker )
			pWindow->HandleMouseClick( x, y );
		else
			pColorPicker->HandleMouseClick( x, y );
	}
	else if ( state )
	{
		if ( !bShowColorPicker )
			pWindow->HandleMouseDrag( x, y );
		else
			pColorPicker->HandleMouseDrag( x, y );
	}
	else
	{
		if ( !bShowColorPicker )
			pWindow->ClearMouseEvent();
		else
			pColorPicker->ClearMouseEvent();
	}
}

void Menu::Draw()
{
	if ( !pWindow )
		CreateControls();
	if ( bShow )
	{
		if ( _pInput->inp_bInputEnabled )
			_pInput->DisableInput();
		pWindow->Render();
	}
	if ( bShowColorPicker )
	{
		pColorPicker->Render();
	}
}

void Menu::CreateControls()
{
#pragma region Menu

	pWindow = new SrsWindow( 50, 50, 300, 265, MENU_BG_COLOR, MENU_FG_COLOR );
	pWindow->SetText( "SeriousBidnezz v1.0" );
	
	int cby = 10;
	auto pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "God Mode" );
	pCB->BindProperty( &O::bGod );
	pWindow->AddChild( pCB );
	
	cby += 15;
	pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "Inf Ammo" );
	pCB->BindProperty( &O::bInfAmmo );
	pCB->SetClickEvent( ToggleInfAmmo );
	pWindow->AddChild( pCB );

	cby += 15;
	pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "Rapidfire" );
	pCB->BindProperty( &O::bRapidfire );
	pCB->SetClickEvent( ToggleRapidFire );
	pWindow->AddChild( pCB );

	cby += 15;
	pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "Noclip / Fly" );
	pCB->BindProperty( &O::bFly );
	pWindow->AddChild( pCB );

	cby += 15;
	pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "[Space] go up" );
	pCB->BindProperty( &O::bSuperJump );
	pWindow->AddChild( pCB );

	cby += 15;
	pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "Slow fall" );
	pCB->BindProperty( &O::bLowGravity);
	pCB->SetClickEvent( ToggleLowGravity );
	pWindow->AddChild( pCB );

	cby += 15;
	pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "Jump Walk" );
	pCB->BindProperty( &O::bJumpWalk );
	pCB->SetClickEvent( ToggleJumpWalk );
	pWindow->AddChild( pCB );

	cby += 15;
	pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "No Collision Dmg" );
	pCB->BindProperty( &O::bNoCollisionDmg );
	pCB->SetClickEvent( ToggleCollisionDmg );
	pWindow->AddChild( pCB );

	cby += 15;
	pCB = new SrsCheckbox( 150, cby, MENU_BG_COLOR, MENU_FG_COLOR, "Triggerbot" );
	pCB->BindProperty( &O::bTriggerbot );
	pWindow->AddChild( pCB );
	
	int by = cby + 15;
	auto pBtn = new SrsButton( 25, 215, 100, 25, BTN_BG_COLOR, MENU_FG_COLOR, "Fill HP/AP" );
	pBtn->SetClickEvent( FillHealthAndArmor );
	pWindow->AddChild( pBtn );

	by += 30;
	pBtn = new SrsButton( 160, 165, 100, 25, BTN_BG_COLOR, MENU_FG_COLOR, "Get All Weaps" );
	pBtn->SetClickEvent( GiveAllWeapons );
	pWindow->AddChild( pBtn );

	by += 30;
	pBtn = new SrsButton( 160, 215, 100, 25, BTN_BG_COLOR, MENU_FG_COLOR, "Fill Ammo" );
	pBtn->SetClickEvent( FillAmmo );
	pWindow->AddChild( pBtn );

#pragma region Aimbot
	auto pGB = new SrsGroupbox( 15, 5, 115, 50, MENU_BG_COLOR, MENU_FG_COLOR, "Aimbot" );
	pWindow->AddChild( pGB );

	pCB = new SrsCheckbox( 5, 5, MENU_BG_COLOR, MENU_FG_COLOR, "Enable" );
	pCB->BindProperty( &O::bAimbot );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 20, MENU_BG_COLOR, MENU_FG_COLOR, "VisCheck" );
	pCB->BindProperty( &O::bAimbotVischeck );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 35, MENU_BG_COLOR, MENU_FG_COLOR, "Use FOV" );
	pCB->BindProperty( &O::bFOV );
	pGB->AddChild( pCB );

#pragma endregion

#pragma region Visualz

	pGB = new SrsGroupbox( 15, 70, 115, 125, MENU_BG_COLOR, MENU_FG_COLOR, "Visualz" );
	pWindow->AddChild( pGB );

	pCB = new SrsCheckbox( 5, 5, MENU_BG_COLOR, MENU_FG_COLOR, "Enable ESP" );
	pCB->BindProperty( &O::bESP );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 20, MENU_BG_COLOR, MENU_FG_COLOR, "VisCheck" );
	pCB->BindProperty( &O::bESPVisCheck );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 35, MENU_BG_COLOR, MENU_FG_COLOR, "Box" );
	pCB->BindProperty( &O::bBoxESP );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 50, MENU_BG_COLOR, MENU_FG_COLOR, "Line" );
	pCB->BindProperty( &O::bLineESP );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 65, MENU_BG_COLOR, MENU_FG_COLOR, "Name" );
	pCB->BindProperty( &O::bNameESP );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 80, MENU_BG_COLOR, MENU_FG_COLOR, "Distance" );
	pCB->BindProperty( &O::bDistanceESP );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 95, MENU_BG_COLOR, MENU_FG_COLOR, "Item" );
	pCB->BindProperty( &O::bItemESP );
	pGB->AddChild( pCB );

	pCB = new SrsCheckbox( 5, 110, MENU_BG_COLOR, MENU_FG_COLOR, "Item Desc" );
	pCB->BindProperty( &O::bItemDescESP );
	pGB->AddChild( pCB );

	//auto pColor = new SrsColorBox( 100, 5, (COLOR*) &O::uColEnemyESP );
	//pGB->AddControl( pColor );

	//pBtn = new SrsButton( 100, 30, 95, 20, BTN_BG_COLOR, MENU_FG_COLOR, "Enemy Color" );
	//pBtn->SetClickEvent( ::PickEnemyColor );
	//pGB->AddControl( pBtn );

	//pColor = new SrsColorBox( 100, 55, (COLOR*) &O::uColItemESP );
	//pGB->AddControl( pColor );

	//pBtn = new SrsButton( 100, 80, 95, 20, BTN_BG_COLOR, MENU_FG_COLOR, "Item Color" );
	////pBtn->SetClickEvent();
	//pGB->AddControl( pBtn );
// Visualz
#pragma endregion 
// Menu
#pragma endregion 
/*
#pragma region ColorPicker
	pColorPicker = new SrsColorPicker( (Draw::pBaseDP->dp_Width / 2) - 150, 150, 260, 260, MENU_BG_COLOR, MENU_FG_COLOR );
	pColorPicker->SetText( "Color Picker" );

	pBtn = new SrsButton( 20, 200, 100, 25, BTN_BG_COLOR, MENU_FG_COLOR, "OK" );
	pBtn->SetClickEvent( OkColorPicker );
	pColorPicker->AddControl( pBtn );

	pBtn = new SrsButton( 140, 200, 100, 25, BTN_BG_COLOR, MENU_FG_COLOR, "Close" );
	pBtn->SetClickEvent( CloseColorPicker );
	pColorPicker->AddControl( pBtn );

	auto pSlider = new SrsSlider( 15, 15, 150, 15, MENU_BG_COLOR, MENU_FG_COLOR, SrsSlider::HORZ );
	pColorPicker->AddControl( pSlider );

	pSlider = new SrsSlider( 15, 45, 150, 15, MENU_BG_COLOR, MENU_FG_COLOR, SrsSlider::HORZ );
	pColorPicker->AddControl( pSlider );

	pSlider = new SrsSlider( 15, 75, 150, 15, MENU_BG_COLOR, MENU_FG_COLOR, SrsSlider::HORZ );
	pColorPicker->AddControl( pSlider );

#pragma endregion*/

}

void Menu::ColorPickerOk()
{
	pColorPicker->ConfirmSelection();
	bShowColorPicker = false;
}

void Menu::ColorPickerClose()
{
	bShowColorPicker = false;
}

void Menu::PickEnemyColor()
{
	pColorPicker->BindProperty( (COLOR*) &O::uColEnemyESP );
	bShowColorPicker = true;
}


