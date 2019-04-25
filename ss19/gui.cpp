#include "gui.h"

SrsControl::SrsControl( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor )
{
	this->posX = x;
	this->posY = y;
	this->width = w;
	this->height = h;
	this->bgColor = bgColor;
	this->foreColor = foreColor;
	this->pDraw = Draw::Get();
}

void SrsControl::SetText( const char * szText )
{
	this->sText = szText;
}

void SrsControl::UpdatePos()
{
	if ( pParent )
	{
		posX = pParent->posX + relX;
		posY = pParent->posY + relY;
	}
	if ( pNext )
		pNext->UpdatePos();
}

void SrsControl::AddControl( SrsControl * pControl )
{
	//if ( !pNext )
	//{
	//	pNext = pControl;
	//	pControl->pPrev = this;

	//	pControl->relX = pControl->posX;
	//	pControl->relY = pControl->posY;

	//	if ( pParent )
	//	{
	//		pControl->pParent = pParent;
	//		// position relative to parent
	//		pControl->posX = pParent->posX + pControl->posX;
	//		pControl->posY = pParent->posY + pControl->posY;
	//	}
	//	else
	//	{
	//		pControl->pParent = this;
	//		pChildren = pControl;
	//		pControl->posX = posX + pControl->posX;
	//		pControl->posY = posY + pControl->posY;
	//	}
	//}

	//if ( !pParent || pChildren )
	//{
	//	pControl->pParent = this;
	//	if ( pChildren && (!pParent || pChildren->pPrev) )
	//	{
	//		pChildren->AddControl( pControl );
	//	}
	//	else
	//	{
	//		pChildren = pControl;
	//		pChildren->pPrev = this;
	//		pControl->relX = pControl->posX;
	//		pControl->relY = pControl->posY;
	//		pControl->posX += posX;
	//		pControl->posY += posY;
	//	}
	//}
	//else if ( !pNext )
	//{
	//	pNext = pControl;
	//	pControl->pPrev = this;
	//	pControl->relX = pControl->posX;
	//	pControl->relY = pControl->posY;
	//	pControl->posX += pParent->posX;
	//	pControl->posY += pParent->posY;
	//}
	//else
	//{
	//	pNext->AddControl( pControl );
	//}

	if ( pNext )
		pNext->AddControl(pControl);
	else
	{
		pNext = pControl;
		pControl->pPrev = this;
		pControl->relX = pControl->posX;
		pControl->relY = pControl->posY;
		pControl->posX += pParent->posX;
		pControl->posY += pParent->posY;
	}
}

void SrsControl::AddChild( SrsControl * pControl )
{
	pControl->pParent = this;

	if ( !pChildren )
	{
		pChildren = pControl;
		//pNext = pControl;
		pControl->pPrev = this;
		pControl->relX = pControl->posX;
		pControl->relY = pControl->posY;
		pControl->posX += posX;
		pControl->posY += posY;
		return;
	}

	pChildren->AddControl( pControl );
}

void SrsControl::AddCheckbox( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor, const char* szText )
{
	pNext = new SrsCheckbox( posX + x, posY + y, w, h, bgColor, foreColor, szText );
}

void SrsControl::Render()
{
	Draw();
	if ( pChildren )
		pChildren->Render();
	if ( pNext )
		pNext->Render();
}

void SrsControl::Draw( int x, int y )
{
	this->posX = x;
	this->posY = y;
	Draw();
}

bool SrsControl::HandleMouseClick( int x, int y )
{
	if ( pChildren )
	{
		if ( pChildren->HandleMouseClick( x, y ) )
			return true;
	}

	if ( pNext )
	{
		if ( pNext->HandleMouseClick( x, y ) )
			return true;
		else
		{
			if ( x >= posX && x <= posX + width )
				if ( y >= posY && y <= posY + height )
				{
					MouseClickEvent( x, y );
					return true;
				}
		}
	}
	else
	{
		if ( x >= posX && x <= posX + width )
			if ( y >= posY && y <= posY + height )
			{
				MouseClickEvent( x, y );
				return true;
			}
	}
	return false;
}

bool SrsControl::HandleMouseDrag( int x, int y )
{
	if ( x >= posX && x <= posX + width )
		if ( y >= posY && y <= posY + height )
		{
			MouseDragEvent( x, y );
			return true;
		}
	return false;
}

void SrsControl::SetClickEvent( SrsBtnCallback pCallback )
{
	this->pBtnCallback = pCallback;
}

void SrsControl::SetClickEvent( SrsMouseCallback pCallback )
{
	this->pClickCallback = pCallback;
}

SrsWindow::SrsWindow( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor )
	:
	SrsControl( x, y, w, h, bgColor, foreColor )
{
}

void SrsWindow::AddControl( SrsControl * pControl )
{
	posY += WND_TB_HEIGHT;
	SrsControl::AddControl( pControl );
	posY -= WND_TB_HEIGHT;
}

void SrsWindow::AddChild( SrsControl * pControl )
{
	posY += WND_TB_HEIGHT;
	SrsControl::AddChild( pControl );
	posY -= WND_TB_HEIGHT;
}

void SrsWindow::Draw()
{
	posY -= WND_TB_HEIGHT;
	pDraw->Rect( posX, posY, width, height, bgColor, alpha );
	pDraw->Box( posX, posY, width, height, foreColor, 0xFF );
	pDraw->Box( posX, posY, width, WND_TB_HEIGHT, foreColor, 0xFF );
	pDraw->TextCXY( sText.c_str(), posX + (width / 2), posY + (WND_TB_HEIGHT / 2) + 1, foreColor, 0xFF );
	posY += WND_TB_HEIGHT;
}

void SrsWindow::Render()
{
	posY += WND_TB_HEIGHT;
	SrsControl::Render();
	posY -= WND_TB_HEIGHT;
}

bool bOnce = true;
int xOffset;// = x - posX;
int yOffset;// = y - posY;
void SrsWindow::MouseDragEvent( int x, int y )
{
	if ( bOnce )
	{
		xOffset = x - posX;
		yOffset = y - posY;
		bOnce = false;
	}
	posX = x - xOffset;
	posY = y - yOffset;
	if ( pNext )
	{
		posY += WND_TB_HEIGHT;
		pNext->UpdatePos();
		posY -= WND_TB_HEIGHT;
	}
}

void SrsWindow::ClearMouseEvent()
{
	xOffset = 0;
	yOffset = 0;
	bOnce = true;
}

SrsCheckbox::SrsCheckbox( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor, const char * szText )
	:
	SrsControl( x, y, w, h, bgColor, foreColor )
{
	this->sText = szText;
}

SrsCheckbox::SrsCheckbox( int x, int y, COLOR bgColor, COLOR foreColor, const char * szText )
	:
	SrsControl( x, y, 100, 10, bgColor, foreColor )
{
	this->sText = szText;
	CTString str;
	str.Create( szText );
	this->width = 11 + Draw::Get()->TextWidth( szText );
}

void SrsCheckbox::Draw()
{
	posX = pParent->posX + relX;
	posY = pParent->posY + relY;
	pDraw->Rect( posX, posY, CB_WIDTH, CB_HEIGHT, bgColor, 0xFF );
	pDraw->Box( posX, posY, CB_WIDTH, CB_HEIGHT, foreColor, 0xFF );
	pDraw->Text( sText.c_str(), posX + CB_WIDTH, posY + 1, foreColor, 0xFF );
	if ( (pBool && *pBool) || bChecked )
		pDraw->Rect( posX + 3, posY + 3, CB_WIDTH - 6, CB_HEIGHT - 6, foreColor, 0xFF );
}

void SrsCheckbox::MouseClickEvent( int x, int y )
{
	if ( !pBool )
		bChecked = !bChecked;
	else
		*pBool = !(*pBool);

	if ( pBtnCallback )
		pBtnCallback();
}

void SrsCheckbox::BindProperty( bool * pBool )
{
	this->pBool = pBool;
}

SrsButton::SrsButton( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor, const char * szText )
	:
	SrsControl( x, y, w, h, bgColor, foreColor )
{
	this->sText = szText;
}

void SrsButton::Draw()
{
	posX = pParent->posX + relX;
	posY = pParent->posY + relY;
	pDraw->Rect( posX, posY, width, height, bgColor, 0xFF );
	pDraw->Box( posX, posY, width, height, foreColor, 0xFF );
	pDraw->TextCXY( sText.c_str(), posX + (width / 2), posY + (height / 2), foreColor, 0xFF );
}

void SrsButton::MouseClickEvent( int x, int y )
{
	if ( pBtnCallback )
		pBtnCallback();
}

SrsGroupbox::SrsGroupbox( int x, int y, int w, int h, COLOR bgColor, COLOR foreColor, const char * szTitle )
	:
	SrsControl( x, y, w, h, bgColor, foreColor )
{
	this->sText = szTitle;
}

void SrsGroupbox::AddControl( SrsControl * pControl )
{
	//pControl->posY += 10;
	if ( pControl->pParent )
		SrsControl::AddControl( pControl );
	else
	{
		if ( pChildren )
		{
			pControl->pParent = this;
			pChildren->AddControl( pControl );
		}
		else
		{
			pChildren = pControl;
			SrsControl::AddControl( pControl );
		}
	}
}

void SrsGroupbox::AddChild( SrsControl * pControl )
{
	pControl->posY += 10;
	SrsControl::AddChild( pControl );
}

void SrsGroupbox::Render()
{
	SrsControl::Render();
}

void SrsGroupbox::Draw()
{
	posX = pParent->posX + relX;
	posY = pParent->posY + relY;
	pDraw->Text( sText.c_str(), posX, posY, foreColor, 0xFF );
	pDraw->Box( posX, posY + 10, width, height, foreColor, 0xFF );
}

SrsColorBox::SrsColorBox( int x, int y, COLOR* pColor )
	:
	SrsControl( x, y, 20, 20, 0, *pColor )
{
	this->pColor = pColor;
}

void SrsColorBox::Draw()
{
	posX = pParent->posX + relX;
	posY = pParent->posY + relY;
	pDraw->Rect( posX, posY, width, height, foreColor, 0xFF );
}

void SrsColorBox::BindProperty( COLOR * pColor )
{
	this->pColor = pColor;
}

SrsSlider* pCurrentSlider = nullptr;
void SrsSliderMouseHandler( int x, int y )
{
	//pCurrentSlider->MouseClick( x, y );
}

void SrsSliderBtnPlus()
{
	pCurrentSlider->Increase();
}

void SrsSliderBtnMinus()
{
	pCurrentSlider->Decrease();
}

SrsSlider::SrsSlider( int x, int y, int w, int h, COLOR bgColor, COLOR fgColor, SLDR_ORIENTATION soo )
	:
	SrsControl( x, y, w, h, bgColor, fgColor )
{
	this->soOrientation = soo;
	this->pClickCallback = SrsSliderMouseHandler;
}

void SrsSlider::Init()
{
	bReady = true;
	posX = pParent->posX + relX;
	posY = pParent->posY + relY;
	auto pBtn = new SrsButton( relX, relY, height, height, bgColor, foreColor, "<" );
	pChildren = pBtn;
	pBtn->SetClickEvent( SrsSliderBtnMinus );
	AddControl( pBtn );
	pBtn = new SrsButton( relX + width - height, relY, height, height, bgColor, foreColor, ">" );
	pBtn->SetClickEvent( SrsSliderBtnPlus );
	AddControl( pBtn );
}

void SrsSlider::SetBounds( int min, int max )
{
	this->min = min;
	this->max = max;
}

void SrsSlider::Draw()
{
	if ( !bReady )
		Init();
	posX = pParent->posX + relX;
	posY = pParent->posY + relY;
	if ( soOrientation == HORZ )
	{
		pDraw->Line( posX + height, posY + (height / 2), posX + width - height, posY + (height / 2), foreColor, 0xFF );
		//pDraw->Rect( posX, posY, height, height, foreColor, 0xFF );
		//pDraw->Rect( posX + width, posY, height, height, foreColor, 0xFF );
		auto offset = ((float) cursorPos / float( max - min )) * float( width - height * 2 - height / 2 );
		pDraw->Box( int( posX + height + offset ), posY, height / 2, height, foreColor, 0xFF );
	}
	else
	{
		// if i ever need em
	}
	posX = pParent->posX;
	posY = pParent->posY;
}

bool SrsSlider::HandleMouseClick( int x, int y )
{
	pCurrentSlider = this;
	posX += relX;
	posY += relY;
	return SrsControl::HandleMouseClick( x, y );
	posX -= relX;
	posY -= relY;
}

void SrsSlider::MouseClickEvent( int x, int y )
{
	if ( soOrientation == HORZ )
	{
		x += 2;
		auto dx = x - posX - height;
		auto d = float( dx ) / (width - height * 2 + height / 2);
		cursorPos = (int) (d * float( max - min ));

	}
	else
	{
		// ...
	}
}

void SrsSlider::Decrease()
{
	cursorPos -= 5;
	if ( cursorPos < min )
		cursorPos = min;
}

void SrsSlider::Increase()
{
	cursorPos += 5;
	if ( cursorPos > max )
		cursorPos = max;
}
