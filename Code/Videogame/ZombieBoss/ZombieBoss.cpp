#include "ZombieBoss.h"
#include "CoreFIH.h"
#include "Lanza/GestionLanzas.h"

#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "EjeMovimientoIA.h"
#include "Trigger/TriggerManager.h"
#include "Trigger/Trigger.h"
#include "CharacterController/PhysicController.h"
#include "Cal3d/Cal3d.h"

#include "SoundManager.h"
#include "ScriptManager.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>

#include "Utils/BaseUtils.h"
#include "RandomArray.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CZombieBoss::Init(const CXMLTreeNode &m)
{
  m_bIsOk = Inherited::Init(m);

  if (!m_bIsOk)
  {
    Done();								//We call Done()  to release before the parent class
  }

  //m_triggerManoDerecha->SetDimensions(Vect3f(1.4f,1.4f,1.4f));
  
  m_fTiempoRecuperacionTrasDarGolpe = m.GetFloatProperty("recuperacion_tras_dar_golpe", 1.0f);
  m_fTiempoMinEntreGolpeYGolpe      = m.GetFloatProperty("tiempoMinEntreGolpeyGolpe", 1.0f);
  m_fTiempoMaxEntreGolpeYGolpe      = m.GetFloatProperty("tiempoMaxEntreGolpeyGolpe", 1.0f);
  
  if(m_fTiempoMaxEntreGolpeYGolpe <= m_fTiempoMinEntreGolpeYGolpe)
    m_fTiempoMaxEntreGolpeYGolpe = m_fTiempoMinEntreGolpeYGolpe + 1.0f;
    
  m_fSpeedRetroceso                 = m.GetFloatProperty("speed_retroceso",1.0f);
  m_fSpeedEmbestida                 = m.GetFloatProperty("speed_embestida",1.0f);
  m_fTiempoInactivoRecibeGolpe      = m.GetFloatProperty("tiempo_inactivo_tras_recibir_golpe",1.0f);
  m_fTiempoInvencibilidad           = m.GetFloatProperty("tiempo_invencibilidad",1.0f);
  
  m_fTiempoEncarando                = m.GetFloatProperty("tiempo_max_encarando", 1.0f);
    
  //nombre de la mesh con la que va a colisionar y pasar a la animacion CRASH
  m_sNameBlockColliding             = m.GetPszProperty("nombre_mesh_colision_crash","");
  
  //valores del ataque pisoton
  m_uiNumeroLanzas                  = m.GetIntProperty("numero_lanzas_pisoton", 6);
  m_uiNumeroTotalIteracionesLanzas  = m.GetIntProperty("numero_iter_lanzas_pisoton", 4);
  
  //genera valores aleatorios
  m_vAleatorios = new CRandomArray();

  if(m_vAleatorios)
  {  
    if(m_vAleatorios->Init())
    {
      m_vAleatorios->GenerateArray(256);
    }
  }
  else
  {
    m_bIsOk = false;
  }
  
  m_pGLanzas = new CGestionLanzas();
  
  if(m_pGLanzas)
  {
    m_pGLanzas->Init(m_uiNumeroLanzas, m_uiNumeroTotalIteracionesLanzas);
  }
  else
  {
    m_bIsOk = false;
  }
  
  SetState(CZombieBoss::IDLE);
  RegistraBossLUA();
  
  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Init data (No utilizable)
//----------------------------------------------------------------------------
bool CZombieBoss::Init(const CZombie &z)
{
  m_bIsOk = Inherited::Init(z);

  if (!m_bIsOk)
  {
    Done();								//We call Done()  to release before the parent class
  }

  SetState(CZombieBoss::IDLE);

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CZombieBoss::Done()
{
	Inherited::Done();			//Parent class Done
  if (IsOk())
  {
    Release();
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CZombieBoss::Release()
{
//free memory
  CHECKED_DELETE(m_vAleatorios);
  CHECKED_DELETE(m_pGLanzas); 
}


void CZombieBoss::Update(float elapsedtime)
{
  //evitar caida del controller
  Vect3f aux_pos = GetPhysicController()->GetPosition();
  if(aux_pos.y < 2.8f)
  {
    aux_pos.y = 2.8f;
    GetPhysicController()->SetPosition(aux_pos);
  }  
//  SetState(RecalculateIA(GetPositionHero()));
  
  UpdateLanzasPisoton( elapsedtime );
  
	std::vector<CPhysicUserData*>	m_vImpactObjects;
//testend
  std::string action = "";
  switch(GetState())
  {
    case IDLE:
      baseUtils::FormatSrting(action, "update_idle( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
    break;
    case ATAQUECONARMA:
      baseUtils::FormatSrting(action, "update_ataque_con_arma( %f )", elapsedtime); 
      CORE->GetScriptManager()->RunCode(action);
			
    break;
    case EMBESTIDA:
      baseUtils::FormatSrting(action, "update_embestida( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
			
      //control del overlap      
      CORE->GetPhysicsManager()->OverlapSphereActor(
        1.5f
        ,GetPhysicController()->GetPosition() + (GetEjeMovimientoIA()->GetVectorDirector()/**1.8f*/)
        ,m_vImpactObjects
        ,PX_MSK_ALL
        ,true
      );
		  if(!m_vImpactObjects.empty())
		  {
        std::vector<CPhysicUserData*>::iterator l_iter = m_vImpactObjects.begin();
        for(;l_iter != m_vImpactObjects.end(); l_iter++)
        {
          if (!(*l_iter)->GetName().compare(m_sNameBlockColliding))
          { 
            //el boss choca contra el muro, se pasa a la animacion crash desde LUA
            GetUserData()->SetIsActionFinalized( CZombieBoss::EMBESTIDA );
          }
        }
      }
      
    break;
    case PISOTON:
      baseUtils::FormatSrting(action, "update_pisoton( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
    break;
    case ATAQUEREVERSO:
      baseUtils::FormatSrting(action, "update_ataque_reverso( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
    break;
    case RECIBIENDOGOLPE:
      baseUtils::FormatSrting(action, "update_recibiendo_golpe( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
    break;
    case MURIENDO:
      baseUtils::FormatSrting(action, "update_muriendo( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
    break;
    case PERSEGUIR:
      baseUtils::FormatSrting(action, "update_perseguir( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
      AcumulaTiempoEntreGolpeYGolpe(elapsedtime);
      Perseguir(GetPositionHero(), elapsedtime,1.0f);
      
    break;
    case RETIRADA:
      baseUtils::FormatSrting(action, "update_retirada( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
      AcumulaTiempoEntreGolpeYGolpe(elapsedtime);
      Evadir(GetPositionHero(), elapsedtime, m_fSpeedRetroceso,1.0f);
    break;
    case ENCARAR:
      baseUtils::FormatSrting(action, "update_encarar( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);
    break;
    case INACTIVO:
      baseUtils::FormatSrting(action, "update_inactivo( %f )", elapsedtime);
      CORE->GetScriptManager()->RunCode(action);        
//      AcumulaTiempoInactivo(elapsedtime);
    break;
  }
}

void CZombieBoss::UpdateLanzasPisoton(float _elapsedTime)
{
  //Se produce pisoton(LUA)->m_cStartLanzas=1 en estado pisoton
  switch (m_cStartLanzas)
  {
    //case 0: inactivo 
    case 1:
      m_pGLanzas->SetNumTotalIteracionLanzas(m_uiNumeroTotalIteracionesLanzas);
			CORE->GetSoundManager()->PlaySound("lanzas");
      m_cStartLanzas = 2;
    case 2:
      m_pGLanzas->Update(_elapsedTime);
      //Iteraciones finalizadas
      if (m_pGLanzas->GetNumIteracionLanzas() > m_pGLanzas->GetNumTotalIteracionLanzas())
      {
				CORE->GetSoundManager()->PlaySound("lanzas2");
        m_pGLanzas->SetNumIteracionLanzas(0);
        m_cStartLanzas = 0;
				
      }
    break;
  }
}

void CZombieBoss::Render(CRenderManager* rm)
{
  //punyo
  Mat44f l_t,res;
  Mat44f l_vMat44 = GetInstance()->GetMat44();

  l_t.SetIdentity();
  l_t.Translate(V3PosicionManoDerecha);
  res.SetIdentity();
  res = l_vMat44*l_t;
  CORE->GetRenderManager()->SetTransform(res);
  CORE->GetRenderManager()->DrawCube(0.5f,colRED);

  GetEjeMovimientoIA()->RenderMovIA(rm, GetPositionHero(), GetInstance()->GetPosition(),30.0f);
  //smelling y viewing
  l_t.SetIdentity();
  rm->SetTransform(l_t);
  rm->DrawCircleH(GetInstance()->GetPosition().x,GetInstance()->GetPosition().z, GetRadiusSmelling(),colYELLOW);

  l_t.SetIdentity();
  rm->SetTransform(l_t);
  rm->DrawCircleH(GetInstance()->GetPosition().x, GetInstance()->GetPosition().z, GetRadiusViewing(),colRED);
  
  //overlap
  l_t.SetIdentity();
  l_t.Translate( GetPhysicController()->GetPosition() + (GetEjeMovimientoIA()->GetVectorDirector()/**1.8f*/));
  rm->SetTransform(l_t);
  rm->DrawSphere(1.5f, colYELLOW, 5);
  
  //director y optimo
  float x = GetInstance()->GetPosition().x;
  float z = GetInstance()->GetPosition().z;
  
  Mat44f mat,m44_encarar;
//  m44_encarar.SetIdentity();
//  
//  rm->SetTransform(m44_encarar);
////  rm->DrawLine(Vect3f(x,1.0f,z), Vect3f(x + l_vDireccion.x, 1.0f, z + l_vDireccion.z), colWHITE);
//  rm->DrawLine(Vect3f(x,1.0f,z), Vect3f(x + GetEjeMovimientoIA()->GetVectorDirector().x* 5.0f, 1.0f, z + GetEjeMovimientoIA()->GetVectorDirector().z* 5.0f), colWHITE);
//
//  m44_encarar.SetIdentity();
//  rm->SetTransform(m44_encarar);
////  rm->DrawLine(Vect3f(x,1.0f,z), GetEjeMovimientoIA()->GetVectorDirectorOptimo(), colRED);
//  rm->DrawLine(Vect3f(x,1.0f,z), Vect3f(x + GetEjeMovimientoIA()->GetVectorDirectorOptimo().x* 6.0f, 1.0f, z + GetEjeMovimientoIA()->GetVectorDirectorOptimo().z * 6.0f), colRED);
  
  //estado
  mat.SetIdentity();

  switch(GetState())
  {
    case IDLE:
      rm->SetTransform(mat);
      rm->DrawCircleH(x,z,2.0f, colCYAN);
    break;
    case ATAQUECONARMA:
      rm->SetTransform(mat);
      rm->DrawCircleH(x,z,2.0f, colRED);
    break;
    case EMBESTIDA:
      rm->SetTransform(mat);
      rm->DrawCircleH(x,z,2.0f, colGREEN);
    break;
    case PISOTON:
      rm->SetTransform(mat);
      rm->DrawCircleH(x,z,2.0f, colBLUE);
    break;
    case ATAQUEREVERSO:
      rm->SetTransform(mat);
      rm->DrawCircleH(x,z,2.0f, colWHITE);
      rm->DrawCircleH(x,z,2.3f, colWHITE);
    break;
    case RECIBIENDOGOLPE:
      rm->SetTransform(mat);
//      rm->SetTransform(GetInstance()->GetMat44());
      rm->DrawCube(2.0f, colRED);    
    break;
    case MURIENDO:
//      rm->SetTransform(GetInstance()->GetMat44());
      rm->DrawSphere(2.0f, colRED, 6);    
    break;
    case PERSEGUIR:
      rm->SetTransform(mat);
      rm->DrawCircleH(x,z,2.0f, colYELLOW);
    break;
    case RETIRADA:
      rm->SetTransform(mat);
      rm->DrawCircleH(x,z,2.0f, colMAGENTA);    
    break;
    case ENCARAR:
      rm->SetTransform(mat);
      rm->DrawCircleH(x,z,2.0f, colWHITE);
    break;
    case INACTIVO:
      rm->SetTransform(GetInstance()->GetMat44());
      rm->DrawSphere(2.0f, colWHITE, 6);    
    break;
  }
  
  //lanzas pisoton
  m_pGLanzas->Render(rm);
}

char CZombieBoss::RecalculateIA(Vect3f _heroPosition)//{return (char)255;}// = 0;
{
  return GetState();
}

//-----------------------------------------------
// GeneraAleatorioAtaqueZombi: Cuando llega el 
//   momento de hacer un ataque genera por porcentaje 
//   el ataque a realizar
//-----------------------------------------------
char CZombieBoss::GeneraAleatorioAtaqueZombi()
{
  float l_fValue = m_vAleatorios->GetValue() * 100.0f;
 
  //valores de porcentaje mejor por XML   
  if(l_fValue < 15.f)
  {
    return 1;
  }
  else if((l_fValue > 15.f) && (l_fValue < 55.f))
  {
    return 2;
  }
  else //if((l_fValue > 70.f) && (l_fValue < 100.f))
  {
    return 3;
  }
}

void CZombieBoss::CalculaTiempoEntreGolpeYGolpe()
{
  float randValue = m_vAleatorios->GetValue();
  
  m_fTiempoEntreGolpeYGolpe = randValue * (m_fTiempoMaxEntreGolpeYGolpe - m_fTiempoMinEntreGolpeYGolpe) + m_fTiempoMinEntreGolpeYGolpe;
}

//------------------------------------------------------------------------------------------------------------------------------
void CZombieBoss::Test()
{
  CTrigger* t = CORE->GetTriggerManager()->GetTrigger("z_inst_200 ManoDerecha");
  Vect3f postrigger = t->GetPosition();
  CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->SetPosition(postrigger);
}
//------------------------------------------------------------------------------------------------------------------------------

void CZombieBoss::PosicionaManoDerecha()
{
  std::string l_sName = GetInstance()->GetName();
  CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(l_sName);
  CalSkeleton* skeleton = model->GetAnimatedInstanceModel()->GetModelCal3DHW()->getSkeleton();
 
  int l_HuesoManoDer = skeleton->getCoreSkeleton()->getCoreBoneId("arma");
  if (l_HuesoManoDer != -1)
  {
    CalVector l_V3Position = (CalVector)skeleton->getBone(l_HuesoManoDer)->getTranslationAbsolute();
    CalQuaternion V3RotationManoDerecha = (CalQuaternion)skeleton->getBone(l_HuesoManoDer)->getRotationAbsolute();
  
    V3PosicionManoDerecha.x = -l_V3Position.x;
    V3PosicionManoDerecha.y = l_V3Position.y;
    V3PosicionManoDerecha.z = l_V3Position.z;

  //este no rota el trigger
    PosicionaTrigger(V3PosicionManoDerecha, m_actManoDerecha);
  }
  else
  {
    //error 
  }
}

void CZombieBoss::EscondeTriggerManoDerecha()
{
  V3PosicionManoDerecha.x = 0.0f;
  V3PosicionManoDerecha.y = -5.0f;
  V3PosicionManoDerecha.z = 0.0f;

  PosicionaTrigger(V3PosicionManoDerecha, m_actManoDerecha);
}

bool CZombieBoss::GeneraManoDerecha()
{
  m_triggerManoDerecha = new CTrigger();
  assert (m_triggerManoDerecha);

  m_bIsOk = m_triggerManoDerecha->Init();
  if(!m_bIsOk)
  {
    LOGGER->AddNewLog(ELL_WARNING,"a CTrigger instace couldnt allocate memory");  
    return false;
  }
  m_triggerManoDerecha->SetActive(true);
  m_triggerManoDerecha->SetDimensions(Vect3f(1.2f,0.4f,1.3f));
  
  m_triggerManoDerecha->SetEntity("mi_amigo");
  //m_triggerManoDerecha->SetPosition(Vect3f(0,2,0));
  m_triggerManoDerecha->SetPosition(Vect3f(0,-2,0));
  std::string l_sNameTrigger = GetUserData()->GetName();
  
  l_sNameTrigger += " ManoDerecha";
  std::string l_script;
  baseUtils::FormatSrting( l_script, "golpea_mano_derecha(\"%s\", \"%s\")", l_sNameTrigger.c_str(), m_triggerManoDerecha->GetEntity().c_str());
  m_triggerManoDerecha->SetOnEnterCode(l_script);

  m_actManoDerecha = CORE->GetTriggerManager()->GeneraTrigger(l_sNameTrigger,m_triggerManoDerecha, true);
  
  return true;
}


void CZombieBoss::PosicionaTorso()
{
  std::string l_sName = GetInstance()->GetName();
  CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(l_sName);
  CalSkeleton* skeleton = model->GetAnimatedInstanceModel()->GetModelCal3DHW()->getSkeleton();
 
  int l_HuesoTorso = skeleton->getCoreSkeleton()->getCoreBoneId("Bip01 Head");
  CalVector l_V3Position = (CalVector)skeleton->getBone(l_HuesoTorso)->getTranslationAbsolute();

  V3PosicionTorso.x = -l_V3Position.x;
  V3PosicionTorso.y = l_V3Position.y;
  V3PosicionTorso.z = l_V3Position.z;
}

Vect3f CZombieBoss::GetPosicionTorso() /*const*/
{
  Mat44f l_t,l_vMat44;
  l_t.SetIdentity();
  l_t.Translate( V3PosicionTorso );
  l_vMat44.SetIdentity();
  l_vMat44 = GetInstance()->GetMat44();
  Mat44f l_vMataux =  l_vMat44 * l_t;

  return l_vMataux.GetPos();
}  

void CZombieBoss::InicializaController()
{
  CPhysicUserData *userdata = new CPhysicUserData(GetInstance()->GetName());
  userdata->SetColor(colGREEN);
  userdata->SetPaint(true);
  GetInstance()->SetPosition(Vect3f(GetInstance()->GetPosition().x,GetInstance()->GetPosition().y + 2.5f,GetInstance()->GetPosition().z));
//  m_pPhysicController = new CPhysicController(0.5f,2.0f,2.0f,0.1f,2.0f,1,userdata,GetInstance()->GetPosition());
  m_pPhysicController = new CPhysicController(1.5f, 2.0f, 40.0f, 0.01f, 0.3f, PX_MSK_ENEMY_CONTROLLER, userdata, GetInstance()->GetPosition());
  CORE->GetPhysicsManager()->AddPhysicController(m_pPhysicController);
}

void CZombieBoss::GetDuracionAnimacionesBoss()
{
  m_fTiempoAnimAtaqueArma         = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::ATAQUECONARMA - ANIMATION_BOSS)->getDuration();
  m_fTiempoAnimPrerun             = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::IDLE - ANIMATION_BOSS)->getDuration();
  m_fTiempoAnimEmbestida          = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::EMBESTIDA - ANIMATION_BOSS)->getDuration();
  m_fTiempoAnimCrash              = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::CRASHED - ANIMATION_BOSS)->getDuration();
  m_fTiempoAnimPisoton            = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::PISOTON - ANIMATION_BOSS)->getDuration();      
  m_fTiempoAnimAtaqueReverso      = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::ATAQUEREVERSO - ANIMATION_BOSS)->getDuration(); 
  m_fTiempoAnimRecibiendoGolpe    = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::RECIBIENDOGOLPE - ANIMATION_BOSS)->getDuration();
  m_fTiempoAnimMuriendo           = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::MURIENDO - ANIMATION_BOSS)->getDuration();
  m_fTiempoAnimAndando            = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( CZombieBoss::PERSEGUIR - ANIMATION_BOSS)->getDuration();      
}

void CZombieBoss::RegistraBossLUA()
{
  CScriptManager* scriptmanager = CORE->GetScriptManager();
  lua_State* l_pLUAState = scriptmanager->GetLuaState();

  using namespace luabind;

  CCoreFIH* l_pCoreFIH = CCoreFIH::GetSingletonPtr();

  if (!l_pCoreFIH->GetBossRegistered())
  {
	  module(l_pLUAState)  
    [
      class_<CGestionLanzas>("CGestionLanzas")
          .def("set_num_iter_lanzas", &CGestionLanzas::SetNumIteracionLanzas)
    ];
    module(l_pLUAState)  
    [
      class_<CZombieBoss, CZombie>("CZombieBoss")
        // registramos su constructor
          //.def(constructor<>())
          .def("b_get_p_torso", &CZombieBoss::GetPosicionTorso)
          .def("b_get_v_retirada", &CZombieBoss::GetVelocidadRetirada)
          .def("b_get_v_embestida", &CZombieBoss::GetVelocidadEmbestida)
          .def("b_get_t_tras_golpe", &CZombieBoss::GetTiempoRecuperacionTrasDarGolpe)
          .def("b_get_t_entre_golpes", &CZombieBoss::GetTiempoEntreGolpeYGolpe)
          .def("b_get_t_inactivo", &CZombieBoss::GetTiempoInactivo)
          .def("b_get_t_encarando", &CZombieBoss::GetTiempoEncarando)
          .def("b_calc_t_entre_golpes", &CZombieBoss::CalculaTiempoEntreGolpeYGolpe)
          .def("b_acum_dt_tras_golpe", &CZombieBoss::AcumulaTiempoTrasDarGolpe)        
          .def("b_fin_t_tras_golpe", &CZombieBoss::FinTiempoTrasDarGolpe)
          .def("b_acum_dt_entre_golpes", &CZombieBoss::AcumulaTiempoEntreGolpeYGolpe)        
          .def("b_fin_t_entre_golpes", &CZombieBoss::FinTiempoEntreGolpeYGolpe)
          .def("b_acum_dt_inactivo", &CZombieBoss::AcumulaTiempoInactivo)        
          .def("b_fin_t_inactivo", &CZombieBoss::FinTiempoInactivo)
          .def("b_acum_dt_encarando", &CZombieBoss::AcumulaTiempoEncarando)        
          .def("b_fin_t_encarando", &CZombieBoss::FinTiempoEncarando)
          .def("b_reset_t_tras_golpe", &CZombieBoss::ResetTiempoTrasDarGolpe)
          .def("b_reset_t_entre_golpes", &CZombieBoss::ResetTiempoEntreGolpeYGolpe)
          .def("b_reset_t_inactivo", &CZombieBoss::ResetTiempoInactivo)
          .def("b_reset_t_encarando", &CZombieBoss::ResetTiempoEncarando)
          .def("b_genera_ataque", &CZombieBoss::GeneraAleatorioAtaqueZombi)
          .def("b_get_radius_view_original", &CZombieBoss::GetOriginalRadiusViewing)
          .def("b_get_t_anim_ataque_arma", &CZombieBoss::GetTiempoAnimacionAtaqueConArma)
          .def("b_get_t_anim_prerun", &CZombieBoss::GetTiempoAnimacionPreRun)
          .def("b_get_t_anim_embestida", &CZombieBoss::GetTiempoAnimacionEmbestida)
          .def("b_get_t_anim_crash", &CZombieBoss::GetTiempoAnimacionCrash)
          .def("b_get_t_anim_pisoton", &CZombieBoss::GetTiempoAnimacionPisoton)
          .def("b_get_t_anim_ataque_reverso", &CZombieBoss::GetTiempoAnimacionAtaqueReverso)
          .def("b_get_t_anim_recibiendo_golpe", &CZombieBoss::GetTiempoAnimacionRecibiendoGolpe)
          .def("b_get_t_anim_muriendo", &CZombieBoss::GetTiempoAnimacionMuriendo)
          .def("b_get_t_anim_andando", &CZombieBoss::GetTiempoAnimacionAndando)
          .def("b_acum_dt_anim_ataque_arma", &CZombieBoss::AcumulaTiempoAnimAtaqueArma)        
          .def("b_fin_t_anim_ataque_arma", &CZombieBoss::FinTiempoAnimAtaqueArma)
          .def("b_reset_t_anim_ataque_arma", &CZombieBoss::ResetTiempoAnimAtaqueArma)
          .def("b_momento_golpe", &CZombieBoss::MomentoDelGolpe)
          .def("b_acum_dt_anim_prerun", &CZombieBoss::AcumulaTiempoAnimPrerun)        
          .def("b_fin_t_anim_prerun", &CZombieBoss::FinTiempoAnimPrerun)
          .def("b_reset_t_anim_prerun", &CZombieBoss::ResetTiempoAnimPrerun)
          .def("b_acum_dt_anim_embestida", &CZombieBoss::AcumulaTiempoAnimEmbestida)        
          .def("b_fin_t_anim_embestida", &CZombieBoss::FinTiempoAnimEmbestida)
          .def("b_reset_t_anim_embestida", &CZombieBoss::ResetTiempoAnimEmbestida)
          .def("b_acum_dt_anim_crash", &CZombieBoss::AcumulaTiempoAnimCrash)        
          .def("b_fin_t_anim_crash", &CZombieBoss::FinTiempoAnimCrash)
          .def("b_reset_t_anim_crash", &CZombieBoss::ResetTiempoAnimCrash)
          .def("b_acum_dt_anim_pisoton", &CZombieBoss::AcumulaTiempoAnimPisoton)        
          .def("b_fin_t_anim_pisoton", &CZombieBoss::FinTiempoAnimPisoton)
          .def("b_reset_t_anim_pisoton", &CZombieBoss::ResetTiempoAnimPisoton)
          .def("b_acum_dt_anim_ataque_reverso", &CZombieBoss::AcumulaTiempoAnimAtaqueReverso)        
          .def("b_fin_t_anim_ataque_reverso", &CZombieBoss::FinTiempoAnimAtaqueReverso)
          .def("b_reset_t_anim_ataque_reverso", &CZombieBoss::ResetTiempoAnimAtaqueReverso)
          .def("b_acum_dt_anim_recibiendo_golpe", &CZombieBoss::AcumulaTiempoAnimRecibiendoGolpe)        
          .def("b_fin_t_anim_recibiendo_golpe", &CZombieBoss::FinTiempoAnimRecibiendoGolpe)
          .def("b_reset_t_anim_recibiendo_golpe", &CZombieBoss::ResetTiempoAnimRecibiendoGolpe)
          .def("b_acum_dt_anim_muriendo", &CZombieBoss::AcumulaTiempoAnimMuriendo)        
          .def("b_fin_t_anim_muriendo", &CZombieBoss::FinTiempoAnimMuriendo)
          .def("b_reset_t_anim_muriendo", &CZombieBoss::ResetTiempoAnimMuriendo)
          .def("b_acum_dt_anim_andando", &CZombieBoss::AcumulaTiempoAnimAndando)        
          .def("b_fin_t_anim_andando", &CZombieBoss::FinTiempoAnimAndando)
          .def("b_reset_t_anim_andando", &CZombieBoss::ResetTiempoAnimAndando)
          .def("b_posiciona_torso", &CZombieBoss::PosicionaTorso)
          .def("b_posiciona_arma", &CZombieBoss::PosicionaManoDerecha)
          .def("b_esconde_trigger_arma", &CZombieBoss::EscondeTriggerManoDerecha)        
          .def("b_inicia_lanzas", &CZombieBoss::IniciaLanzas)
          .def("b_get_gestion_lanzas", &CZombieBoss::GetGestionLanzas)
          .def("b_get_invencible", &CZombieBoss::GetInvencible)
          .def("b_set_invencible", &CZombieBoss::SetInvencible)
          .def("b_acum_dt_invencible", &CZombieBoss::AcumulaTiempoInvencibilidad)
          .def("b_fin_t_invencible", &CZombieBoss::FinTiempoInvencibilidad)
          .def("b_reset_t_invencible", &CZombieBoss::ResetTiempoInvencibilidad)        
    ];

    l_pCoreFIH->SetBossRegistered(true);
  }

  globals(l_pLUAState)["boss"] = this;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------