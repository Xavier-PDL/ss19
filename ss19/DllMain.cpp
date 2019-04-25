#include "Hack.h"
#include "Console.h"

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch ( fdwReason )
	{
		case DLL_PROCESS_ATTACH:
		{
			if ( !Hack::Install( hinstDLL ) )
				return FALSE;
			break;
		}

		case DLL_PROCESS_DETACH:
			return TRUE;

		default:
			break;
	}
	return TRUE;
}