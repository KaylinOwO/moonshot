#include "Interfaces.h"
#include "../Main/GlobalInfo/GlobalInfo.h"

#define CHECKNULL(func, message) if (!func) MessageBoxA(NULL, message, "nullptr cyka(", MB_ICONERROR);

#define CLIENT_DLL		"client.dll"
#define ENGINE_DLL		"engine.dll"
#define SERVER_DLL		"server.dll"
#define VGUI2_DLL		"vgui2.dll"
#define MATSURFACE_DLL	"vguimatsurface.dll"
#define VSTDLIB_DLL		"vstdlib.dll"
#define STEAMCLIENT_DLL "SteamClient.dll"
#define MATSYSTEM_DLL "materialsystem.dll"

void C_Interfaces::Init()
{
	
	Client = gInterface::Get<C_BaseClientDLL>(CLIENT_DLL, CLIENT_DLL_INTERFACE_VERSION);
	CHECKNULL(Client, "Client = nullptr!");

	ClientShared = gInterface::Get<C_ClientDLLSharedAppSystems>(CLIENT_DLL, CLIENT_DLL_SHARED_APPSYSTEMS);
	CHECKNULL(ClientShared, "ClientShared = nullptr!");

	EntityList = gInterface::Get<C_ClientEntityList>(CLIENT_DLL, VCLIENTENTITYLIST_INTERFACE_VERSION);
	CHECKNULL(EntityList, "EntityList = nullptr!");

	Prediction = gInterface::Get<C_Prediction>(CLIENT_DLL, VCLIENT_PREDICTION_INTERFACE_VERSION);
	CHECKNULL(Prediction, "Prediction = nullptr!");

	GameMovement = gInterface::Get<C_GameMovement>(CLIENT_DLL, CLIENT_GAMEMOVEMENT_INTERFACE_VERSION);
	CHECKNULL(GameMovement, "GameMovement = nullptr!");

	ModelInfo = gInterface::Get<C_ModelInfoClient>(ENGINE_DLL, VMODELINFO_CLIENT_INTERFACE_VERSION);
	CHECKNULL(ModelInfo, "ModelInfo = nullptr!");

	Engine = gInterface::Get<C_EngineClient>(ENGINE_DLL, VENGINE_CLIENT_INTERFACE_VERSION_13);
	CHECKNULL(Engine, "Engine = nullptr!");

	EngineTrace = gInterface::Get<C_EngineTrace>(ENGINE_DLL, VENGINE_TRACE_CLIENT_INTERFACE_VERSION);
	CHECKNULL(EngineTrace, "EngineTrace = nullptr!");

	Panel = gInterface::Get<C_Panel>(VGUI2_DLL, VGUI_PANEL_INTERFACE_VERSION);
	CHECKNULL(Panel, "Panel = nullptr!");

	Surface = gInterface::Get<C_Surface>(MATSURFACE_DLL, VGUI_SURFACE_INTERFACE_VERSION);
	CHECKNULL(Surface, "Surface = nullptr!");

	CVars = gInterface::Get<ICvar>(VSTDLIB_DLL, VENGINE_CVAR_INTERFACE_VERSION);
	CHECKNULL(CVars, "CVars = nullptr!");

	PlayerInfo = gInterface::Get<C_PlayerInfoManager>("server.dll", "PlayerInfoManager");
	CHECKNULL(PlayerInfo, "PlayerInfo = nullptr!");

	GlobalVars = PlayerInfo->GetGlobalVars();
	CHECKNULL(GlobalVars, "GlobalVars = nullptr!");

	ClientMode = gInterface::GetVMT<C_ClientModeShared>((uintptr_t)Client, 10, 0x5);
	CHECKNULL(ClientMode, "ClientMode = nullptr!");

	Input = gInterface::GetVMT<C_SourceInput>((uintptr_t)Client, 20, 0x5);
	CHECKNULL(Input, "Input = nullptr!");

	EngineVGui = gInterface::Get<C_EngineVGui>(ENGINE_DLL, VENGINE_VGUI_VERSION);
	CHECKNULL(EngineVGui, "EngineVGui = nullptr!");

	D3DDevice = **(IDirect3DDevice9***)(gPattern.Find("shaderapidx9.dll", "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
	CHECKNULL(D3DDevice, "D3DDevice = nullptr!");

	GameEvent = gInterface::Get<C_GameEventManager>(ENGINE_DLL, GAMEEVENTSMANAGER_ENGINE_INTERFACE);
	CHECKNULL(GameEvent, "GameEvent = nullptr!");

	ModelRender = gInterface::Get<CModelRender>(ENGINE_DLL, VENGINE_MODELRENDER_INTERFACE);
	CHECKNULL(ModelRender, "ModelRender = nullptr!");

	MatSystem = gInterface::Get<CMaterialSystem>(MATSYSTEM_DLL, VMATERIALSYSTEM_INTERFACE);
	CHECKNULL(MatSystem, "MatSystem = nullptr!");

	ViewRender = gInterface::GetVMT<IViewRender>((uintptr_t)Client, 2, 0xA6); //
	CHECKNULL(ViewRender, "ViewRender = nullptr!");

	RenderView = gInterface::Get<C_RenderView>(ENGINE_DLL, VENGINE_RENDERVIEW_INTERFACE_VERSION);
	CHECKNULL(RenderView, "RenderView = nullptr!");

	DebugOverlay = gInterface::Get<IDebugOverlay>(ENGINE_DLL, VENGINE_DEBUGOVERLAY_INTERFACE_VERSION);
	CHECKNULL(RenderView, "DebugOverlay = nullptr!");

	LuaShared = gInterface::Get<C_LuaShared>("lua_shared.dll", "LUASHARED003");
	CHECKNULL(LuaShared, "LuaShared = nullptr!");

	UniformRandomStream = gInterface::GetVMT<IUniformRandomStream>((uintptr_t)GetProcAddress(GetModuleHandleA("vstdlib.dll"), "RandomSeed"), 0x5);
	CHECKNULL(UniformRandomStream, "UniformRandomStream = nullptr!");
}

C_Interfaces gInts;