#pragma once
#include "../../SDK/SDK.h"

namespace PredictionHook
{
	extern VMT::Table Table;

	namespace RunCommand
	{
		const int index = 17;
		typedef void(__thiscall * RunCommand_t)(C_Prediction *, C_BaseEntity *, C_UserCmd *, C_MoveHelper *);
		inline RunCommand_t o_RunCommand;
		void __stdcall Hook(C_BaseEntity *pEntity, C_UserCmd *pCmd, C_MoveHelper *pMoveHelper);
	}
}