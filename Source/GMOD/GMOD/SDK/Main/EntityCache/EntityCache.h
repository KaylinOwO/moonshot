#pragma once

#include "../BaseEntity/BaseEntity.h"
#include "../BaseCombatWeapon/BaseCombatWeapon.h"
#include "../BaseObject/BaseObject.h"

enum struct GroupType_t
{
	PLAYERS_ALL,
	PLAYERS_ENEMIES,
	PLAYERS_TEAMMATES,
	BUILDINGS_ALL,
	BUILDINGS_ENEMIES,
	BUILDINGS_TEAMMATES,
	PLAYERS_AND_BUILDINGS_ALL,
	PLAYERS_AND_BUILDINGS_ENEMIES,
	PLAYERS_AND_BUILDINGS_TEAMMATES,

	WORLD_BOMB
};

class C_EntityCache
{
private:
	std::map<GroupType_t, std::vector<C_BaseEntity *>> Groups;

public:
	C_BaseEntity *pLocal = nullptr;
	C_BaseCombatWeapon *pLocalWeapon = nullptr;

	void Fill();
	void Clear();
	const std::vector<C_BaseEntity *> &GetGroup(const GroupType_t &group);
};

extern C_EntityCache gEntCache;