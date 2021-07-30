#pragma once

#include "../../SDK/SDK.h"

namespace ClientHook
{
	extern VMT::Table Table;

	namespace FrameStageNotify
	{
		const int index = 35;
		typedef void(__thiscall * FrameStageNotify_t)(C_BaseClientDLL *, ClientFrameStage_t);
		inline FrameStageNotify_t o_FrameStageNotify;
		void __stdcall Hook(ClientFrameStage_t FrameStage);
	}
}