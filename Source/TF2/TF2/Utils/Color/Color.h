#pragma once
#include "../Math/Math.h"

using byte = unsigned char;

struct Color_t { byte r = 0, g = 0, b = 0, a = 0; };

namespace Color
{
	inline float TOFLOAT(byte x) {
		return (static_cast<float>(x) / 255.0f);
	}

	inline Vec3 TOVEC3(const Color_t &x) {
		return Vec3(TOFLOAT(x.r), TOFLOAT(x.g), TOFLOAT(x.b));
	}

	inline unsigned long TODWORD(const Color_t &x) {
		return (static_cast<unsigned long>(((x.r & 0xFF) << 24) | ((x.g & 0xFF) << 16) | ((x.b & 0xFF) << 8) | (x.a & 0xFF)));
	}

	inline Color_t HEXTORGB(int hexValue)
	{
		struct Color_t Col;
		Col.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
		Col.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
		Col.b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte

		return Col;
	}
}