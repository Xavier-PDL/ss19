#include "DrawPort.h"

void CDrawPort::SetOrtho( void ) const
{
	using fnSetOrtho = void( __thiscall* )(void* pThis);
	static fnSetOrtho fpSetOrtho = nullptr;
	if ( !fpSetOrtho )
		fpSetOrtho = (fnSetOrtho) GetProcAddress( GetModuleHandle( "engine.dll" ), "?SetOrtho@CDrawPort@@QBEXXZ" );
	if ( fpSetOrtho )
		fpSetOrtho( (void*)this );
}

CDrawPort * CDrawPort::Clone( CDrawPort * pdpBase, BOOL bLeft )
{
	using fnClone = CDrawPort * (__thiscall*)(void* pThis, CDrawPort* pdpBase, BOOL bLeft);
	static fnClone fpClone = nullptr;
	if ( !fpClone )
		fpClone = (fnClone) GetProcAddress( GetModuleHandle( "engine.dll" ), "??0CDrawPort@@QAE@PAV0@H@Z" );
	if ( fpClone )
		return fpClone( this, pdpBase, bLeft );
	return nullptr;
}

void CDrawPort::SetFont( CFontData * pfd )
{
	using fnSetFont = void( __thiscall* )(void* pThis, CFontData* pFD);
	static fnSetFont fpSetFont = nullptr;
	if ( !fpSetFont )
		fpSetFont = (fnSetFont) GetProcAddress( GetModuleHandle( "engine.dll" ), "?SetFont@CDrawPort@@QAEXPAVCFontData@@@Z" );
	if ( fpSetFont )
		fpSetFont( this, pfd );
}

ULONG CDrawPort::GetTextWidth( const CTString & strText ) const
{
	using fnGetTextWidth = ULONG( __thiscall* )(void* pThis, const CTString& strText);
	static fnGetTextWidth fpGetTextWidth = nullptr;
	if ( !fpGetTextWidth )
		fpGetTextWidth = (fnGetTextWidth) GetProcAddress( GetModuleHandle( "engine.dll" ), "?GetTextWidth@CDrawPort@@QBEKABVCTString@@@Z" );

	if ( fpGetTextWidth )
		return fpGetTextWidth( (void*) this, strText );
	return 0;
}

void CDrawPort::PutText( const CTString & strText, PIX pixX0, PIX pixY0, const COLOR colBlend ) const
{
	using fnPutText = void( __thiscall* )(void* pThis, const CTString& strText, PIX pixX0, PIX pixY0, const COLOR colBlend);
	static fnPutText fpPutText = nullptr;
	if ( !fpPutText )
		fpPutText = (fnPutText) GetProcAddress( GetModuleHandle( "engine.dll" ), "?PutText@CDrawPort@@QBEXABVCTString@@JJK@Z" );

	if ( fpPutText )
		fpPutText( (void*)this, strText, pixX0, pixY0, colBlend );
}

void CDrawPort::PutTextC( const CTString & strText, PIX pixX0, PIX pixY0, const COLOR colBlend ) const
{
	using fnPutTextC = void( __thiscall* )(void* pThis, const CTString& strText, PIX pixX0, PIX pixY0, const COLOR colBlend);
	static fnPutTextC fpPutTextC = nullptr;
	if ( !fpPutTextC )
		fpPutTextC = (fnPutTextC) GetProcAddress( GetModuleHandle( "engine.dll" ), "?PutTextC@CDrawPort@@QBEXABVCTString@@JJK@Z" );

	if ( fpPutTextC )
		fpPutTextC( (void*) this, strText, pixX0, pixY0, colBlend );
}

void CDrawPort::PutTextCXY( const CTString & strText, PIX pixX0, PIX pixY0, const COLOR colBlend ) const
{
	using fnPutTextCXY = void( __thiscall* )(void* pThis, const CTString& strText, PIX pixX0, PIX pixY0, const COLOR colBlend);
	static fnPutTextCXY fpPutTextCXY = nullptr;
	if ( !fpPutTextCXY )
		fpPutTextCXY = (fnPutTextCXY) GetProcAddress( GetModuleHandle( "engine.dll" ), "?PutTextCXY@CDrawPort@@QBEXABVCTString@@JJK@Z" );

	if ( fpPutTextCXY )
		fpPutTextCXY( (void*) this, strText, pixX0, pixY0, colBlend );
}

void CDrawPort::PutTextR( const CTString & strText, PIX pixX0, PIX pixY0, const COLOR colBlend ) const
{
	using fnPutTextR = void( __thiscall* )(void* pThis, const CTString& strText, PIX pixX0, PIX pixY0, const COLOR colBlend);
	static fnPutTextR fpPutTextR = nullptr;
	if ( !fpPutTextR )
		fpPutTextR = (fnPutTextR) GetProcAddress( GetModuleHandle( "engine.dll" ), "?PutTextR@CDrawPort@@QBEXABVCTString@@JJK@Z" );

	if ( fpPutTextR )
		fpPutTextR( (void*) this, strText, pixX0, pixY0, colBlend );
}

BOOL CDrawPort::Lock( void )
{
	using fnLock = BOOL( __thiscall* )(void* pThis);
	static fnLock fpLock = nullptr;
	
	if ( !fpLock )
		fpLock = (fnLock) GetProcAddress( GetModuleHandle( "engine.dll" ), "?Lock@CDrawPort@@QAEHXZ" );

	if ( fpLock )
		return fpLock( this );
	
	return 0;
}

void CDrawPort::Unlock( void )
{
	using fnUnlock = void( __thiscall* )(void* pThis);
	static fnUnlock fpLock = nullptr;

	if ( !fpLock )
		fpLock = (fnUnlock) GetProcAddress( GetModuleHandle( "engine.dll" ), "?Lock@CDrawPort@@QAEHXZ" );

	if ( fpLock )
		fpLock( this );
}

void CDrawPort::DrawLine( PIX pixI0, PIX pixJ0, PIX pixI1, PIX pixJ1, COLOR col, ULONG typ ) const
{
	using fnDrawLine = void( __thiscall* )(void* pThis, PIX pixI0, PIX pixJ0, PIX pixI1, PIX pixJ1, COLOR col, ULONG typ);
	static fnDrawLine fpDrawLine = nullptr;

	if ( !fpDrawLine )
		fpDrawLine = (fnDrawLine) GetProcAddress( GetModuleHandle( "engine.dll" ), "?DrawLine@CDrawPort@@QBEXJJJJKK@Z" );

	if ( fpDrawLine )
		fpDrawLine( (void*) this, pixI0, pixJ0, pixI1, pixJ1, col, typ );
}

void CDrawPort::DrawBorder( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR col, ULONG typ ) const
{
	using fnDrawBorder = void( __thiscall* )(void* pThis, PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR col, ULONG typ);
	static fnDrawBorder fpDrawBorder = nullptr;

	if ( !fpDrawBorder )
		fpDrawBorder = (fnDrawBorder) GetProcAddress( GetModuleHandle( "engine.dll" ), "?DrawBorder@CDrawPort@@QBEXJJJJKK@Z" );

	if ( fpDrawBorder )
		fpDrawBorder( (void*) this, pixI, pixJ, pixWidth, pixHeight, col, typ );
}

void CDrawPort::Fill( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR col ) const
{
	using fnFill = void( __thiscall* )(void* pThis, PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR col);
	static fnFill fpFill = nullptr;

	if ( !fpFill )
		fpFill = (fnFill) GetProcAddress( GetModuleHandle( "engine.dll" ), "?Fill@CDrawPort@@QBEXJJJJK@Z" );

	if ( fpFill )
		fpFill( (void*) this, pixI, pixJ, pixWidth, pixHeight, col );
}

void CDrawPort::Fill( PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR colUL, COLOR colUR, COLOR colDL, COLOR colDR ) const
{
	using fnFillBC = void( __thiscall* )(void* pThis, PIX pixI, PIX pixJ, PIX pixWidth, PIX pixHeight, COLOR colUL, COLOR colUR, COLOR colDL, COLOR colDR);
	static fnFillBC fpFill = nullptr;

	if ( !fpFill )
		fpFill = (fnFillBC) GetProcAddress( GetModuleHandle( "engine.dll" ), "?Fill@CDrawPort@@QBEXJJJJKKKK@Z" );

	if ( fpFill )
		fpFill( (void*) this, pixI, pixJ, pixWidth, pixHeight, colUL, colUR, colDL, colDR );
}

