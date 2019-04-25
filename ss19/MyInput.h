#pragma once
struct KeyState
{
	int iState;
	KeyState( int state ) { iState = state; }
	bool isUp() { return iState & 0xC0000000; }
};
class Input
{
	static int vKeys[256];
	static int bLeftMouse;
public:
	static void SetKeyState( int vKey, int bState );
	static KeyState GetKeyState( int vKey );
	static KeyState GetKeyState( char vKey );
	static void ClearStates();
};

