#include "ClientHook.h"
#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/Prediction/Prediction.h"
#include "../../Features/Movement/Movement.h"

void __stdcall ClientHook::FrameStageNotify::Hook(ClientFrameStage_t FrameStage)
{

	auto pLocal = gInts.EntityList->GetClientEntity(gInts.Engine->GetLocalPlayer());
	if (pLocal && pLocal->IsAlive())
		pLocal->GetPunch() = Vec3(0, 0, 0);

	o_FrameStageNotify(gInts.Client, FrameStage);

	switch (FrameStage)
	{
		case ClientFrameStage_t::FRAME_NET_UPDATE_START:
		{
			gEntCache.Clear();
			break;
		}

		case ClientFrameStage_t::FRAME_NET_UPDATE_END:
		{
			gEntCache.Fill();
			break;
		}

		case ClientFrameStage_t::FRAME_RENDER_START:
		{
			if (gAimbot.DisableInterpolation)
			{
				for (int i = 1; i < gInts.Engine->GetMaxClients(); i++)
				{
					auto pEntity = gInts.EntityList->GetClientEntity(i);
					if (!pEntity) continue;
					if (pEntity->IsDormant() || !pEntity->IsAlive()) continue;

					if (!pEntity->SetupBones(gGlobalInfo.UninterpolatedBoneMatrix[i], 256, 256, 0)) continue;
					pEntity->SetAbsOrigin(pEntity->GetVecOrigin());
				}
			}
			break;
		}
	}
}

VMT::Table ClientHook::Table;