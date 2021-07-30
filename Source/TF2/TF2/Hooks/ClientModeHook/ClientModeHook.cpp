#include "ClientModeHook.h"

#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/Prediction/Prediction.h"
#include "../../Features/Movement/Movement.h"
#include "../../Features/Misc/Misc.h"

void __stdcall ClientModeHook::OverrideView::Hook(C_ViewSetup *pView)
{
	Table.Original<fn>(index)(gInts.ClientMode, pView);

	pView->fov = 120.0f;
}

bool __stdcall ClientModeHook::ShouldDrawViewModel::Hook()
{
	return Table.Original<fn>(index)(gInts.ClientMode);

	//...
}


void ChokePackets(C_UserCmd* pCmd, bool* pSendPacket)
{
	auto pNetChannel = gInts.Engine->GetNetChannelInfo();
	if (!pNetChannel) return;
	if (gMisc.iDTMode == 0 ) return;

	auto GetLatency = TIME_TO_TICKS(pNetChannel->GetLatency(FLOW_OUTGOING) + pNetChannel->GetLatency(FLOW_INCOMING));

	if ((gMisc.iDTMode == 1 && gGlobalInfo.ShouldChoke))
	{
		*pSendPacket = false;
		return;
	}

	*pSendPacket = true;
}

bool __stdcall ClientModeHook::CreateMove::Hook(float input_sample_frametime, C_UserCmd* pCmd)
{
	if (!pCmd || !pCmd->command_number)
		return false;

	uintptr_t _bp; __asm mov _bp, ebp;
	bool* pSendPacket = (bool*)(***(uintptr_t***)_bp - 0x1);

	//"Cache" this ticks weaponinfo
	{
		auto pLocal = gEntCache.pLocal;
		auto pLocalWeapon = gEntCache.pLocalWeapon;

		if (pLocal && pLocalWeapon)
		{
			//gGlobalInfo.iTickCount = pCmd->tick_count;

			gGlobalInfo.bWeaponCanShoot = pLocalWeapon->CanShoot(pLocal);
			gGlobalInfo.bWeaponCanHeadShot = Util::WeaponCanHeadshot(pLocal);
			gGlobalInfo.pCmd = pCmd;
		//	if (!pLocalWeapon->CanShoot(pLocal)) pCmd->buttons &= ~IN_ATTACK;
		}
	}

	if (gGlobalInfo.IsDTing)
		Util::ForceStop(gEntCache.pLocal, gGlobalInfo.pCmd);



	gGlobalInfo.vOldAngles = pCmd->viewangles;
	float flOldSideMove = pCmd->sidemove;
	float flOldForwardMove = pCmd->forwardmove;

	gMovement.DoStrafer(pCmd);
	gMisc.Run(pCmd);
	gMovement.DoBunnyhop(pCmd);


	Table.Original<fn>(index)(gInts.ClientMode, input_sample_frametime, pCmd); //Call the origial createmove	

	gPrediction.Start(pCmd);
	{
		ChokePackets(pCmd, pSendPacket);
		gAimbot.Run(gEntCache.pLocal, pCmd);
	}
	gPrediction.End(pCmd);

	Util::FixMovement(pCmd, gGlobalInfo.vOldAngles, flOldSideMove, flOldForwardMove);

	return false;
}

VMT::Table ClientModeHook::Table;