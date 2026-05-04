#include "ZombieNormal.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "../IA/EjeMovimientoIA.h"
//TODO..borrar
#include "Trigger/Trigger.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CZombieNormal::Init(const CXMLTreeNode &m){
	bool bIsOk = Inherited::Init(m);
  if (!bIsOk)
  {
		printf("a CZombieNormal instace couldnt allocate memory");
    Done();								//We call Done()  to release before the parent class
	}
  else
  {
    m_uiAgressivity =	m.GetIntProperty("agressivity");
  }
  return bIsOk;
}

//----------------------------------------------------------------------------
// Init data
////----------------------------------------------------------------------------
bool CZombieNormal::Init(CZombie &z){
	bool bIsOk = Inherited::Init(z);
  if (!bIsOk)
  {
		printf("a CZombieNormal instace couldnt allocate memory");
    Done();								//We call Done()  to release before the parent class
	}
 // else
 // {
 //   SetHealth(z.GetHealth());
 //   SetInstance(z.GetInstance());
 //   SetModelFilename(z.GetModelFilename());
 //   SetMoveSpeed(z.GetMoveSpeed());
 //   SetRotSpeed(z.GetRotSpeed());
 //   GetInstance()->SetScalar(z.GetInstance()->GetScalarX(),z.GetInstance()->GetScalarY(),z.GetInstance()->GetScalarZ());

 //   m_movIA = new CEjeMovimiento();
 //   bIsOk = m_movIA->Init();
 //   if (!bIsOk)
 //   {
 //     LOGGER->AddNewLog(ELL_WARNING,"a CEjeMovimiento instace couldnt allocate memory");
 //   }

 // }

  return bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void
CZombieNormal::Done(){
	Inherited::Done();			//Parent class Done
  bool l_bIsOk = true;
  if (l_bIsOk)
  {
    Release();
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CZombieNormal::Release()
{
//free memory
//  CHECKED_DELETE(m_pUserData);
  CHECKED_DELETE(m_movIA);
}

//----------------------------------------------------------------------------
// Recalculate IA
//----------------------------------------------------------------------------
char CZombieNormal::RecalculateIA(Vect3f hisPosition)
{
  return CZombie::RecalculateIA(hisPosition); 
  //return (char) IDLE;
}

//----------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------
void CZombieNormal::Update(float elapsedTime)
{
    Vect3f hisPosition = GetPositionHero();
    //dibujar cuando la distancia es la mitad a smelling
    if((hisPosition.SqDistance(GetInstance()->GetPosition()) < GetRadiusSmelling() * GetRadiusSmelling()))
    {
      GetInstance()->SetVisible(true);
    }
    else
    {
      GetInstance()->SetVisible(false);
    }
    SetState( RecalculateIA(hisPosition) );

    switch(GetState())
    { 
      case (char)IDLE:
        m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->BlendCycle(IDLE,elapsedTime);
      break;
      case (char)OLIENDO:
        m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->BlendCycle(OLIENDO,elapsedTime);
        Perseguir(hisPosition,elapsedTime);
      break;
      case (char)MIRANDO:
        m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->BlendCycle(MIRANDO,elapsedTime);
        Perseguir(hisPosition,elapsedTime);
      break;
      case (char)ANDANDO:
      break;
      case (char)ATACANDO:
        //PosicionaManoDerecha();
        //PosicionaManoIzquierda();
      break;
      case (char)RECIBIENDOGOLPE:
      break;
      case (char)VOLVIENDOINICIO:
        m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->BlendCycle(0,elapsedTime);
        VolviendoInicio(elapsedTime);
      break;
      case (char)MURIENDO:
      break;
    }
}

//----------------------------------------------------------------------------
// Render
//----------------------------------------------------------------------------
void CZombieNormal::Render(CRenderManager* rm)
{//TODO..
    Mat44f l_t;
    l_t.SetIdentity();
    rm->SetTransform(l_t);
    rm->DrawCircleH(GetInstance()->GetPosition().x,GetInstance()->GetPosition().z, GetRadiusSmelling(),colYELLOW);

    l_t.SetIdentity();
    rm->SetTransform(l_t);
    rm->DrawCircleH(GetInstance()->GetPosition().x, GetInstance()->GetPosition().z, GetRadiusViewing(),colRED);
    //l_t.SetIdentity();
    //l_t.Translate(V3PosicionManoIzquierda);
    //Mat44f l_vMat44 = GetInstance()->GetMat44();
    //Mat44f posmanoizq = l_vMat44 * l_t;
    ////l_vMat44.Translate(V3PosicionManoIzquierda);
    //CORE->GetRenderManager()->SetTransform(l_vMat44*l_t);
    //CORE->GetRenderManager()->DrawCube(0.4f,colRED);

    //l_t.SetIdentity();
    //l_t.Translate(V3PosicionManoDerecha);
    ///*l_vMat44.m00 = -l_vMat44.m00;
    //l_vMat44.m10 = -l_vMat44.m10;
    //l_vMat44.m20 = -l_vMat44.m20;
    //l_vMat44.m30 = l_vMat44.m30;*/
    ////l_vMat44.Translate(V3PosicionManoIzquierda);
    //CORE->GetRenderManager()->SetTransform(l_vMat44*l_t);
    //CORE->GetRenderManager()->DrawCube(0.4f,colRED);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
