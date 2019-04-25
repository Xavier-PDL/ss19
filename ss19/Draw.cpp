#include "Draw.h"
#include "TMem.h"

Draw* Draw::pInstance = nullptr;
CDrawPort* Draw::pBaseDP = nullptr;
TMem* pMem = nullptr;

CTString* pStr = nullptr;
void TextDrawSetup( const char* szText )
{
	if ( pStr )
	{
		delete pStr;
	}

	pStr = new CTString( szText );
}

Draw::Draw()
{
}


Draw::~Draw()
{
}

Draw * Draw::Get()
{
	if ( !pInstance )
	{
		pMem = TMem::Get();
		pBaseDP = **(CDrawPort***) ((BYTE*) pMem->ScanPattern( "A1 ? ? ? ? 8B 48 14 89 4D FC" ) + 1);
		pInstance = new Draw();
	}
	return pInstance;
}

void Draw::Destroy()
{
	delete pInstance;
}

CDrawPort dpLocal;
BOOL Draw::PreDraw()
{
	if ( !pFontConsole )
	{
		auto pResult = (BYTE*) pMem->ScanPattern( GetModuleHandle( "engine.dll" ), "53 89 1D ? ? ? ? 89 1D ? ? ? ? FF 15" );
		pFontConsole = **(CFontData***) (pResult + 9);

		if ( !pFontConsole )
			return FALSE;
		pTempDP = dpLocal.Clone( pBaseDP, TRUE );
		pTempDP->SetFont( pFontConsole );
	}
	return pTempDP->Lock();
}

void Draw::PostDraw()
{
	pTempDP->Unlock();
}

ULONG Draw::TextWidth( const char * szText )
{
	TextDrawSetup( szText );
	return pTempDP->GetTextWidth( *pStr );
}

void Draw::Text( const char * szText, int x, int y, COLOR color, DWORD alpha )
{
	TextDrawSetup( szText );
	pTempDP->PutText( *pStr, x, y, color | alpha );
}

void Draw::TextC( const char * szText, int x, int y, COLOR color, DWORD alpha )
{
	TextDrawSetup( szText );
	pTempDP->PutTextC( *pStr, x, y, color | alpha );
}

void Draw::TextCXY( const char * szText, int x, int y, COLOR color, DWORD alpha )
{
	TextDrawSetup( szText );
	pTempDP->PutTextCXY( *pStr, x, y, color | alpha );
}

void Draw::TextR( const char * szText, int x, int y, COLOR color, DWORD alpha )
{
	TextDrawSetup( szText );
	pTempDP->PutTextR( *pStr, x, y, color | alpha );
}

void Draw::Rect( int x, int y, int width, int height, COLOR color, DWORD alpha )
{
	if ( x < 0 )
		width += x;
	if ( y < 0 )
		height += y;
	pTempDP->Fill( x, y, width, height, color | alpha );
}

void Draw::Box( int x, int y, int w, int h, COLOR color, DWORD alpha )
{
	pTempDP->DrawBorder( x, y, w, h, color | alpha );
}

void Draw::Line( int x0, int y0, int x1, int y1, COLOR color, DWORD alpha )
{
	pTempDP->DrawLine( x0, y0, x1, y1, color | alpha );
}
