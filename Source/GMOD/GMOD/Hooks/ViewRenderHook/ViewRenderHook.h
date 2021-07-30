#pragma once
#include "../../SDK/SDK.h"

namespace ViewRenderHook
{
	extern VMT::Table Table;

	namespace ViewRender
	{
		typedef void(__thiscall* ViewRender_t)(C_BaseClientDLL*, vrect_t*);
		inline ViewRender_t o_ViewRender;
		using fn = void(__thiscall*)(C_BaseClientDLL*, vrect_t*);
		const int index = 5;
		void __fastcall Hook(C_BaseClientDLL* client, void* edx, vrect_t* rect);
	}
}