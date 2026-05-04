#include "Item.h"
#include "Core.h"
#include "RenderManager.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "Actor/PhysicActor.h"
#include "RenderablePhysX/RenderablePhysXObject.h"
#include "RenderablePhysX/RenderablePhysXManager.h"
#include "RenderableObjects/RenderableObject.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "PhysicsManager.h"
#include "Object3D/BoundingSphere.h"


#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

#define TIME_MAX 6.0f

CItem::CItem()
{
	m_fTime = 0.0f;
}

bool CItem::Init()
{
	m_bIsOk = true;
	
	if (!m_bIsOk)
  {
    Release();
  }

	return m_bIsOk;

}

void CItem::CreateItem(std::string _sName, Vect3f _Position)
{
	m_iCountDownTimer=1;
	m_sName=_sName;
	m_bStatus=true;

	CORE->GetRenderableObjectsManager()->AddMeshInstance("calabaa01",m_sName,_Position,0.0f,0.0f,0.0f,1.0f,1.0f,1.0f);

	m_pItem = CORE->GetRenderableObjectsManager()->GetInstance(m_sName);
	m_pItem->SetVisible(true);
	m_pItem->InitializeBoundingObject(BOUNDING_SPHERE);
	((CBoundingSphere*)m_pItem->GetBoundingObject())->Init(Vect3f(0.211795f,-0.181064f,0.225646f),Vect3f(-0.208564f,0.224833f,-0.212607f),BOUNDING_SPHERE);
	
	m_pUserData = new CPhysicUserData(m_sName);
	m_pUserData->SetPaint(true);
	m_pUserData->SetColor(colYELLOW);

	Vect3f l_MiddlePoint = m_pItem->GetBoundingObject()->GetMiddlePoint();
	float l_fRadius=m_pItem->GetBoundingObject()->GetMaxRadius();
	Vect3f l_positionObject3D = m_pItem->GetPosition();
	l_MiddlePoint.x = 0.0f;
	l_MiddlePoint.y = 0.0f;
  m_pPhysXActor = new CPhysicActor(m_pUserData);
	m_pPhysXActor->AddSphereShape(l_fRadius*0.5f,l_MiddlePoint,0,0);
	m_pPhysXActor->CreateBody(0.5f);

	CPhysicsManager * l_pPhysxmanager = CORE->GetPhysicsManager();
	l_pPhysxmanager->AddPhysicActor(m_pPhysXActor);

	CORE->GetRenderablePhysXManager()->AddRenderablePhysXObject(m_sName, Vect3f(0.211795f,-0.181064f,0.225646f),Vect3f(-0.208564f,0.224833f,-0.212607f), 0.5f, 0.0f, BOUNDING_SPHERE, true);

	m_bIsOk=true;
}

void CItem::Update(float ElapsedTime)
{
	m_fTime += ElapsedTime;
  if (m_fTime > TIME_MAX)
	{
		m_pItem->SetPosition(Vect3f(0.0f,-20.0f,0.0f));
		m_pItem->SetVisible(false);
		CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject(m_sName)->GetPhysXActor()->SetMat44(m_pItem->GetMat44());
		CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject(m_sName)->GetUserData()->SetPaint(false);
	}
}

void CItem::Done()
{
   Release();
   m_bIsOk = false;
}

void CItem::Release()
{
  CHECKED_DELETE(m_pUserData);
	CHECKED_DELETE(m_pPhysXActor);
}

