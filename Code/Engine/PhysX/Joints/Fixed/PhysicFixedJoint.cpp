#include "Joints/Fixed/PhysicFixedJoint.h"
#include "NxFixedJointDesc.h"
#include "Actor/PhysicActor.h"
#include "Logger/Logger.h"
#include "Math/MathUtils.h"
#include "Math/Vector3.h"
#include "Base.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CPhysicFixedJoint::CPhysicFixedJoint()
: m_pJoint(NULL)
, m_pFixedDesc(NULL)
{
	m_pFixedDesc = new NxFixedJointDesc;
}

CPhysicFixedJoint::~CPhysicFixedJoint()
{
	CHECKED_DELETE(m_pFixedDesc);
}

void CPhysicFixedJoint::CreateJoint(NxJoint* joint)
{
	assert(joint);
	m_pJoint = joint;
	CHECKED_DELETE(m_pFixedDesc);
}

void CPhysicFixedJoint::SetInfo	(CPhysicActor* actorA,  CPhysicActor* actorB)
{
	if (actorA==NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "CPhysicFixedJoint:: El primer actor pasado como argumento no puede ser null");
		return;
	} 

m_pFixedDesc->actor[0] = actorA->GetPhXActor();
	if (actorB!=NULL)
	{
		m_pFixedDesc->actor[1] = actorB->GetPhXActor();	
	}
	else
	{
		m_pFixedDesc->actor[1] = NULL;	
	}
}