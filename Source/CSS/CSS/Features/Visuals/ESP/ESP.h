#pragma once

#include "../../../SDK/SDK.h"

class C_ESP
{
public:
	bool bActive = true;
	bool bOutline = false;

	bool bPlayers = true, bBuildings = true;
	int nNoTeammatePlayers = 2; //0 off 1 all 2 keep friends
	bool bPlayerName = true;
	bool bPlayerClass = true;
	bool bPlayerHealth = false;
	bool bPlayerCond = true;
	bool bPlayerHealthBar = true;
	int nPlayerBox = 2; //0 off 1 simple 2 corners

	bool bDebug = false;

private:
	std::wstring ConvertUtf8ToWide(const std::string& str);
	Color_t GetHealthColor(int HP, int MaxHP) //love u spook
	{
		HP = std::max(0, std::min(HP, MaxHP));
		byte r = std::min((510 * (MaxHP - HP)) / MaxHP, 200), g = std::min((510 * HP) / MaxHP, 200);
		return Color_t{ r, g, 0, 255 };
	}
	struct SBoundingBox {
		SBoundingBox() {
			this->x = 0;
			this->y = 0;
			this->w = 0;
			this->h = 0;
		}

		SBoundingBox(int x, int y, int w, int h) {
			this->x = x;
			this->y = x;
			this->w = w;
			this->h = h;
		}
		int x, y, w, h;
	};
	bool GetBoundingBoxPositions(C_BaseEntity* pEntity, SBoundingBox& BBox) { // https://www.unknowncheats.me/forum/counterstrike-global-offensive/124388-bounding-esp-boxes.html
		SBoundingBox output{ };

		Vec3 trans = pEntity->GetAbsOrigin();

		Vec3 min, max;

		min = pEntity->GetCollideableMins();
		max = pEntity->GetCollideableMaxs();

		Vec3 point_list[] = {
			Vec3(min.x, min.y, min.z),
			Vec3(min.x, max.y, min.z),
			Vec3(max.x, max.y, min.z),
			Vec3(max.x, min.y, min.z),
			Vec3(max.x, max.y, max.z),
			Vec3(min.x, max.y, max.z),
			Vec3(min.x, min.y, max.z),
			Vec3(max.x, min.y, max.z)
		};

		Vec3 transformed[8];

		for (int i = 0; i < 8; i++) {
			transformed[i].x = point_list[i].x + trans.x;
			transformed[i].y = point_list[i].y + trans.y;
			transformed[i].z = point_list[i].z + trans.z;
		}

		Vector flb, brt{}, blb{}, frt{}, frb{}, brb{}, blt{}, flt{};

		if (Math::W2S(transformed[3], flb) && Math::W2S(transformed[5], brt)
			&& Math::W2S(transformed[0], blb) && Math::W2S(transformed[4], frt)
			&& Math::W2S(transformed[2], frb) && Math::W2S(transformed[1], brb)
			&& Math::W2S(transformed[6], blt) && Math::W2S(transformed[7], flt))
		{
			Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

			float left = flb.x;
			float top = flb.y;
			float right = flb.x;
			float bottom = flb.y;

			for (int i = 0; i < 8; i++) {
				if (left > arr[i].x)
					left = arr[i].x;
				if (top < arr[i].y)
					top = arr[i].y;
				if (right < arr[i].x)
					right = arr[i].x;
				if (bottom > arr[i].y)
					bottom = arr[i].y;
			}

			BBox.x = left;
			BBox.y = bottom;
			BBox.w = right - left;
			BBox.h = top - bottom;
			return true;
		}

		return false;

	}

private:
	bool ShouldRun();
	Color_t GetDrawColor(C_BaseEntity* pLocal, C_BaseEntity* pEntity);
	void BoundingBoxESP(C_BaseEntity* pLocal, C_BaseEntity* pEntity, SBoundingBox Box);
public:
	void Paint();
};

extern C_ESP gESP;