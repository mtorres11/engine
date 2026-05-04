//----------------------------------------------------------------------------------
// CPhysicsManager class
// Author: Enric Vergara
//
// Description:
// This secures availability of the necessary physic functions.
// It internally uses the PhysX library.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSICS_MANAGER_H_
#define INC_PHYSICS_MANAGER_H_

#include "Math/Color.h"
#include "Math/MathUtils.h"
#include "MapManager/MapManager.h"
#include "Named/Named.h"
#include "Math/Vector3.h"
#include <vector>
#include <string>

//---Forward Declarations---
class NxPhysicsSDK;
class NxScene;
class NxControllerManager;
class CPhysicUserAllocator;
class CPhysicCookingMesh;
class CPhysicActor;
class NxActor;
class NxCCDSkeleton;
class CPhysicController;
class CRenderManager;
class CPhysicSphericalJoint;
class CPhysicRevoluteJoint;
class CPhysicFixedJoint;
class CPhysicControllerReport;
class CPhysicTriggerReport;
class CScriptManager;
//--------------------------

enum PHYSX_ENUM_TYPE
{
  PX_CG_CHARACTER,
  PX_CG_ENEMY,
  PX_CG_SCENARIO,
  PX_CG_STATIC_ELEMENT_DENSITY,
  PX_CG_STATIC_ELEMENT,
  PX_CG_ITEM,
  PX_CG_TRIGGER,
};
/*MASK*/
#define PX_MSK_NO_COLLITION             (0)
#define PX_MSK_CHARACTER                (1<<PX_CG_CHARACTER)
#define PX_MSK_ENEMY                    (1<<PX_CG_ENEMY)
#define PX_MSK_SCENARIO                 (1<<PX_CG_SCENARIO)
#define PX_MSK_STATIC_ELEMENT_DENSITY   (1<<PX_CG_STATIC_ELEMENT_DENSITY)
#define PX_MSK_STATIC_ELEMENT           (1<<PX_CG_STATIC_ELEMENT)
#define PX_MSK_ITEM                     (1<<PX_CG_ITEM)
#define PX_MSK_TRIGGER                  (1<<PX_CG_TRIGGER)

#define PX_MSK_ALL (PX_MSK_CHARACTER|PX_MSK_ENEMY|PX_MSK_SCENARIO|PX_MSK_STATIC_ELEMENT_DENSITY|PX_MSK_STATIC_ELEMENT|PX_MSK_ITEM|PX_CG_TRIGGER)
#define PX_MSK_ALL_SCENARIO (PX_MSK_SCENARIO|PX_MSK_STATIC_ELEMENT_DENSITY|PX_MSK_STATIC_ELEMENT)
#define PX_MSK_ENEMY_CONTROLLER (PX_MSK_CHARACTER|PX_MSK_ENEMY|PX_MSK_SCENARIO|PX_MSK_STATIC_ELEMENT_DENSITY|PX_MSK_STATIC_ELEMENT)

#define PX_MSK_VOMITO_ZOMBIE (PX_MSK_CHARACTER|PX_MSK_ENEMY|PX_MSK_SCENARIO|PX_MSK_STATIC_ELEMENT_DENSITY|PX_MSK_STATIC_ELEMENT)


//---- Declaracion de nuevos tipos------------
struct SPhysicsInitParams
{
	SPhysicsInitParams::SPhysicsInitParams():	m_fSkinWidth(0.001f), m_fGravity(-9.81f), 
																						m_Restitution_DefMat(0.4f), m_StaticFriction_DefMat(0.5f),
																						m_DynamicFriction_DefMat(0.5f) {}
	float		m_fSkinWidth;
	float		m_fGravity;
	float		m_Restitution_DefMat;				//Restitution of the default material
	float		m_StaticFriction_DefMat;		//Static Friction of the default material
	float		m_DynamicFriction_DefMat;		//Dynamic Friction of the default material
};

struct SCollisionInfo
{
	float		m_fDistance;
	Vect3f	m_Normal;
	Vect3f	m_CollisionPoint;
};

class CPhysicUserData : public CNamed
{
public:
  CPhysicUserData(std::string Name): CNamed(Name)
    , m_bPaintPhysicObject(false)
    , m_ColorPhysicObject(colWHITE)
    {}
	virtual   ~CPhysicUserData(){/*Nothing*/}

	void					SetPaint	(bool flag)						{m_bPaintPhysicObject = flag;}
  void					SetColor	(const CColor& color) {m_ColorPhysicObject = color;}

	bool					GetPaint	() const							{return m_bPaintPhysicObject;}
	const CColor&	GetColor	() const							{return m_ColorPhysicObject;}

private:
	bool		m_bPaintPhysicObject;
	CColor	m_ColorPhysicObject;

};
//--------------------------------------------

class CPhysicsManager
{

public:
	//--- Init and End protocols	
	CPhysicsManager()	:	m_bIsOk(false), m_bDebugRenderMode(false),
											m_pPhysicsSDK(0), m_pScene(0),
											m_pControllerManager(0), m_pMyAllocator(0),
											m_pCookingMesh(0), m_InitParams() {}

	~CPhysicsManager() {Done();}

	bool								Init												();
	void								Done												();
	bool								IsOk												() const { return m_bIsOk; }
	void								SetInitParams								(const SPhysicsInitParams& initParams) {m_InitParams = initParams;}
  NxScene*						GetScene                    ()       {return m_pScene;}

	//----CScriptRegister interface-------------------
	//virtual void				RegisterFunctions					(CScriptManager* scriptManager);

	//--- Intersection Functions:
	CPhysicUserData*		RaycastClosestActor					(const Vect3f& posRay, Vect3f& dirRay, uint32 impactMask, CPhysicUserData* impactObject, SCollisionInfo& info);
	void								OverlapSphereActor					(float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> &impactObjects, uint32 GroupMask=0xffffffff, bool Accurate=false);
  void								OverlapBoxActor					    (Vect3f SideLength, Mat44f Transform, std::vector<CPhysicUserData*> &impactObjects, uint32 GroupMask=0xffffffff, bool Accurate=false);


	//--- Get CookingMesh
	CPhysicCookingMesh*	GetCookingMesh							() {return m_pCookingMesh;}


	//--- Rendering Stuff:
	void								DebugRender									(CRenderManager* render);
	void								SetDebugRenderMode					(bool flag) {m_bDebugRenderMode = flag;}
	bool								GetDebugRenderMode					() const		{return m_bDebugRenderMode ;}

	//----Update
	void								Update											(float elapsedTime);

	//--- Add/Release Actors
	bool								AddPhysicActor							(CPhysicActor* actor);
	bool								ReleasePhysicActor					(CPhysicActor* actor);

  //--- Get Physic Actor
  CPhysicActor *      GetPhysicActor              (std::string name);

	//--- Add/Release CharacterControllers
	bool								AddPhysicController					(CPhysicController* controller);
	bool								ReleasePhysicController			(CPhysicController* controller);

	//--- Add/Release Joints
	bool								AddPhysicSphericalJoint			(CPhysicSphericalJoint* joint);
	bool								RelasePhysicSphericalJoint	(CPhysicSphericalJoint* joint);
	bool								AddPhysicRevoluteJoint			(CPhysicRevoluteJoint* joint);
	bool								RelasePhysicRevoluteJoint		(CPhysicRevoluteJoint* joint);
	bool								AddPhysicFixedJoint					(CPhysicFixedJoint* joint);
	bool								RelasePhysicFixedJoint			(CPhysicFixedJoint* joint);

	//--- Create CCDSkeleton
	NxCCDSkeleton*			CreateCCDSkeleton						(float size);

	void								SetTriggerReport						(CPhysicTriggerReport* report);

  bool                LoadInvisibleBlockingBoxes    (std::string _sFileName);
  bool                ReloadInvisibleBlockingBoxes  ();
  void                SetInvisbleBoxPosition        (std::string _sName, Vect3f _V3fPos);

private:
	void								Release											();
	void								DrawActor										(NxActor* actor, CRenderManager* render);


private:
	bool									      m_bIsOk;
	bool									      m_bDebugRenderMode;

  CMapManager<CPhysicActor>     m_MapInvisibleBoxes;
  //std::vector<CPhysicActor*>     m_vInvisibleBlockingBoxes;
  std::vector<CPhysicUserData*>  m_vInvisibleBlockingBoxesUserData;

  std::string           m_sInvisibleBoxesFileName;

	//---PhysX------------------------------
	NxPhysicsSDK*					m_pPhysicsSDK;
	NxScene*							m_pScene;
	NxControllerManager*	m_pControllerManager;
	CPhysicUserAllocator*	m_pMyAllocator;
	CPhysicCookingMesh*		m_pCookingMesh;
	SPhysicsInitParams		m_InitParams;
	//-------------------------------------------
};

#endif // INC_PHYSICS_MANAGER_H_