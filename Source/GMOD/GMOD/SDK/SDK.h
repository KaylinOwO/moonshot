#pragma once

#include "Main/BaseEntity/BaseEntity.h"
#include "Main/BaseCombatWeapon/BaseCombatWeapon.h"
#include "Main/BaseObject/BaseObject.h"

#include "Main/DrawUtils/DrawUtils.h"
#include "Main/EntityCache/EntityCache.h"
#include "Main/VisCheck/VisCheck.h"
#include "Main/GlobalInfo/GlobalInfo.h"
#include "Main/ConVars/ConVars.h"

#pragma warning( disable : 4996 )

#define TIME_TO_TICKS( dt )	( (int)( 0.5f + (float)(dt) / gInts.GlobalVars->interval_per_tick ) )
#define TICKS_TO_TIME( t )	( gInts.GlobalVars->interval_per_tick * ( t ) )
#define TICK_INTERVAL		( gInts.GlobalVars->interval_per_tick )
#define	IMPORT(DLL, FUNC) GetProcAddress(GetModuleHandleA(DLL), FUNC)

struct ShaderStencilState_t
{
	bool                        m_bEnable;
	StencilOperation_t          m_FailOp;
	StencilOperation_t          m_ZFailOp;
	StencilOperation_t          m_PassOp;
	StencilComparisonFunction_t m_CompareFunc;
	int                         m_nReferenceValue;
	uint32                      m_nTestMask;
	uint32                      m_nWriteMask;

	ShaderStencilState_t();
	void SetStencilState(IMatRenderContext* pRenderContext);
};

inline ShaderStencilState_t::ShaderStencilState_t()
{
	m_bEnable = false;
	m_PassOp = m_FailOp = m_ZFailOp = STENCILOPERATION_KEEP;
	m_CompareFunc = STENCILCOMPARISONFUNCTION_ALWAYS;
	m_nReferenceValue = 0;
	m_nTestMask = m_nWriteMask = 0xFFFFFFFF;
}

inline void ShaderStencilState_t::SetStencilState(IMatRenderContext* pRenderContext)
{
	pRenderContext->SetStencilEnable(m_bEnable);
	pRenderContext->SetStencilFailOperation(m_FailOp);
	pRenderContext->SetStencilZFailOperation(m_ZFailOp);
	pRenderContext->SetStencilPassOperation(m_PassOp);
	pRenderContext->SetStencilCompareFunction(m_CompareFunc);
	pRenderContext->SetStencilReferenceValue(m_nReferenceValue);
	pRenderContext->SetStencilTestMask(m_nTestMask);
	pRenderContext->SetStencilWriteMask(m_nWriteMask);
}

namespace Util
{
	inline void TraceHull(const Vec3& vecStart, const Vec3& vecEnd, const Vec3& vecHullMin, const Vec3& vecHullMax,
		unsigned int nMask, C_TraceFilter* pFilter, C_GameTrace* pTrace)
	{
		Ray_t ray;
		ray.Init(vecStart, vecEnd, vecHullMin, vecHullMax);
		gInts.EngineTrace->TraceRay(ray, nMask, pFilter, pTrace);
	}

	inline void InitDebug() {
#ifdef _DEBUG
		AllocConsole();
		if (freopen("CONOUT$", "w", stdout) == NULL) return;
#endif
	}

	inline void ReleaseDebug() {
#ifdef _DEBUG
		FreeConsole();
		system("exit");
#endif
	}


	inline auto strstrcase(const char* szPattern, const char* szString)
	{
		std::string strPattern = szPattern;
		std::string strString = szString;
		for (unsigned int i = 0; i < strPattern.size(); i++)
			strPattern[i] = tolower(strPattern[i]);
		for (unsigned int j = 0; j < strString.size(); j++)
			strString[j] = tolower(strString[j]);

		return strstr(strPattern.c_str(), strString.c_str());
	}


	inline void ForceStop(C_BaseEntity* pLocal, C_UserCmd* pCmd)
	{
		if (pLocal->IsDormant() || !pLocal->IsAlive()) return;
		Vec3 pVelocity  = pLocal->GetVelocity();

		static auto pCVFriction = gInts.CVars->FindVar("sv_friction");
		static auto pCVStopSpeed = gInts.CVars->FindVar("sv_stopspeed");

		float pSpeed = pVelocity.Lenght2D(), pFriction = pCVFriction->GetFloat(), pControl = (pSpeed < pCVStopSpeed->GetFloat()) ? pCVStopSpeed->GetFloat() : pSpeed, pDrop = pControl * pFriction * gInts.GlobalVars->interval_per_tick;

		if (pSpeed > pDrop - 1.0f)
		{
			Vector pVecVelocity = pVelocity, pDirection;
			Math::VectorAngles(pVelocity, pDirection);
			float pfSpeed = pVecVelocity.Lenght();

			pDirection.y = pCmd->viewangles.y - pDirection.y;

			Vector pForward;
			Math::AngleVectors(pDirection, &pForward);
			Vector NegatedDirection = pForward * -pfSpeed;

			pCmd->forwardmove = NegatedDirection.x;
			pCmd->sidemove = NegatedDirection.y;
		}
		else
			pCmd->forwardmove = pCmd->sidemove = 0.0f;
	}

	inline bool CenterOfHitbox(C_BaseEntity* player, matrix3x4* bones, int joint, Vec3& out)
	{

		if (!player || !bones || !player->IsAlive())
			return false;

		model_t* model = (model_t*)player->GetClientRenderable()->GetModel();
		if (!model)
			return false;

		mstudiohitboxset_t* hitboxset = gInts.ModelInfo->GetStudioModel(model)->pHitboxSet(player->GetHitboxSet());

		if (!hitboxset)
			return false;

		mstudiobbox_t* hitbox = hitboxset->hitbox(joint);
		if (!hitbox)
			return false;

		Vec3 mins = hitbox->bbmin; Vec3 maxs = hitbox->bbmax;
		Vec3 omin, omax;

		Math::VectorTransform(mins, bones[joint], omin);
		Math::VectorTransform(maxs, bones[joint], omax);

		out = (omin + omax) * 0.5;
		return true;
	}

	inline void FixMovement(C_UserCmd* pCmd, Vec3 vOldAngles, float fOldSidemove, float fOldForward)
	{
		int e = rand() % 3;

		Vec3 curAngs = pCmd->viewangles;
		float fRot = 90;
		//Vector nwANG = Vector(-89, curAngs.y + fRot, 0);
		//pCmd->viewangles = nwANG;

		float deltaView = pCmd->viewangles.y - vOldAngles.y;
		float f1;
		float f2;

		if (vOldAngles.y < 0.f)
			f1 = 360.0f + vOldAngles.y;
		else
			f1 = vOldAngles.y;

		if (pCmd->viewangles.y < 0.0f)
			f2 = 360.0f + pCmd->viewangles.y;
		else
			f2 = pCmd->viewangles.y;

		if (f2 < f1)
			deltaView = abs(f2 - f1);
		else
			deltaView = 360.0f - abs(f1 - f2);
		deltaView = 360.0f - deltaView;

		pCmd->forwardmove = cos(DEG2RAD(deltaView)) * fOldForward + cos(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
		pCmd->sidemove = sin(DEG2RAD(deltaView)) * fOldForward + sin(DEG2RAD(deltaView + 90.f)) * fOldSidemove;
	}

	inline const char* GetLuaEntBase(C_BaseCombatWeapon* _this)
	{
		C_LuaInterface* Lua = gInts.LuaShared->GetLuaInterface(0);  // ent.Base
		if (!_this->UsesLua())
			return "";
		_this->PushEntity();
		Lua->GetField(-1, "Base");
		const char* out = "";
		if (Lua->IsType(-1, LuaObjectType::STRING))
			out = Lua->GetString(-1, NULL);
		Lua->Pop(2);
		return out;
	}

	inline double GetLuaWeaponDamage(C_BaseCombatWeapon* _this)
	{
		C_LuaInterface* Lua = gInts.LuaShared->GetLuaInterface(0);  // ent.Base
		if (!_this->UsesLua())
			return 0.f;
		double damage = 1.f;
		int topop = 3;
		_this->PushEntity();
		Lua->GetField(-1, "Primary");
		if (!Lua->IsType(-1, LuaObjectType::TABLE))
		{
			--topop;
			Lua->Pop(1);
		}
		Lua->GetField(-1, "Damage");
		if (Lua->IsType(-1, LuaObjectType::NUMBER))
		{
			damage = Lua->GetNumber(-1);
		}
		Lua->Pop(topop);

		return damage;
	}

	inline const char* GetLuaEntName(C_BaseCombatWeapon* _this)
	{
		C_LuaInterface* Lua = gInts.LuaShared->GetLuaInterface(0); // ent.PrintName
		if (!_this->UsesLua())
			return "";
		_this->PushEntity();
		Lua->GetField(-1, "PrintName");
		const char* out = "";
		if (Lua->IsType(-1, LuaObjectType::STRING))
			out = Lua->GetString(-1, NULL);
		Lua->Pop(2);
		return out;
	}



	inline void PrintDebug(const char* szString) {
#ifdef _DEBUG
		printf(szString);
#endif
	}


	namespace Lua {

		inline double Rand(double Min, double Max)
		{
			auto pLua = gInts.LuaShared->GetLuaInterface(0);
			C_LuaAutoPop AP(pLua);

			pLua->PushSpecial(0); //1
			pLua->GetField(-1, "math"); //2
			pLua->GetField(-1, "Rand"); //2
			pLua->PushNumber(Min); //3
			pLua->PushNumber(Max); //4
			pLua->Call(2, 1); //3
			auto out = pLua->GetNumber();
			return out;
		}

		inline void RandomSeed(double Seed)
		{
			auto pLua = gInts.LuaShared->GetLuaInterface(0);
			C_LuaAutoPop AP(pLua);

			pLua->PushSpecial(0); //1
			pLua->GetField(-1, "math"); //2
			pLua->GetField(-1, "randomseed"); //2
			pLua->PushNumber(Seed); //3
			pLua->Call(1, 0); //2
		}
	}
}