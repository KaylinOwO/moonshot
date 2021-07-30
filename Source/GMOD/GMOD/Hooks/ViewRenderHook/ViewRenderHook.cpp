#include "ViewRenderHook.h"
#include "../../Features/Visuals/ESP/ESP.h"
#include "../../Features/Visuals/Radar/Radar.h"
#include "../../Features/Menu/Menu.h"


VMT::Table ViewRenderHook::Table;

void __fastcall ViewRenderHook::ViewRender::Hook(C_BaseClientDLL* client, void* edx, vrect_t* rect) // https://www.unknowncheats.me/forum/garry-s-mod/322679-perfect-anti-screenshot.html <- Legend
{
	static auto StartDrawing = reinterpret_cast<void(__thiscall*)(void*)>(gPattern.Find("vguimatsurface.dll", "55 8B EC 64 A1 ? ? ? ? 6A FF 68 ? ? ? ? 50 64 89 25 ? ? ? ? 83 EC 14"));
	static auto FinishDrawing = reinterpret_cast<void(__thiscall*)(void*)>(gPattern.Find("vguimatsurface.dll", "55 8B EC 6A FF 68 ? ? ? ? 64 A1 ? ? ? ? 50 64 89 25 ? ? ? ? 51 56 6A 00"));

	o_ViewRender(client, rect);
//	Table.Original<fn>(index)(client, rect);

	// anti-screengrab; anti-screenshot
	static BYTE* IsRecordingMovie = *(BYTE**)(gPattern.Find("engine.dll", "55 8B EC A1 ? ? ? ? 81 EC ? ? ? ? D9 45 18"/*CL_StartMovie*/) + 0x3E);
	gGlobalInfo.bScreenshotDetected = (gInts.Engine->IsTakingScreenshot() || *IsRecordingMovie);
	if (gGlobalInfo.bScreenshotDetected)
	{
		gInts.Surface->PlaySound("buttons\\button10.wav");//
		return;
	}
		

	C_ViewSetup ViewSetup = {};

	if (gInts.Client->GetPlayerView(ViewSetup))
	{
		VMatrix WorldToView = {}, ViewToProjection = {}, WorldToPixels = {};
		gInts.RenderView->GetMatricesForView(ViewSetup, &WorldToView, &ViewToProjection, &gGlobalInfo.WorldToProjection, &WorldToPixels);
	}

	StartDrawing(gInts.Surface);
	{
		gESP.Paint();
		gMenu.Run();
	}
	FinishDrawing(gInts.Surface);
}