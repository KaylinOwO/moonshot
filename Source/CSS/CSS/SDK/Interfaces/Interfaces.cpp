#include "Interfaces.h"

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

	GlobalVars = *reinterpret_cast<C_GlobalVarsBase**>(gPattern.Find(ENGINE_DLL, "A1 ? ? ? ? 8B 11 68") + 0x8);
	CHECKNULL(GlobalVars, "GlobalVars = nullptr!");

	void* ClientTable = reinterpret_cast<void*>(gPattern.Find(CLIENT_DLL, "8B 0D ? ? ? ? 8B 01 5D FF 60 28 CC"));
	CHECKNULL(ClientTable, "ClientTable = nullptr!");

	ClientMode = **reinterpret_cast<C_ClientModeShared***>(reinterpret_cast<DWORD>(ClientTable) + 2);
	CHECKNULL(ClientMode, "ClientMode = nullptr!");

	EngineVGui = gInterface::Get<C_EngineVGui>(ENGINE_DLL, VENGINE_VGUI_VERSION);
	CHECKNULL(EngineVGui, "EngineVGui = nullptr!");

	RandomSeed = *reinterpret_cast<int32_t**>(gPattern.Find(CLIENT_DLL, "C7 05 ? ? ? ? ? ? ? ? 5D C3 8B 40 34") + 0x2);
	CHECKNULL(RandomSeed, "RandomSeed = nullptr!");

	DemoPlayer = **reinterpret_cast<void***>(gPattern.Find(ENGINE_DLL, "8B 0D ? ? ? ? 85 C9 74 3B 8B 01 8B 40 18 FF D0 84 C0 74 30") + 0x2);
	CHECKNULL(DemoPlayer, "DemoPlayer = nullptr!");

	GameEvent = gInterface::Get<C_GameEventManager>(ENGINE_DLL, GAMEEVENTSMANAGER_ENGINE_INTERFACE);
	CHECKNULL(GameEvent, "GameEvent = nullptr!");

	ModelRender = gInterface::Get<CModelRender>(ENGINE_DLL, VENGINE_MODELRENDER_INTERFACE);
	CHECKNULL(ModelRender, "ModelRender = nullptr!");

	MatSystem = gInterface::Get<CMaterialSystem>(MATSYSTEM_DLL, VMATERIALSYSTEM_INTERFACE);
	CHECKNULL(MatSystem, "MatSystem = nullptr!");

	auto pdwClient = reinterpret_cast<PDWORD>(Client);
	auto pdwTable = *reinterpret_cast<PDWORD*>(pdwClient);
	ViewRender = **reinterpret_cast<IViewRender***>(static_cast<DWORD>(pdwTable[27]) + 5);
	CHECKNULL(ViewRender, "ViewRender = nullptr!");

	RenderView = gInterface::Get<C_RenderView>(ENGINE_DLL, VENGINE_RENDERVIEW_INTERFACE_VERSION);
	CHECKNULL(RenderView, "RenderView = nullptr!");

	DebugOverlay = gInterface::Get<IDebugOverlay>(ENGINE_DLL, VENGINE_DEBUGOVERLAY_INTERFACE_VERSION);
	CHECKNULL(RenderView, "DebugOverlay = nullptr!");

	UniformRandomStream = *reinterpret_cast<IUniformRandomStream**>(gPattern.Find(VSTDLIB_DLL, "B9 ? ? ? ? 85 C0 0F 45 C8 89 0D ? ? ? ? 5D C3") + 0x1);
	CHECKNULL(UniformRandomStream, "UniformRandomStream = nullptr!");
}

C_Interfaces gInts;

void C_SteamInterfaces::Init()
{
	/*gSteam.Client = reinterpret_cast<ISteamClient017*>(gInterface::Get(STEAMCLIENT_DLL, STEAMCLIENT_INTERFACE_VERSION_017));

	HSteamPipe hsNewPipe = gSteam.Client->CreateSteamPipe();
	HSteamPipe hsNewUser = gSteam.Client->ConnectToGlobalUser(hsNewPipe);

	gSteam.Friends002 = reinterpret_cast<ISteamFriends002*>(gSteam.Client->GetISteamFriends(hsNewUser, hsNewPipe, STEAMFRIENDS_INTERFACE_VERSION_002));
	gSteam.Friends015 = reinterpret_cast<ISteamFriends015*>(gSteam.Client->GetISteamFriends(hsNewUser, hsNewPipe, STEAMFRIENDS_INTERFACE_VERSION_015));
	gSteam.Utils007 = reinterpret_cast<ISteamUtils007*>(gSteam.Client->GetISteamUtils(hsNewUser, STEAMUTILS_INTERFACE_VERSION_007));
	gSteam.SteamApps = reinterpret_cast<ISteamApps006*>(gSteam.Client->GetISteamApps(hsNewUser, hsNewPipe, STEAMAPPS_INTERFACE_VERSION_006));
	gSteam.UserStats = reinterpret_cast<ISteamUserStats011*>(gSteam.Client->GetISteamUserStats(hsNewUser, hsNewPipe, STEAMUSERSTATS_INTERFACE_VERSION_011));
	gSteam.User = reinterpret_cast<ISteamUser017*>(gSteam.Client->GetISteamUser(hsNewUser, hsNewPipe, STEAMUSER_INTERFACE_VERSION_017));*/
}

C_SteamInterfaces gSteam;