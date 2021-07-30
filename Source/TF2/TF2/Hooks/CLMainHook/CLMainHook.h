#pragma once

#include "../../SDK/SDK.h"

namespace CLMainHook
{
	extern VMT::Table Table;

	namespace CLMove
	{
		inline int Choked = 0;
		inline bool Shifting = false;

		inline DetourHook HookD;

		typedef void(*CLMoveFn)(float accumulated_extra_samples, bool bFinalTick);
		void Hook(float accumulated_extra_samples, bool bFinalTick);
	}
}