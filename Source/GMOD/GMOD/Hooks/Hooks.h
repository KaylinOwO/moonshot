#pragma once

#include "ClientHook/ClientHook.h"
#include "ClientModeHook/ClientModeHook.h"
#include "PredictionHook/PredictionHook.h"
#include "SurfaceHook/SurfaceHook.h"
#include "D3DHook/D3DHook.h"
#include "PanelHook/PanelHook.h"
#include "EngineVGuiHook/EngineVGuiHook.h"
#include "EngineClientHook/EngineClientHook.h"
#include "WndProcHook/WndProcHook.h"
#include "Detours/DetourHooks.h"
#include "ViewRenderHook/ViewRenderHook.h"

class C_Hooks
{
private:
	HWND Window = 0;
	unsigned int GetV(void* class_, unsigned int index) { return (unsigned int)(*(int**)class_)[index]; }
public:
	void Init();
	void Release();
};

extern C_Hooks gHooks;