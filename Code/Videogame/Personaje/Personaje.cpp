#include "Personaje.h"
#include "XML\XMLTreeNode.h"
#include "PhysicsManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "CharacterController/PhysicController.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CPersonaje::CPersonaje():
m_bIsOk(false),
m_V3PosIni(),
m_V3RotIni(),
m_fMoveSpeed(0.0f),
m_fRotSpeed(0.0f),
m_fHealth(0.0f),
m_sMesh(""),
m_pPhysicController(0)
{}

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CPersonaje::Init(const CXMLTreeNode &m)
{
    m_bIsOk = true;    
    return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void
CPersonaje::Done(){
  //if (IsOk())
  //{
    Release();
    m_bIsOk = false;
//  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CPersonaje::Release()
{
//free memory
  CHECKED_DELETE (m_pPhysicController);
}

//----------------------------------------------------------------------------
// Inicializa controller
//----------------------------------------------------------------------------
/*void CPersonaje::InicializaController()
{
  CPhysicUserData *userdata = new CPhysicUserData(GetInstance()->GetName());
  userdata->SetColor(colGREEN);
  userdata->SetPaint(true);
  GetInstance()->SetPosition(Vect3f(GetInstance()->GetPosition().x,GetInstance()->GetPosition().y + 1.0f,GetInstance()->GetPosition().z));
//  m_pPhysicController = new CPhysicController(0.5f,2.0f,2.0f,0.1f,2.0f,1,userdata,GetInstance()->GetPosition());
  m_pPhysicController = new CPhysicController(0.5f, 1.3f, 40.0f, 0.01f, 0.3f, PX_MSK_ALL, userdata, GetInstance()->GetPosition());
  m_pPhysicController->GetUserData()->SetCollisionGroup(PX_MSK_ALL);
  CORE->GetPhysicsManager()->AddPhysicController(m_pPhysicController);
}*/

//----------------------------------------------------------------------------
// Read Functions
//----------------------------------------------------------------------------
float CPersonaje::ReceiveDamage(float _damage)
{
	return (m_fHealth -= _damage);
}
