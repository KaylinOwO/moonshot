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


	inline void PrintDebug(const char* szString) {
#ifdef _DEBUG
		printf(szString);
#endif
	}
}