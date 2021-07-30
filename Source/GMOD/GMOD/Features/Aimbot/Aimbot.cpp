#include "Aimbot.h"
#include "../Misc/Misc.h"


Vec3 C_Aimbot::GetBestHitbox(C_BaseEntity* pEntity, C_BaseEntity* pLocal)
{
	Vec3 PreferredHitbox = iHitbox == 2 ? pEntity->GetHitboxPos(GetNearestHitbox(pEntity, pLocal)) : pEntity->GetHitboxPos(HitboxArr[iHitbox]);

	if (auto pWeapon = pLocal->GetActiveWeapon()) if (Util::strstrcase(pWeapon->GetPrintName(), "Crossbow")) PreferredHitbox = pEntity->GetHitboxPos("Spine");
	for (int i = 0; i < pEntity->GetNumOfHitboxes(); i++)
		if (!gVisCheck.Pos(pLocal, pEntity, pLocal->GetEyePosition(), PreferredHitbox))
			PreferredHitbox = pEntity->GetHitboxPos(i, DisableInterpolation);

	return PreferredHitbox;
}

Vec3 C_Aimbot::GetBestAngle(C_BaseEntity* pEntity, C_BaseEntity* pLocal)
{
	auto Hitbox = GetBestHitbox(pEntity, pLocal);
	if (Hitbox.IsZero()) return Vec3();

	if (auto pWeapon = pLocal->GetActiveWeapon())
	{
		if (Util::strstrcase(pWeapon->GetPrintName(), "Crossbow"))
		{
			float Time = (pLocal->GetEyePosition().DistTo(Hitbox) / 3500.0f); //1100 = rocket velocity
			Hitbox.x += (pEntity->GetVelocity().x * Time);
			Hitbox.y += (pEntity->GetVelocity().y * Time);
		}
	}

	return Math::CalcAngle(pLocal->GetEyePosition(), Hitbox);
}

C_BaseEntity* C_Aimbot::GetBestTarget(C_BaseEntity* pLocal)
{
	C_BaseEntity* BestEntity = nullptr;
	float FOV = FLT_MAX;
	
	for (int i = 1; i < gInts.Engine->GetMaxClients(); i++)
	{
		auto pEntity = gInts.EntityList->GetClientEntity(i);
		if (!pEntity) continue;
		if (pEntity->IsDormant() || !pEntity->IsAlive() || pEntity == pLocal) continue;
		if (!gVisCheck.Pos(pLocal, pEntity, pLocal->GetEyePosition(), GetBestHitbox(pEntity, pLocal))) continue; // Visibility check

		int GetFOV = Math::GetFov(gInts.Engine->GetViewAngles(), pLocal->GetEyePosition(), pEntity->GetHitboxPos(GetNearestHitbox(pEntity, pLocal), DisableInterpolation)); //This might be a "controversial" way of doing it but instead of getting the closest to crosshair entity via selected hitbox, it uses the nearest hitbox. This is to prevent say if I'm looking at the floor, the hitbox is out of FOV but the player is not, it won't aim at them.
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

	Vec3 Angle = GetBestAngle(pEntity, pLocal);
	if (Angle.IsZero()) return;

	if (pCmd->buttons & IN_ATTACK && gGlobalInfo.bWeaponCanShoot)
	{
		pCmd->viewangles = Angle;
		if (!bSilentAim) gInts.Engine->SetViewAngles(Angle);
	}
}

void C_Aimbot::NoSpread(C_BaseEntity* pLocal, C_UserCmd* pCmd, Vec3& Angle)
{
	if (!gMisc.bRemoveSpread || !(pCmd->buttons & IN_ATTACK)) return;

	auto pWeapon = pLocal->GetActiveWeapon();
	auto pLua = gInts.LuaShared->GetLuaInterface(0);

	if (!pWeapon) return;

	auto pWeaponName = pWeapon->GetName(); Vec3 pBulletSpread = pWeapon->GetBulletSpread();
	if ((pWeapon->UsesLua() && !pLua) || Util::strstrcase(pWeaponName, "weapon_crossbow") || Util::strstrcase(pWeaponName, "weapon_357") || Util::strstrcase(pWeaponName, "m9k_svu") || Util::strstrcase(pWeaponName, "m9k_dragunov") || Util::strstrcase(pWeaponName, "m9k_contender") || Util::strstrcase(pWeaponName, "m9k_barret_m82") || Util::strstrcase(pWeaponName, "m9k_aw50") || Util::strstrcase(pWeaponName, "m9k_sl8") || Util::strstrcase(pWeaponName, "m9k_intervention") || Util::strstrcase(pWeaponName, "m9k_svt40") || Util::strstrcase(pWeaponName, "m9k_m24") || Util::strstrcase(pWeaponName, "m9k_psg1")) return;

	double pSpread = FLT_MAX;
	if (!pWeapon->UsesLua())
		pSpread = (pBulletSpread.x + pBulletSpread.y + pBulletSpread.z) / 3;
	else if (pLua) {
		if (!strcmp(Util::GetLuaEntBase(pWeapon), "cw_base")) // if the gun's base == cw2
		{
			pWeapon->PushEntity();
			pLua->GetField(-1, "MaxSpreadInc");
			float curcone = pLua->GetNumber(-1);
			pLua->Pop(2);
		}
		else
		{
			int topop = 3;
			pWeapon->PushEntity();
			pLua->GetField(-1, "Primary");
			if (!pLua->IsType(-1, LuaObjectType::TABLE))
			{
				--topop;
				pLua->Pop(1);
			}
			pLua->GetField(-1, "Spread");
			if (pLua->IsType(-1, LuaObjectType::NUMBER))
			{
				pSpread = pLua->GetNumber(-1);
			}
			pLua->Pop(topop);
		}
	}

	if (pSpread == FLT_MAX) return;

	BYTE pSeed = MD5_PseudoRandom(pCmd->command_number) & 0xFF;
	gInts.UniformRandomStream->SetSeed(pSeed);

	Vec3 pEngineSpread(gInts.UniformRandomStream->RandomFloat(-0.5f, 0.5f) + gInts.UniformRandomStream->RandomFloat(-0.5f, 0.5f), gInts.UniformRandomStream->RandomFloat(-0.5f, 0.5f) + gInts.UniformRandomStream->RandomFloat(-0.5f, 0.5f), 0);

	Vector ShootDirection = Vector(1.f, 1.f, 1.f);
	ShootDirection.y = (pSpread * pEngineSpread.y);
	ShootDirection.z = -(pSpread * pEngineSpread.x);

	Angle += ShootDirection.ToAngle();
}

void C_Aimbot::NoRecoil(C_BaseEntity* pLocal, C_UserCmd* pCmd, Vec3& Angle)
{
	if (!gMisc.bRemoveRecoil) return;

	auto pWeapon = pLocal->GetActiveWeapon(); if (!pWeapon) return;
	auto pLua = gInts.LuaShared->GetLuaInterface(0);

	if (!pLua || !pWeapon->UsesLua()) return;

	pWeapon->PushEntity();

	if (!strcmp(Util::GetLuaEntBase(pWeapon), "bobs_gun_base")) // if the gun's base == m9k
	{
		pLua->GetField(-1, "Primary");
		if (pLua->IsType(-1, LuaObjectType::TABLE)) // if SWEP.Primary is a table
		{
			pLua->PushNumber(0);
			pLua->SetField(-2, "IronAccuracy"); // SWEP.Primary.IronAccuracy = 0
			pLua->PushNumber(0);
			pLua->SetField(-2, "KickHorizontal"); // SWEP.Primary.KickHorizontal = 0
			pLua->PushNumber(0);
			pLua->SetField(-2, "KickUp"); // SWEP.Primary.KickUp = 0
			pLua->PushNumber(0);
			pLua->SetField(-2, "KickDown"); // SWEP.Primary.KickDown = 0
			pLua->Pop(2);
		}
		else pLua->Pop(2);
	}
	else if (!strcmp(Util::GetLuaEntBase(pWeapon), "cw_base")) // if the gun's base == cw2
	{
		pLua->PushNumber(0);
		pLua->SetField(-2, "Recoil");
		pLua->PushBool(true);
		pLua->SetField(-2, "NoFreeAim"); // <-- That is the secret :)

		pLua->Pop(1);
	}
	else if (!strcmp(Util::GetLuaEntBase(pWeapon), "fas2_base")) // if the gun's base == fas2
	{
		pLua->PushNumber(0);
		pLua->SetField(-2, "Recoil"); // SWEP.Recoil = 0
		pLua->PushNumber(0);
		pLua->SetField(-2, "ViewKick"); // SWEP.ViewKick = 0

		pLua->Pop(1);
	}
	else {

		int topop = 2;
		pLua->GetField(-1, "Primary");
		if (!pLua->IsType(-1, LuaObjectType::TABLE))
		{
			topop = 1;
			pLua->Pop(1);
		}

		pLua->PushNumber(0);
		pLua->SetField(-2, "Recoil");
		pLua->PushNumber(0);
		pLua->SetField(-2, "ViewKick");
		pLua->PushNumber(0);
		pLua->SetField(-2, "KickHorizontal");
		pLua->PushNumber(0);
		pLua->SetField(-2, "KickUp");
		pLua->PushNumber(0);
		pLua->SetField(-2, "KickDown");

		pLua->Pop(topop);
	}
}