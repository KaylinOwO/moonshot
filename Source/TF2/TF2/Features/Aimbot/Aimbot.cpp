#include "Aimbot.h"
//might look over complicated currently as we have many features without much purpose but their purpose will come soon


Vec3 C_Aimbot::GetBestHitbox(C_BaseEntity* pEntity, C_BaseEntity* pLocal)
{
	int PreferredHitbox = HITBOX_BODY;

	if (gGlobalInfo.bWeaponCanHeadShot && !pEntity->IsBuilding())
		PreferredHitbox = HITBOX_HEAD;

	/*Hitscan*/
	for (int i = PreferredHitbox; i < pEntity->GetNumOfHitboxes(); i++) //we loop through max hitboxes, starting value being our current preferred
	{
		if (!gVisCheck.Pos(pLocal, pEntity, pLocal->GetEyePosition(), pEntity->GetHitboxPos(PreferredHitbox))) //if the preferred hitbox isn't visible then set it to one of the avaliable. Once avaliable again, since the function runs every tick, it'll get set back automatically.
			PreferredHitbox = i;
	}

	gGlobalInfo.iAimHitbox = PreferredHitbox;

	return pEntity->GetHitboxPos(PreferredHitbox);
}

Vec3 C_Aimbot::GetBestAngle(C_BaseEntity* pEntity, C_BaseEntity* pLocal)
{
	auto Hitbox = GetBestHitbox(pEntity, pLocal);
	if (Hitbox.IsZero()) return Vec3();

	return Math::CalcAngle(pLocal->GetEyePosition(), GetBestHitbox(pEntity, pLocal));
}

C_BaseEntity* C_Aimbot::GetBestTarget(C_BaseEntity* pLocal)
{
	C_BaseEntity* BestEntity = nullptr;
	float FOV = FLT_MAX;
	
	for (const auto& pEntity : gEntCache.GetGroup(GroupType_t::PLAYERS_AND_BUILDINGS_ENEMIES))
	{
		if (!pEntity) continue;
		if (pEntity->IsDormant() || !pEntity->IsAlive() || pEntity == pLocal) continue;

		int GetFOV = Math::GetFov(gInts.Engine->GetViewAngles(), pLocal->GetEyePosition(), pEntity->GetHitboxPos(GetNearestHitbox(pEntity, pLocal))); //This might be a "controversial" way of doing it but instead of getting the closest to crosshair entity via selected hitbox, it uses the nearest hitbox. This is to prevent say if I'm looking at the floor, the hitbox is out of FOV but the player is not, it won't aim at them.
		if (GetFOV < FOV && GetFOV < iFOV)
		{
			FOV = GetFOV;
			BestEntity = pEntity;
		}
	}

	return BestEntity;
}


void C_Aimbot::Run(C_BaseEntity* pLocal, C_UserCmd* pCmd)
{
	if (!bEnable) return;

	auto pEntity = GetBestTarget(pLocal);
	if (!pEntity || !pLocal) return;
	if (pLocal->IsDormant() || !pLocal->IsAlive()) return;
	if (!gVisCheck.Pos(pLocal, pEntity, pLocal->GetEyePosition(), GetBestHitbox(pEntity, pLocal))) return; // Visibility check

	Vec3 Angle = GetBestAngle(pEntity, pLocal);
	if (Angle.IsZero()) return;

	if (pCmd->buttons & IN_ATTACK)
	{
		pCmd->viewangles = Angle;
		if (!bSilentAim) gInts.Engine->SetViewAngles(Angle);

		if (DisableInterpolation) pCmd->tick_count = TIME_TO_TICKS(pEntity->GetSimulationTime() + std::max(gInts.CVars->FindVar("cl_interp")->GetFloat(), gInts.CVars->FindVar("cl_interp_ratio")->GetFloat() / gInts.CVars->FindVar("cl_updaterate")->GetFloat()));
	}
}
