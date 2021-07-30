#pragma once
#include <Windows.h>
#include <Psapi.h>

#define INRANGE(x,a,b)	(x >= a && x <= b) 
#define GetBits(x)		(INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetByte(x)		(GetBits(x[0]) << 4 | GetBits(x[1]))

class CPattern
{
private:
	HMODULE GetModuleHandleSafe(PCCH szModuleName);
public:
	DWORD FindPattern(DWORD dwAddress, DWORD dwLength, PCCH szPattern);
	DWORD Find(PCCH szModuleName, PCCH szPattern);
};

extern CPattern gPattern;