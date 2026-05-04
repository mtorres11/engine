#include "Trigger.h"
#include "ScriptManager.h"
#include "Base.h"
#include "Math/Matrix44.h"
#include "Core.h"
#include "RenderManager.h"
#include "RenderablePhysX/RenderablePhysXManager.h"
#include "../Actor/PhysicActor.h"
#include "RenderableObjects/RenderableObject.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

/// <summary>
/// Inicializa los triggers.
/// </summary>
bool CTrigger::Init()
{
  m_bIsOk = true;
  m_Transformation.SetIdentity();

  return m_bIsOk;
}

/// <summary>
/// Finalize data.
/// </summary>
void CTrigger::Done()
{
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

/// <summary>
/// Libera memoria.
/// </summary>
void CTrigger::Release()
{
	//CHECKED_DELETE(m_RenderObj);
	//CHECKED_DELETE(m_PhysXObj);
}

void CTrigger::Render(Vect3f size, Vect3f position)
{
	Mat44f l_t;
	l_t.SetIdentity();
	CRenderManager * l_rm = CORE->GetRenderManager();
	l_t.Translate(position);
	l_t.RotByAngleX(m_fRotationX);
	l_t.RotByAngleY(m_fRotationY);
	l_t.RotByAngleZ(m_fRotationZ);
	l_rm->SetTransform(l_t);
	l_rm->DrawCube(size,colRED);
}

void CTrigger::CalculateMatriz44f()
{
	Mat44f l_t;
	l_t.SetIdentity();
	l_t.Translate(m_v3Position);
	l_t.RotByAngleY(m_fRotationY);
	CRenderManager * l_rm = CORE->GetRenderManager();
	l_rm->SetTransform(l_t);
	SetMatrizTrigger(l_t);
}

void CTrigger::MoveRenderableObject(std::string idObject)
{
	m_RenderObj = CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject(idObject)->GetRenderableObject();
	m_RenderObj->SetPosition(Vect3f(0.0f,-20.0f,0.0f));
	m_PhysXObj = CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject(idObject);
	Mat44f matriz2 = m_PhysXObj->GetRenderableObject()->GetMat44();
	m_PhysXObj->GetPhysXActor()->SetMat44(matriz2);
}

