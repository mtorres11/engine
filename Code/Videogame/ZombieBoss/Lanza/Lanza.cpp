#include "Lanza.h"
#include "RenderManager.h"
#include "Utils/BaseUtils.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "Cal3d/Cal3d.h"
#include "PhysicsManager.h"
#include "CoreFIH.h"
#include "Player/Player.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CLanza::Init(int i)
{
  std::string l_sNombreLanza = "";
  baseUtils::FormatSrting(l_sNombreLanza, "Lanza%d",i);
  
  m_pRenderableAnimatedInstanceModel = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->AddAnimatedModelInstance(
    "lanza", 
    l_sNombreLanza,
    Vect3f(0.0f,-5.0f,0.0f), 
    0.0f,
    0.0f,
    0.0f);


  m_bIsOk = m_pRenderableAnimatedInstanceModel? true:false;
  if (!m_bIsOk)
  {
    Release();
  }

  m_fTiempoAnimacion = GetTiempoAnimacion();
  if(m_fTiempoAnimacion == 0.0f)
  {
    Release();
  }
  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CLanza::Done()
{
    Release();
    m_bIsOk = false;
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CLanza::Release()
{
//free memory
//  CHECKED_DELETE(m_pRenderableAnimatedInstanceModel);
}

void CLanza::Update(float _elapsedTime)
{
  if (m_bIniciadaAnimacion)
  {
    AcumulaTiempoAnimacion(_elapsedTime);
    PosicionaPuntaLanza();
  }
  else
  {
    ResetTiempoAnimacion();
//    m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->ClearCycle(_elapsedTime);
    m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->ExecuteActionBlocked(0, 0.0f);
    m_pRenderableAnimatedInstanceModel->SetVisible(true);
    m_bIniciadaAnimacion = true;
    m_bActiva = true;
  }
  
  
  if (FinAnimacionLanza())
  {
    m_bIniciadaAnimacion = false;
    m_pRenderableAnimatedInstanceModel->SetVisible(false);
    m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->RemoveAction(0);
    m_bActiva = false;
  }
}

void CLanza::PosicionaPuntaLanza()
{
//  Mat44f worldmatrix = GetInstance()->GetMat44();
  std::string l_sName = GetInstance()->GetName();
  CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(l_sName);
  CalSkeleton* skeleton = model->GetAnimatedInstanceModel()->GetModelCal3DHW()->getSkeleton();
 
  int l_HuesoManoDer = skeleton->getCoreSkeleton()->getCoreBoneId("hueso master");
  
  if(l_HuesoManoDer != -1)
  {
    CalVector l_V3Position = (CalVector)skeleton->getBone(l_HuesoManoDer)->getTranslationAbsolute();

    V3PosicionPuntaLanza.x = -l_V3Position.x;
    V3PosicionPuntaLanza.y = l_V3Position.y;
    V3PosicionPuntaLanza.z = l_V3Position.z;

	  std::vector<CPhysicUserData*>	m_vImpactObjects;

    Mat44f l_t,res;
    Mat44f l_vMat44 = GetInstance()->GetMat44();

    l_t.SetIdentity();
    V3PosicionPuntaLanza.y += 1.3f;
    l_t.Translate(V3PosicionPuntaLanza);
    res.SetIdentity();
    res = l_vMat44*l_t;

    CORE->GetPhysicsManager()->OverlapSphereActor(
      0.7f
      ,res.GetPos()
      ,m_vImpactObjects
      ,PX_MSK_ALL
      ,true
    );
    if(!m_vImpactObjects.empty())
    {
      std::vector<CPhysicUserData*>::iterator l_iter = m_vImpactObjects.begin();
      for(;l_iter != m_vImpactObjects.end(); l_iter++)
      {
        if (!(*l_iter)->GetName().compare("mi_amigo"))
        { 
          //hit player
          if((res.GetPos().y > 0.2f) && (m_bActiva))  //empieza a salir la lanza
            if(CCoreFIH::GetSingletonPtr()->GetPlayer()->CollideEnemies())
            {
              CCoreFIH::GetSingletonPtr()->GetPlayer()->Hit(5);
              m_bActiva = false;
            }
          }
        }
      }
    }
}


void CLanza::Render(CRenderManager* rm)
{
  Mat44f l_t,res;
  Mat44f l_vMat44 = GetInstance()->GetMat44();

  l_t.SetIdentity();
  V3PosicionPuntaLanza.y += 1.3f;
  l_t.Translate(V3PosicionPuntaLanza);
  res.SetIdentity();
  res = l_vMat44*l_t;
  CORE->GetRenderManager()->SetTransform(res);
  
  if (m_bActiva)
    rm->DrawSphere(0.5f,colWHITE,10);
  else 
    rm->DrawSphere(0.5f,colYELLOW,10);
  
}

float CLanza::GetTiempoAnimacion()
{
  return GetInstance()->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation(0)->getDuration();
}

void CLanza::GetSalidaLanza(Vect3f &_pos, const std::string &_collidemesh)
{
  CPhysicUserData impacted(_collidemesh);
  impacted.Init();

  SCollisionInfo info;
  Vect3f g(0.0f,-1.0f,0.0f);
  CORE->GetPhysicsManager()->RaycastClosestActor(_pos,g,PX_MSK_SCENARIO,&impacted, info);
  
  _pos.y = info.m_CollisionPoint.y;  
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------