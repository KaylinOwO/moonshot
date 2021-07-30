#pragma once
#include "../../SDK/Includes/Includes.h"

/*Hardcoded HWID List*/
#define ReD 1213810804
#define callie 4238478963
#define malice 329139879

#define WEBSTRING XorStr("hLclUyX2mLxw5ZMGamGlFKioefa89j8fjeAFj494883jJJ").c_str()

class C_Security 
{
private:
    int SerialHWID();
	bool ConnectionChecks();
public:
	bool Run();
};

inline C_Security gSecurity;