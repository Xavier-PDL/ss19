#include <Windows.h>
#include "CTString.h"

CTString::~CTString()
{
	using fnDtor = void( __thiscall* )(void* pThis);
	static fnDtor fpDtor = nullptr;
	if(!fpDtor)
		fpDtor = (fnDtor) GetProcAddress( GetModuleHandle( "engine.dll" ), "??1CTString@@QAE@XZ" );
	if ( fpDtor )
		fpDtor( this );

	
	//delete[] pString;
}

CTString * CTString::Create( const char * str )
{
	using fnCreate = CTString * (__thiscall*)(void* pThis, const char* pStr);
	static fnCreate fpCreate = nullptr;
	if ( !fpCreate )
		fpCreate = (fnCreate) GetProcAddress( GetModuleHandle( "engine.dll" ), "??0CTString@@QAE@PBD@Z" );
	if ( fpCreate )
		return fpCreate( this, str );
	return nullptr;
}
