#include "CharacterController/PhysicController.h"
#include "CharacterController/PhysicControllerReport.h"

//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
#include "NxController.h"
#include "NxCapsuleController.h"
#include "Base.h"
//---------------------//

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CPhysicController::CPhysicController(float radius, float height, float slope, float skinwidth, float stepOffset,
																		 uint32 collisionGroups, CPhysicUserData* userData, const Vect3f& pos, float gravity, float Velocity)
																		 : m_pPhXController(NULL)
																		 , m_pPhXControllerDesc(NULL)
																		 , m_pUserData(userData)
																		 , m_fRadius_Capsule(radius)
																		 , m_fHeight_Capsule(height)
																		 , m_fSlopeLimit_Capsule(slope)
																		 , m_fSkinWidth_Capsule(skinwidth)
																		 , m_fStepOffset_Capsule(stepOffset)
																		 , m_fGravity(gravity)
																		 , m_uCollisionGroups(collisionGroups)
                                     , m_PhXControllerHitEvent(0)
                                     , m_VerticalVelocity(Velocity)
{
	assert(userData);

	//---- Crear un nuevo NxController----
	m_pPhXControllerDesc = new NxCapsuleControllerDesc();

	m_pPhXControllerDesc->position.x		= pos.x;
	m_pPhXControllerDesc->position.y		= pos.y;
	m_pPhXControllerDesc->position.z		= pos.z;
	m_pPhXControllerDesc->radius				= m_fRadius_Capsule;
	m_pPhXControllerDesc->height				= m_fHeight_Capsule;
	m_pPhXControllerDesc->slopeLimit		= cosf(NxMath::degToRad(m_fSlopeLimit_Capsule));
	m_pPhXControllerDesc->skinWidth			= m_fSkinWidth_Capsule;
	m_pPhXControllerDesc->stepOffset		= m_fStepOffset_Capsule;
	m_pPhXControllerDesc->upDirection		= NX_Y;
  //TODO..Actualmente se crea un callback por cada controller, se deberia solo uno para todos.
  m_PhXControllerHitEvent             = new CPhysicControllerReport();
  m_pPhXControllerDesc->callback      = m_PhXControllerHitEvent;
}


CPhysicController::~CPhysicController()
{
  CHECKED_DELETE(m_PhXControllerHitEvent);
  CHECKED_DELETE(m_pPhXControllerDesc);
  CHECKED_DELETE(m_pUserData);
  //CHECKED_DELETE(m_pPhXController);
  //CHECKED_DELETE(m_pPhXScene);


}


void CPhysicController::CreateController (NxController* controller, NxScene* scene)
{
	m_pPhXScene = scene;
	m_pPhXController = controller;
	assert(m_pPhXScene);
	assert(m_pPhXController);
	CHECKED_DELETE(m_pPhXControllerDesc);//Porque esto se destruye aqui???
}

void CPhysicController::SetPosition	(const Vect3f& pos)
{
	if (m_pPhXController != NULL)
	{
		NxExtendedVec3 position;
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
		m_pPhXController->setPosition(position);
	}
	else
	{
		m_pPhXControllerDesc->position.x		= pos.x;
		m_pPhXControllerDesc->position.y		= pos.y;
		m_pPhXControllerDesc->position.z		= pos.z;
	}
	CObject3D::m_Position = pos;
}

Vect3f CPhysicController::GetPosition ()
{
	Vect3f vec;
	if (m_pPhXController != NULL)
	{
		NxExtendedVec3 tmp = m_pPhXController->getPosition();
		vec.x = (float)tmp.x;
		vec.y = (float)tmp.y;
		vec.z = (float)tmp.z;
	}
	else
	{
		vec.x = (float)m_pPhXControllerDesc->position.x;
		vec.y = (float)m_pPhXControllerDesc->position.y;
		vec.z = (float)m_pPhXControllerDesc->position.z;
	}
	return vec;
}

void CPhysicController::Jump(float ammount, float _directionX, float _directionZ)
{
	m_Jump.StartJump(ammount, m_Position.y);
  m_fJumpDirectionX = _directionX;
  m_fJumpDirectionZ = _directionZ;
}

void CPhysicController::Move (const Vect3f& direction, float elapsedTime)
{
	assert (m_pPhXController!=NULL);

  /*Si quiero hacer la caida mas realista tengo que meter aqui la formula de gravedad.*/
  //m_VerticalVelocity =  m_VerticalVelocity + (m_fGravity*elapsedTime);
	
  NxVec3 d(direction.x, direction.y + m_VerticalVelocity, direction.z);
	
  NxF32 sharpness = 1.0f;
	NxU32 collisionFlags = 0;
	
  if (m_Jump.IsOnJump())
	{
    float l_fJumpHeight = m_Jump.GetHeight(elapsedTime);
	  Vect3f l_V3fControllerPos = GetPosition();
    
    d.y = (l_fJumpHeight - l_V3fControllerPos.y);
		
    d.x = m_fJumpDirectionX;
		d.z = m_fJumpDirectionZ;
	}

  d *= elapsedTime;

	m_pPhXController->move(d, m_uCollisionGroups, 0.000001f, collisionFlags, sharpness);
	
  if(	(collisionFlags & NXCC_COLLISION_DOWN) || (collisionFlags & NXCC_COLLISION_UP) )
	{
		m_Jump.StopJump();
    m_VerticalVelocity = 0.f;
	}
  else
  {
    m_VerticalVelocity = m_fGravity;
  }
	
  NxExtendedVec3 tmp = m_pPhXController->getPosition();
	CObject3D::m_Position.x = (float)tmp.x;
	CObject3D::m_Position.y = (float)tmp.y;
	CObject3D::m_Position.z = (float)tmp.z;
}

void CPhysicController::SetCollision (bool flag)
{
	assert(m_pPhXController);
	m_pPhXController->setCollision(flag);
}

bool CPhysicController::UpdateCharacterExtents (bool bent, float ammount)
{
	NxF32 height = m_fHeight_Capsule;
	NxF32 radius = m_fRadius_Capsule;
	NxExtendedVec3 pos = m_pPhXController->getPosition();
	if ( bent )
	{
		//Ponerse de pie
		height += ammount;
		pos.y += ammount*0.5f;
	} 
	else 
	{ 
		//Agacharse
		height -= ammount;
		pos.y -= ammount*0.5f;
	}

	NxCapsule worldCapsule;
	worldCapsule.p0.x = worldCapsule.p1.x = (NxReal)pos.x;
	worldCapsule.p0.y = worldCapsule.p1.y = (NxReal)pos.y;
	worldCapsule.p0.z = worldCapsule.p1.z = (NxReal)pos.z;
	worldCapsule.p0.y -= height*0.5f;
	worldCapsule.p1.y += height*0.5f;
	worldCapsule.radius = radius;
	m_pPhXController->setCollision(false);	// Avoid checking overlap with ourself
	bool Status = m_pPhXScene->checkOverlapCapsule(worldCapsule);
	m_pPhXController->setCollision(true);
	if(Status)
	{
		return false;
	}

	NxExtendedVec3 position(pos.x, pos.y, pos.z);
	m_pPhXController->setPosition(position);
	CObject3D::m_Position.x = (float)pos.x;
	CObject3D::m_Position.y = (float)pos.y;
	CObject3D::m_Position.z = (float)pos.z;
	NxCapsuleController* c = static_cast<NxCapsuleController*> (m_pPhXController);
	c->setHeight(height);
	m_fHeight_Capsule = height;
	return true;
}
unsigned short CPhysicController::GetGroup(){
  return (unsigned short)m_pPhXController->getActor()->getGroup();
}
