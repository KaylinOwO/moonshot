#pragma once
#include "../../SDK/SDK.h"

class CMenu
{
public:
	void Run();

	int CurTab = 0;
	std::string TabArr[4] = { "Aimbot", "ESP", "Misc" };
	const char* DTArr[3] = { "OFF", "PASSIVE", "AGGRESSIVE" };
};

extern CMenu gMenu;