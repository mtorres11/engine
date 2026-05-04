#include "RenderablePhysXObject.h"
#include "Actor/PhysicActor.h"
#include "RenderableObjects/RenderableObject.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderManager.h"
#include "Core.h"
#include "Math/Matrix44.h"
#include "Actor/PhysicActor.h"
#include "PhysicsManager.h"
#include "Object3D/BoundingObject.h"
#include "Object3D/BoundingBox.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CRenderablePhysXObject::CRenderablePhysXObject(std::string RenderableId)
{
  m_Name = RenderableId;
  m_RenderableObject = CORE->GetRenderableObjectsManager()->GetInstance(m_Name);
  m_UserData = new CPhysicUserData(RenderableId);
  m_PhysXActor = new CPhysicActor(m_UserData);
}

CRenderablePhysXObject::~CRenderablePhysXObject(void)
{
	CHECKED_DELETE(m_UserData);
	CHECKED_DELETE(m_PhysXActor);
}
void CRenderablePhysXObject::Update(float ElapsedTime)
{
  Mat44f l_MatrixPhysX;
  //Vect3f l_pos;
  m_PhysXActor->GetMat44(l_MatrixPhysX);
  //l_MatrixPhysX.SetPos(l_MatrixPhysX.GetPos().y - m_RenderableObject->GetBoundingObject()->GetMiddlePoint().y);
  //l_pos = l_MatrixPhysX.GetPos();
  //l_pos.y = l_pos.y + m_RenderableObject->GetBoundingObject()->GetMiddlePoint().y;
  //l_MatrixPhysX.SetPos(l_pos);
  m_RenderableObject->SetMat44(l_MatrixPhysX);
}
void CRenderablePhysXObject::SetTransform(Mat44f Transform)
{
  m_PhysXActor->SetMat44(Transform);
  m_RenderableObject->SetMat44(Transform);
}
Mat44f CRenderablePhysXObject::GetTransform()
{
  Mat44f l_Transform;
  m_PhysXActor->GetMat44(l_Transform);
  return l_Transform;
}