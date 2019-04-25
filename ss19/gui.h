#pragma once
#include <string>
//#include <sdk.h>
#include "Draw.h"
#define WND_TB_HEIGHT 15	// Window top border height
#define CB_WIDTH 10			// checkbox width
#define CB_HEIGHT 10		// and height

class SrsWindow;
class SrsCheckbox;

using SrsBtnCallback = void(*)();
using SrsMouseCallback = void(*)(int x, int y);
class SrsControl
{
public:
	int posX = 0;
	int posY = 0;
	int relX = 0;
	int relY = 0;
	int width = 0;
	int height = 0;
	COLOR bgColor = 0;
	COLOR foreColor = 0;
	DWORD alpha = 0xFF;
	SrsControl* pParent = nullptr;
	SrsControl* pNext = nullptr;
	SrsControl* pPrev = nullptr;
	SrsControl* pChildren = nullptr;
	std::string sText;
	SrsBtnCallback pBtnCallback = nullptr;
	SrsMouseCallback pClickCallback = nullptr;
	Draw* pDraw = nullptr;
public:
	SrsControl(int x, int y, int w, int h, COLOR bgColor, COLOR foreColor);
	~SrsControl() = default;

	virtual void SetText( const char* szText );
	virtual void UpdatePos();

	// x and y is relative to x and y of parent control
	virtual void AddControl( SrsControl* pControl );
	virtual void AddChild( SrsControl* pControl );
	virtual void AddCheckbox( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor, const char* szText );
	
	virtual void Render();
private:
	virtual void Draw() {};
	virtual void Draw(int x, int y);
public:
	virtual bool HandleMouseClick( int x, int y );
	virtual void MouseClickEvent(int x, int y) {}
	virtual bool HandleMouseDrag( int x, int y );
	virtual void MouseDragEvent( int x, int y ) {}
	virtual void SetClickEvent( SrsBtnCallback pCallback );
	virtual void SetClickEvent( SrsMouseCallback pCallback );
	//virtual BOOL HandleMsg( UINT uMsg, WPARAM wParam, LPARAM lParam );
};

class SrsWindow : public SrsControl
{
public:
	SrsWindow( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor );
	~SrsWindow() = default;
	virtual void AddControl( SrsControl* pControl );
	virtual void AddChild( SrsControl* pControl );
	virtual void Draw();
	virtual void Render();
	virtual void MouseDragEvent( int x, int y );
	void ClearMouseEvent();
};

class SrsCheckbox : public SrsControl
{
	bool bChecked = false;
	bool* pBool = nullptr;
public:
	SrsCheckbox( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor, const char* szText );
	SrsCheckbox( int x, int y, COLOR bgColor, COLOR foreColor, const char* szText );
	~SrsCheckbox() = default;

	virtual void Draw();
	virtual void MouseClickEvent(int x, int y);
	
	void BindProperty( bool* pBool );
};

class SrsButton : public SrsControl
{
public:
	SrsButton( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor, const char* szText );
	virtual void Draw();
	virtual void MouseClickEvent(int x, int y);
};

class SrsGroupbox : public SrsControl
{
public:
	SrsGroupbox( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor, const char* szTitle );
	virtual void AddControl( SrsControl* pControl );
	virtual void AddChild( SrsControl* pControl );
	virtual void Render();
	virtual void Draw();
};

class SrsColorBox : public SrsControl
{
	COLOR* pColor = nullptr;
public:
	SrsColorBox(int x, int y, COLOR* color);
	virtual void Draw();
	void BindProperty( COLOR* pColor );
};

class SrsSlider : public SrsControl
{
public:
	enum SLDR_ORIENTATION
	{
		VERT,
		HORZ,
	};
private:
	SLDR_ORIENTATION soOrientation = HORZ;
	int min = 0;
	int max = 255;
	int cursorPos = 0;
	bool bReady = false;
public:
	SrsSlider( int x, int y, int w, int h, COLOR bgColor, COLOR fgColor, SLDR_ORIENTATION soo );
	void Init();
	void SetBounds( int min, int max );
	
	virtual void Draw();
	virtual bool HandleMouseClick( int x, int y );
	void MouseClickEvent( int x, int y );
	void Decrease();
	void Increase();
};