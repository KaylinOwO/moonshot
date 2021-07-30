#include "Hooks.h"

void C_Hooks::Init()
{
	while (!Window) Window = FindWindowA("Valve001", NULL);

	auto FrameStageNotify_t = reinterpret_cast<void*>(GetV(gInts.Client, ClientHook::FrameStageNotify::index));
	auto CreateMove_t = reinterpret_cast<void*>(GetV(gInts.ClientMode, ClientModeHook::CreateMove::index));
	auto OverrideView_t = reinterpret_cast<void*>(GetV(gInts.ClientMode, ClientModeHook::OverrideView::index));
	auto GetViewmodelFOV_t = reinterpret_cast<void*>(GetV(gInts.ClientMode, ClientModeHook::GetViewmodelFOV::index));
	auto RunCommand_t = reinterpret_cast<void*>(GetV(gInts.Prediction, PredictionHook::RunCommand::index));
	auto Paint_t = reinterpret_cast<void*>(GetV(gInts.EngineVGui, EngineVGuiHook::Paint::index));
	auto ViewRender_t = reinterpret_cast<void*>(GetV(gInts.ViewRender, ViewRenderHook::ViewRender::index));
	auto EndScene_t = reinterpret_cast<void*>(GetV(gInts.D3DDevice, D3DHook::EndScene::index));

	MH_Initialize();
	{
		/*Client*/
		MH_CreateHook(FrameStageNotify_t, &ClientHook::FrameStageNotify::Hook, reinterpret_cast<void**>(&ClientHook::FrameStageNotify::o_FrameStageNotify));

		/*ClientMode*/
		MH_CreateHook(CreateMove_t, &ClientModeHook::CreateMove::Hook, reinterpret_cast<void**>(&ClientModeHook::CreateMove::o_CreateMove));
		MH_CreateHook(OverrideView_t, &ClientModeHook::OverrideView::Hook, reinterpret_cast<void**>(&ClientModeHook::OverrideView::o_OverrideView));
		MH_CreateHook(GetViewmodelFOV_t, &ClientModeHook::GetViewmodelFOV::Hook, reinterpret_cast<void**>(&ClientModeHook::GetViewmodelFOV::o_GetViewmodelFOV));

		/*Prediction*/
		MH_CreateHook(RunCommand_t, &PredictionHook::RunCommand::Hook, reinterpret_cast<void**>(&PredictionHook::RunCommand::o_RunCommand));

		/*EngineVGui*/
		MH_CreateHook(Paint_t, &EngineVGuiHook::Paint::Hook, reinterpret_cast<void**>(&EngineVGuiHook::Paint::o_Paint));

		/*ViewRender*/
		MH_CreateHook(ViewRender_t, &ViewRenderHook::ViewRender::Hook, reinterpret_cast<void**>(&ViewRenderHook::ViewRender::o_ViewRender));

		/*D3D*/
		MH_CreateHook(EndScene_t, &D3DHook::EndScene::Hook, reinterpret_cast<void**>(&D3DHook::EndScene::o_EndScene));

		/*WndProc*/
		WndProcHook::WndProc = (WNDPROC)SetWindowLongPtr(Window, GWL_WNDPROC, (LONG_PTR)WndProcHook::Hook);

		/*Detours*/
		DetourHooks::CLMove::Detour.Init(gPattern.Find("engine.dll", "55 8B EC B8 10 10 00"), (void*)DetourHooks::CLMove::Hook);
	}
	MH_EnableHook(MH_ALL_HOOKS);
}

void C_Hooks::Release()
{
	auto FrameStageNotify_t = reinterpret_cast<void*>(GetV(gInts.Client, ClientHook::FrameStageNotify::index));
	auto CreateMove_t = reinterpret_cast<void*>(GetV(gInts.ClientMode, ClientModeHook::CreateMove::index));
	auto OverrideView_t = reinterpret_cast<void*>(GetV(gInts.ClientMode, ClientModeHook::OverrideView::index));
	auto GetViewmodelFOV_t = reinterpret_cast<void*>(GetV(gInts.ClientMode, ClientModeHook::GetViewmodelFOV::index));
	auto RunCommand_t = reinterpret_cast<void*>(GetV(gInts.Prediction, PredictionHook::RunCommand::index));
	auto Paint_t = reinterpret_cast<void*>(GetV(gInts.EngineVGui, EngineVGuiHook::Paint::index));
	auto ViewRender_t = reinterpret_cast<void*>(GetV(gInts.ViewRender, ViewRenderHook::ViewRender::index));

	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();

	SetWindowLongPtr(Window, GWL_WNDPROC, (LONG_PTR)WndProcHook::WndProc);

	Util::PrintDebug("Released all.\n");
}

C_Hooks gHooks;