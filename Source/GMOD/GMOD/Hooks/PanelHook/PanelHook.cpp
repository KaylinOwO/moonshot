#include "PanelHook.h"
#include "../../Features/Visuals/ESP/ESP.h"

void __stdcall PanelHook::PaintTraverse::Hook(unsigned int vgui_panel, bool force_repaint, bool allow_force)
{
	Table.Original<fn>(index)(gInts.Panel, vgui_panel, force_repaint, allow_force);

	static unsigned int FocusOverlayPanel, msp;

	const char* PanelName = gInts.Panel->GetName(vgui_panel);
	if (!FocusOverlayPanel)
		if (PanelName[0] == 'F' && PanelName[5] == 'O' && PanelName[12] == 'P')
		{
			FocusOverlayPanel = vgui_panel;
		}

	if (vgui_panel == FocusOverlayPanel)
	{
	//	gESP.Paint();
	}
	//...
}

VMT::Table PanelHook::Table;