#pragma once
#include "../../SDK/SDK.h"

namespace D3DHook
{
	extern VMT::Table Table;

	namespace EndScene
	{
		const int index = 42;
		typedef long(__stdcall* EndScene_t)(IDirect3DDevice9*);
		inline EndScene_t o_EndScene;
		long __stdcall Hook(IDirect3DDevice9* device);
	}
}