#include "Input.h"
#include "../../../Draw.h"

CInput * _pInput = nullptr;

void CInput::EnableInput( CViewPort * pvp )
{
	using fnEnableInput = void( __thiscall* )(void* pThis, CViewPort* pVP);
	static fnEnableInput fpEnableInput = nullptr;
	if ( !fpEnableInput )
		fpEnableInput = (fnEnableInput) GetProcAddress( GetModuleHandle( "engine.dll" ), "?EnableInput@CInput@@QAEXPAVCViewPort@@@Z" );
	if ( fpEnableInput )
	{
		fpEnableInput( this, pvp );
	}
}

void CInput::DisableInput( void )
{
	//?DisableInput@CInput@@QAEXXZ
	using fnDisableInput = void( __thiscall* )(void* pThis);
	static fnDisableInput fpDisableInput = nullptr;
	if ( !fpDisableInput )
		fpDisableInput = (fnDisableInput) GetProcAddress( GetModuleHandle( "engine.dll" ), "?DisableInput@CInput@@QAEXXZ" );
	if ( fpDisableInput )
		fpDisableInput( this );
}