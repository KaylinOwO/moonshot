#pragma once
#include "../../Includes/Includes.h"

class C_GlobalVarsBase
{
public:
	float			realtime;
	int				framecount;
	float			absoluteframetime;
	float			curtime;
	float			frametime;
	int				maxClients;
	int				tickcount;
	float			interval_per_tick;
	float			interpolation_amount;
	int				simTicksThisFrame;
	int				network_protocol;
	void* pSaveData;
private:
	bool			m_bClient;
	int				nTimestampNetworkingBase;
	int				nTimestampRandomizeWindow;
};


class C_PlayerInfoManager
{
private:
	virtual void function0() = 0;
public:
	virtual C_GlobalVarsBase* GetGlobalVars() = 0;
};