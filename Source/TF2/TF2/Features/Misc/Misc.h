#pragma once
#include "../../SDK/SDK.h"

class C_Misc
{
public:
	void Run(C_UserCmd* pCmd);

	int iDTMode;
};

inline C_Misc gMisc;