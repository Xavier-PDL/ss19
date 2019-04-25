#include "Player.h"

BOOL CPlayer::ReceiveItem( const CEntityEvent & ee )
{
	using fnReceiveItem = BOOL(__thiscall*)( CPlayer* pThis, const CEntityEvent & ee );
	auto fpReceiveItem = (fnReceiveItem) GetProcAddress( GetModuleHandle( "Entities.dll" ), "?ReceiveItem@CPlayer@@UAEHABVCEntityEvent@@@Z" );
	if(fpReceiveItem)
		return fpReceiveItem( this, ee );
	return FALSE;
}
