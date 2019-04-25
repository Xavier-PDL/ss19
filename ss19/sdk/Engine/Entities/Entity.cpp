#include "Entity.h"
#include <Engine/Network/Network.h>
#include <Engine/Network/SessionState.h>
CEntity * CEntity::GetPlayerEntity( INDEX iPlayer )
{
	CSessionState &ses = _pLibrary->ga_sesSessionState;
	
	if ( ses.ses_apltPlayers.sa_Array[iPlayer].plt_bActive )
	{
		return (CEntity*)ses.ses_apltPlayers.sa_Array[iPlayer].plt_penPlayerEntity;
	}

	return nullptr;
}

BOOL IsOfClass( CEntity * pen, const char * pstrClassName )
{
	using fnIsOfClass = BOOL( __cdecl* )(CEntity* pEnt, const char* szClassName);
	static fnIsOfClass fpIsOfClass = nullptr;
	if ( !fpIsOfClass )
	{
		fpIsOfClass = (fnIsOfClass) GetProcAddress( GetModuleHandle( "engine.dll" ), "?IsOfClass@@YAHPAVCEntity@@PBD@Z" );
	}
	if ( fpIsOfClass )
		return fpIsOfClass( pen, pstrClassName );
	return FALSE;
}

BOOL IsOfSameClass( CEntity * pen1, CEntity * pen2 )
{
	using fnIsOfSameClass = BOOL( __cdecl* )(CEntity * pen1, CEntity * pen2);
	static fnIsOfSameClass fpIsOfSameClass = nullptr;
	if ( !fpIsOfSameClass )
	{
		fpIsOfSameClass = (fnIsOfSameClass) GetProcAddress( GetModuleHandle( "engine.dll" ), "?IsOfSameClass@@YAHPAVCEntity@@0@Z" );
	}
	if ( fpIsOfSameClass )
		return fpIsOfSameClass( pen1, pen2 );
	return FALSE;
}

BOOL IsDerivedFromClass( CEntity * pen, const char * pstrClassName )
{
	using fnIsDerivedOfClass = BOOL( __cdecl* )(CEntity* pEnt, const char* szClassName);
	static fnIsDerivedOfClass fpIsDerivedOfClass = nullptr;
	if ( !fpIsDerivedOfClass )
	{
		fpIsDerivedOfClass = (fnIsDerivedOfClass)GetProcAddress( GetModuleHandle( "engine.dll" ), "?IsDerivedFromClass@@YAHPAVCEntity@@PBD@Z" );
	}
	if ( fpIsDerivedOfClass )
		return fpIsDerivedOfClass( pen, pstrClassName );
	return FALSE;
}
