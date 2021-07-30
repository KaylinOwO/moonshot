#pragma once
#include "../../Utils/Utils.h"

#include "Const.h"

struct Rect_t { int x, y, w, h; };
struct model_t;
class IClientRenderable;
typedef unsigned short ModelInstanceHandle_t;
struct StudioData_t;
struct StudioDecalHandle_t;
class Quaternion { };
class RadianEuler { };
struct mstudiobonecontroller_t {}; struct mstudioanimdesc_t {}; struct mstudioseqdesc_t {}; struct mstudiotexture_t {}; struct mstudioflexdesc_t {}; struct mstudiobodyparts_t {}; struct mstudioflexcontroller_t {};
struct mstudioattachment_t {}; struct LocalFlexController_t {}; struct mstudioflexrule_t {}; struct mstudioikchain_t {};
struct mstudiomouth_t {}; struct mstudioposeparamdesc_t {}; struct mstudiomodelgroup_t {}; struct virtualmodel_t {}; struct mstudioanimblock_t {};
struct mstudioflexcontrollerui_t {}; struct mstudiosrcbonetransform_t {}; struct mstudioboneflexdriver_t {};
struct mstudiolinearbone_t {}; struct mstudioiklock_t {};

struct studiohdr2_t
{
	// NOTE: For forward compat, make sure any methods in this struct
	// are also available in studiohdr_t so no leaf code ever directly references
	// a studiohdr2_t structure
	int numsrcbonetransform;
	int srcbonetransformindex;

	int	illumpositionattachmentindex;
	inline int			IllumPositionAttachmentIndex() const { return illumpositionattachmentindex; }

	float flMaxEyeDeflection;
	inline float		MaxEyeDeflection() const { return flMaxEyeDeflection != 0.0f ? flMaxEyeDeflection : 0.866f; } // default to cos(30) if not set

	int linearboneindex;
	inline mstudiolinearbone_t* pLinearBones() const { return (linearboneindex) ? (mstudiolinearbone_t*)(((byte*)this) + linearboneindex) : NULL; }

	int sznameindex;
	inline char* pszName() { return (sznameindex) ? (char*)(((byte*)this) + sznameindex) : NULL; }

	int m_nBoneFlexDriverCount;
	int m_nBoneFlexDriverIndex;
	inline mstudioboneflexdriver_t* pBoneFlexDriver(int i) const { return (mstudioboneflexdriver_t*)(((byte*)this) + m_nBoneFlexDriverIndex) + i; }

	int reserved[56];
};

struct mstudiobbox_t
{
	int					bone;
	int					group;				// intersection group
	Vec3				bbmin;				// bounding box
	Vec3				bbmax;
	int					szhitboxnameindex;	// offset to the name of the hitbox.
	int					unused[8];

	const char* pszHitboxName()
	{
		if (szhitboxnameindex == 0)
			return "";

		return ((const char*)this) + szhitboxnameindex;
	}

	mstudiobbox_t() {}

private:
	// No copy constructors allowed
	mstudiobbox_t(const mstudiobbox_t& vOther);
};

struct mstudiobone_t
{
	int                                     sznameindex;
	inline char* const GetName(void) const { return ((char*)this) + sznameindex; }
	int                                     parent;
	int                                     bonecontroller[6];

	Vec3                          pos;
	float                           quat[4];
	Vec3                          rot;
	Vec3                          posscale;
	Vec3                          rotscale;

	matrix3x4					poseToBone;
	float                           qAlignment[4];
	int                                     flags;
	int                                     proctype;
	int                                     procindex;              // procedural rule
	mutable int                     physicsbone;    // index into physically simulated bone
	inline void* GetProcedure() const { if (procindex == 0) return NULL; else return  (void*)(((BYTE*)this) + procindex); };
	int                                     surfacepropidx; // index into string tablefor property name
	inline char* const GetSurfaceProps(void) const { return ((char*)this) + surfacepropidx; }
	int                                     contents;               // See BSPFlags.h for the contents flags

	int                                     unused[8];              // remove as appropriate
};

struct mstudiohitboxset_t
{
	int sznameindex;
	inline char* const name(void) const { return ((char*)this) + sznameindex; }
	int numhitboxes;
	int hitboxindex;
	mstudiobbox_t* hitbox(int i) const { return (mstudiobbox_t*)(((BYTE*)this) + hitboxindex) + i; };
};


struct studiohdr_t
{
	int					id;
	int					version;

	int					checksum;		// this has to be the same in the phy and vtx files to load!

	inline const char* pszName(void) const { if (studiohdr2index && pStudioHdr2()->pszName()) return pStudioHdr2()->pszName(); else return name; }
	char				name[64];
	int					length;


	Vec3				eyeposition;	// ideal eye position

	Vec3				illumposition;	// illumination center

	Vec3				hull_min;		// ideal movement hull size
	Vec3				hull_max;

	Vec3				view_bbmin;		// clipping bounding box
	Vec3				view_bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;
	inline mstudiobone_t* pBone(int i) const { return (mstudiobone_t*)(((byte*)this) + boneindex) + i; };
	int					RemapSeqBone(int iSequence, int iLocalBone) const;	// maps local sequence bone to global bone
	int					RemapAnimBone(int iAnim, int iLocalBone) const;		// maps local animations bone to global bone

	int					numbonecontrollers;		// bone controllers
	int					bonecontrollerindex;
	inline mstudiobonecontroller_t* pBonecontroller(int i) const { return (mstudiobonecontroller_t*)(((byte*)this) + bonecontrollerindex) + i; };

	int					numhitboxsets;
	int					hitboxsetindex;

	// Look up hitbox set by index
	mstudiohitboxset_t* pHitboxSet(int i) const
	{
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline mstudiobbox_t* pHitbox(int i, int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);
		if (!s)
			return NULL;

		return s->hitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline int			iHitboxCount(int set) const
	{
		mstudiohitboxset_t const* s = pHitboxSet(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};

	// file local animations? and sequences
//private:
	int					numlocalanim;			// animations/poses
	int					localanimindex;		// animation descriptions
	inline mstudioanimdesc_t* pLocalAnimdesc(int i) const { if (i < 0 || i >= numlocalanim) i = 0; return (mstudioanimdesc_t*)(((byte*)this) + localanimindex) + i; };

	int					numlocalseq;				// sequences
	int					localseqindex;
	inline mstudioseqdesc_t* pLocalSeqdesc(int i) const { if (i < 0 || i >= numlocalseq) i = 0; return (mstudioseqdesc_t*)(((byte*)this) + localseqindex) + i; };

	//public:
	bool				SequencesAvailable() const;
	int					GetNumSeq() const;
	mstudioanimdesc_t& pAnimdesc(int i) const;
	mstudioseqdesc_t& pSeqdesc(int i) const;
	int					iRelativeAnim(int baseseq, int relanim) const;	// maps seq local anim reference to global anim index
	int					iRelativeSeq(int baseseq, int relseq) const;		// maps seq local seq reference to global seq index

//private:
	mutable int			activitylistversion;	// initialization flag - have the sequences been indexed?
	mutable int			eventsindexed;
	//public:
	int					GetSequenceActivity(int iSequence);
	void				SetSequenceActivity(int iSequence, int iActivity);
	int					GetActivityListVersion(void);
	void				SetActivityListVersion(int version) const;
	int					GetEventListVersion(void);
	void				SetEventListVersion(int version);

	// raw textures
	int					numtextures;
	int					textureindex;
	inline mstudiotexture_t* pTexture(int i) const { return (mstudiotexture_t*)(((byte*)this) + textureindex) + i; };


	// raw textures search paths
	int					numcdtextures;
	int					cdtextureindex;
	inline char* pCdtexture(int i) const { return (((char*)this) + *((int*)(((byte*)this) + cdtextureindex) + i)); };

	// replaceable textures tables
	int					numskinref;
	int					numskinfamilies;
	int					skinindex;
	inline short* pSkinref(int i) const { return (short*)(((byte*)this) + skinindex) + i; };

	int					numbodyparts;
	int					bodypartindex;
	inline mstudiobodyparts_t* pBodypart(int i) const { return (mstudiobodyparts_t*)(((byte*)this) + bodypartindex) + i; };

	// queryable attachable points
//private:
	int					numlocalattachments;
	int					localattachmentindex;
	inline mstudioattachment_t* pLocalAttachment(int i) const { return (mstudioattachment_t*)(((byte*)this) + localattachmentindex) + i; };
	//public:
	int					GetNumAttachments(void) const;
	const mstudioattachment_t& pAttachment(int i) const;
	int					GetAttachmentBone(int i);
	// used on my tools in hlmv, not persistant
	void				SetAttachmentBone(int iAttachment, int iBone);

	// animation node to animation node transition graph
//private:
	int					numlocalnodes;
	int					localnodeindex;
	int					localnodenameindex;
	inline char* pszLocalNodeName(int iNode) const { return (((char*)this) + *((int*)(((byte*)this) + localnodenameindex) + iNode)); }
	inline byte* pLocalTransition(int i) const { return (byte*)(((byte*)this) + localnodeindex) + i; };

	//public:
	int					EntryNode(int iSequence);
	int					ExitNode(int iSequence);
	char* pszNodeName(int iNode);
	int					GetTransition(int iFrom, int iTo) const;

	int					numflexdesc;
	int					flexdescindex;
	inline mstudioflexdesc_t* pFlexdesc(int i) const { return (mstudioflexdesc_t*)(((byte*)this) + flexdescindex) + i; };

	int					numflexcontrollers;
	int					flexcontrollerindex;
	inline mstudioflexcontroller_t* pFlexcontroller(LocalFlexController_t i) const { return 0; };

	int					numflexrules;
	int					flexruleindex;
	inline mstudioflexrule_t* pFlexRule(int i) const { return (mstudioflexrule_t*)(((byte*)this) + flexruleindex) + i; };

	int					numikchains;
	int					ikchainindex;
	inline mstudioikchain_t* pIKChain(int i) const { return (mstudioikchain_t*)(((byte*)this) + ikchainindex) + i; };

	int					nummouths;
	int					mouthindex;
	inline mstudiomouth_t* pMouth(int i) const { return (mstudiomouth_t*)(((byte*)this) + mouthindex) + i; };

	//private:
	int					numlocalposeparameters;
	int					localposeparamindex;
	inline mstudioposeparamdesc_t* pLocalPoseParameter(int i) const { return (mstudioposeparamdesc_t*)(((byte*)this) + localposeparamindex) + i; };
	//public:
	int					GetNumPoseParameters(void) const;
	const mstudioposeparamdesc_t& pPoseParameter(int i);
	int					GetSharedPoseParameter(int iSequence, int iLocalPose) const;

	int					surfacepropindex;
	inline char* const pszSurfaceProp(void) const { return ((char*)this) + surfacepropindex; }

	// Key values
	int					keyvalueindex;
	int					keyvaluesize;
	inline const char* KeyValueText(void) const { return keyvaluesize != 0 ? ((char*)this) + keyvalueindex : NULL; }

	int					numlocalikautoplaylocks;
	int					localikautoplaylockindex;
	inline mstudioiklock_t* pLocalIKAutoplayLock(int i) const { return (mstudioiklock_t*)(((byte*)this) + localikautoplaylockindex) + i; };

	int					GetNumIKAutoplayLocks(void) const;
	const mstudioiklock_t& pIKAutoplayLock(int i);
	int					CountAutoplaySequences() const;
	int					CopyAutoplaySequences(unsigned short* pOut, int outCount) const;
	int					GetAutoplayList(unsigned short** pOut) const;

	// The collision model mass that jay wanted
	float				mass;
	int					contents;

	// external animations, models, etc.
	int					numincludemodels;
	int					includemodelindex;
	inline mstudiomodelgroup_t* pModelGroup(int i) const { return (mstudiomodelgroup_t*)(((byte*)this) + includemodelindex) + i; };
	// implementation specific call to get a named model
	const studiohdr_t* FindModel(void** cache, char const* modelname) const;

	// implementation specific back pointer to virtual data
	mutable void* virtualModel;
	virtualmodel_t* GetVirtualModel(void) const;

	// for demand loaded animation blocks
	int					szanimblocknameindex;
	inline char* const pszAnimBlockName(void) const { return ((char*)this) + szanimblocknameindex; }
	int					numanimblocks;
	int					animblockindex;
	inline mstudioanimblock_t* pAnimBlock(int i) const { return (mstudioanimblock_t*)(((byte*)this) + animblockindex) + i; };
	mutable void* animblockModel;
	byte* GetAnimBlock(int i) const;

	int					bonetablebynameindex;
	inline const byte* GetBoneTableSortedByName() const { return (byte*)this + bonetablebynameindex; }

	// used by tools only that don't cache, but persist mdl's peer data
	// engine uses virtualModel to back link to cache pointers
	void* pVertexBase;
	void* pIndexBase;

	// if STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT is set,
	// this value is used to calculate directional components of lighting 
	// on static props
	byte				constdirectionallightdot;

	// set during load of mdl data to track *desired* lod configuration (not actual)
	// the *actual* clamped root lod is found in studiohwdata
	// this is stored here as a global store to ensure the staged loading matches the rendering
	byte				rootLOD;

	// set in the mdl data to specify that lod configuration should only allow first numAllowRootLODs
	// to be set as root LOD:
	//	numAllowedRootLODs = 0	means no restriction, any lod can be set as root lod.
	//	numAllowedRootLODs = N	means that lod0 - lod(N-1) can be set as root lod, but not lodN or lower.
	byte				numAllowedRootLODs;

	byte				unused[1];

	int					unused4; // zero out if version < 47

	int					numflexcontrollerui;
	int					flexcontrolleruiindex;
	mstudioflexcontrollerui_t* pFlexControllerUI(int i) const { return (mstudioflexcontrollerui_t*)(((byte*)this) + flexcontrolleruiindex) + i; }

	float				flVertAnimFixedPointScale;
	inline float		VertAnimFixedPointScale() const { return (flags & 6) ? flVertAnimFixedPointScale : 1.0f / 4096.0f; }

	int					unused3[1];

	// FIXME: Remove when we up the model version. Move all fields of studiohdr2_t into studiohdr_t.
	int					studiohdr2index;
	studiohdr2_t* pStudioHdr2() const { return (studiohdr2_t*)(((byte*)this) + studiohdr2index); }

	// Src bone transforms are transformations that will convert .dmx or .smd-based animations into .mdl-based animations
	int					NumSrcBoneTransforms() const { return studiohdr2index ? pStudioHdr2()->numsrcbonetransform : 0; }
	const mstudiosrcbonetransform_t* SrcBoneTransform(int i) const { return (mstudiosrcbonetransform_t*)(((byte*)this) + pStudioHdr2()->srcbonetransformindex) + i; }

	inline int			IllumPositionAttachmentIndex() const { return studiohdr2index ? pStudioHdr2()->IllumPositionAttachmentIndex() : 0; }

	inline float		MaxEyeDeflection() const { return studiohdr2index ? pStudioHdr2()->MaxEyeDeflection() : 0.866f; } // default to cos(30) if not set

	inline mstudiolinearbone_t* pLinearBones() const { return studiohdr2index ? pStudioHdr2()->pLinearBones() : NULL; }

	inline int			BoneFlexDriverCount() const { return studiohdr2index ? pStudioHdr2()->m_nBoneFlexDriverCount : 0; }
	inline const mstudioboneflexdriver_t* BoneFlexDriver(int i) const { return studiohdr2index ? pStudioHdr2()->pBoneFlexDriver(i) : NULL; }

	// NOTE: No room to add stuff? Up the .mdl file format version 
	// [and move all fields in studiohdr2_t into studiohdr_t and kill studiohdr2_t],
	// or add your stuff to studiohdr2_t. See NumSrcBoneTransforms/SrcBoneTransform for the pattern to use.
	int					unused2[1];

	studiohdr_t() {}

private:
	// No copy constructors allowed
	studiohdr_t(const studiohdr_t& vOther);

	friend struct virtualmodel_t;
};








struct Ray_t
{
	VectorAligned vaStartP;
	VectorAligned vaDelta;
	VectorAligned vaStartOffset;
	VectorAligned vaExtents;

	bool IsRay;
	bool IsSwept;

	void Init(const Vec3& source, const Vec3& destination)
	{
		vaDelta = destination - source;

		IsSwept = (vaDelta.LenghtSqr() != 0);

		vaExtents.Set();
		IsRay = true;

		vaStartOffset.Set();
		vaStartP = source;
	}

	void Init(const Vec3& source, const Vec3& destination, const Vec3& min, const Vec3& max)
	{
		vaDelta = destination - source;

		IsSwept = (vaDelta.LenghtSqr() != 0);

		vaExtents = max - min;
		vaExtents *= 0.5f;
		IsRay = (vaExtents.LenghtSqr() < 1e-6);

		vaStartOffset = min + max;

		vaStartOffset *= 0.5f;
		vaStartP = source - vaStartOffset;
		vaStartOffset *= -1.0f;
	}
};

struct cplane_t
{
	Vec3 normal;
	float dist;
	BYTE type;
	BYTE sign_bits;
	BYTE pad[2];
};

struct csurface_t
{
	const char* name;
	short surface_props;
	unsigned short flags;
};

struct PlayerInfo_t
{
	char name[128];
	int userID; // local server user ID, unique while server is running <- THIS IS NOT STEAMID
	char guid[33]; // that is Steamid
	unsigned int friendsid;
	char friendsname[128];
	bool fakeplayer; // true, if player is a bot controlled by game.dll
	bool ishltv;
	unsigned int customfiles[4]; // custom files CRC for this player
	unsigned char filesdownloaded; // this counter increases each time the server downloaded a new file
};

struct DrawModelState_t
{
	studiohdr_t* m_pStudioHdr;
	StudioData_t* m_pStudioData;
	IClientRenderable* m_pRenderable;
	const matrix3x4* m_pModelToWorld;
	StudioDecalHandle_t* m_hDecals;
	int					 m_DrawFlags;
	int					 m_Lod;
};

struct ModelRenderInfo_t
{
	Vec3			      m_vOrigin;
	Vec3			      m_vAngles;
	IClientRenderable* m_pRenderable;
	const model_t* m_pModel;
	const matrix3x4* m_pModelToWorld;
	const matrix3x4* m_pLightingOffset;
	const Vec3* m_pLightingOrigin;
	int				      m_nFlags;
	int				      m_nEntIndex;
	int				      m_nSkin;
	int					  m_nBody;
	int				      m_nHitboxSet;
	ModelInstanceHandle_t m_hMdlInstance;

	ModelRenderInfo_t()
	{
		m_pModelToWorld = NULL;
		m_pLightingOffset = NULL;
		m_pLightingOrigin = NULL;
	}
};

struct StaticPropRenderInfo_t
{
	const matrix3x4* m_pModelToWorld;
	const model_t* m_pModel;
	IClientRenderable* m_pRenderable;
	Vec3* m_pLightOrigin;
	short					m_sSkin;
	ModelInstanceHandle_t	m_hMdlInstance;
};

struct vrect_t
{
	int				x, y, width, height;
	vrect_t* pnext;
};
