#pragma once
#pragma once
#include "../BaseEntity/BaseEntity.h"

struct GlobalInfo_t
{
	//vars
	int nCurrentTargetIdx = 0;
	bool bScreenshotDetected = false;
	bool bWeaponCanShoot = false;
	matrix3x4 UninterpolatedBoneMatrix[256][256] = {};
	Vec3 TestOrigin1, TestOrigin2;
	float fChargeTime = 0.0f;
	Vec3 vecUCMDAngles = Vec3();
	int iAimHitbox = 0;
	C_UserCmd* pCmd = nullptr;

	int CurrentShift, CurrentCharge;
	bool IsRecharging, IsShifting, IsDTing, ShouldChoke, ShouldShift, ShouldDT;

	//temporary place for some features I guess
	bool bNoInterpolation = true;
	Vec3 vecPredictedPos = Vec3();
	void* CMoveOriginal = NULL;

	float fTickDiff; int iTickCount;

	//Fuck
	bool m_bDrawMenu = false;
	int m_nFocusOverlay = 0;

	VMatrix WorldToProjection = {};

	bool bShouldUseInterwebz = false;
};

inline GlobalInfo_t gGlobalInfo;