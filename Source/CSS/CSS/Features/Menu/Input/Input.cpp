#include "Input.h"

void C_Input::Update()
{
	for (auto &Key : gInput.m_Keyboard) {
		if (Key.second != EKeyState::NONE)
			Key.second = EKeyState::HELD;
	}

	if (gInput.m_RMouse != EKeyState::NONE)
		gInput.m_RMouse = EKeyState::HELD;

	if (gInput.m_LMouse != EKeyState::NONE)
		gInput.m_LMouse = EKeyState::HELD;

	if (gInput.m_MMouse != EKeyState::NONE)
		gInput.m_MMouse = EKeyState::HELD;

	if (gInput.m_Wheel != EScrollWheelState::NONE) // To prevent it from having a seizure, as soon as its set we set it back. Maybe improper but its the best solution I could think of, also fit with the rest of the original function in a way.
		gInput.m_Wheel = EScrollWheelState::NONE;
}

EKeyState C_Input::GetKey(int key)
{
	return m_Keyboard[key];
}

EKeyState C_Input::GetMouse(int key)
{
	switch (key)
	{
		case VK_RBUTTON: { return m_RMouse; }
		case VK_LBUTTON: { return m_LMouse; }
		case VK_MBUTTON: { return m_MMouse; }
	}

	return {};
}

C_Input gInput;