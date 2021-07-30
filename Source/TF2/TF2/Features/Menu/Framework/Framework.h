#pragma once
#include "../../../SDK/SDK.h"

// I didn't wanna paste here but I've never done a framework, I only did the actual design. Last text ui I made had no framework which was disgusting so I took urosaurus' framework and modified it https://github.com/urosaurus/thonk/blob/master/TF2/menu/menu.cpp

struct MenuItem_t
{
	std::string Name;

	union {
		float* m_Float;
		int* m_Int;
		bool* m_Bool;
	};

	float	Float_Step;
	int		Int_Step;
	int		Type;
	bool IsTab;
	const char* Mode;
	std::string TabText;

	union {
		float	Float_Min;
		int		Int_Min;
	};

	union {
		float	Float_Max;
		int		Int_Max;
	};

}; inline MenuItem_t Items[120];


class C_Framework
{
public:
	void Setup();
	void HandleInput();

	int AddOption(int i, std::string name, bool* value)
	{
		Items[i].Name = name;
		Items[i].m_Bool = value;
		Items[i].Type = 0;
		Items[i].IsTab = false;
		Items[i].Mode = "";

		return (i + 1);
	}

	int AddOption(int i, std::string name, int* value, int min, int max, int step, const char* mode = "", bool tab = false)
	{
		Items[i].Name = name;
		Items[i].m_Int = value;
		Items[i].Int_Min = min;
		Items[i].Int_Max = max;
		Items[i].Int_Step = step;
		Items[i].Type = 1;
		Items[i].IsTab = tab;
		Items[i].Mode = mode;

		return (i + 1);
	}

	int AddOption(int i, std::string name, float* value, float min, float max, float step)
	{
		Items[i].Name = name;
		Items[i].m_Float = value;
		Items[i].Float_Min = min;
		Items[i].Float_Max = max;
		Items[i].Float_Step = step;
		Items[i].Type = 2;
		Items[i].IsTab = false;
		Items[i].Mode = "";

		return (i + 1);
	}

	bool b_mOpen;
	int CurrentPosition = 0, TotalItems = 0;
	Vec2 MenuPos = Vec2(255, 255);
};

inline C_Framework gFramework;