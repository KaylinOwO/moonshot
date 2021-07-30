#include "D3DHook.h"

long __stdcall D3DHook::EndScene::Hook(IDirect3DDevice9* device)
{
	return o_EndScene(device);
}