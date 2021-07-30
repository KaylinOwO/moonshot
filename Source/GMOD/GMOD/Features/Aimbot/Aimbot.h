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
		for (int i = 0; i < pEntity->GetNumOfHitboxes(); i++) {
			float FOV = Math::GetFov(gInts.Engine->GetViewAngles(), pLocal->GetEyePosition(), pEntity->GetHitboxPos(i, DisableInterpolation));
			if (FOV < MaxFOV) {
				MaxFOV = FOV;
				Best = i;
			}
		}
		return Best;
	}
public:
	void Run(C_BaseEntity* pLocal, C_UserCmd* pCmd);

	void NoSpread(C_BaseEntity* pLocal, C_UserCmd* pCmd, Vec3& Angle);
	void NoRecoil(C_BaseEntity* pLocal, C_UserCmd* pCmd, Vec3& Angle);

	bool bEnable = true, bSilentAim = true, DisableInterpolation = true;
	int iFOV = 30, iHitbox = 0;

	const char* HitboxArr[3] = {"HEAD", "SPINE", "CLOSEST"};
};

inline C_Aimbot gAimbot;