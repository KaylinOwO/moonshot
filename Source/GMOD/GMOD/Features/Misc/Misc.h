#pragma once
#include "../../SDK/SDK.h"

class C_Misc
{
public:
	void Run(C_UserCmd* pCmd);

	bool bServerLagger = false, bRemoveSpread = true, bRemoveRecoil = true, bPassiveRecharge = false;
	int iFakelag = 11, iDTMode = 1;

	float fFOV = 0.0f, fViewmodelFOV = 0.0f;
};

inline C_Misc gMisc;