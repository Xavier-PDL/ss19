#pragma once
#include <sdk.h>

class Draw
{
public:
	static CDrawPort* pBaseDP;
private:
	static Draw* pInstance;
	CDrawPort* pTempDP = nullptr;
	CFontData* pFontConsole = nullptr;
	Draw();
public:
	~Draw();
	static Draw* Get();
	static void Destroy();
	BOOL PreDraw();
	void PostDraw();
	ULONG TextWidth( const char* szText );
	void Text( const char* szText, int x, int y, COLOR color, DWORD alpha );
	void TextC( const char* szText, int x, int y, COLOR color, DWORD alpha );
	void TextCXY( const char* szText, int x, int y, COLOR color, DWORD alpha );
	void TextR( const char* szText, int x, int y, COLOR color, DWORD alpha );
	void Rect( int x, int y, int width, int height, COLOR color, DWORD alpha );
	void Box( int x, int y, int w, int h, COLOR color, DWORD alpha );
	void Line( int x0, int y0, int x1, int y1, COLOR color, DWORD alpha );
};

