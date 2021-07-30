#include "Movement.h"

void C_Movement::DoBunnyhop(C_UserCmd* pCmd)
{
	if (auto pLocal{ gEntCache.pLocal })
	{
		if (!bBunnyhop || !pLocal->IsAlive())
			return;

		static bool bJumpState = false;

		if (pCmd->buttons & IN_JUMP)
		{
			if (!bJumpState && !pLocal->IsOnGround())
				pCmd->buttons &= ~IN_JUMP;

			else if (bJumpState)
				bJumpState = false;
		}

		else if (!bJumpState)
			bJumpState = true;
	}
}

int m_strafe_index;
void C_Movement::DoStrafer(C_UserCmd* pCmd)
{
	if (auto pLocal{ gEntCache.pLocal })
	{
		if (!pLocal->IsAlive()) return;
		//if (pLocal->GetMoveType() == MOVETYPE_NOCLIP || pLocal->GetMoveType() == MOVETYPE_LADDER) return;
		if ((pLocal->GetFlags() & FL_ONGROUND)) return;
		
		Vec3 velocity = pLocal->GetVelocity();

		// get the velocity len2d ( speed ).
		float m_speed = velocity.Lenght2D();

		// compute the ideal strafe angle for our velocity.
		float m_ideal = (m_speed > 0.f) ? RAD2DEG(std::asin(15.f / m_speed)) : 90.f;
		float m_ideal2 = (m_speed > 0.f) ? RAD2DEG(std::asin(30.f / m_speed)) : 90.f;

		// some additional sanity.
		Math::Clamp(m_ideal, 0.f, 90.f);
		Math::Clamp(m_ideal2, 0.f, 90.f);

		// save entity bounds ( used much in circle-strafer ).
		Vec3 m_mins = pLocal->GetCollideableMins();
		Vec3 m_maxs = pLocal->GetCollideableMaxs();

		// save our origin
		Vec3 m_origin = pLocal->GetVecOrigin();

		// for changing direction.
		// we want to change strafe direction every call.
		static float m_switch_value = -1.f;
		m_switch_value *= -1.f;

		// for allign strafer.

		++m_strafe_index;

		static float yaw_add = 0.f;
		static const auto cl_sidespeed = gInts.CVars->FindVar("cl_sidespeed");

		bool back = pCmd->buttons & IN_BACK;
		bool forward = pCmd->buttons & IN_FORWARD;
		bool right = pCmd->buttons & IN_MOVELEFT;
		bool left = pCmd->buttons & IN_MOVERIGHT;

		if (back) {
			yaw_add = -180.f;
			if (right)
				yaw_add -= 45.f;
			else if (left)
				yaw_add += 45.f;
		}
		else if (right) {
			yaw_add = 90.f;
			if (back)
				yaw_add += 45.f;
			else if (forward)
				yaw_add -= 45.f;
		}
		else if (left) {
			yaw_add = -90.f;
			if (back)
				yaw_add -= 45.f;
			else if (forward)
				yaw_add += 45.f;
		}
		else {
			yaw_add = 0.f;
		}

		gGlobalInfo.vOldAngles += yaw_add;
		pCmd->forwardmove = 0.f;
		pCmd->sidemove = 0.f;

		float delta = Math::NormalizeAngle(gGlobalInfo.vOldAngles.y - RAD2DEG(atan2(pLocal->GetVelocity().y, pLocal->GetVelocity().x)));

		pCmd->sidemove = delta > 0.f ? -cl_sidespeed->GetFloat() : cl_sidespeed->GetFloat();

		gGlobalInfo.vOldAngles.y = Math::NormalizeAngle(gGlobalInfo.vOldAngles.y - delta);
	}
}

C_Movement gMovement;