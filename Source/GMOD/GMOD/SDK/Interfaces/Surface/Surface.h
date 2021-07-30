#pragma once
#include "../../Includes/Includes.h"

#undef CreateFont
#undef PlaySound

class C_Surface
{
public:
	void SetDrawColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* FN)(PVOID, int, int, int, int);
		GetVFunc<FN>(this, 11)(this, r, g, b, a);
	}

	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* FN)(PVOID, int, int, int, int);
		GetVFunc<FN>(this, 12)(this, x0, y0, x1, y1);
	}

	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* FN)(PVOID, int, int, int, int);
		GetVFunc<FN>(this, 14)(this, x0, y0, x1, y1);
	}

	void SetTextFont(unsigned long font)
	{
		typedef void(__thiscall* FN)(PVOID, unsigned long);
		GetVFunc<FN>(this, 17)(this, font);
	}

	void SetTextColor(int r, int g, int b, int a)
	{
		typedef void(__thiscall* FN)(PVOID, int, int, int, int);
		GetVFunc<FN>(this, 19)(this, r, g, b, a);
	}

	void SetTextPos(int x, int y)
	{
		typedef void(__thiscall* FN)(PVOID, int, int);
		GetVFunc<FN>(this, 20)(this, x, y);
	}

	void PrintText(const wchar_t* text, int text_len)
	{
		typedef void(__thiscall* FN)(PVOID, const wchar_t*, int, int);
		return GetVFunc<FN>(this, 22)(this, text, text_len, 0);
	}

	void DrawLine(int x, int y, int x1, int y1)
	{
		typedef void(__thiscall* FN)(PVOID, int, int, int, int);
		return GetVFunc<FN>(this, 15)(this, x, y, x1, y1);
	}

	unsigned long CreateFont()
	{
		typedef unsigned int(__thiscall* FN)(PVOID);
		return GetVFunc<FN>(this, 66)(this);
	}

	void SetFontGlyphSet(unsigned long& font, const char* windows_font_name, int tall, int weight, int blur, int scanlines, int flags)
	{
		typedef void(__thiscall* FN)(PVOID, unsigned long, const char*, int, int, int, int, int, int, int);
		GetVFunc<FN>(this, 67)(this, font, windows_font_name, tall, weight, blur, scanlines, flags, 0, 0);
	}

	void GetTextSize(unsigned long font, const wchar_t* text, int& wide, int& tall)
	{
		typedef void(__thiscall* FN)(PVOID, unsigned long, const wchar_t*, int&, int&);
		GetVFunc<FN>(this, 76)(this, font, text, wide, tall);
	}

	void GetCursorPos(int& x_out, int& y_out)
	{
		typedef void(__thiscall* FN)(PVOID, int&, int&);
		GetVFunc<FN>(this, 97)(this, x_out, y_out);
	}

	void SetCursorAlwaysVisible(bool b)
	{
		typedef void(__thiscall* FN)(PVOID, bool);
		GetVFunc<FN>(this, 52)(this, b);
	}

	bool IsCursorVisible()
	{
		typedef bool(__thiscall* FN)(PVOID);
		return GetVFunc<FN>(this, 53)(this);
	}

	void DrawFilledRectFade(int x0, int y0, int x1, int y1, unsigned int alpha0, unsigned int alpha1, bool horizontal)
	{
		typedef void(__thiscall* FN)(PVOID, int, int, int, int, unsigned int, unsigned int, bool);
		GetVFunc<FN>(this, 118)(this, x0, y0, x1, y1, alpha0, alpha1, horizontal);
	}

	void DrawOutlinedCircle(int x, int y, float radius, int segments)
	{
		typedef void(__thiscall* FN)(PVOID, int, int, float, int);
		GetVFunc<FN>(this, 99)(this, x, y, radius, segments); //doesn work?
	}

	void UnlockCursor()
	{
		typedef void(__thiscall* FN)(PVOID);
		return GetVFunc<FN>(this, 61)(this);
	}

	void LockCursor()
	{
		typedef void(__thiscall* FN)(PVOID);
		return GetVFunc<FN>(this, 62)(this);
	}

	int CreateNewTextureID(bool procedural = true) {
		typedef int(__thiscall* FN)(PVOID, bool);
		return GetVFunc<FN>(this, 37)(this, procedural);
	}

	bool DrawGetTextureFile(int id, char* filename, int maxlen) {
		typedef bool(__thiscall* FN)(PVOID, int, char*, int);
		return GetVFunc<FN>(this, 29)(this, id, filename, maxlen);
	};

	void DrawSetTextureFile(int id, const char* filename, int hardwareFilter, bool forceReload) {
		typedef void(__thiscall* FN)(PVOID, int, const char*, int, bool);
		return GetVFunc<FN>(this, 30)(this, id, filename, hardwareFilter, forceReload);
	};

	void DrawSetTextureRGBA(int id, unsigned char const* rgba, int wide, int tall, int hardwareFilter = 0, bool forceReload = false) {
		typedef void(__thiscall* FN)(PVOID, int, unsigned char const*, int, int, int, bool);
		return GetVFunc<FN>(this, 31)(this, id, rgba, wide, tall, hardwareFilter, forceReload);
	}

	void DrawSetTexture(int id) {
		typedef void(__thiscall* FN)(PVOID, int);
		return GetVFunc<FN>(this, 32)(this, id);
	}

	void DrawGetTextureSize(int id, int& wide, int& tall) {
		typedef void(__thiscall* FN)(PVOID, int, int&, int&);
		return GetVFunc<FN>(this, 33)(this, id, wide, tall);
	};

	bool IsTextureIDValid(int id) {
		typedef bool(__thiscall* FN)(PVOID, int);
		return GetVFunc<FN>(this, 35)(this, id);
	}

	void DrawTexturedRect(int x, int y, int w, int h) {
		typedef void(__thiscall* FN)(PVOID, int, int, int, int);
		return GetVFunc<FN>(this, 34)(this, x, y, x + w, y + h);
	}

	void PlaySound(const char* szFile)
	{
		typedef void(__thiscall* FN)(PVOID, const char*);
		return GetVFunc<FN>(this, 79)(this, szFile);
	}
};

#define VGUI_SURFACE_INTERFACE_VERSION "VGUI_Surface"