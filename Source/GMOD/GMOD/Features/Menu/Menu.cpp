#include "Menu.h"
#include "Input/Input.h"
#include "Framework/Framework.h"

#include "../Aimbot/Aimbot.h"
#include "../Visuals/ESP/ESP.h"
#include "../Movement/Movement.h"
#include "../Misc/Misc.h"

void CMenu::Run()
{
	gFramework.Setup();
	gFramework.HandleInput();
	gInput.Update();

	if (gFramework.b_mOpen)
	{
		//if (GetAsyncKeyState(VK_MENU)) gInts.Panel->SetMouseInputEnabled(0, false);
		int i = 0;

		i = gFramework.AddOption(i, TabArr[CurTab], &CurTab, 0, 3, 1, "", true);
		switch (CurTab)
		{
			case 0 /*Aimbot*/:
			{
				i = gFramework.AddOption(i, "- Enable", &gAimbot.bEnable);
				i = gFramework.AddOption(i, "- Silent Aim", &gAimbot.bSilentAim);
				i = gFramework.AddOption(i, "- FOV", &gAimbot.iFOV, 1, 180, 1);
				i = gFramework.AddOption(i, "- Hitbox", &gAimbot.iHitbox, 0, 2, 1, gAimbot.HitboxArr[gAimbot.iHitbox]);
				i = gFramework.AddOption(i, "- Disable Interpolation", &gAimbot.DisableInterpolation);
				break;
			}
			case 1 /*ESP*/:
			{
				i = gFramework.AddOption(i, "- Master Switch", &gESP.bEnabled); // adding this for when we inevitably add more features to ESP. 
				i = gFramework.AddOption(i, "- Players", &gESP.bPlayers);
				break;
			}
			case 2 /*Visuals*/:
			{
				i = gFramework.AddOption(i, "- FOV", &gMisc.fFOV, 0.0f, 120.0f, 10.0f);
				i = gFramework.AddOption(i, "- Viewmodel FOV", &gMisc.fViewmodelFOV, 0.0f, 120.0f, 10.0f);
				break;
			}
			case 3: /*Misc*/
			{
				i = gFramework.AddOption(i, "- Bunnyhop", &gMovement.bBunnyhop);
				i = gFramework.AddOption(i, "- Server Lagger", &gMisc.bServerLagger);
				i = gFramework.AddOption(i, "- Remove Recoil", &gMisc.bRemoveRecoil);
				i = gFramework.AddOption(i, "- Remove Spread", &gMisc.bRemoveSpread);

				i = gFramework.AddOption(i, "- Doubletap", &gMisc.iDTMode, 0, 2, 1, DTArr[gMisc.iDTMode]);

				//i = gFramework.AddOption(i, "- Fakelag", &gMisc.iFakelag, 0, 14, 1);
				break;
			}
		}

		gFramework.TotalItems = i;
	}
}

CMenu gMenu;