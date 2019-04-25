#include "EntityProperties.h"

CEntityProperty * CDLLEntityClass::PropertyForName( const CTString & strPropertyName )
{
	using fnPropertyForName = CEntityProperty * (__thiscall*)(void* pThis, const CTString& strPropertyName);
	static fnPropertyForName fpPropertyForName = nullptr;
	if ( !fpPropertyForName )
		fpPropertyForName = (fnPropertyForName) GetProcAddress( GetModuleHandle( "engine.dll" ), "?PropertyForName@CDLLEntityClass@@QAEPAVCEntityProperty@@ABVCTString@@@Z" );
	if ( fpPropertyForName )
		return fpPropertyForName( this, strPropertyName );
	return nullptr;
}

CEntityProperty * CDLLEntityClass::PropertyForTypeAndID( CEntityProperty::PropertyType eptType, ULONG ulID )
{
	using fnPropertyForTypeAndID = CEntityProperty * (__thiscall*)(void* pThis, CEntityProperty::PropertyType eptType, ULONG ulID);
	static fnPropertyForTypeAndID fpPropertyForTypeAndID = nullptr;
	if ( !fpPropertyForTypeAndID )
		fpPropertyForTypeAndID = (fnPropertyForTypeAndID) GetProcAddress( GetModuleHandle( "engine.dll" ), "?PropertyForTypeAndID@CDLLEntityClass@@QAEPAVCEntityProperty@@W4PropertyType@2@K@Z" );
	if ( fpPropertyForTypeAndID )
		return fpPropertyForTypeAndID( this, eptType, ulID );
	return nullptr;
}
