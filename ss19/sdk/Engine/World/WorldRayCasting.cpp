#include "WorldRayCasting.h"
#include <Engine/Entities/Entity.h>

void CCastRay::TestModel( CEntity * penModel )
{
	using fnTestModel = void(__thiscall*)(void* pThis, CEntity *);
	static fnTestModel fpTestModel = nullptr;
	if ( !fpTestModel )
		fpTestModel = (fnTestModel) GetProcAddress( GetModuleHandle( "engine.dll" ), "?TestModel@CCastRay@@QAEXPAVCEntity@@@Z" );
	if ( fpTestModel )
		fpTestModel( this, penModel );
}

CCastRay::CCastRay( CEntity * penOrigin, const CPlacement3D & plOrigin )
{
	Create( penOrigin, plOrigin );
}

CCastRay * CCastRay::Create( CEntity * penOrigin )
{
	return Create(penOrigin, penOrigin->GetPlacement());
}

CCastRay * CCastRay::Create( CEntity * penOrigin, const CPlacement3D & plOrigin )
{
	using fnCreate = CCastRay * (__thiscall*)(void* pThis, CEntity *, const CPlacement3D &);
	static fnCreate fpCreate = nullptr;
	if ( !fpCreate )
		fpCreate = (fnCreate) GetProcAddress( GetModuleHandle( "engine.dll" ), "??0CCastRay@@QAE@PAVCEntity@@ABVCPlacement3D@@@Z" );
	if ( fpCreate )
		return fpCreate( this, penOrigin, plOrigin );
	return nullptr;
}

void CCastRay::Cast( CWorld * pwoWorld )
{
	using fnCast = CCastRay * (__thiscall*)(void* pThis, CWorld*);
	static fnCast fpCast = nullptr;
	if ( !fpCast )
		fpCast = (fnCast) GetProcAddress( GetModuleHandle( "engine.dll" ), "?Cast@CCastRay@@QAEXPAVCWorld@@@Z" );
	if ( fpCast )
		fpCast( this, pwoWorld );
	
}
