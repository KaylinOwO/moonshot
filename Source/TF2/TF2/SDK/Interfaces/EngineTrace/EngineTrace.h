#pragma once
#include "../../Includes/Includes.h"

class C_EngineTrace
{
public:
	void TraceRay(const Ray_t &Ray, unsigned int Mask, C_TraceFilter *TraceFilter, C_GameTrace *GameTrace)
	{
		typedef void(__thiscall *FN)(PVOID, const Ray_t &, unsigned int, C_TraceFilter *, C_GameTrace *);
		return GetVFunc<FN>(this, 4)(this, Ray, Mask, TraceFilter, GameTrace);
	}
};

#define VENGINE_TRACE_CLIENT_INTERFACE_VERSION "EngineTraceClient003"