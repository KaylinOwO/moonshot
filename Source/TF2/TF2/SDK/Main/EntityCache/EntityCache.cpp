#include "EntityCache.h"

void C_EntityCache::Fill()
{
	C_BaseEntity *_pLocal = gInts.EntityList->GetClientEntity(gInts.Engine->GetLocalPlayer());

	if (_pLocal && _pLocal->IsInValidTeam())
	{
		pLocal = _pLocal;
		pLocalWeapon = pLocal->GetActiveWeapon();

		for (int i = 1; i < gInts.EntityList->GetHighestEntityIndex(); i++)
		{
			C_BaseEntity *pEntity = gInts.EntityList->GetClientEntity(i);

			if (!pEntity || pEntity->IsDormant())
				continue;

			if (pEntity->IsPlayer())
			{
				Groups[GroupType_t::PLAYERS_ALL].push_back(pEntity);
				Groups[pEntity->GetTeamNum() != pLocal->GetTeamNum() ? GroupType_t::PLAYERS_ENEMIES : GroupType_t::PLAYERS_TEAMMATES].push_back(pEntity);
			}

			if (pEntity->IsBuilding())
			{
				Groups[GroupType_t::BUILDINGS_ALL].push_back(pEntity);
				Groups[pEntity->GetTeamNum() != pLocal->GetTeamNum() ? GroupType_t::BUILDINGS_ENEMIES : GroupType_t::BUILDINGS_TEAMMATES].push_back(pEntity);
			}

			if (pEntity->IsPlayer() || pEntity->IsBuilding())
			{
				Groups[GroupType_t::PLAYERS_AND_BUILDINGS_ALL].push_back(pEntity);
				Groups[pEntity->GetTeamNum() != pLocal->GetTeamNum() ? GroupType_t::PLAYERS_AND_BUILDINGS_ENEMIES : GroupType_t::PLAYERS_AND_BUILDINGS_TEAMMATES].push_back(pEntity);
			}
		}
	}
}

void C_EntityCache::Clear()
{
	pLocal = nullptr;
	pLocalWeapon = nullptr;

	for (auto &group : Groups)
		group.second.clear();
}

const std::vector<C_BaseEntity *> &C_EntityCache::GetGroup(const GroupType_t &group)
{
	return Groups[group];
}

C_EntityCache gEntCache;