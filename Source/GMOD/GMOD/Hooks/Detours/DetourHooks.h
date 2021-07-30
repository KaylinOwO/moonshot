#pragma once
#include "../../SDK/SDK.h"
#include "../../Utils/Detours/Detours.h"

namespace DetourHooks
{
	namespace CLMove
	{
		inline DetourHook Detour;
		typedef void(*CLMoveFn)(float accumulated_extra_samples, bool bFinalTick);
		void Hook(float accumulated_extra_samples, bool bFinalTick);
	}
}