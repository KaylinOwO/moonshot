#pragma once
#include "../../SDK/SDK.h"

class CMenu
{
public:
	void Run();

	int CurTab = 0;
	std::string Tabs()
	{
		switch (CurTab)
		{
		case 0:
			return "Aimbot";
			break;
		case 1:
			return "ESP";
			break;
		case 2:
			return "Misc";
			break;
		}

		return "";
	}
};

extern CMenu gMenu;