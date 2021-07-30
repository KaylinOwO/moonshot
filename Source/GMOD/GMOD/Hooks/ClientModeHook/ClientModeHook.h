#pragma once
#include "../../SDK/SDK.h"

namespace ClientModeHook
{
	namespace OverrideView
	{
		const int index = 16;
		typedef void(__thiscall * OverrideView_t)(C_ClientModeShared *, C_ViewSetup *);
		inline OverrideView_t o_OverrideView;
		void __stdcall Hook(C_ViewSetup *pView);
	}

	namespace GetViewmodelFOV
	{
		const int index = 32;

		typedef float(__thiscall* GetViewmodelFOV_t)(void*);
		inline GetViewmodelFOV_t o_GetViewmodelFOV;

		float __fastcall Hook(void* ecx);
	}

	namespace CreateMove
	{
		const int index = 21;
		typedef bool(__thiscall* CreateMove_t)(C_ClientModeShared*, float, C_UserCmd*);
		inline CreateMove_t o_CreateMove;
		bool __stdcall Hook(float input_sample_frametime, C_UserCmd* pCmd);
	}
}