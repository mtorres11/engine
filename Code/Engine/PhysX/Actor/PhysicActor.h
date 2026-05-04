//----------------------------------------------------------------------------------
// CPhysicActor class
// Author: Enric Vergara
//
// Description:
// Esta clase representa un actor físico.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_ACTOR_H_
#define INC_PHYSIC_ACTOR_H_

#include <vector>
#include "Math/MathUtils.h"
#include "Math/Vector3.h"

//---Forward Declarations---
class NxActor;
class NxActorDesc;
class NxBodyDesc;
class NxCCDSkeleton;
class NxTriangleMesh;
class NxBoxShapeDesc;
class NxBoxShapeDesc;
class NxTriangleMeshShapeDesc;
class NxCapsuleShapeDesc;
class NxSphereShapeDesc;
class NxPlaneShapeDesc;
class CPhysicUserData;
//--------------------------

class CPhysicActor
{
public:
	CPhysicActor::CPhysicActor(CPhysicUserData* userData);
	CPhysicActor::~CPhysicActor();

	CPhysicUserData*	GetUserData				() {return m_pUserData;}
	void							SetLinearVelocity	(const Vect3f& velocity);
	Vect3f						GetPosition				();
	void							CreateBody				(float density, float angularDamping = 0.5f);
	void							SetGlobalPosition	(const Vect3f& pos = Vect3f(0.f,0.f,0.f));

	//---AddShape Functions-----
	void							AddSphereShape		(float radius, const Vect3f& localPos = v3fZERO, NxCCDSkeleton* skeleton = 0, uint32 group = 0);
	void							AddBoxSphape			(const Vect3f& size, const Vect3f& localPos = v3fZERO, NxCCDSkeleton* skeleton = 0, uint32 group = 0);
	void							AddCapsuleShape		(float radius, float height, const Vect3f& localPos = v3fZERO, NxCCDSkeleton* skeleton = 0, uint32 group = 0);
	void							AddMeshShape			(NxTriangleMesh* mesh, const Vect3f& localPos = v3fZERO, uint32 group = 0);
	void							AddPlaneShape			(const Vect3f& normal, float distance, uint32 group = 0);

	//---Trigger Function---
	void							CreateBoxTrigger	(const Vect3f& size, uint32 group = 0);

	//---Get Info-------
	void							GetMat44					(Mat44f& matrix) const;
	void							SetMat44					(const Mat44f& matrix);

	//---Get PhsX Info---
	void							CreateActor				(NxActor* actor);
	NxActor*					GetPhXActor				() {return m_pPhXActor;}
	NxActorDesc*			GetActorDesc			() {return m_pPhXActorDesc;}
  unsigned short    GetGroup          ();

private:
	void							DeInit						();

private:

	CPhysicUserData*											m_pUserData;
	NxActor*															m_pPhXActor;
	NxActorDesc*													m_pPhXActorDesc;
	NxBodyDesc*														m_pPhXBodyDesc;

	std::vector<NxBoxShapeDesc*>					m_vBoxDesc;
	std::vector<NxTriangleMeshShapeDesc*>	m_vMeshDesc;
	std::vector<NxCapsuleShapeDesc*>			m_vCapsuleDesc;
	std::vector<NxSphereShapeDesc*>				m_vSphereDesc;
	std::vector<NxPlaneShapeDesc*>				m_vPlaneDesc;
};

#endif //INC_PHYSIC_ACTOR_H_