#pragma once

#include "Classes.h"
#include "Const.h"
#include "Enums.h"
#include "Structs.h"
#include "MD5.h"
#include "icons.h"

#include <deque>
#include <mutex>

class IRefCounted
{
public:
	virtual int AddRef() = 0;
	virtual int Release() = 0;
};