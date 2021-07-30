#pragma once

#include "../../SDK/SDK.h"

class C_Aimbot
{
private:
	Vec3 GetBestHitbox(C_BaseEntity* pEntity, C_BaseEntity* pLocal);
	Vec3 GetBestAngle(C_BaseEntity* pEntity, C_BaseEntity* pLocal);
	C_BaseEntity* GetBestTarget(C_BaseEntity* pLocal);
	int GetNearestHitbox(C_BaseEntity* pEntity, C_BaseEntity* pLocal)
	{
		int Best = 0;
		float MaxFOV = FLT_MAX;
		//const int Size = 9;
		//int Bones[Size] = { 15, 12, 9, 1, 2, 3, 4, 5, 0 };
		Vec3 origin = pEntity->GetAbsOrigin();
		for (int i = 0; i < pEntity->GetNumOfHitboxes(); i++) {
			float FOV = Math::GetFov(gInts.Engine->GetViewAngles(), pLocal->GetEyePosition(), pEntity->GetHitboxPos(i));
			if (FOV < MaxFOV) {
				MaxFOV = FOV;
				Best = i;
			}
		}
		return Best;
	}
public:
	void Run(C_BaseEntity* pLocal, C_UserCmd* pCmd);

	bool bEnable = true, bSilentAim = true, DisableInterpolation = true;
	int iFOV = 30;
};

inline C_Aimbot gAimbot;