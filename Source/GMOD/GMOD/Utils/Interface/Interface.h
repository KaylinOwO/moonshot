#pragma once
#include <Windows.h>
#include <xstring>

using CreateInterfaceFn = void* (__cdecl*)(const char*, int*);
using InstantiateInterfaceFn = void* (__cdecl*)(void);
using DemoCustomDataCallbackFn = void(__cdecl*)(unsigned char*, std::size_t);
class CInterfaceRegister
{
public:
	InstantiateInterfaceFn	pCreateFn;		//0x0
	const char* szName;			//0x4
	CInterfaceRegister* pNext;			//0x8
};

namespace gInterface
{
	template<typename T>
	T* GetVMT(uintptr_t address, int index, uintptr_t offset) // Address must be a VTable pointer, not a VTable !
	{
		uintptr_t instruction = ((*(uintptr_t**)(address))[index] + offset);
		return *(T**)(*(uintptr_t*)(instruction));
	}

	template<typename T>
	T* GetVMT(uintptr_t address, uintptr_t offset) // This doesn't reads from the VMT, address must be the function's base ! Not a pointer!
	{
		uintptr_t instruction = (address + offset);
		return *(T**)(*(uintptr_t*)(instruction));
	}

	template <typename T>
	inline T* Get(const char* szModule, std::string_view szInterface) // https://github.com/rollraw/qo0-base/blob/26aad70c4c3974b9fe340ffe3c1ac351f241ff5c/base/core/interfaces.cpp#L98 <3
	{
		static auto GetRegisterList = [szModule]() -> CInterfaceRegister*
		{
			FARPROC o_CreateInterface = nullptr;

			if (const auto hModule = GetModuleHandleA(szModule); hModule != nullptr)
				o_CreateInterface = GetProcAddress(hModule, "CreateInterface");

			if (o_CreateInterface == nullptr)
				throw printf("Failed get CreateInterface address.");

			const std::uintptr_t CreateInterfaceJmp = reinterpret_cast<std::uintptr_t>(o_CreateInterface) + 0x4;
			const std::int32_t iJmpDisp = *reinterpret_cast<std::int32_t*>(CreateInterfaceJmp + 0x1);
			const std::uintptr_t CreateInterface = CreateInterfaceJmp + 0x5 + iJmpDisp;
			return **reinterpret_cast<CInterfaceRegister***>(CreateInterface + 0x6);
		};

		for (auto pRegister = GetRegisterList(); pRegister != nullptr; pRegister = pRegister->pNext)
		{
			if ((!std::string(pRegister->szName).compare(0U, szInterface.length(), szInterface) && std::atoi(pRegister->szName + szInterface.length()) > 0) || !szInterface.compare(pRegister->szName))
			{
				return static_cast<T*>(pRegister->pCreateFn());
			}
		}

		return nullptr;
	}
};