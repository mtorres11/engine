#include "ZombieManager.h"
#include "XML/XMLTreeNode.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "AnimationModel/AnimatedCoreModel.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "CharacterController/PhysicController.h"
#include "InputManager.h"
#include "ActionToInput.h"

#include "CoreFIH.h"
#include "EjeMovimientoIA.h"
#include "../ZombieResources/ZombieResources.h"
//#include "../Personaje/Personaje.h"
#include "../Zombie/Zombie.h"
#include "../ZombieRapido/ZombieRapido.h"
#include "../ZombieNormal/ZombieNormal.h"
#include "../ZombieVomitador/ZombieVomitador.h"
#include "../ZombieOlfateador/ZombieOlfateador.h"
#include "../ZombieBoss/ZombieBoss.h"
#include "../ZombieResources/ZombieAnimationReport.h"
#include "../ZombieResources/ZombieVomAnimationReport.h"

#include "ScriptManager.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>

#include "Cal3d/global.h"
#include "Cal3d/model.h"
#include "Cal3d/animcallback.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CZombieManager::Init()
{
  m_bIsOk = true;

  if (!m_bIsOk)
  {
    Release();
  }
  return m_bIsOk;
}

bool CZombieManager::Load(const char* file)
{
    //TODO..esto no deberia ir aqui
    m_bIsOk = true;
  
    m_sFilename = file;
    CXMLTreeNode newFile;

    if (!newFile.LoadFile(file))
    {
      LOGGER->AddNewLog(ELL_ERROR, "CZombieManager->Load: Error al intentar abrir el archivo: ");
      return false;
    }

    CXMLTreeNode  t = newFile["corezombie"];
    CXMLTreeNode  m = newFile["instancezombie"];

    if (t.Exists())
    {
      //std::string l_coremesh = m.GetPszProperty("core_animated_model","");
      int count = t.GetNumChildren();
//      CORE->GetAnimatedModelManager()->Load(file);//("./Data/ZombiesLoader.xml");
      for (int i = 0; i < count; ++i)
      {
        if(!t(i).IsComment())
        {
          CreateCoreZombie(t(i));
        }
      }
    }

    if (m.Exists())
    {
      int count = m.GetNumChildren();
      for (int i = 0; i < count; ++i)
      {
        if(!m(i).IsComment())
        {
          CreateZombie(m(i));
        }
      }
    }      
    return true;		//Init OK!
}

//----------------------------------------------------------------------------
// Crea Zombies
//----------------------------------------------------------------------------
void CZombieManager::CreateZombie(CXMLTreeNode& m)
{
  std::string szType	=	m.GetPszProperty("type");															 
  std::map<std::string, SCoreZombies*>::iterator itZombie = m_vCoreZombies.find(szType);

  if(itZombie->second->m_type != "")
  {
    CZombie *miZombie = NULL; 
    if (!strcmp(szType.c_str(), "ZOMBIE_NORMAL"))
    { 						
      miZombie = new CZombieNormal();																
    }
    
    if (!strcmp(szType.c_str(), "ZOMBIE_RAPIDO"))
    { 						
      miZombie = new CZombieRapido();																
    }

    if (!strcmp(szType.c_str(), "ZOMBIE_VOMITADOR"))
    { 						
      miZombie = new CZombieVomitador();																
    }

    if (!strcmp(szType.c_str(), "ZOMBIE_OLFATEADOR"))
    { 						
      miZombie = new CZombieOlfateador;																
    }

    if (!strcmp(szType.c_str(), "ZOMBIE_BOSS"))
    { 						
      miZombie = new CZombieBoss;																
    }

    if(!miZombie)
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"Se crea un zombie normal por defecto. Tipo incorrecto.");
      miZombie = new CZombieNormal();																
    }

    std::string l_sName = m.GetPszProperty("name");
    miZombie->InicializaUserData(l_sName);

    miZombie->Init( m );
    AsignaCoreCorrespondiente(*miZombie,*itZombie->second);


    miZombie->SetPosInicial((Vect3f*)&m.GetVect3fProperty("pos",Vect3f(0.f,0.f,0.f)));  
    Vect3f rotacionInicial = m.GetVect3fProperty("rot",Vect3f(0.f,0.f,0.f));
    miZombie->SetRotInicial(&rotacionInicial);

    miZombie->SetInstance(
      (CRenderableAnimatedInstanceModel *)
      CORE->GetRenderableObjectsManager()->AddAnimatedModelInstance(
      itZombie->second->m_coremesh,
      l_sName,
      miZombie->GetPosInicial(),
      miZombie->GetRotInicial().x,
      miZombie->GetRotInicial().z,
      miZombie->GetRotInicial().y,
      itZombie->second->m_fScaled,
      itZombie->second->m_fScaled,
      itZombie->second->m_fScaled
      )       
    );
    miZombie->GetInstance()->SetVisible(false);

    miZombie->InicializaController();
    miZombie->GetInstance()->GetAnimatedInstanceModel()->GetModelCal3DHW()->setUserData((CZombieUserData*) miZombie->GetUserData());

    if (!strcmp(szType.c_str(), "ZOMBIE_NORMAL"))
    {
      AddResource(miZombie->GetInstance()->GetName(), (CZombieNormal*)miZombie);
      m_vZombiesNormales.push_back((CZombieNormal*)miZombie);
    }
    if (!strcmp(szType.c_str(), "ZOMBIE_RAPIDO"))
    { 						
      AddResource(miZombie->GetInstance()->GetName(), (CZombieRapido*)miZombie);
      m_vZombiesRapidos.push_back((CZombieRapido*)miZombie);
    }

    if (!strcmp(szType.c_str(), "ZOMBIE_VOMITADOR"))
    { 						
      AddResource(miZombie->GetInstance()->GetName(), (CZombieVomitador*)miZombie);
      m_vZombiesVomitadores.push_back((CZombieVomitador*)miZombie);
    }

    if (!strcmp(szType.c_str(), "ZOMBIE_OLFATEADOR"))
    { 						
      AddResource(miZombie->GetInstance()->GetName(), (CZombieOlfateador*)miZombie);
      m_vZombiesOlfateadores.push_back((CZombieOlfateador*)miZombie);
    }

    if (!strcmp(szType.c_str(), "ZOMBIE_BOSS"))
    { 						
      AddResource(miZombie->GetInstance()->GetName(), (CZombieBoss*)miZombie);
      m_zombieBoss = (CZombieBoss*) miZombie;  
      //aqui estos tres elementos??
//      RegistraBossLUA();

      //valor permanente del radio de vision para recuperarlo despues de la embestida
      m_zombieBoss->SetOriginalRadiusViewing( m_zombieBoss->GetRadiusViewing() );

      m_zombieBoss->GetDuracionAnimacionesBoss();
    
    }
  }
  else
  {
    std::string msg_warning = "ZombieManager::Load->Error al intentar leer el <zombies> en archivo xml correspondiente ";
    LOGGER->AddNewLog(ELL_WARNING, msg_warning.c_str());
  }
}

void CZombieManager::CreateZombie(std::string _type, std::string _name, Vect3f _initPos, Vect3f _initRot)
{
  std::map<std::string, SCoreZombies*>::iterator itZombie = m_vCoreZombies.find(_type);

  if(itZombie->second->m_type != "")
  {
    CZombie *miZombie = NULL; 
    if (!strcmp(_type.c_str(), "ZOMBIE_NORMAL"))
    { 						
      miZombie = new CZombieNormal();																
    }
    if (!strcmp(_type.c_str(), "ZOMBIE_RAPIDO"))
    { 						
      miZombie = new CZombieRapido();																
    }

    if (!strcmp(_type.c_str(), "ZOMBIE_VOMITADOR"))
    { 						
      miZombie = new CZombieVomitador();																
    }

    if (!strcmp(_type.c_str(), "ZOMBIE_OLFATEADOR"))
    { 						
      miZombie = new CZombieOlfateador;																
    }

    if (!strcmp(_type.c_str(), "ZOMBIE_BOSS"))
    { 						
      miZombie = new CZombieBoss;																
    }

    if(!miZombie)
    {
      LOGGER->AddNewLog(ELL_INFORMATION,"Se crea un zombie normal por defecto. Tipo incorrecto.");
      miZombie = new CZombieNormal();																
    }
    miZombie->InicializaUserData(_name);

    miZombie->Init( (const CZombie&)miZombie );
    AsignaCoreCorrespondiente(*miZombie,*itZombie->second);


    miZombie->SetPosInicial(&_initPos);  
    miZombie->SetRotInicial(&_initRot);

    miZombie->SetInstance(
      (CRenderableAnimatedInstanceModel *)
      CORE->GetRenderableObjectsManager()->AddAnimatedModelInstance(
      itZombie->second->m_coremesh,
      _name,
      miZombie->GetPosInicial(),
      miZombie->GetRotInicial().x,
      miZombie->GetRotInicial().z,
      miZombie->GetRotInicial().y,
      itZombie->second->m_fScaled,
      itZombie->second->m_fScaled,
      itZombie->second->m_fScaled
      )       
    );

    miZombie->GetInstance()->SetVisible(false);

    miZombie->InicializaController();
    miZombie->GetInstance()->GetAnimatedInstanceModel()->GetModelCal3DHW()->setUserData((CZombieUserData*) miZombie->GetUserData());
    miZombie->GetInstance()->SetVisible(true);

    if (!strcmp(_type.c_str(), "ZOMBIE_NORMAL"))
    {
      AddResource(miZombie->GetInstance()->GetName(), (CZombieNormal*)miZombie);
      m_vZombiesNormales.push_back((CZombieNormal*)miZombie);
    }
    if (!strcmp(_type.c_str(), "ZOMBIE_RAPIDO"))
    { 						
      AddResource(miZombie->GetInstance()->GetName(), (CZombieRapido*)miZombie);
      m_vZombiesRapidos.push_back((CZombieRapido*)miZombie);
    }

    if (!strcmp(_type.c_str(), "ZOMBIE_VOMITADOR"))
    { 						
      AddResource(miZombie->GetInstance()->GetName(), (CZombieVomitador*)miZombie);
      m_vZombiesVomitadores.push_back((CZombieVomitador*)miZombie);
    }

    if (!strcmp(_type.c_str(), "ZOMBIE_OLFATEADOR"))
    { 						
      AddResource(miZombie->GetInstance()->GetName(), (CZombieOlfateador*)miZombie);
      m_vZombiesOlfateadores.push_back((CZombieOlfateador*)miZombie);
    }

    if (!strcmp(_type.c_str(), "ZOMBIE_BOSS"))
    { 						
      AddResource(miZombie->GetInstance()->GetName(), (CZombieBoss*)miZombie);
      m_zombieBoss = (CZombieBoss*) miZombie;  
    }
  }
  else
  {
    std::string msg_warning = "ZombieManager::Load->Error al intentar leer el <zombies> en archivo xml correspondiente ";
    LOGGER->AddNewLog(ELL_WARNING, msg_warning.c_str());
  }

}

//----------------------------------------------------------------------------
// Crea Core de Zombie (Valores comunes para todos los de un tipo)
//----------------------------------------------------------------------------
void CZombieManager::CreateCoreZombie(CXMLTreeNode& m)
{
  SCoreZombies *zombie = new SCoreZombies;
  zombie->m_type = m.GetPszProperty("name","zombie");
  zombie->m_coremesh = m.GetPszProperty("core_animated_model","");
  zombie->m_fMoveSpeed = m.GetFloatProperty("move_speed",0.f);
  zombie->m_fRotSpeed = m.GetFloatProperty("rotate_speed",0.f);
  zombie->m_fRadiusSmelling = m.GetFloatProperty("radius_smelling",0.f);
  zombie->m_fRadiusViewing = m.GetFloatProperty("radius_viewing",0.f);
  zombie->m_fScaled = m.GetFloatProperty("scaled",1.f);
  zombie->m_fHealth = m.GetFloatProperty("health", 0.0f);

  CAnimatedCoreModel* animCoreModel = CORE->GetAnimatedModelManager()->GetCore(zombie->m_coremesh,"");
  if(!animCoreModel)
  {
     std::string msg_error = "CZombieManager::CreateCoreZombie-> No hay core";
     LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
  }
  else
  {
    m_vCoreZombies[zombie->m_type] = zombie;
  }
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CZombieManager::Done()
{
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CZombieManager::Release(){
  //free memory

  if(CORE->GetAnimatedModelManager()->GetResource("zombie"))
  {
    CHECKED_DELETE(m_zombieAttackCallback);
    CHECKED_DELETE(m_zombieDieCallback );
    CHECKED_DELETE(m_zombieReceiveHitCallback );
  }
  
  if(CORE->GetAnimatedModelManager()->GetResource("zombierapido"))
  {
    CHECKED_DELETE(m_zombieFastAttackCallback);
    CHECKED_DELETE(m_zombieFastDieCallback );
    CHECKED_DELETE(m_zombieFastReceiveHitCallback );
  }
    
  if(CORE->GetAnimatedModelManager()->GetResource("zombievomitador"))
  {
    CHECKED_DELETE(m_zombieVomAppearCallback);
    CHECKED_DELETE(m_zombieVomShootCallback);
    CHECKED_DELETE(m_zombieVomReceiveHitCallback);
    CHECKED_DELETE(m_zombieVomGoOutCallback);
    CHECKED_DELETE(m_zombieVomDieCallback);
  
    CHECKED_DELETE(m_craterCallback);
  }
  //  DeregistroAnimacionesCallback();

  std::map<std::string,SCoreZombies*>::iterator l_iter = m_vCoreZombies.begin();
  std::map<std::string,SCoreZombies*>::iterator l_iterEnd = m_vCoreZombies.end();

  for (;l_iter != l_iterEnd;++l_iter)
  {
    CHECKED_DELETE(l_iter->second);
  }
  m_vCoreZombies.clear();

  m_vZombiesNormales.clear();
  m_vZombiesRapidos.clear();
  m_vZombiesVomitadores.clear();
  m_vZombiesOlfateadores.clear();

  //destruye boss
  
  Destroy();

  if (m_zombieBoss)
  {
    m_zombieBoss = NULL;
  }
}

//----------------------------------------------------------------------------
// Asigna valores de core a un zombie en concreto
//----------------------------------------------------------------------------
void CZombieManager::AsignaCoreCorrespondiente(CZombie& zombie, SCoreZombies core)
{
  zombie.SetMoveSpeed(core.m_fMoveSpeed);
  zombie.SetRotSpeed(core.m_fRotSpeed);
  zombie.SetRadiuwViewing(core.m_fRadiusViewing);
  zombie.SetHealth(core.m_fHealth);
  zombie.SetModelFilename(core.m_coremesh);
  zombie.SetRadiusSmelling(core.m_fRadiusSmelling);
}

//----------------------------------------------------------------------------
// reload
//----------------------------------------------------------------------------
void CZombieManager::Reload()
{
  Release();
  //Init(m_sFilename.c_str());
}

//----------------------------------------------------------------------------
// update
//----------------------------------------------------------------------------
void CZombieManager::Update(float _elapsedTime)
{
  if(CORE->GetInputManager()->GetActionToInput()->DoAction("Reload_Zombie_Manager") != 0.0f )
  {
    Reload();
  }

  std::vector<CZombieNormal*>::iterator l_Iter1 = m_vZombiesNormales.begin();
  for(;l_Iter1 != m_vZombiesNormales.end();l_Iter1++)
  {
		if(((*l_Iter1)->GetState() != (char)CZombie::INACTIVO)&&((*l_Iter1)->GetState() != (char)CZombie::MUERTO))
    {
      (*l_Iter1)->Update(_elapsedTime);
    }
  }

  std::vector<CZombieRapido*>::iterator l_Iter2 = m_vZombiesRapidos.begin();
  for(;l_Iter2 != m_vZombiesRapidos.end();++l_Iter2)
  {
    if(((*l_Iter2)->GetState() != (char)CZombie::INACTIVO)&&((*l_Iter2)->GetState() != (char)CZombie::MUERTO))
    {
      (*l_Iter2)->Update(_elapsedTime);
    }
  }

  std::vector<CZombieVomitador*>::iterator l_Iter3 = m_vZombiesVomitadores.begin();
  for(;l_Iter3 != m_vZombiesVomitadores.end();++l_Iter3)
  {
    if(((*l_Iter3)->GetState() != (char)CZombie::INACTIVO)&&((*l_Iter3)->GetState() != (char)CZombie::MUERTO))
    {
      (*l_Iter3)->Update(_elapsedTime);
    }
    
    if((*l_Iter3)->GetState() == (char)CZombie::INACTIVO)  
    {
      (*l_Iter3)->GetInstance()->SetVisible(false);
    }
  }

  std::vector<CZombieOlfateador*>::iterator l_Iter4 = m_vZombiesOlfateadores.begin();
  for(;l_Iter4 != m_vZombiesOlfateadores.end();++l_Iter4)
  {
    if(((*l_Iter4)->GetState() != (char)CZombie::INACTIVO)&&((*l_Iter4)->GetState() != (char)CZombie::MUERTO))
    {
      (*l_Iter4)->Update(_elapsedTime);
    }
  }
  
  //BOSS
  if(m_zombieBoss)
  {
//    if((m_zombieBoss->GetState() != (char)CZombieBoss::INACTIVO)&&(m_zombieBoss->GetState() != (char)CZombieBoss::MUERTO))
    if(m_zombieBoss->GetState() != (char)CZombieBoss::MUERTO)
    {
      char x = m_zombieBoss->GetState();
      m_zombieBoss->Update(_elapsedTime);
    }
  }
}


//----------------------------------------------------------------------------
// Render
//----------------------------------------------------------------------------
void CZombieManager::Render(CRenderManager *rm)
{
  std::vector<CZombieNormal*>::iterator l_Iter1 = m_vZombiesNormales.begin();
  for(;l_Iter1 != m_vZombiesNormales.end();l_Iter1++)
  {
    (*l_Iter1)->Render(rm);
  }
  std::vector<CZombieRapido*>::iterator l_Iter2 = m_vZombiesRapidos.begin();
  for(;l_Iter2 != m_vZombiesRapidos.end();l_Iter2++)
  {
    (*l_Iter2)->Render(rm);
  }
  std::vector<CZombieVomitador*>::iterator l_Iter3 = m_vZombiesVomitadores.begin();
  for(;l_Iter3 != m_vZombiesVomitadores.end();l_Iter3++)
  {
    (*l_Iter3)->Render(rm);
  }

  std::vector<CZombieOlfateador*>::iterator l_Iter4 = m_vZombiesOlfateadores.begin();
  for(;l_Iter4 != m_vZombiesOlfateadores.end();++l_Iter4)
  {
    (*l_Iter4)->Render(rm);
  }
  
  //BOSS
  if(m_zombieBoss)
  {
    m_zombieBoss->Render(rm);
  }
}

//----------------------------------------------------------------------------
// Registro de animaciones callbacks
//----------------------------------------------------------------------------
void CZombieManager::RegistroAnimacionesCallback()
{
  if(CORE->GetAnimatedModelManager()->GetResource("zombie"))
  {
    //se registra la animacion de atacar. 
    m_zombieAttackCallback = new CZombieAnimationReport();
    CalCoreAnimation* animAtacarZombi = CORE->GetAnimatedModelManager()->GetResource("zombie")->GetCoreModel()->getCoreAnimation(CZombie::ATACANDO);
    animAtacarZombi->registerCallback(m_zombieAttackCallback,0.1f);
    //  animAtacarZombi->getCallbackList().push_back(animAtacarZombi);

    //se registra la animacion de atacar. 
    m_zombieDieCallback = new CZombieAnimationReport();
    CalCoreAnimation* animMorirZombi = CORE->GetAnimatedModelManager()->GetResource("zombie")->GetCoreModel()->getCoreAnimation(CZombie::MURIENDO);
    animMorirZombi->registerCallback(m_zombieDieCallback,0.1f);
    //  animAtacarZombi->getCallbackList().push_back(animAtacarZombi);

    //se registra la animacion de recibir golpe. 
    m_zombieReceiveHitCallback = new CZombieAnimationReport();
    CalCoreAnimation* animRecibirZombi = CORE->GetAnimatedModelManager()->GetResource("zombie")->GetCoreModel()->getCoreAnimation(CZombie::RECIBIENDOGOLPE);
    animRecibirZombi->registerCallback(m_zombieReceiveHitCallback,0.1f);
    //  animAtacarZombi->getCallbackList().push_back(animAtacarZombi);
  }

  if(CORE->GetAnimatedModelManager()->GetResource("zombierapido"))
  {
    //se registra la animacion de atacar. 
    m_zombieFastAttackCallback  = new CZombieAnimationReport();
    CalCoreAnimation* animAtacarZombiRapido = CORE->GetAnimatedModelManager()->GetResource("zombierapido")->GetCoreModel()->getCoreAnimation(CZombieRapido::ATACANDO);
    animAtacarZombiRapido->registerCallback(m_zombieFastAttackCallback,0.1f);
    //  animAtacarZombi->getCallbackList().push_back(animAtacarZombi);

    //se registra la animacion de atacar. 
    m_zombieFastDieCallback = new CZombieAnimationReport();
    CalCoreAnimation* animMorirZombiRapido = CORE->GetAnimatedModelManager()->GetResource("zombierapido")->GetCoreModel()->getCoreAnimation(CZombieRapido::MURIENDO);
    animMorirZombiRapido->registerCallback(m_zombieFastDieCallback,0.1f);
    //  animAtacarZombi->getCallbackList().push_back(animAtacarZombi);

    //se registra la animacion de recibir golpe. 
    m_zombieFastReceiveHitCallback = new CZombieAnimationReport();
    CalCoreAnimation* animRecibirZombiRapido = CORE->GetAnimatedModelManager()->GetResource("zombierapido")->GetCoreModel()->getCoreAnimation(CZombieRapido::RECIBIENDOGOLPE);
    animRecibirZombiRapido->registerCallback(m_zombieFastReceiveHitCallback,0.1f);
    //  animAtacarZombi->getCallbackList().push_back(animAtacarZombi);
  }
  
  if(CORE->GetAnimatedModelManager()->GetResource("zombievomitador"))
  {
    m_zombieVomAppearCallback = new CZombieVomAnimationReport();
    CalCoreAnimation* animApareceZombiVom = CORE->GetAnimatedModelManager()->GetResource("zombievomitador")->GetCoreModel()->getCoreAnimation(CZombieVomitador::VOM_APARECIENDO - OFFSET_ANIMATIONS);
    animApareceZombiVom->registerCallback(m_zombieVomAppearCallback,0.1f);

    m_zombieVomShootCallback = new CZombieVomAnimationReport();
    CalCoreAnimation* animDisparaZombiVom = CORE->GetAnimatedModelManager()->GetResource("zombievomitador")->GetCoreModel()->getCoreAnimation(CZombieVomitador::VOM_DISPARANDO - OFFSET_ANIMATIONS);
    animDisparaZombiVom->registerCallback(m_zombieVomShootCallback,0.1f);  

    m_zombieVomReceiveHitCallback = new CZombieVomAnimationReport();
    CalCoreAnimation* animRecibirZombiVom = CORE->GetAnimatedModelManager()->GetResource("zombievomitador")->GetCoreModel()->getCoreAnimation(CZombieVomitador::VOM_RECIBEGOLPE - OFFSET_ANIMATIONS);
    animRecibirZombiVom->registerCallback(m_zombieVomReceiveHitCallback,0.1f);

    m_zombieVomGoOutCallback = new CZombieVomAnimationReport();
    CalCoreAnimation* animSaleZombiVom = CORE->GetAnimatedModelManager()->GetResource("zombievomitador")->GetCoreModel()->getCoreAnimation(CZombieVomitador::VOM_SALIDA - OFFSET_ANIMATIONS);
    animSaleZombiVom->registerCallback(m_zombieVomGoOutCallback,0.1f);  

    m_zombieVomDieCallback = new CZombieVomAnimationReport();
    CalCoreAnimation* animMorirZombiVom = CORE->GetAnimatedModelManager()->GetResource("zombievomitador")->GetCoreModel()->getCoreAnimation(CZombieVomitador::VOM_MURIENDO - OFFSET_ANIMATIONS);
    animMorirZombiVom->registerCallback(m_zombieVomDieCallback,0.1f);

    //m_craterCallback = new CZombieVomAnimationReport();
    //CalCoreAnimation* animCraterZombiVom = CORE->GetAnimatedModelManager()->GetResource("arenazombievomitador")->GetCoreModel()->getCoreAnimation(1);//aparicion crater
    //animCraterZombiVom->registerCallback(m_craterCallback,0.1f);
  }
}


//----------------------------------------------------------------------------
// Deregistro de animaciones callback
//----------------------------------------------------------------------------
void CZombieManager::DeregistroAnimacionesCallback(void)
{
  //CalCoreAnimation* animAtacarZombi = CORE->GetAnimatedModelManager()->GetResource("zombie")->GetCoreModel()->getCoreAnimation(4);
  //for(int i=0;i<animAtacarZombi->getCallbackList().size(); ++i)
  //{
  //  animAtacarZombi->removeCallback(animAtacarZombi->getCallbackList().pop_back();
  //}
  //animAtacarZombi->getCallbackList().erase();
}

void CZombieManager::ActivaDesactivaZombies(bool flag)
{
  char estado = CZombie::INACTIVO;
  std::vector<CZombieNormal*>::iterator l_Iter1 = m_vZombiesNormales.begin();
  for(;l_Iter1 != m_vZombiesNormales.end();l_Iter1++)
  {
    if((*l_Iter1)->GetState() != CZombie::MUERTO)
    {
      estado = flag? CZombie::IDLE : CZombie::INACTIVO;
      (*l_Iter1)->SetState(estado);
    }
  }
  std::vector<CZombieRapido*>::iterator l_Iter2 = m_vZombiesRapidos.begin();
  for(;l_Iter2 != m_vZombiesRapidos.end();l_Iter2++)
  {
    if((*l_Iter2)->GetState() != CZombie::MUERTO)
    {
      estado = flag? CZombie::IDLE : CZombie::INACTIVO;
      (*l_Iter2)->SetState(estado);
    }
  }
  std::vector<CZombieVomitador*>::iterator l_Iter3 = m_vZombiesVomitadores.begin();
  for(;l_Iter3 != m_vZombiesVomitadores.end();l_Iter3++)
  {
    if((*l_Iter3)->GetState() != CZombie::MUERTO)
    {
      estado = flag? CZombie::IDLE : CZombie::INACTIVO;
      (*l_Iter3)->SetState(estado);
    }
  }

  std::vector<CZombieOlfateador*>::iterator l_Iter4 = m_vZombiesOlfateadores.begin();
  for(;l_Iter4 != m_vZombiesOlfateadores.end();++l_Iter4)
  {
    if((*l_Iter4)->GetState() != CZombie::MUERTO)
    {
      estado = flag? CZombie::IDLE : CZombie::INACTIVO;
      (*l_Iter4)->SetState(estado);
    }
  }
  
  //BOSS
  if(m_zombieBoss)
  {
    if(m_zombieBoss->GetState() != CZombie::MUERTO)
    {
      estado = flag? CZombieBoss::IDLE : CZombieBoss::INACTIVO;
      m_zombieBoss->SetState(estado);
    }
  }
}

void CZombieManager::SetActiveZombies(bool status)
{
  unsigned int estado = CZombie::INACTIVO;
  if(status)
  {
    estado = CZombie::IDLE;
  }
  SetStatus(estado);
}

void CZombieManager::SetStatus(unsigned int status)
{
  std::vector<CZombieNormal*>::iterator l_Iter1 = m_vZombiesNormales.begin();
  for(;l_Iter1 != m_vZombiesNormales.end();l_Iter1++)
  {
    (*l_Iter1)->SetState(status);
  }

  std::vector<CZombieRapido*>::iterator l_Iter2 = m_vZombiesRapidos.begin();
  for(;l_Iter2 != m_vZombiesRapidos.end();l_Iter2++)
  {
    (*l_Iter2)->SetState(status);
  }
  std::vector<CZombieVomitador*>::iterator l_Iter3 = m_vZombiesVomitadores.begin();
  for(;l_Iter3 != m_vZombiesVomitadores.end();l_Iter3++)
  {
    (*l_Iter3)->SetState(status);
  }

  std::vector<CZombieOlfateador*>::iterator l_Iter4 = m_vZombiesOlfateadores.begin();
  for(;l_Iter4 != m_vZombiesOlfateadores.end();++l_Iter4)
  {
    (*l_Iter4)->SetState(status);
  }
  
  //BOSS siempre activo de momento
  if(m_zombieBoss)
    m_zombieBoss->SetState(CZombieBoss::IDLE);
    
}

void CZombieManager::DestroyZombie(const std::string &_name)
{ 
  if(GetResource(_name))
  {
    //borra todo del zombie
    GetResource(_name)->DestruyeZombie(0);
  }
}

CCoreFIH* GetCoreFIH()
{

//  return CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetResource("")->GetInstance()->SetVisible(true);
//  return CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetResource("")->GetPhysicController()->se>GetInstance()->SetPosition(0,0,0);
  return CCoreFIH::GetSingletonPtr();
}

//----------------------------------------------------------------------------
// Registro de funciones LUA
//----------------------------------------------------------------------------
void CZombieManager::RegistroFuncionesLUA()
{
  CScriptManager* scriptmanager = CORE->GetScriptManager();
  lua_State* l_pLUAState = scriptmanager->GetLuaState();

	using namespace luabind;
	// ahora registramos lo que querramos

  module(l_pLUAState)  
  [
    def("corefih", GetCoreFIH)
  ];
	
  
  module(l_pLUAState)  
  [
    class_<CCoreFIH>("CCoreFIH")
    .def("get_z_manager", &CCoreFIH::GetZombieManager)
  ];


  module (l_pLUAState) 
  [
    class_<CMapManager<CZombie>>("CMapManager"), class_<CZombieManager, CMapManager<CZombie>>("CZombieManager")
      .def("get_z_normales", &CZombieManager::GetZombiesNormales)
      .def("get_z_rapidos", &CZombieManager::GetZombiesRapidos)
      .def("get_z_vomitadores",  &CZombieManager::GetZombiesVomitadores)
      .def("get_z_olfateadores", &CZombieManager::GetZombiesOlfateadores)
      .def("get_resource", &CZombieManager::GetResource)
      .def("activa_desactiva_zombies", &CZombieManager::ActivaDesactivaZombies)
      .def("set_active_zombies", &CZombieManager::SetActiveZombies)
      .def("create_zombie", (void(CZombieManager::*)(std::string, std::string,Vect3f,Vect3f)) &CZombieManager::CreateZombie)//(std::string _type, std::string _name, Vect3f _initPos, Vect3f _initRot));
      .def("destroy_zombie", &CZombieManager::DestroyZombie)
  ];

  // EjeMovimientoIA  
  module(l_pLUAState)
    [
      class_<CEjeMovimiento>("CEjeMovimiento")
      .def("mov_agent_ia", &CEjeMovimiento::MovAgentIA)
      .def("get_vector_director", &CEjeMovimiento::GetVectorDirector)
      .def("get_vector_director_optimo", &CEjeMovimiento::GetVectorDirectorOptimo)
      .def("comprueba_dentro_area_vision", &CEjeMovimiento::CompruebaPlayerDentroAreaVision)
      .def("reset_datos_inicializacion", &CEjeMovimiento::ResetDatosInicializacion)
      .def_readwrite("angle_director", &CEjeMovimiento::m_fAngleTotalPlayer2)
    ];

  // ZombieUserData  
  module(l_pLUAState)
    [
      class_<CZombieUserData>("CZombieUserData")
      .def("set_name", &CZombieUserData::SetName)
      .def("get_name", &CZombieUserData::GetName)
      .def("set_is_collided", &CZombieUserData::SetIsCollided)
      .def("get_is_collided", &CZombieUserData::GetIsCollided)
      .def("set_is_action_finalized", &CZombieUserData::SetIsActionFinalized)
      .def("get_is_action_finalized", &CZombieUserData::GetIsActionFinalized)
      .def("set_current_action", &CZombieUserData::SetCurrentAction)
      .def("get_current_action", &CZombieUserData::GetCurrentAction)
    ];

  module(l_pLUAState)  
  [
      class_<CPersonaje>("CPersonaje"),class_<CZombie, CPersonaje>("CZombie")
      // registramos su constructor
        .def(constructor<>())
        .def("z_get_instance", &CPersonaje::GetInstance)
        .def("z_get_physic_controller",&CPersonaje::GetPhysicController)
        .def("z_receive_damage", &CPersonaje::ReceiveDamage)
        .def("z_get_pos_ini", &CPersonaje::GetPosInicial)
        .def("z_is_dead", &CPersonaje::IsDead)
        .def("z_get_health", &CZombie::GetHealth)
        .def("z_set_health", &CZombie::SetHealth)
        .def("z_set_state", &CZombie::SetState)
        .def("z_get_state",&CZombie::GetState)
        .def("z_set_fase_estado",&CZombie::SetFaseEstado)
        .def("z_get_fase_estado",&CZombie::GetFaseEstado)
        //.def("z_get_actor_mano_derecha",&CZombie::GetPxActorTriggerManoDerecha)
        //.def("z_get_actor_mano_izquierda",&CZombie::GetPxActorTriggerManoIzquierda)
        //.def("z_get_trigger_mano_derecha",&CZombie::GetTriggerManoDerecha)
        //.def("z_get_trigger_mano_izquierda",&CZombie::GetTriggerManoIzquierda)
        .def("z_posiciona_mano_izquierda",&CZombie::PosicionaManoIzquierda)
        .def("z_posiciona_mano_derecha",&CZombie::PosicionaManoDerecha)
        .def("z_esconde_punyos",&CZombie::EscondePunyos)
        .def("z_posiciona_trigger",&CZombie::PosicionaTrigger)
        .def("z_get_eje_ia", &CZombie::GetEjeMovimientoIA)
        .def("z_persigue", &CZombie::Perseguir)
        .def("z_evadir", &CZombie::Evadir)
        .def("z_get_rad_smell", &CZombie::GetRadiusSmelling)
        .def("z_get_rad_view", &CZombie::GetRadiusViewing)
        .def("z_set_rad_smell", &CZombie::SetRadiusSmelling)
        .def("z_set_rad_view", &CZombie::SetRadiuwViewing)
        .def("z_encarando", &CZombie::Encarando)
        .def("z_get_userdata", &CZombie::GetUserData)
        .def("z_set_userdata", &CZombie::SetUserData)
  ];

  
  //module
  //[
  //  class_<CalAnimationCallback>("CalAnimationCallback"), class_<CZombieAnimationReport, CalAnimationCallback>("CZombieAnimationReport")
  //    .def("animation_update" , AnimationUpdate)
  //    .def("animation_complete" , AnimationComplete)
  //];
  
}




//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------