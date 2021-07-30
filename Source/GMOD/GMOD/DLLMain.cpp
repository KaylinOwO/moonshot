#include "Hooks/Hooks.h"

//If you want to make this a mess which has features for many different games
//you need to make this process automated.
//Or then just don't hook WndProc idc

DWORD WINAPI MainThread(LPVOID lpParam)
{
	if (gSecurity.Run())
	{
		Util::InitDebug();

		gInts.Init();
		gNetVars.Init();
		gHooks.Init();
		gConVars.Init();

		gDraw.InitFonts
		({
			//FONT_ESP
			{ 0x0, "Tahoma", 12, 0, FONTFLAG_OUTLINE },
			//FONT_ESP_NAME
			{ 0x0, "Tahoma", 13, 0, FONTFLAG_OUTLINE },
			//FONT_ESP_COND
			{ 0x0, "Consolas", 10, 0, FONTFLAG_OUTLINE },
			//FONT_ESP_PICKUPS
			{ 0x0, "Tahoma", 11, 0, FONTFLAG_OUTLINE },
			//FONT_DEBUG
			{ 0x0, "Arial", 16, 0, FONTFLAG_OUTLINE },
			//FONT_MENU
			{ 0x0, "Tahoma", 15, 0, FONTFLAG_ANTIALIAS | FONTFLAG_OUTLINE }
		});

		//Stuck at this, as long as "panic" key is not pressed.
		while (!(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_DELETE)))
			std::this_thread::sleep_for(std::chrono::seconds(1));

		gHooks.Release();
		Util::ReleaseDebug();
		FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
	}
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH: {
		if (HANDLE hMain = CreateThread(0, 0, MainThread, hinstDLL, 0, 0)) CloseHandle(hMain);
		break;
	}

	case DLL_PROCESS_DETACH: {
		break;
	}

	default: break;
	}

	return TRUE;
}