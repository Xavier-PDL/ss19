#include "Console.h"
#include <stdio.h>

FILE * i = nullptr;
FILE * o = nullptr;
FILE * e = nullptr;

Console::Console()
{
	AllocConsole();
	AttachConsole(ATTACH_PARENT_PROCESS);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


	freopen_s(&i, "CONIN$", "r", stdin);
	freopen_s(&o, "CONOUT$", "w", stdout);
	freopen_s(&e, "CONOUT$", "w", stderr);

}


Console::~Console()
{
	freopen_s(&i, "IN", "r", stdin);
	freopen_s(&o, "OUT", "w", stdout);
	freopen_s(&e, "OUT", "w", stderr);
	FreeConsole();
}

void Console::print(const char * sString)
{
	printf(sString);
}

void Console::printf(const char * sFormat, ...)
{
	char buf[0x200]{ 0 };
	va_list args;
	va_start(args, sFormat);
	vsprintf_s(buf, sizeof(buf), sFormat, args);
	va_end(args);
	::printf(buf);
}


