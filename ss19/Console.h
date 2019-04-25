#pragma once
#include <Windows.h>
class Console
{
public:
	Console();
	~Console();
	void print(const char* sString);
	void printf(const char* sFormat, ...);

private:
	HANDLE hConsole;
};

