#include "ConVars.h"

void ConVars_t::Init()
{
	cl_interp = gInts.CVars->FindVar("cl_interp");
	cl_interp_ratio = gInts.CVars->FindVar("cl_interp_ratio");
	cl_updaterate = gInts.CVars->FindVar("cl_updaterate");

	sv_gravity = gInts.CVars->FindVar("sv_gravity");
	cl_flipviewmodels = gInts.CVars->FindVar("cl_flipviewmodels");

	sv_maxusrcmdprocessticks = gInts.CVars->FindVar("sv_maxusrcmdprocessticks");
}

ConVars_t gConVars;