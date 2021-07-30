#include "Security.h"

UCHAR uFileSystem[255], uVolumeName[255];
DWORD dMFL, dFlags, dSerial;
LPCTSTR CDrive = L"C:\\";

std::string GetStringFromURL(const char* URL, LPCSTR UserAgent = "hLclUyX2mLxw5ZMGamGl")
{
    DeleteUrlCacheEntryA(URL);
    HINTERNET Internet = InternetOpenA(UserAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);
    HINTERNET URLFile;
    std::string rtn;
    if (Internet) {
		URLFile = InternetOpenUrlA(Internet, URL, NULL, NULL, NULL, NULL);
        if (URLFile) {
            char buffer[2000];
            DWORD bytesRead;
            do {
                InternetReadFile(URLFile, buffer, 2000, &bytesRead);
                rtn.append(buffer, bytesRead);
                memset(buffer, 0, 2000);
            } while (bytesRead);
            InternetCloseHandle(Internet);
            InternetCloseHandle(URLFile);
            return rtn;
        }
    }
    InternetCloseHandle(Internet);
    return rtn;
}

int C_Security::SerialHWID()
{
	GetVolumeInformation(CDrive, (LPTSTR)uVolumeName, 255, &dSerial, &dMFL, &dFlags, (LPTSTR)uFileSystem, 255);

	if (dSerial == ReD) return 1; // If user is x/y, return auth level 1 (user)
	if (dSerial == callie || dSerial == malice) return 2; // If user is x/y, return auth level 2 (developer)

	return 0;
}

bool C_Security::ConnectionChecks()
{
	auto WEBPAGE = GetStringFromURL(XorStr("https://webszzz.000webhostapp.com/tmp/trans[1].gif").c_str());
	if (!strstr(WEBPAGE.c_str(), WEBSTRING)) //Does the webpage contain our text? 
		return false;

	return true;
}

bool C_Security::Run()
{
	if (SerialHWID() <= 0)
	{
		std::string sz = XorStr("HWID Mismatch: ");
		sz += std::to_string(dSerial);
		sz += "\n";
		sz += XorStr("CTRL + C To Copy To Clipboard!");
		MessageBoxA(NULL, sz.c_str(), XorStr("moonshot <3").c_str(), 0);
		return false;
	}

/*	if (!ConnectionChecks())
	{
		MessageBoxA(NULL, XorStr("cock").c_str(), XorStr("moonshot <3").c_str(), 0);
		return false;
	}

	
	DeleteUrlCacheEntryA("https://webszzz.000webhostapp.com/tmp/trans[1].gif");
	*/
	return true;
}