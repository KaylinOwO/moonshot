#pragma once

#include "EngineClient/EngineClient.h"
#include "BaseClientDLL/BaseClientDLL.h"
#include "ClientDLLSharedAppSystems/ClientDLLSharedAppSystems.h"
#include "ClientModeShared/ClientModeShared.h"
#include "GlobalVarsBase/GlobalVarsBase.h"
#include "ClientEntityList/ClientEntityList.h"
#include "ModelInfo/ModelInfo.h"
#include "EngineTrace/EngineTrace.h"
#include "GameMovement/GameMovement.h"
#include "MoveHelper/MoveHelper.h"
#include "ConVar/ConVar.h"
#include "Prediction/Prediction.h"
#include "Panel/Panel.h"
#include "Surface/Surface.h"
#include "RenderView/RenderView.h"
#include "EngineVGui/EngineVGui.h"
#include "NetChannel/NetChannel.h"
#include "GameEvent/GameEvent.h"
#include "Material/Material.h"
#include "ModelRender/ModelRender.h"
#include "ViewRender/ViewRender.h"
#include "DebugOverlay/DebugOverlay.h"
#include "Steam/Steamworks.h"
#include "Steam/SteamTypes.h"
#include "UniformRandomStream/UniformRandomStream.h"

class C_Interfaces
{
public:
	C_BaseClientDLL* Client = nullptr;
	C_ClientDLLSharedAppSystems* ClientShared = nullptr;
	C_ClientModeShared* ClientMode = nullptr;
	C_EngineClient* Engine = nullptr;
	C_Panel* Panel = nullptr;
	C_Surface* Surface = nullptr;
	IDirect3DDevice9* D3DDevice = NULL;
	C_ClientEntityList* EntityList = nullptr;
	C_ModelInfoClient* ModelInfo = nullptr;
	C_EngineTrace* EngineTrace = nullptr;
	C_Prediction* Prediction = nullptr;
	C_GameMovement* GameMovement = nullptr;
	C_MoveHelper* MoveHelper = nullptr;
	ICvar* CVars = nullptr; //hello, I am ICvar and I do not associate with C_ niggas
	C_PlayerInfoManager* PlayerInfo = nullptr;
	C_GlobalVarsBase* GlobalVars = nullptr;
	C_SourceInput* Input = nullptr;
	C_EngineVGui* EngineVGui = nullptr;
	int32_t* RandomSeed = nullptr; //What about me?!?!
	void* DemoPlayer = nullptr; //hHaha we're getting STRONG!!1
	C_GameEventManager* GameEvent = nullptr; //Normally this would be another traitor but I added C_
	CModelRender* ModelRender = nullptr;
	CMaterialSystem* MatSystem = nullptr;
	C_RenderView* RenderView = nullptr;
	IViewRender* ViewRender = nullptr;
	IDebugOverlay* DebugOverlay = nullptr; //Back at it MF
	IUniformRandomStream* UniformRandomStream = nullptr;
	C_LuaShared* LuaShared = nullptr;

	void Init();
};

extern C_Interfaces gInts;