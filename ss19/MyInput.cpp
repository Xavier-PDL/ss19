#include "MyInput.h"
#include <memory.h>
int Input::vKeys[256]{ 0 };
int Input::bLeftMouse;

void Input::SetKeyState( int vKey, int bState )
{
	vKeys[vKey] = bState;
}

KeyState Input::GetKeyState( int vKey )
{
	return KeyState(vKeys[vKey]);
}

KeyState Input::GetKeyState( char vKey )
{
	return GetKeyState( int( vKey ) );
}

void Input::ClearStates()
{
	memset( vKeys, 0, sizeof( int ) * 256 );
}
