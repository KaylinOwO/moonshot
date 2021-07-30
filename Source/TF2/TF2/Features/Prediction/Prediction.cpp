#include "Prediction.h"

int C_LocalPrediction::GetTickbase(C_UserCmd *pCmd, C_BaseEntity *pLocal)
{
	static int nTick = 0;
	static C_UserCmd *pLastCmd = nullptr;

	if (pCmd)
	{
		if (!pLastCmd || pLastCmd->hasbeenpredicted)
			nTick = pLocal->GetTickBase();

		else nTick++;

		pLastCmd = pCmd;
	}

	return nTick;
}

void C_LocalPrediction::Start(C_UserCmd *pCmd)
{
	if (!gInts.MoveHelper)
		return;

	C_BaseEntity *pLocal = gEntCache.pLocal;

	if (pLocal && pLocal->IsAlive())
	{
		static DWORD ResetInstanceCounters = gPattern.Find("client.dll", "68 ? ? ? ? 6A ? 68 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? E8 ? ? ? ? 83 C4 ? C3");
		reinterpret_cast<void(*)()>(ResetInstanceCounters)();

		pLocal->SetCurrentCmd(pCmd);
		
		static DWORD SetPredictionRandomSeed = gPattern.Find("client.dll", "55 8B EC 8B 45 ? 85 C0 75 ? C7 05");
		reinterpret_cast<void(*)(const C_UserCmd*)>(SetPredictionRandomSeed)(pCmd);

		fOldCurrentTime = gInts.GlobalVars->curtime;
		fOldFrameTime	= gInts.GlobalVars->frametime;
		nOldTickCount	= gInts.GlobalVars->tickcount;
		nOldFlags       = pLocal->GetFlags();

		const int nOldTickBase				= pLocal->GetTickBase();
		const bool bOldIsFirstPrediction	= gInts.Prediction->m_bFirstTimePredicted;
		const bool bOldInPrediction			= gInts.Prediction->m_bInPrediction;
		
		gInts.GlobalVars->curtime	= TICKS_TO_TIME(GetTickbase(pCmd, pLocal));
		gInts.GlobalVars->frametime = (gInts.Prediction->m_bEnginePaused ? 0.0f : TICK_INTERVAL);
		gInts.GlobalVars->tickcount = GetTickbase(pCmd, pLocal);

		gInts.Prediction->m_bFirstTimePredicted = false;
		gInts.Prediction->m_bInPrediction		= true;

		//skipped buttons

		//TODO: CheckMovingGround(pLocal, gInts.GlobalVars->frametime);

		gInts.Prediction->SetLocalViewAngles(pCmd->viewangles);

		gInts.MoveHelper->SetHost(pLocal);

		gInts.Prediction->SetupMove(pLocal, pCmd, gInts.MoveHelper, &moveData);
		gInts.GameMovement->ProcessMovement(pLocal, &moveData);
		gInts.Prediction->FinishMove(pLocal, pCmd, &moveData);

		pLocal->SetTickBase(nOldTickBase);

		gInts.Prediction->m_bInPrediction		= bOldInPrediction;
		gInts.Prediction->m_bFirstTimePredicted = bOldIsFirstPrediction;
	}
}

void C_LocalPrediction::End(C_UserCmd *pCmd)
{
	if (!gInts.MoveHelper)
		return;

	C_BaseEntity *pLocal = gEntCache.pLocal;

	if (pLocal && pLocal->IsAlive())
	{
		gInts.MoveHelper->SetHost(nullptr);

		gInts.GlobalVars->curtime	= fOldCurrentTime;
		gInts.GlobalVars->frametime = fOldFrameTime;
		gInts.GlobalVars->tickcount = nOldTickCount;
		
		pLocal->SetCurrentCmd(nullptr);

		//TODO: Use less memory

		static DWORD SetPredictionRandomSeed = gPattern.Find("client.dll", "55 8B EC 8B 45 ? 85 C0 75 ? C7 05");
		reinterpret_cast<void(*)(const C_UserCmd*)>(SetPredictionRandomSeed)(nullptr);
	}
}

C_LocalPrediction gPrediction;