#pragma once
#include "../../SDK/SDK.h"

namespace EngineVGuiHook
{
	extern VMT::Table Table;

	namespace Paint
	{
		const int index = 13;
		typedef void(__thiscall* Paint_t)(C_EngineVGui*, int);
		inline Paint_t o_Paint;
		void __stdcall Hook(int mode);
	}
}