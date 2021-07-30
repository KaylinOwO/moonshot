#include "Menu.h"
#include "Input/Input.h"
#include "Framework/Framework.h"

#include "../Aimbot/Aimbot.h"
#include "../Visuals/ESP/ESP.h"
#include "../Movement/Movement.h"

void CMenu::Run()
{
	gFramework.Setup();
	gFramework.HandleInput();
	gInput.Update();

	if (gFramework.b_mOpen)
	{
		//if (GetAsyncKeyState(VK_MENU)) gInts.Panel->SetMouseInputEnabled(0, false);
		int i = 0;

		i = gFramework.AddOption(i, Tabs(), &CurTab, 0, 2, 1, true);
		switch (CurTab)
		{
			case 0 /*Aimbot*/:
			{
				i = gFramework.AddOption(i, "- Enable", &gAimbot.bEnable);
				i = gFramework.AddOption(i, "- Silent Aim", &gAimbot.bSilentAim);
				i = gFramework.AddOption(i, "- FOV", &gAimbot.iFOV, 1, 180, 1);
				i = gFramework.AddOption(i, "- Disable Interpolation", &gAimbot.DisableInterpolation);
				break;
			}
			case 1 /*ESP*/:
			{
				i = gFramework.AddOption(i, "- Players", &gESP.bPlayers);
				break;
			}
			case 2: /*Misc*/
			{
				i = gFramework.AddOption(i, "- Bunnyhop", &gMovement.bBunnyhop);
				break;
			}
		}

		gFramework.TotalItems = i;
	}
}

CMenu gMenu;