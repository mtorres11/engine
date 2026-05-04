//----------------------------------------------------------------------------------
// CPhysicController class
// Author: Enric Vergara
//
// Description:
// Esta clase representa un controlador de caracter.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSIC_CONTROLLER_H
#define INC_PHYSIC_CONTROLLER_H

#include "Jump.h"
#include "Object3D/Object3D.h"

//---Forward Declarations---
class NxController;
class NxCapsuleControllerDesc;
class NxScene;
class CPhysicUserData;
class CPhysicControllerReport;
//--------------------------

class CPhysicController:public CObject3D
{
public:
	CPhysicController(	float radius, float height, float slope, float skinwidth, 
		float stepOffset, uint32 collisionGroups, CPhysicUserData* userData, 
		const Vect3f& pos = Vect3f(0.f,0.f,0.f), float gravity = -9.8f, float m_VerticalVelocity = 0.0f);

	~CPhysicController();

	CPhysicUserData*					GetUserData							() {return m_pUserData;}
	void											SetCollision						(bool flag);
	void											Move										(const Vect3f& direction, float elapsedTime);
  void											Jump										(float ammount, float _directionX, float _directionZ);
	bool											UpdateCharacterExtents	(bool bent, float ammount);
	Vect3f										GetPosition							();
	void											SetPosition							(const Vect3f& pos);
	float											GetGravity							() const {return m_fGravity;}
	void											SetGravity							(float gravity)  {m_fGravity = gravity;}
  float                     GetTotalHeight          () { return m_fHeight_Capsule + m_fRadius_Capsule * 2; }
  float                     GetHeight               () { return m_fHeight_Capsule; }
  float                     GetRadius               () { return m_fRadius_Capsule; }

	//---PhsX Info---
	NxController*							GetPhXController				() {return m_pPhXController;}
	NxCapsuleControllerDesc*	GetPhXControllerDesc		() {return m_pPhXControllerDesc;}
	void											CreateController				(NxController* controller, NxScene* scene);
  unsigned short            GetGroup                ();

private:

	CPhysicUserData*	m_pUserData;
	uint32						m_uCollisionGroups;
	CJump							m_Jump;
	float							m_fGravity;
	float							m_fRadius_Capsule;
	float							m_fHeight_Capsule;
	float							m_fSlopeLimit_Capsule;
	float							m_fSkinWidth_Capsule;
	float							m_fStepOffset_Capsule;
  float             m_VerticalVelocity;
  float             m_fJumpDirectionX;
  float             m_fJumpDirectionZ;

	//--- PhysX--
	NxCapsuleControllerDesc*	m_pPhXControllerDesc;
	NxController*							m_pPhXController;
	NxScene*									m_pPhXScene;

  CPhysicControllerReport*  m_PhXControllerHitEvent;

};

#endif //INC_PHYSIC_CONTROLLER_H