#include "Movement.h"

void C_Movement::DoBunnyhop(C_UserCmd* pCmd)
{
	if (auto pLocal = gInts.EntityList->GetClientEntity(gInts.Engine->GetLocalPlayer()))
	{
		if (!bBunnyhop || !pLocal->IsAlive())
			return;

		static bool bJumpState = false;

		if (pCmd->buttons & IN_JUMP)
		{
			if (!bJumpState && !pLocal->IsOnGround())
				pCmd->buttons &= ~IN_JUMP;

			else if (bJumpState)
				bJumpState = false;
		}

		else if (!bJumpState)
			bJumpState = true;

	}
}

C_Movement gMovement;