#include "Joints/Spherical/PhysicSphericalJoint.h"
#include "NxSphericalJointDesc.h"
#include "Actor/PhysicActor.h"
#include "Logger/Logger.h"
#include "Base.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CPhysicSphericalJoint::CPhysicSphericalJoint()
: m_pJoint(NULL)
, m_pSphericalDesc(NULL)
{
	m_pSphericalDesc = new NxSphericalJointDesc;
}

CPhysicSphericalJoint::~CPhysicSphericalJoint()
{
	CHECKED_DELETE(m_pSphericalDesc);
}

void CPhysicSphericalJoint::CreateJoint(NxJoint* joint)
{
	assert(joint);
	m_pJoint = joint;
	CHECKED_DELETE(m_pSphericalDesc);
}

void CPhysicSphericalJoint::SetInfo	(const Vect3f& anchor, CPhysicActor* actorA,  CPhysicActor* actorB)
{
	if (actorA==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "PhysicSphericalJoint:: El primer actor pasado como argumento no puede ser null");
		return;
	}

	NxVec3 pos(anchor.x, anchor.y, anchor.z );
	m_pSphericalDesc->setToDefault();
	m_pSphericalDesc->actor[0] = actorA->GetPhXActor();
	if (actorB!=NULL)
	{
		m_pSphericalDesc->actor[1] = actorB->GetPhXActor();	
	}
	else
	{
		m_pSphericalDesc->actor[1] = NULL;	
	}

	m_pSphericalDesc->setGlobalAnchor(pos);
}