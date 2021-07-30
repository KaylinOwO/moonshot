#pragma once
#include "../../../SDK/Includes/Includes.h"

enum struct EKeyState {
	NONE,
	PRESSED,
	HELD
};

enum struct EScrollWheelState {
	NONE,
	UP,
	DOWN
};

class C_Input
{
public:
	std::map<int, EKeyState> m_Keyboard = {};	
	EKeyState m_RMouse					= {};
	EKeyState m_LMouse					= {};
	EKeyState m_MMouse					= {};
	EScrollWheelState m_Wheel			= {};

	void Update();

	EKeyState GetKey(int key);
	EKeyState GetMouse(int key);
	enum State_t
	{
		NONE,
		PRESSED,
		HELD
	};

	State_t GetKey(int16_t key)
	{
		static State_t keys[256] = { NONE };

		if (GetKeyState(key) & 0x8000)
		{
			if (keys[key] == PRESSED)
				keys[key] = HELD;

			else if (keys[key] != HELD)
				keys[key] = PRESSED;
		}

		else keys[key] = NONE;

		return keys[key];
	}

	bool IsPressed(int16_t key)
	{
		return (GetKey(key) == PRESSED);
	}

	bool IsHeld(int16_t key)
	{
		return (GetKey(key) == HELD);
	}

	bool IsPressedAndHeld(int16_t key)
	{
		State_t key_state = GetKey(key);

		static std::chrono::time_point<std::chrono::steady_clock> start[256] = { std::chrono::steady_clock::now() };

		if (key_state == PRESSED) {
			start[key] = std::chrono::steady_clock::now();
			return true;
		}

		if (key_state == HELD && std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start[key]).count() > 400)
			return true;

		return false;
	}
};

extern C_Input gInput;