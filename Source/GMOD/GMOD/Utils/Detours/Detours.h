#pragma once

#include <cstdint>
#include <memory>

#include "BytePatch.h"

#define foffset(p, i) ((unsigned char *) &p)[i]

class DetourHook
{
	uintptr_t oAddress;
	std::unique_ptr<BytePatch> Patch;
	void* HookFn;

public:
	DetourHook()
	{
	}

	DetourHook(uintptr_t original, void* hook_func)
	{
		Init(original, hook_func);
	}

	void Init(uintptr_t original, void* hook_func)
	{
		oAddress = original;
		HookFn = hook_func;
		uintptr_t rel_addr = ((uintptr_t)hook_func - ((uintptr_t)oAddress)) - 5;
		Patch.reset(new BytePatch(original, { 0xE9, foffset(rel_addr, 0), foffset(rel_addr, 1), foffset(rel_addr, 2), foffset(rel_addr, 3) }));
		InitializeBytePatch();
	}

	void InitializeBytePatch()
	{
		if (Patch)
			(*Patch).Patch();
	}

	void Shutdown()
	{
		if (Patch)
			(*Patch).Shutdown();
	}

	~DetourHook()
	{
		Shutdown();
	}

	// Gets the original function with no patches
	void* GetOriginal() const
	{
		if (Patch)
		{
			// Unpatch
			(*Patch).Shutdown();
			return (void*)oAddress;
		}
		// No patch, no func.
		return nullptr;
	}

	// Restore the patches
	inline void RestorePatch()
	{
		InitializeBytePatch();
	}
};