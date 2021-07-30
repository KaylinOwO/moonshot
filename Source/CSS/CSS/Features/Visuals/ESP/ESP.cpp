#include "ESP.h"

#include "../../Aimbot/Aimbot.h"

Color_t OutlineColor = { 0, 0, 0, 180 };
Color_t TargetColor = { 153, 128, 250, 255 };
Color_t FriendColor = { 152, 255, 152, 255 };

Color_t HealthColor = { 0, 230, 64, 255 };
Color_t AmmoColor = { 191, 191, 191, 255 };

std::wstring C_ESP::ConvertUtf8ToWide(const std::string& str)
{
	int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
	std::wstring wstr(count, 0);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
	return wstr;
}

bool C_ESP::ShouldRun()
{
	if (!bActive)
		return false;

	if (!gInts.Engine->IsInGame() || !gInts.Engine->IsConnected() || gInts.Engine->Con_IsVisible())
		return false;

	if (gInts.EngineVGui->IsGameUIVisible())
		return false;

	return true;
}

Color_t C_ESP::GetDrawColor(C_BaseEntity* pLocal, C_BaseEntity* pEntity)
{
	Color_t out = gDraw.GetTeamColor(pEntity->GetTeamNum());

	//instead of the team color let's give it the friend's color, let's keep all the other overrides
	if (pLocal->IsPlayerOnSteamFriendList(pEntity))
		out = FriendColor;

	//target color should override all others
	if (pEntity->GetIndex() == gGlobalInfo.nCurrentTargetIdx)
		out = TargetColor;

	return out;
}

void C_ESP::BoundingBoxESP(C_BaseEntity* pLocal, C_BaseEntity* pEntity, SBoundingBox Box)
{
	int Health, UnclampedHealth, MaxHealth;

	C_BaseObject* pObject = reinterpret_cast<C_BaseObject*>(pEntity);
	Health = pEntity->GetHealth();
	UnclampedHealth = pEntity->GetHealth();
	MaxHealth = pEntity->GetMaxHealth();

	Health = std::clamp(Health, 0, MaxHealth);
	const auto HealthBarHeight = (Health * (Box.h + 1)) / MaxHealth;

	/*Box*/
	gDraw.OutlinedRect(Box.x - 1, Box.y - 1, Box.w + 2, Box.h + 2, Color_t{ 0, 0, 0, 255 });
	gDraw.OutlinedRect(Box.x, Box.y, Box.w, Box.h, GetDrawColor(pLocal, pEntity));
	gDraw.OutlinedRect(Box.x + 1, Box.y + 1, Box.w - 2, Box.h - 2, Color_t{ 0, 0, 0, 255 });


	/*Health Bar*/
	gDraw.OutlinedRect(Box.x - 6, Box.y, 2, Box.h, Color_t{ 0, 0, 0, 200 });
	gDraw.OutlinedRect(Box.x - 6, Box.y + ((Box.h) - HealthBarHeight), 2, HealthBarHeight, GetHealthColor(Health, MaxHealth));
	gDraw.OutlinedRect(Box.x - 6 - 1, Box.y - 1, 2 + 2, Box.h + 2, Color_t{ 0, 0, 0, 225 });
	
	/*Health Text*/
	if (UnclampedHealth < MaxHealth || UnclampedHealth > MaxHealth)
		gDraw.StringCenter(FONT_ESP_NAME, Box.x - (14 / 2), Box.y + ((Box.h) - HealthBarHeight) - 12, Color_t{ 255, 255, 255, 255 }, std::to_string(UnclampedHealth).c_str());
}

void C_ESP::Paint()
{
	if (gInts.EngineVGui->IsGameUIVisible()) return;

	if (auto pLocal{ gEntCache.pLocal })
	{
		static bool DebugESP = false; if (GetAsyncKeyState(VK_HOME) & 1) DebugESP = !DebugESP;

		if (DebugESP) //we probably won't need this since the base already dumps classids but I may use it for just instances in which I need to know an id quick without tabbing out or loop through entities for smth else.
		{
			for (int i = 1; i < gInts.EntityList->GetHighestEntityIndex(); i++)
			{
				auto Entity = gInts.EntityList->GetClientEntity(i);
				if (!Entity) continue;
				Vec3 Waa;
				Math::W2S(Entity->GetWorldSpaceCenter(), Waa);
				std::string DrawIt = Entity->GetClientClass()->szNetworkName; // is there a better way to do this than adding to std::string? seems messy
				DrawIt += " - ";
				DrawIt += std::to_string(Entity->GetClassId());
				gDraw.String(FONT_ESP_NAME, Waa.x, Waa.y, Color_t{ 255, 255, 255, 255 }, DrawIt.c_str());
			}
		}

		for (const auto& pEntity : gEntCache.GetGroup(GroupType_t::PLAYERS_ALL))
		{
			if (pEntity == pLocal) continue;

			if (pEntity->IsDormant() || !pEntity->IsAlive()) continue;

			if ((pEntity->IsPlayer() && !bPlayers)) continue;
			 
			bool bIsFriend = pLocal->IsPlayerOnSteamFriendList(pEntity);

			int TeamNum = pEntity->GetTeamNum();
			switch (nNoTeammatePlayers)
			{
				case 0: { break; }
				case 1: { if (TeamNum == pLocal->GetTeamNum()) { continue; } break; }
				case 2: { if (TeamNum == pLocal->GetTeamNum() && !bIsFriend) { continue; } break; }
			}

			SBoundingBox Box;
			PlayerInfo_t PlayerInfo;
			if (!gInts.Engine->GetPlayerInfo(pEntity->GetIndex(), &PlayerInfo) && pEntity->IsPlayer()) continue;

			if (GetBoundingBoxPositions(pEntity, Box))
			{
				BoundingBoxESP(pLocal, pEntity, Box);
				gDraw.StringCenter(FONT_ESP_NAME, Box.x + (Box.w / 2), Box.y - 14, Color_t{ 255, 255, 255, 255 }, PlayerInfo.name);
			}
		}
	}
}

C_ESP gESP;