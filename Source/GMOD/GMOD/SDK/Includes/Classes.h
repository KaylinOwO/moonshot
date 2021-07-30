#pragma once
#include "../../Utils/Utils.h"

#include "Enums.h"
#include "Structs.h"

#include "dt_common.h"
#include "dt_recv.h"

typedef unsigned long CRC32_t;
typedef unsigned short MaterialVarSym_t;
class ITexture;
class IMaterial;

class MaterialPropertyTypes_t;
class VertexFormat_t;

class VMatrix {
private:
	Vec3 m[4][4];

public:
	const matrix3x4& As3x4() const {
		return *((const matrix3x4*)this);
	}
};

class C_ViewSetup
{
public:
	// left side of view window
	int			x;
	int			m_nUnscaledX;
	// top side of view window
	int			y;
	int			m_nUnscaledY;
	// width of view window
	int			width;
	int			m_nUnscaledWidth;
	// height of view window
	int			height;
	// which eye are we rendering?
	StereoEye_t m_eStereoEye;
	int			m_nUnscaledHeight;

	// the rest are only used by 3D views

		// Orthographic projection?
	bool		m_bOrtho;
	// View-space rectangle for ortho projection.
	float		m_OrthoLeft;
	float		m_OrthoTop;
	float		m_OrthoRight;
	float		m_OrthoBottom;

	// horizontal FOV in degrees
	float		fov;
	// horizontal FOV in degrees for in-view model
	float		fovViewmodel;

	// 3D origin of camera
	Vec3		origin;

	// heading of camera (pitch, yaw, roll)
	Vec3		angles;
	// local Z coordinate of near plane of camera
	float		zNear;
	// local Z coordinate of far plane of camera
	float		zFar;

	// local Z coordinate of near plane of camera ( when rendering view model )
	float		zNearViewmodel;
	// local Z coordinate of far plane of camera ( when rendering view model )
	float		zFarViewmodel;

	// set to true if this is to draw into a subrect of the larger screen
	// this really is a hack, but no more than the rest of the way this class is used
	bool		m_bRenderToSubrectOfLargerScreen;

	// The aspect ratio to use for computing the perspective projection matrix
	// (0.0f means use the viewport)
	float		m_flAspectRatio;

	// Controls for off-center projection (needed for poster rendering)
	bool		m_bOffCenter;
	float		m_flOffCenterTop;
	float		m_flOffCenterBottom;
	float		m_flOffCenterLeft;
	float		m_flOffCenterRight;

	// Control that the SFM needs to tell the engine not to do certain post-processing steps
	bool		m_bDoBloomAndToneMapping;

	// Cached mode for certain full-scene per-frame varying state such as sun entity coverage
	bool		m_bCacheFullSceneState;

	// If using VR, the headset calibration will feed you a projection matrix per-eye.
	// This does NOT override the Z range - that will be set up as normal (i.e. the values in this matrix will be ignored).
	bool        m_bViewToProjectionOverride;
	VMatrix     m_ViewToProjection;
};

class C_MoveData
{
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;

	void* m_nPlayerHandle; // edict index on server, client entity handle on client

	int    m_nImpulseCommand; // Impulse command issued.
	Vec3 m_vecViewAngles; // Command view angles (local space)
	Vec3 m_vecAbsViewAngles; // Command view angles (world space)
	int    m_nButtons; // Attack buttons.
	int    m_nOldButtons; // From host_client->oldbuttons;
	float  m_flForwardMove;
	float  m_flSideMove;
	float  m_flUpMove;

	float m_flMaxSpeed;
	float m_flClientMaxSpeed;

	// Variables from the player edict (sv_player) or entvars on the client.
	// These are copied in here before calling and copied out after calling.
	Vec3 m_vecVelocity; // edict::velocity		// Current movement direction.
	Vec3 m_vecAngles; // edict::angles
	Vec3 m_vecOldAngles;

	// Output only
	float  m_outStepHeight; // how much you climbed this move
	Vec3 m_outWishVel; // This is where you tried
	Vec3 m_outJumpVel; // This is your jump velocity

					   // Movement constraints	(radius 0 means no constraint)
	Vec3 m_vecConstraintCenter;
	float  m_flConstraintRadius;
	float  m_flConstraintWidth;
	float  m_flConstraintSpeedFactor;

	void        SetAbsOrigin(const Vec3& vec);
	const Vec3& GetAbsOrigin() const;

	// private:
	Vec3 m_vecAbsOrigin; // edict::origin
};

class CEntityGroundContact
{
public:
	int		entindex;
	float	minheight;
	float	maxheight;
};

class C_UserCmd
{
public:
	// For matching server and client commands for debugging
	int     command_number;
	// the tick the client created this command
	int     tick_count;

	// Player instantaneous view angles.
	Vec3  viewangles;
	// Intended velocities
	//  forward velocity.
	float   forwardmove;
	//  sideways velocity.
	float   sidemove;
	//  upward velocity.
	float   upmove;
	// Attack button states
	int     buttons;
	// Impulse command issued.
	BYTE    impulse;
	// Current weapon id
	int     weaponselect;
	int     weaponsubtype;

	int     random_seed;    // For shared random functions

	short   mousedx;        // mouse accum in x from create move
	short   mousedy;        // mouse accum in y from create move

	// Client only, tracks whether we've predicted this command at least once
	bool    hasbeenpredicted;

	// Back channel to communicate IK state

}; 

typedef void* (*CreateClientClassFn)(int entnum, int serialNum);
typedef void* (*CreateEventFn)();
class C_ClientClass
{
public:
	C_ClientClass(const char* pNetworkName, CreateClientClassFn createFn, CreateEventFn createEventFn, RecvTable* pRecvTable)
	{
		m_pNetworkName = pNetworkName;
		m_pCreateFn = createFn;
		m_pCreateEventFn = createEventFn;
		m_pRecvTable = pRecvTable;

		// Link it in
		//m_pNext = g_pClientClassHead;
		//g_pClientClassHead = this;
	}

	const char* GetName()
	{
		return m_pNetworkName;
	}

public:
	CreateClientClassFn		m_pCreateFn;
	CreateEventFn			m_pCreateEventFn;	// Only called for event objects.
	const char* m_pNetworkName;
	RecvTable* m_pRecvTable;
	C_ClientClass* m_pNext;
	int						m_ClassID;	// Managed by the engine.
};

class C_BaseEntity; //forward declare

class C_TraceFilter
{
public:
	virtual bool ShouldHitEntity(void* entity, int contents_mask) = 0;
	virtual TraceType_t GetTraceType() const = 0;
};

class C_BaseTrace
{
public:
	bool IsDispSurface(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceSurfProp1(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceSurfProp2(void) { return ((DispFlags & SurfaceFlags_t::DISPSURF_FLAG_SURFPROP2) != 0); }

public:
	Vec3			vStartPos = Vec3();
	Vec3			vEndPos = Vec3();
	cplane_t		Plane = {};
	float			flFraction = 0.0f;
	int				iContents = 0;
	unsigned short	DispFlags = 0;
	bool			bAllSolid = false;
	bool			bStartSolid = false;

	C_BaseTrace()
	{

	}

private:
	C_BaseTrace(const C_BaseTrace& othr);
};

class C_GameTrace : public C_BaseTrace
{
public:
	bool DidHitWorld() const;
	bool DidHitNonWorldEntity() const;
	int  GetEntityIndex() const;

	bool DidHit() const {
		return (flFraction < 1 || bAllSolid || bStartSolid);
	}

public:
	float			fraction_left_solid;
	csurface_t		surface;
	int				hit_group;
	short			physics_bone;
	C_BaseEntity* entity;
	int				hitbox;

	C_GameTrace() { }
	C_GameTrace(const C_GameTrace& othr);
};

struct CHudTexture;

class CFileWeaponInfo
{
public:

	CFileWeaponInfo();

	// Each game can override this to get whatever values it wants from the script.
	virtual void Parse(void* pKeyValuesData, const char* szWeaponName);


public:
	bool					bParsedScript;
	bool					bLoadedHudElements;

	// SHARED
	char					szClassName[MAX_WEAPON_STRING];
	char					szPrintName[MAX_WEAPON_STRING];			// Name for showing in HUD, etc.

	char					szViewModel[MAX_WEAPON_STRING];			// View model of this weapon
	char					szWorldModel[MAX_WEAPON_STRING];		// Model of this weapon seen carried by the player
	char					szAnimationPrefix[MAX_WEAPON_PREFIX];	// Prefix of the animations that should be used by the player carrying this weapon
	int						iSlot;									// inventory slot.
	int						iPosition;								// position in the inventory slot.
	int						iMaxClip1;								// max primary clip size (-1 if no clip)
	int						iMaxClip2;								// max secondary clip size (-1 if no clip)
	int						iDefaultClip1;							// amount of primary ammo in the gun when it's created
	int						iDefaultClip2;							// amount of secondary ammo in the gun when it's created
	int						iWeight;								// this value used to determine this weapon's importance in autoselection.
	int						iRumbleEffect;							// Which rumble effect to use when fired? (xbox)
	bool					bAutoSwitchTo;							// whether this weapon should be considered for autoswitching to
	bool					bAutoSwitchFrom;						// whether this weapon can be autoswitched away from when picking up another weapon or ammo
	int						iFlags;									// miscellaneous weapon flags
	char					szAmmo1[MAX_WEAPON_AMMO_NAME];			// "primary" ammo type
	char					szAmmo2[MAX_WEAPON_AMMO_NAME];			// "secondary" ammo type

	// Sound blocks
	char					aShootSounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];

	int						iAmmoType;
	int						iAmmo2Type;
	bool					m_bMeleeWeapon;		// Melee weapons can always "fire" regardless of ammo.

	// This tells if the weapon was built right-handed (defaults to true).
	// This helps cl_righthand make the decision about whether to flip the model or not.
	bool					m_bBuiltRightHanded;
	bool					m_bAllowFlipping;	// False to disallow flipping the model, regardless of whether
												// it is built left or right handed.

// CLIENT DLL
	// Sprite data, read from the data file
	int						iSpriteCount;
	CHudTexture* iconActive;
	CHudTexture* iconInactive;
	CHudTexture* iconAmmo;
	CHudTexture* iconAmmo2;
	CHudTexture* iconCrosshair;
	CHudTexture* iconAutoaim;
	CHudTexture* iconZoomedCrosshair;
	CHudTexture* iconZoomedAutoaim;
	CHudTexture* iconSmall;

	// TF2 specific
	bool					bShowUsageHint;							// if true, then when you receive the weapon, show a hint about it

// SERVER DLL

};

struct WeaponData_t
{
	int		m_nDamage;
	int		m_nBulletsPerShot;
	float	m_flRange;
	float	m_flSpread;
	float	m_flPunchAngle;
	float	m_flTimeFireDelay;				// Time to delay between firing
	float	m_flTimeIdle;					// Time to idle after firing
	float	m_flTimeIdleEmpty;				// Time to idle after firing last bullet in clip
	float	m_flTimeReloadStart;			// Time to start into a reload (ie. shotgun)
	float	m_flTimeReload;					// Time to reload
	bool	m_bDrawCrosshair;				// Should the weapon draw a crosshair
	int		m_iProjectile;					// The type of projectile this mode fires
	int		m_iAmmoPerShot;					// How much ammo each shot consumes
	float	m_flProjectileSpeed;			// Start speed for projectiles (nail, etc.); NOTE: union with something non-projectile
	float	m_flSmackDelay;					// how long after swing should damage happen for melee weapons
	bool	m_bUseRapidFireCrits;

	void Init(void)
	{
		m_nDamage = 0;
		m_nBulletsPerShot = 0;
		m_flRange = 0.0f;
		m_flSpread = 0.0f;
		m_flPunchAngle = 0.0f;
		m_flTimeFireDelay = 0.0f;
		m_flTimeIdle = 0.0f;
		m_flTimeIdleEmpty = 0.0f;
		m_flTimeReloadStart = 0.0f;
		m_flTimeReload = 0.0f;
		m_iProjectile = 0;
		m_iAmmoPerShot = 0;
		m_flProjectileSpeed = 0.0f;
		m_flSmackDelay = 0.0f;
		m_bUseRapidFireCrits = false;
	};
};

class CTFWeaponInfo : public CFileWeaponInfo
{
public:

	CTFWeaponInfo();
	~CTFWeaponInfo();

	virtual void Parse(void* pKeyValuesData, const char* szWeaponName);

	WeaponData_t const& GetWeaponData(int iWeapon) const { return m_WeaponData[iWeapon]; }

public:

	WeaponData_t	m_WeaponData[2];

	int		m_iWeaponType;

	// Grenade.
	bool	m_bGrenade;
	float	m_flDamageRadius;
	float	m_flPrimerTime;
	bool	m_bLowerWeapon;
	bool	m_bSuppressGrenTimer;

	// Skins
	bool	m_bHasTeamSkins_Viewmodel;
	bool	m_bHasTeamSkins_Worldmodel;

	// Muzzle flash
	char	m_szMuzzleFlashModel[128];
	float	m_flMuzzleFlashModelDuration;
	char	m_szMuzzleFlashParticleEffect[128];

	// Tracer
	char	m_szTracerEffect[128];

	// Eject Brass
	bool	m_bDoInstantEjectBrass;
	char	m_szBrassModel[128];

	// Explosion Effect
	char	m_szExplosionSound[128];
	char	m_szExplosionEffect[128];
	char	m_szExplosionPlayerEffect[128];
	char	m_szExplosionWaterEffect[128];

	bool	m_bDontDrop;
};

class IMaterialVar;

class IMaterial
{
public:
	virtual const char* GetName() const = 0;
	virtual const char* GetTextureGroupName() const = 0;

	virtual EPreviewImageRetVal GetPreviewImageProperties(int* width, int* height,
		EImageFormat* imageFormat, bool* isTranslucent) const = 0;

	virtual EPreviewImageRetVal GetPreviewImage(unsigned char* data,
		int width, int height,
		EImageFormat imageFormat) const = 0;

	virtual int				GetMappingWidth() = 0;
	virtual int				GetMappingHeight() = 0;
	virtual int				GetNumAnimationFrames() = 0;
	virtual bool			InMaterialPage(void) = 0;
	virtual	void			GetMaterialOffset(float* pOffset) = 0;
	virtual void			GetMaterialScale(float* pScale) = 0;
	virtual IMaterial* GetMaterialPage(void) = 0;
	virtual IMaterialVar* FindVar(const char* varName, bool* found, bool complain = true) = 0;
	virtual void			IncrementReferenceCount(void) = 0;
	virtual void			DecrementReferenceCount(void) = 0;
	virtual int 			GetEnumerationID(void) const = 0;
	virtual void			GetLowResColorSample(float s, float t, float* color) const = 0;
	virtual void			RecomputeStateSnapshots() = 0;
	virtual bool			IsTranslucent() = 0;
	virtual bool			IsAlphaTested() = 0;
	virtual bool			IsVertexLit() = 0;
	virtual VertexFormat_t	GetVertexFormat() const = 0;
	virtual bool			HasProxy(void) const = 0;
	virtual bool			UsesEnvCubemap(void) = 0;
	virtual bool			NeedsTangentSpace(void) = 0;
	virtual bool			NeedsPowerOfTwoFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			NeedsFullFrameBufferTexture(bool bCheckSpecificToThisFrame = true) = 0;
	virtual bool			NeedsSoftwareSkinning(void) = 0;
	virtual void			AlphaModulate(float alpha) = 0;
	virtual void			ColorModulate(float r, float g, float b) = 0;
	virtual void			SetMaterialVarFlag(EMaterialVarFlags flag, bool on) = 0;
	virtual bool			GetMaterialVarFlag(EMaterialVarFlags flag) const = 0;
	virtual void			GetReflectivity(Vec3& reflect) = 0;
	virtual bool			GetPropertyFlag(MaterialPropertyTypes_t type) = 0;
	virtual bool			IsTwoSided() = 0;
	virtual void			SetShader(const char* pShaderName) = 0;
	virtual int				GetNumPasses(void) = 0;
	virtual int				GetTextureMemoryBytes(void) = 0;
	virtual void			Refresh() = 0;
	virtual bool			NeedsLightmapBlendAlpha(void) = 0;
	virtual bool			NeedsSoftwareLighting(void) = 0;
	virtual int				ShaderParamCount() const = 0;
	virtual IMaterialVar** GetShaderParams(void) = 0;
	virtual bool			IsErrorMaterial() const = 0;
	virtual void			SetUseFixedFunctionBakedLighting(bool bEnable) = 0;
	virtual float			GetAlphaModulation() = 0;
	virtual void			GetColorModulation(float* r, float* g, float* b) = 0;
	virtual MorphFormat_t	GetMorphFormat() const = 0;
	virtual IMaterialVar* FindVarFast(char const* pVarName, unsigned int* pToken) = 0;
	virtual void			SetShaderAndParams(void* pKeyValues) = 0;
	virtual const char* GetShaderName() const = 0;
	virtual void			DeleteIfUnreferenced() = 0;
	virtual bool			IsSpriteCard() = 0;
	virtual void			CallBindProxy(void* proxyData) = 0;
	virtual IMaterial* CheckProxyReplacement(void* proxyData) = 0;
	virtual void			RefreshPreservingMaterialVars() = 0;
	virtual bool			WasReloadedFromWhitelist() = 0;
	virtual bool			IsPrecached() const = 0;
};

class IMaterialVar
{
public:
	virtual ITexture* GetTextureValue(void) = 0;

	virtual char const* GetName(void) const = 0;
	virtual MaterialVarSym_t	GetNameAsSymbol() const = 0;

	virtual void			SetFloatValue(float val) = 0;

	virtual void			SetIntValue(int val) = 0;

	virtual void			SetStringValue(char const* val) = 0;
	virtual char const* GetStringValue(void) const = 0;

	// Use FourCC values to pass app-defined data structures between
	// the proxy and the shader. The shader should ignore the data if
	// its FourCC type not correct.	
	virtual void			SetFourCCValue(void* type, void* pData) = 0;
	virtual void			GetFourCCValue(void* type, void** ppData) = 0;

	// Vec (dim 2-4)
	virtual void			SetVecValue(float const* val, int numcomps) = 0;
	virtual void			SetVecValue(float x, float y) = 0;
	virtual void			SetVecValue(float x, float y, float z) = 0;
	virtual void			SetVecValue(float x, float y, float z, float w) = 0;
	virtual void			GetLinearVecValue(float* val, int numcomps) const = 0;

	// revisit: is this a good interface for textures?
	virtual void			SetTextureValue(ITexture*) = 0;

	virtual IMaterial* GetMaterialValue(void) = 0;
	virtual void			SetMaterialValue(IMaterial*) = 0;

	virtual bool			IsDefined() const = 0;
	virtual void			SetUndefined() = 0;

	// Matrix
	virtual void			SetMatrixValue(VMatrix const& matrix) = 0;
	virtual const VMatrix& GetMatrixValue() = 0;
	virtual bool			MatrixIsIdentity() const = 0;

	// Copy....
	virtual void			CopyFrom(IMaterialVar* pMaterialVar) = 0;

	virtual void			SetValueAutodetectType(char const* val) = 0;

	virtual IMaterial* GetOwningMaterial() = 0;

	//set just 1 component
	virtual void			SetVecComponentValue(float fVal, int nComponent) = 0;

protected:
	virtual int				GetIntValueInternal(void) const = 0;
	virtual float			GetFloatValueInternal(void) const = 0;
	virtual float const* GetVecValueInternal() const = 0;
	virtual void			GetVecValueInternal(float* val, int numcomps) const = 0;
	virtual int				VectorSizeInternal() const = 0;
};

class KeyValues
{
public:
	int m_iKeyName;
	char* m_sValue;
	wchar_t* m_wsValue;

	union {
		int m_iValue;
		float m_flValue;
		void* m_pValue;
		unsigned char m_Color[4];
	};

	char m_iDataType;
	char m_bHasEscapeSequences;
	char m_bEvaluateConditionals;
	char unused[1];

	KeyValues* m_pPeer;
	KeyValues* m_pSub;
	KeyValues* m_pChain;
};

class C_VerifiedUserCmd
{
public:
	C_UserCmd  m_cmd;
	CRC32_t   m_crc;
};

class C_SourceInput
{
	std::uint8_t __pad[sizeof(std::int32_t) * 49];

public:
	C_UserCmd* cmds;
	C_VerifiedUserCmd* verified_cmds;
};

class ILuaCallback
{
public:

	/*0*/	virtual void* CreateLuaObject() = 0;
	/*1*/	virtual void DestroyLuaObject(void* pObject) = 0;

	/*2*/	virtual void ErrorPrint(const char* strError) = 0;
	/*3*/	virtual void Msg(const char* strMsg) = 0;

	/*4*/	virtual bool CanRunScript(const char* strFilename, unsigned long CRC) = 0;
	/*5*/	virtual void onRunScript(const char* strFilename, bool bRun, const char* strScriptContents) = 0;
};

class C_LuaInterface
{
public:
	/*0*/	virtual int Top(void) = 0;
	/*1*/	virtual void* Push(int iStackPos) = 0;
	/*2*/	virtual void* Pop(int iStackPos) = 0;
	/*3*/	virtual void* GetTable(int iStackPos) = 0;
	/*4*/	virtual void* const GetField(int iStackPos, char const*) = 0;
	/*5*/	virtual void SetField(int iStackPos, char const*) = 0;
	/*6*/	virtual void* CreateTable(void) = 0;
	/*7*/	virtual void* SetTable(int) = 0;
	/*8*/	virtual void* SetMetaTable(int) = 0;
	/*9*/	virtual void* GetMetaTable(int) = 0;
	/*10*/	virtual void* Call(int iStackPos, int) = 0;
	/*11*/	virtual void* PCall(int, int, int) = 0;
	/*12*/	virtual void* Equal(int, int) = 0;
	/*13*/	virtual void* RawEqual(int, int) = 0;
	/*14*/	virtual void* Insert(int) = 0;
	/*15*/	virtual void* Remove(int) = 0;
	/*16*/	virtual void* Next(int) = 0;
	/*17*/	virtual void* NewUserdata(unsigned int) = 0;
	/*18*/	virtual void* ThrowError(char const*) = 0;
	/*19*/	virtual void* CheckType(int, int) = 0;
	/*20*/	virtual void* ArgError(int, char const*) = 0;
	/*21*/	virtual void* RawGet(int iStackPos) = 0;
	/*22*/	virtual void* RawSet(int iStackPost) = 0;
	/*23*/	virtual const char* GetString(int = -1, unsigned int* iOutLen = NULL) = 0;
	/*24*/	virtual double GetNumber(int i = -1) = 0;
	/*25*/	virtual bool GetBool(int i = -1) = 0;
	/*26*/	virtual void* GetCFunction(int) = 0;
	/*27*/	virtual void* GetUserdata(int) = 0;
	/*28*/	virtual void PushNil(void) = 0;
	/*29*/	virtual void PushString(char const*, unsigned int) = 0;
	/*30*/	virtual void PushNumber(double) = 0;
	/*31*/	virtual void PushBool(bool) = 0;
	/*32*/	virtual void PushCFunction(int (*)(void*)) = 0;
	/*33*/	virtual void PushCClosure(int (*)(void*), int) = 0;
	/*34*/	virtual void PushUserdata(void*) = 0;
	/*35*/	virtual void* ReferenceCreate(void) = 0;
	/*36*/	virtual void* ReferenceFree(int) = 0;
	/*37*/	virtual void* ReferencePush(int) = 0;
	/*38*/	virtual void* PushSpecial(int) = 0;
	/*39*/	virtual bool IsType(int, LuaObjectType) = 0;
	/*40*/	virtual LuaObjectType GetType(int) = 0;
	/*41*/	virtual void* GetTypeName(int) = 0;
	/*42*/	virtual void* CreateMetaTableType(char const*, int) = 0;
	/*43*/	virtual void* CheckString(int) = 0;
	/*44*/	virtual void* CheckNumber(int) = 0;
	/*45*/	virtual void* ObjLen(int) = 0;
	/*46*/	virtual void* GetAngle(int) = 0;
	/*47*/	virtual void* GetVector(int) = 0;
	/*48*/	virtual void* PushAngle(Vec3 const&) = 0;
	/*49*/	virtual void* PushVector(Vec3 const&) = 0;
	/*50*/	virtual void* SetState(void*) = 0;
	/*51*/	virtual void* CreateMetaTable(char const*) = 0;
	/*52*/	virtual void* PushMetaTable(int) = 0;
	/*53*/	virtual void* PushUserType(void*, int) = 0;
	/*54*/	virtual void* SetUserType(int, void*) = 0;
	/*55*/	virtual void* Init(ILuaCallback*, bool) = 0;
	/*56*/	virtual void* Shutdown(void) = 0;
	/*57*/	virtual void* Cycle(void) = 0;
	/*58*/	virtual void* Global(void) = 0;
	/*59*/	virtual void* GetObjectt(int) = 0;
	/*60*/	virtual void* PushLuaObject(void*) = 0;
	/*61*/	virtual void* PushLuaFunction(int (*)(void*)) = 0;
	/*62*/	virtual void* LuaError(char const*, int) = 0;
	/*63*/	virtual void* TypeError(char const*, int) = 0;
	/*64*/	virtual void* CallInternal(int, int) = 0;
	/*65*/	virtual void* CallInternalNoReturns(int) = 0;
	/*66*/	virtual void* CallInternalGetBool(int) = 0;
	/*67*/	virtual void* CallInternalGetString(int) = 0;
	/*68*/	virtual void* CallInternalGet(int, void*) = 0;
	/*69*/	virtual void* NewGlobalTable(char const*) = 0;
	/*70*/	virtual void* NewTemporaryObject(void) = 0;
	/*71*/	virtual bool isUserData(int) = 0;
	/*72*/	virtual void* GetMetaTableObject(char const*, int) = 0;
	/*73*/	virtual void* GetMetaTableObject(int) = 0;
	/*74*/	virtual void* GetReturn(int) = 0;
	/*75*/	virtual bool IsServer(void) = 0;
	/*76*/	virtual bool IsClient(void) = 0;
	/*77*/	virtual bool IsMenu(void) = 0;
	/*78*/	virtual void* DestroyObject(void*) = 0;
	/*79*/	virtual void* CreateObject(void) = 0;
	/*80*/	virtual void* SetMember(void*, void*, void*) = 0;
	/*81*/	virtual void* GetNewTable(void) = 0;
	/*82*/	virtual void* SetMember(void*, float) = 0;
	/*83*/	virtual void* SetMember(void*, float, void*) = 0;
	/*84*/	virtual void* SetMember(void*, char const*) = 0;
	/*85*/	virtual void* SetMember(void*, char const*, void*) = 0;
	/*86*/	virtual void* SetType(unsigned char) = 0;
	/*87*/	virtual void* PushLong(long) = 0;
	/*88*/	virtual void* GetFlags(int) = 0;
	/*89*/	virtual void* FindOnObjectsMetaTable(int, int) = 0;
	/*90*/	virtual void* FindObjectOnTable(int, int) = 0;
	/*91*/	virtual void* SetMemberFast(void*, int, int) = 0;
	/*92*/	virtual void* RunString(char const*, char const*, char const*, bool, bool) = 0;
	/*93*/	virtual bool IsEqual(void*, void*) = 0;
	/*94*/	virtual void* Error(char const*) = 0;
	/*95*/	virtual void* GetStringOrError(int) = 0;
	/*96*/	virtual void* RunLuaModule(char const*) = 0;
	/*97*/	virtual void* FindAndRunScript(char const*, bool, bool, char const*, bool) = 0;
	/*98*/	virtual void* SetPathID(char const*) = 0;
	/*99*/	virtual void* GetPathID(void) = 0;
	/*100*/	virtual void* ErrorNoHalt(char const*, ...) = 0;
	/*101*/	virtual void* Msg(char const*, ...) = 0;
	/*102*/	virtual void* PushPath(char const*) = 0;
	/*103*/	virtual void* PopPath(void) = 0;
	/*104*/	virtual void* GetPath(void) = 0;
	/*105*/	virtual void* GetColor(int) = 0;
	/*106*/	virtual void* PushColor(Color_t) = 0;
	/*107*/	virtual void* GetStack(int, void*) = 0;
	/*108*/	virtual void* GetInfo(char const*, void*) = 0;
	/*109*/	virtual void* GetLocal(void*, int) = 0;
	/*110*/	virtual void* GetUpvalue(int, int) = 0;
	/*111*/	virtual void* RunStringEx(char const*, char const*, char const*, bool, bool, bool, bool) = 0;
	/*112*/	virtual void* GetDataString(int, void**) = 0;
	/*113*/	virtual void* ErrorFromLua(char const*, ...) = 0;
	/*114*/	virtual void* GetCurrentLocation(void) = 0;
	/*115*/	virtual void* MsgColour(Color_t const&, char const*, ...) = 0;
	/*116*/	virtual void* GetCurrentFile(std::string, std::string) = 0;
	/*117*/	virtual void* CompileString(void*, std::string const&) = 0;
	/*118*/	virtual void* CallFunctionProtected(int, int, bool) = 0;
	/*119*/	virtual void* Require(char const*) = 0;
	/*120*/	virtual void* GetActualTypeName(int) = 0;
	/*121*/	virtual void* PreCreateTable(int, int) = 0;
	/*122*/	virtual void* PushPooledString(int) = 0;
	/*123*/	virtual void* GetPooledString(int) = 0;
	/*124*/	virtual void* AddThreadedCall(void*) = 0;
	/*125*/	virtual void* AppendStackTrace(char*, unsigned long) = 0;
	/*126*/	virtual void* CreateConVar(char const*, char const*, char const*, int) = 0;
	/*127*/	virtual void* CreateConCommand(char const*, char const*, int, void (*)(void const*), int (*)(char const*, char(*)[128])) = 0;
};

enum LuaFindResult
{
	IHaveNoIdea
};

class C_LuaShared
{
public:
	/*0*/	virtual void* destr1() = 0;
	/*2*/	virtual void* Init(void* (*)(char const*, int*), bool, void*, void*) = 0;
	/*3*/	virtual void* Shutdown(void) = 0;
	/*4*/	virtual void* DumpStats(void) = 0;
	/*5*/	virtual C_LuaInterface* CreateLuaInterface(unsigned char, bool) = 0;
	/*6*/	virtual void* CloseLuaInterface(void*) = 0;
	/*7*/	virtual C_LuaInterface* GetLuaInterface(unsigned char) = 0;
	/*8*/	virtual void LoadFile(std::string const&, std::string const&, bool, bool) = 0;
	/*9*/	virtual void* GetCache(std::string const&) = 0;
	/*10*/	virtual void* MountLua(char const*) = 0;
	/*11*/	virtual void* MountLuaAdd(char const*, char const*) = 0;
	/*12*/	virtual void* UnMountLua(char const*) = 0;
	/*13*/	virtual void* SetFileContents(char const*, char const*) = 0;
	/*14*/	virtual void* SetLuaFindHook(void*) = 0;
	/*15*/	virtual void* FindScripts(std::string const&, std::string const&, std::vector<LuaFindResult>&) = 0;
	/*16*/	virtual void* GetStackTraces(void) = 0;
	/*17*/	virtual void* InvalidateCache(std::string const&) = 0;
	/*18*/	virtual void* EmptyCache(void) = 0;
};

class C_LuaAutoPop
{
	C_LuaInterface* i;
	int Top;
public:
	C_LuaAutoPop(C_LuaInterface* intr);
	~C_LuaAutoPop();
};


inline C_LuaAutoPop::C_LuaAutoPop(C_LuaInterface* intr)
{
	Top = intr->Top();
	i = intr;
}

inline C_LuaAutoPop::~C_LuaAutoPop()
{
	i->Pop(i->Top() - Top);
}
