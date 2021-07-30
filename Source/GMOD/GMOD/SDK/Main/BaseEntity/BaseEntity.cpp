#include "BaseEntity.h"

#include "../../SDK.h"

float C_BaseEntity::GetStepSize()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "localdata", "m_flStepSize");
}

float C_BaseEntity::GetConveyorSpeed()
{
	DYNVAR_RETURN(float, this, "DT_FuncConveyor", "m_flConveyorSpeed");
}

Vec3& C_BaseEntity::GetPunch()
{
	return *(Vec3*)((uintptr_t)this + 0x24D0);
}

//bool C_BaseEntity::IsPlayer()
//{
//	return GetClientClass()->iClassID == (int)EClientClass::CGMOD_Player;
//}

std::array<float, MAXSTUDIOPOSEPARAM> C_BaseEntity::GetPoseParam()
{
	static DWORD dwOffset = gNetVars.get_offset("DT_BaseAnimating", "m_flPoseParameter");
	return *reinterpret_cast<std::array<float, MAXSTUDIOPOSEPARAM>*>(this + dwOffset);
}

bool C_BaseEntity::GetHitboxMinsAndMaxsAndMatrix(int nHitbox, Vec3& mins, Vec3& maxs, matrix3x4& matrix, Vec3* center)
{
	model_t* pModel = (model_t*)GetClientRenderable()->GetModel();
	if (!pModel)
		return false;

		studiohdr_t* pHdr = (studiohdr_t*)gInts.ModelInfo->GetStudioModel((const model_t*)pModel);
	if (!pHdr)
		return false;

	matrix3x4 BoneMatrix[128];
	if (!this->SetupBones(BoneMatrix, 128, 0x100, gInts.GlobalVars->curtime))
		return false;

	mstudiohitboxset_t* pSet = pHdr->pHitboxSet(this->GetHitboxSet());
	if (!pSet)
		return false;

	mstudiobbox_t* pBox = pSet->hitbox(nHitbox);
	if (!pBox)
		return false;

	mins = pBox->bbmin;
	maxs = pBox->bbmax;
	memcpy(matrix, BoneMatrix[pBox->bone], sizeof(matrix3x4));

	if (center)
		Math::VectorTransform(((pBox->bbmin + pBox->bbmax) * 0.5f), BoneMatrix[pBox->bone], *center);

	return true;
}

bool C_BaseEntity::GetHitboxMinsAndMaxs(int nHitbox, Vec3& mins, Vec3& maxs, Vec3* center)
{
	model_t* pModel = (model_t*)GetClientRenderable()->GetModel();
	if (!pModel)
		return false;

		studiohdr_t* pHdr = (studiohdr_t*)gInts.ModelInfo->GetStudioModel((const model_t*)pModel);
	if (!pHdr)
		return false;

	matrix3x4 BoneMatrix[128];
	if (!this->SetupBones(BoneMatrix, 128, 0x100, gInts.GlobalVars->curtime))
		return false;

	mstudiohitboxset_t* pSet = pHdr->pHitboxSet(this->GetHitboxSet());
	if (!pSet)
		return false;

	mstudiobbox_t* pBox = pSet->hitbox(nHitbox);
	if (!pBox)
		return false;

	mins = pBox->bbmin;
	maxs = pBox->bbmax;

	if (center)
		Math::VectorTransform(((pBox->bbmin + pBox->bbmax) * 0.5f), BoneMatrix[pBox->bone], *center);

	return true;
}

MoveType_t C_BaseEntity::GetMoveType()
{
	DYNVAR_RETURN(MoveType_t, this, "DT_BaseEntity", "movetype");
}

float C_BaseEntity::GetSurfaceFriction()
{
	return *reinterpret_cast<float*>(this + 0x12D4);
}

float C_BaseEntity::GetMaxSpeed()
{
	DYNVAR_RETURN(float, this, "DT_BasePlayer", "m_flMaxspeed");
}

float C_BaseEntity::GetWaterJumpTime()
{
	return *reinterpret_cast<float*>(this + 0x10FC);

	//reference
	/*
	float			m_flMaxspeed;
	int				m_iBonusProgress;
	int				m_iBonusChallenge;
	CInterpolatedVar< Vector >	m_iv_vecViewOffset;
	Vector			m_vecWaterJumpVel;
	float			m_flWaterJumpTime;
	*/
}

void C_BaseEntity::SetTickBase(int tickbase)
{
	DYNVAR_SET(int, this, tickbase, "DT_BasePlayer", "localdata", "m_nTickBase");
}

bool C_BaseEntity::IsInValidTeam()
{
	int Team = this->GetTeamNum();
	return (Team == 2 || Team == 3);
}

void C_BaseEntity::UpTickBase()
{
	static CDynamicNetvar<int> n("DT_BasePlayer", "localdata", "m_nTickBase");
	n.SetValue(this, n.GetValue(this) + 1);
}

/*
void C_BaseEntity::UpdateClientSideAnimation()
{
	void* renderable = (PVOID)(this + 0x4);
	typedef void(__thiscall* FN)(PVOID);
	return GetVFunc<FN>(renderable, 3)(renderable);
}
*/

void C_BaseEntity::SetAbsOrigin(const Vec3& v)
{
	typedef void(__thiscall* FN)(C_BaseEntity*, const Vec3&);
	static DWORD dwFN = gPattern.Find("client.dll", "55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07");
	FN func = (FN)dwFN;
	func(this, v);
}

void C_BaseEntity::SetAbsAngles(const Vec3& v)
{
	/*typedef void(__thiscall *FN)(C_BaseEntity *, const Vec3 &);
	static DWORD dwFN = gPattern.FindInClient("55 8B EC 83 EC 60 56 57 8B F1 E8 ? ? ? ? 8B 7D 08 F3 0F 10 07 0F 2E 86 ? ? ? ? 9F F6 C4 44 7A 28 F3 0F 10 47 ?");
	FN func = (FN)dwFN;
	func(this, v);*/

	Vec3* pAbsAngles = const_cast<Vec3*>(&this->GetAbsAngles());
	*pAbsAngles = v;
}

Vec3 C_BaseEntity::GetEyeAngles()
{
	DYNVAR_RETURN(Vec3, this, "DT_TFPlayer", "tfnonlocaldata", "m_angEyeAngles[0]");
}
/*
Vec3 C_BaseEntity::GetViewOffset() {
	DYNVAR_RETURN(Vec3, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
}*/

Vec3 C_BaseEntity::GetEyePosition() {
	DYNVAR_RETURN(Vec3, this, "DT_BasePlayer", "localdata", "m_vecViewOffset[0]") + GetAbsOrigin();
}

C_UserCmd* C_BaseEntity::GetCurrentCommand()
{
	DYNVAR_RETURN(C_UserCmd*, (this - 0x4), "DT_BasePlayer", "localdata", "m_hConstraintEntity");
}

int C_BaseEntity::GetChockedTicks()
{
	float flDiff = gInts.GlobalVars->curtime - flSimulationTime();

	float flLatency = 0.0f;
	if (auto pNet = gInts.Engine->GetNetChannelInfo())
		flLatency = pNet->GetAvgLatency(FLOW_INCOMING);

	return TIME_TO_TICKS(std::max(0.0f, flDiff - flLatency));
}

void C_BaseEntity::SetCurrentCmd(C_UserCmd* cmd)
{
	DYNVAR_SET(C_UserCmd*, (this - 0x4), cmd, "DT_BasePlayer", "localdata", "m_hConstraintEntity");
}

Vec3 C_BaseEntity::GetPunchAngles()
{
	return *reinterpret_cast<Vec3*>(this + 0xE48);
}

Vec3 C_BaseEntity::GetVecOrigin()
{
	DYNVAR_RETURN(Vec3, this, "DT_BaseEntity", "m_vecOrigin");
}

void C_BaseEntity::SetVecOrigin(Vec3& vOrigin)
{
	DYNVAR_SET(Vec3, this, vOrigin, "DT_BaseEntity", "m_vecOrigin");
}

bool C_BaseEntity::GetTouched() {
	return *reinterpret_cast<bool*>(this + 0x8F8);
}

int C_BaseEntity::GetType() {
	return *reinterpret_cast<int*>(this + 0x8FC);
}

const char* C_BaseEntity::GetModelName()
{
	return gInts.ModelInfo->GetModelName((model_t*)this->GetClientRenderable()->GetModel());
}

int C_BaseEntity::GetAmmo()
{
	DYNVAR_RETURN(int, (this + 4), "DT_BasePlayer", "localdata", "m_iAmmo");
}

int C_BaseEntity::GetHitboxSet()
{
	DYNVAR_RETURN(int, this, "DT_BaseAnimating", "m_nHitboxSet");
}

int C_BaseEntity::GetTickBase()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "localdata", "m_nTickBase");
}

void C_BaseEntity::IncreaseTickBase()
{
	static CDynamicNetvar<int>n("DT_BasePlayer", "localdata", "m_nTickBase");
	n.SetValue(this, n.GetValue(this) + 1);
}

float C_BaseEntity::flSimulationTime()
{
	DYNVAR_RETURN(float, this, "DT_BaseEntity", "m_flSimulationTime");
}

int C_BaseEntity::GetOwner()
{
	DYNVAR_RETURN(int, this, "DT_BaseEntity", "m_hOwnerEntity");
}

Vec3 C_BaseEntity::GetWorldSpaceCenter()
{
	Vec3 vMin, vMax;

	this->GetClientRenderable()->GetRenderBounds(vMin, vMax);
	Vec3 vWorldSpaceCenter = this->GetAbsOrigin();
	vWorldSpaceCenter.z += (vMin.z + vMax.z) / 2.0f;

	return vWorldSpaceCenter;
}

bool C_BaseEntity::SetupBones(matrix3x4* pBoneToWorldOut, int nMaxBones, int nBoneMask, float fCurrentTime)
{
		return GetClientRenderable()->SetupBones(pBoneToWorldOut, nMaxBones, nBoneMask, fCurrentTime);
}

bool C_BaseEntity::IsDormant()
{
	return *(bool*)((uintptr_t)this + 0x17E);
}

matrix3x4& C_BaseEntity::GetRgflCoordinateFrame()
{
	PVOID pRenderable = (PVOID)(this + 0x4);
	typedef matrix3x4& (__thiscall* FN)(PVOID);
	return GetVFunc<FN>(pRenderable, 34)(pRenderable);
}

Vec3 C_BaseEntity::GetVelocity()
{
	DYNVAR_RETURN(Vec3, this, "DT_BasePlayer", "localdata", "m_vecVelocity[0]");
}

int C_BaseEntity::GetMaxHealth()
{
	typedef int(__thiscall* FN)(PVOID);
	return GetVFunc<FN>(this, 154)(this);
}

int C_BaseEntity::GetHealth()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_iHealth");
}

int C_BaseEntity::GetTeamNum()
{
	DYNVAR_RETURN(int, this, "DT_BaseEntity", "m_iTeamNum");
}

int C_BaseEntity::GetFlags()
{
	DYNVAR_RETURN(int, this, "DT_BasePlayer", "m_fFlags");
}

void C_BaseEntity::SetFlags(int nFlags)
{
	DYNVAR_SET(int, this, nFlags, "DT_BasePlayer", "m_fFlags");
}

BYTE C_BaseEntity::GetLifeState()
{
	DYNVAR_RETURN(BYTE, this, "DT_BasePlayer", "m_lifeState");
}

int C_BaseEntity::GetClassId()
{
	return this->GetClientClass()->m_ClassID;
}

Vec3 C_BaseEntity::GetCollideableMins()
{
	DYNVAR_RETURN(Vec3, this, "DT_BaseEntity", "m_Collision", "m_vecMins");
}

Vec3 C_BaseEntity::GetCollideableMaxs()
{
	DYNVAR_RETURN(Vec3, this, "DT_BaseEntity", "m_Collision", "m_vecMaxs");
}

bool C_BaseEntity::IsOnGround()
{
	return (this->GetFlags() & FL_ONGROUND);
}

bool C_BaseEntity::IsInWater()
{
	return (this->GetFlags() & FL_INWATER); //it's better to use IsSwimming method cuz this one doesn't work iirc.
}

model_t* C_BaseEntity::GetGModel()
{
	PVOID pRenderable = (PVOID)(this + 0x4);
	typedef model_t* (__thiscall* FN)(PVOID);
	return GetVFunc<FN>(pRenderable, 9)(pRenderable);
}

Vec3 C_BaseEntity::GetHitboxPos(int nHitbox, bool Uninterpolated)
{
	model_t* pModel = (model_t*)GetClientRenderable()->GetModel();
	if (!pModel)
		return Vec3();

	studiohdr_t* pHdr = (studiohdr_t*)gInts.ModelInfo->GetStudioModel(pModel);
	if (!pHdr)
		return Vec3();

	mstudiohitboxset_t* pSet = pHdr->pHitboxSet(this->GetHitboxSet());
	if (!pSet)
		return Vec3();

	mstudiobbox_t* pBox = pSet->hitbox(nHitbox);
	if (!pBox)
		return Vec3();

	Vec3 vPos = (pBox->bbmin + pBox->bbmax) * 0.5f;
	Vec3 vOut = Vec3();

	matrix3x4 BoneMatrix[128];
	if (!this->SetupBones(BoneMatrix, 128, 0x100, 0))
		return Vec3();

	Math::VectorTransform(vPos, (Uninterpolated ? gGlobalInfo.UninterpolatedBoneMatrix[this->GetClientNetworkable()->entIndex()][pBox->bone] : BoneMatrix[pBox->bone]), vOut);

//	Math::VectorTransform(vPos, BoneMatrix[pBox->bone], vOut);

	return vOut;
}




Vec3 C_BaseEntity::GetHitboxPos(const char* nHitbox, bool Uninterpolated)
{
	matrix3x4 matrix[128];
	if (!this->SetupBones(matrix, 128, 0x100, 0))
		return Vec3();

	studiohdr_t* pStudioHdr = gInts.ModelInfo->GetStudioModel((model_t*)this->GetClientRenderable()->GetModel());
	mstudiohitboxset_t* set = pStudioHdr->pHitboxSet(0);

	for (int i = 0; i <= set->numhitboxes - 1; i++)
	{
		mstudiobbox_t* hitbox = set->hitbox(i);
		if (Util::strstrcase(pStudioHdr->pBone(hitbox->bone)->GetName(), nHitbox))
		{
			Vector vMin, vMax;

			Math::VectorTransform(hitbox->bbmin, (Uninterpolated ? gGlobalInfo.UninterpolatedBoneMatrix[this->GetClientNetworkable()->entIndex()][hitbox->bone] : matrix[hitbox->bone]), vMin);
			Math::VectorTransform(hitbox->bbmax, (Uninterpolated ? gGlobalInfo.UninterpolatedBoneMatrix[this->GetClientNetworkable()->entIndex()][hitbox->bone] : matrix[hitbox->bone]), vMax);
			return (vMin + vMax) * 0.5f;;
		}
	}

	return GetHitboxPos(0); // If it can't find a bone then aim at the default hitbox for head 
}

int Studio_BoneIndexByName(studiohdr_t* pStudioHdr, char const* pName)
{
	return -1;
}

int C_BaseEntity::LookupBone(const char* szName)
{
	if (!GetModelPtr())
		return -1;

	return Studio_BoneIndexByName(GetModelPtr(), szName);
}

int C_BaseEntity::LookupBone(Joints joint)
{
	switch (joint)
	{
	case Pelvis:
		return LookupBone("ValveBiped.Bip01_Pelvis");
		break;
	case Spine:
		return LookupBone("ValveBiped.Bip01_Spine");
		break;
	case Spine1:
		return LookupBone("ValveBiped.Bip01_Spine1");
		break;
	case Spine2:
		return LookupBone("ValveBiped.Bip01_Spine2");
		break;
	case Spine3:
		return LookupBone("ValveBiped.Bip01_Spine4");
		break;
	case Neck:
		return LookupBone("ValveBiped.Bip01_Neck1");
		break;
	case Head:
		return LookupBone("ValveBiped.Bip01_Head1");
		break;
	case RCalvicle:
		return LookupBone("ValveBiped.Bip01_R_Calvicle");
		break;
	case RShoulder:
		return LookupBone("ValveBiped.Bip01_R_UpperArm");
		break;
	case RForearm:
		return LookupBone("ValveBiped.Bip01_R_Forearm");
		break;
	case RHand:
		return LookupBone("ValveBiped.Bip01_R_Hand");
		break;
	case LCalvicle:
		return LookupBone("ValveBiped.Bip01_L_Calvicle");
		break;
	case LShoulder:
		return LookupBone("ValveBiped.Bip01_L_UpperArm");
		break;
	case LForearm:
		return LookupBone("ValveBiped.Bip01_L_Forearm");
		break;
	case LHand:
		return LookupBone("ValveBiped.Bip01_L_Hand");
		break;
	case RHip:
		return LookupBone("ValveBiped.Bip01_R_Thigh");
		break;
	case RCalf:
		return LookupBone("ValveBiped.Bip01_R_Calf");
		break;
	case RFoot:
		return LookupBone("ValveBiped.Bip01_R_Foot");
		break;
	case RToe:
		return LookupBone("ValveBiped.Bip01_R_Toe0");
		break;
	case LHip:
		return LookupBone("ValveBiped.Bip01_L_Thigh");
		break;
	case LCalf:
		return LookupBone("ValveBiped.Bip01_L_Calf");
		break;
	case LFoot:
		return LookupBone("ValveBiped.Bip01_L_Foot");
		break;
	case LToe:
		return LookupBone("ValveBiped.Bip01_R_Toe0");
		break;
	default:
		return LookupBone("ValveBiped.Bip01_Head1");
		break;
	}
}

int C_BaseEntity::GetNumOfHitboxes()
{
	model_t* pModel = (model_t*)GetClientRenderable()->GetModel();
	if (!pModel)
		return 0;

		studiohdr_t* pHdr = (studiohdr_t*)gInts.ModelInfo->GetStudioModel((const model_t*)pModel);
	if (!pHdr)
		return 0;

	mstudiohitboxset_t* pSet = pHdr->pHitboxSet(this->GetHitboxSet());
	if (!pSet)
		return 0;

	return pSet->numhitboxes;
}

Vec3 C_BaseEntity::GetBonePos(int nBone)
{
	matrix3x4 matrix[128];

	if (this->SetupBones(matrix, 128, 0x100, GetTickCount64()))
		return Vec3(matrix[nBone][0][3], matrix[nBone][1][3], matrix[nBone][2][3]);

	return Vec3(0.0f, 0.0f, 0.0f);
}

float C_BaseEntity::GetNextAttack()
{
	DYNVAR_RETURN(float, this, "DT_BaseCombatCharacter", "bcc_localdata", "m_flNextAttack");
}

bool C_BaseEntity::IsDucking()
{
	return (this->GetFlags() & FL_DUCKING);
}

void C_BaseEntity::UpdateGlowEffect() {
	typedef void(__thiscall* FN)(PVOID);
	return GetVFunc<FN>(this, 226)(this);
}

void* C_BaseEntity::GetThrower()
{
	DYNVAR_RETURN(C_BaseEntity*, this, "DT_BaseGrenade", "m_hThrower");
}

float C_BaseEntity::GetDamageRadius()
{
	DYNVAR_RETURN(float, this, "DT_BaseGrenade", "m_DmgRadius");
}

float C_BaseEntity::GetSimulationTime()
{
	DYNVAR_RETURN(float, this, "DT_BaseEntity", "m_flSimulationTime");
}