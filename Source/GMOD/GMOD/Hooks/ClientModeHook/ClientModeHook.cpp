#include "ClientModeHook.h"

#include "../../Features/Aimbot/Aimbot.h"
#include "../../Features/Backtrack/Backtrack.h"
#include "../../Features/Prediction/Prediction.h"
#include "../../Features/Movement/Movement.h"
#include "../../Features/Misc/Misc.h"

void __stdcall ClientModeHook::OverrideView::Hook(C_ViewSetup *pView)
{
	o_OverrideView(gInts.ClientMode, pView);

	if (!gGlobalInfo.bScreenshotDetected) pView->fov += gMisc.fFOV;
}

float __fastcall ClientModeHook::GetViewmodelFOV::Hook(void* ecx)
{
	if (gMisc.fViewmodelFOV > 0.0f && !gGlobalInfo.bScreenshotDetected)
		return (o_GetViewmodelFOV(ecx) + gMisc.fViewmodelFOV);

	return o_GetViewmodelFOV(ecx);
}

void ChokePackets(bool* pSendPacket)
{
	auto pNetChannel = gInts.Engine->GetNetChannelInfo();
	if (!pNetChannel) return;
	if (gMisc.iDTMode == 0 || gGlobalInfo.CurrentCharge <= 0) return;

	if (gGlobalInfo.ShouldChoke)
	{
		*pSendPacket = false;
		return;
	}

	*pSendPacket = true;
}

bool __stdcall ClientModeHook::CreateMove::Hook(float input_sample_frametime, C_UserCmd* pCmd)
{
	gGlobalInfo.pCmd = pCmd;
	if (!pCmd || !pCmd->command_number || !gGlobalInfo.pCmd) return  o_CreateMove(gInts.ClientMode, input_sample_frametime, pCmd);

	auto pLocal = gInts.EntityList->GetClientEntity(gInts.Engine->GetLocalPlayer());
	if (!pLocal) return o_CreateMove(gInts.ClientMode, input_sample_frametime, pCmd);

	uintptr_t _bp; __asm mov _bp, ebp;
	bool* pSendPacket = (bool*)(***(uintptr_t***)_bp - 0x1);

	if (auto pWeapon = pLocal->GetActiveWeapon()) gGlobalInfo.bWeaponCanShoot = (pWeapon->GetNextPrimaryAttack() <= (float)(pLocal->GetTickBase()) * gInts.GlobalVars->interval_per_tick);
	if (gGlobalInfo.IsDTing) Util::ForceStop(pLocal, gGlobalInfo.pCmd);

	ChokePackets(pSendPacket);

	Vec3  vOldAngles = pCmd->viewangles;
	float flOldSideMove = pCmd->sidemove;
	float flOldForwardMove = pCmd->forwardmove;

	gMisc.Run(pCmd);
	gMovement.DoBunnyhop(pCmd);

	gPrediction.Start(pCmd);
	{
		gAimbot.Run(pLocal, pCmd);

		gAimbot.NoSpread(pLocal, pCmd, pCmd->viewangles);
		gAimbot.NoRecoil(pLocal, pCmd, pCmd->viewangles);
	}
	gPrediction.End(pCmd);

	if (gAimbot.bSilentAim && !gGlobalInfo.IsDTing && !gGlobalInfo.ShouldDT)
	{
		if ((pCmd->buttons & IN_ATTACK && gGlobalInfo.bWeaponCanShoot)) {
			*pSendPacket = false;
		}
		else {
			*pSendPacket = true;
			pCmd->viewangles = vOldAngles;
			pCmd->sidemove = flOldSideMove;
			pCmd->forwardmove = flOldForwardMove;
		}
	}

	return false;
}
