#include "CLMainHook.h"
#include "../../Features/Misc/Misc.h"

bool ShouldRecharge()
{
	float MaxUserProcessCmd = gConVars.sv_maxusrcmdprocessticks->GetFloat() == 0 ? 63 : gConVars.sv_maxusrcmdprocessticks->GetFloat() - 1;

	if (GetAsyncKeyState(VK_XBUTTON2) && gGlobalInfo.CurrentCharge < MaxUserProcessCmd)
		return true;

	return false;
}


void CLMainHook::CLMove::Hook(float accumulated_extra_samples, bool bFinalTick)
{
	auto Original = (CLMoveFn)HookD.GetOriginal();
	auto pLocal = gInts.EntityList->GetClientEntity(gInts.Engine->GetLocalPlayer());


	if (ShouldRecharge())
	{
		gGlobalInfo.CurrentCharge++;
		gGlobalInfo.IsRecharging = true;
	}
	else
	{
		Original(accumulated_extra_samples, bFinalTick);
		gGlobalInfo.IsRecharging = false;
	}

	if (!gInts.Engine->IsInGame() || gInts.Engine->IsDrawingLoadingImage() || !pLocal || pLocal->IsDormant() || !pLocal->IsAlive() || !gGlobalInfo.pCmd || gMisc.iDTMode == 0)
	{
		gGlobalInfo.ShouldShift = false;
		gGlobalInfo.ShouldDT = false;

		gGlobalInfo.IsShifting = false;
		gGlobalInfo.IsDTing = false;
		gGlobalInfo.IsRecharging = false;

		gGlobalInfo.CurrentCharge = 0;
	}
	else
	{
		uintptr_t _bp; __asm mov _bp, ebp;
		bool* pSendPacket = (bool*)(***(uintptr_t***)_bp - 0x1);

		if ((gGlobalInfo.CurrentCharge > 1) && (gGlobalInfo.pCmd->buttons & IN_ATTACK || GetAsyncKeyState(VK_XBUTTON1)))
		{
			gGlobalInfo.IsShifting = true;
			if (gGlobalInfo.pCmd->buttons & IN_ATTACK) gGlobalInfo.IsDTing = true;
			for (int i = 0; i < gGlobalInfo.CurrentCharge; ++i)
			{
				gGlobalInfo.ShouldChoke = false;

				if (i != gGlobalInfo.CurrentCharge - 1)
				{
					gGlobalInfo.pCmd->buttons |= IN_ATTACK;
					gGlobalInfo.ShouldChoke = true;
				}

				Original(accumulated_extra_samples, bFinalTick);
				gGlobalInfo.CurrentCharge--;
			}

			gGlobalInfo.ShouldChoke = false;
			gGlobalInfo.IsShifting = false;
			gGlobalInfo.IsDTing = false;
		}
	}

	HookD.RestorePatch();
}