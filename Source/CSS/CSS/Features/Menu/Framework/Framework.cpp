#include "Framework.h"
#include "../Menu.h"
#include "../Input/Input.h"

void C_Framework::Setup()
{
	if (!gFramework.b_mOpen) return;

	gDraw.String(FONT_MENU, gFramework.MenuPos.x - 15, gFramework.MenuPos.y - 20, Color_t{ 90, 14, 255, 255 }, "moonshot");
	if (gFramework.CurrentPosition == 0 /*Tab Selection*/)
	{
		gDraw.StringCenter(FONT_MENU, gFramework.MenuPos.x - gMenu.Tabs().size() - 25, gFramework.MenuPos.y + 2 + (15 * gFramework.CurrentPosition) + 2, Color_t{ 90, 14, 255, 255 }, ">>");
		gDraw.StringCenter(FONT_MENU, gFramework.MenuPos.x + gMenu.Tabs().size() + 25, gFramework.MenuPos.y + 2 + (15 * gFramework.CurrentPosition) + 2, Color_t{ 90, 14, 255, 255 }, "<<");
	}
	else
		gDraw.StringCenter(FONT_MENU, gFramework.MenuPos.x - 10, gFramework.MenuPos.y + 2 + (15 * gFramework.CurrentPosition) + 4, Color_t{ 90, 14, 255, 255 }, ">>");

	for (int i = 0; i < gFramework.TotalItems; ++i) {

		if (!strncmp(Items[i].Name.c_str(), "[+]", 3) || !strncmp(Items[i].Name.c_str(), "[-]", 3)) {
			gDraw.String(FONT_MENU, gFramework.MenuPos.x, gFramework.MenuPos.y + 2 + (15 * i) + 4, Color_t{ 255,255,255,255 }, Items[i].Name.c_str());
		}
		else {
			switch (Items[i].Type) {
			case 0:
				gDraw.String(FONT_MENU, gFramework.MenuPos.x + 10, gFramework.MenuPos.y + 2 + (15 * i) + 4, Color_t{ 255,255,255,255 }, Items[i].Name.c_str());
				gDraw.String(FONT_MENU, gFramework.MenuPos.x + 150, gFramework.MenuPos.y + 2 + (15 * i) + 4, Color_t{ 255,255,255,255 }, "%s", (*Items[i].m_Bool) ? "ON" : "OFF");
				break;
			case 1:
				if (Items[i].IsTab)
					gDraw.StringCenter(FONT_MENU, gFramework.MenuPos.x, gFramework.MenuPos.y + 2 + (15 * i) + 4, Color_t{ 255,255,255,255 }, Items[i].Name.c_str());
				else
				{
					gDraw.String(FONT_MENU, gFramework.MenuPos.x + 10, gFramework.MenuPos.y + 2 + (15 * i) + 4, Color_t{ 255,255,255,255 }, Items[i].Name.c_str());
					gDraw.String(FONT_MENU, gFramework.MenuPos.x + 150, gFramework.MenuPos.y + 2 + (15 * i) + 4, Color_t{ 255,255,255,255 }, "%i", *Items[i].m_Int);
				}

				break;
			case 2:
				gDraw.String(FONT_MENU, gFramework.MenuPos.x + 10, gFramework.MenuPos.y + 2 + (15 * i) + 4, Color_t{ 255,255,255,255 }, Items[i].Name.c_str());
				gDraw.String(FONT_MENU, gFramework.MenuPos.x + 150, gFramework.MenuPos.y + 2 + (15 * i) + 4, Color_t{ 255,255,255,255 }, "%.2f", *Items[i].m_Float);
				break;
			default:
				break;
			}
		}

	}
}

void C_Framework::HandleInput()
{
	if (gInput.IsPressed(VK_INSERT))
		b_mOpen = !b_mOpen;

	if (b_mOpen)
	{
		if (gInput.IsPressed(VK_UP) || (GetAsyncKeyState(VK_MENU) && gInput.m_Wheel == EScrollWheelState::DOWN))
		{
			if (CurrentPosition > 0)
				CurrentPosition--;
			else
				CurrentPosition = TotalItems - 1;
		}

		if (gInput.IsPressed(VK_DOWN) || (GetAsyncKeyState(VK_MENU) && gInput.m_Wheel == EScrollWheelState::UP))
		{
			if (CurrentPosition < TotalItems - 1)
				CurrentPosition++;
			else
				CurrentPosition = 0;
		}

		if (gInput.IsPressed(VK_LEFT) || (GetAsyncKeyState(VK_MENU) && gInput.IsPressed(VK_LBUTTON)))
		{
			switch (Items[CurrentPosition].Type) {
			case 0:
				*Items[CurrentPosition].m_Bool = !*Items[CurrentPosition].m_Bool;
				break;
			case 1:
				*Items[CurrentPosition].m_Int -= Items[CurrentPosition].Int_Step;
				if (*Items[CurrentPosition].m_Int < Items[CurrentPosition].Int_Min)
					*Items[CurrentPosition].m_Int = Items[CurrentPosition].Int_Max;
				break;
			case 2:
				*Items[CurrentPosition].m_Float -= Items[CurrentPosition].Float_Step;
				if (*Items[CurrentPosition].m_Float < Items[CurrentPosition].Float_Min)
					*Items[CurrentPosition].m_Float = Items[CurrentPosition].Float_Max;
				break;
			default:
				break;
			}
		}

		if (gInput.IsPressed(VK_RIGHT) || (GetAsyncKeyState(VK_MENU) && gInput.IsPressed(VK_RBUTTON)))
		{
			switch (Items[CurrentPosition].Type) {
			case 0:
				*Items[CurrentPosition].m_Bool = !*Items[CurrentPosition].m_Bool;
				break;
			case 1:
				*Items[CurrentPosition].m_Int += Items[CurrentPosition].Int_Step;
				if (*Items[CurrentPosition].m_Int > Items[CurrentPosition].Int_Max) {
					*Items[CurrentPosition].m_Int = Items[CurrentPosition].Int_Min;
				}
				break;
			case 2:
				*Items[CurrentPosition].m_Float += Items[CurrentPosition].Float_Step;
				if (*Items[CurrentPosition].m_Float > Items[CurrentPosition].Float_Max) {
					*Items[CurrentPosition].m_Float = Items[CurrentPosition].Float_Min;
				}
				break;
			default:
				break;
			}
		}
	}
	
}