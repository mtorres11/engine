#include "Zombie.h"
#include "XML\XMLTreeNode.h"
#include "PhysicsManager.h"
#include "RenderablePhysX/RenderablePhysXObject.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "CharacterController/PhysicController.h"
#include "Trigger/TriggerManager.h"
#include "Trigger/Trigger.h"
#include "Utils/BaseUtils.h"
#include "SoundManager.h"
#include "ItemManager.h"
#include "Timer/Timer.h"
#include "BillBoard/BillboardManager.h"
#include "Player/Player.h"

//borrar?
#include "Actor/PhysicActor.h"

#include "../IA/EjeMovimientoIA.h"

#define TIEMPO_DESAPARECE 0.1f

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CZombie::CZombie():CPersonaje()
{
  m_bIsOk = false;
  m_cState = (char)IDLE;
  m_faseEstado = INIT;
  m_movIA = 0;
  V3PosicionManoIzquierda(0.f,0.f,0.f);
  V3PosicionManoDerecha(0.f,0.f,0.f);
  m_actManoDerecha=0;
  m_actManoIzquierda=0;
  m_triggerManoIzquierda = 0;
  m_triggerManoDerecha = 0;
  m_pUserData = 0;
	m_timeDead = 0.0f;
  m_iStepMuerto = 0;
  m_bVisible=true;
}

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CZombie::Init(const CXMLTreeNode &m)
{
  m_bIsOk = Inherited::Init(m);
  if (!m_bIsOk)
  {
    LOGGER->AddNewLog(ELL_WARNING,"a CCZombie instace couldnt allocate memory");
  }

  SetState( IDLE );
  m_movIA = new CEjeMovimiento();
  assert (m_movIA);
  m_bIsOk = m_movIA->Init();
  if (!m_bIsOk)
  {
    LOGGER->AddNewLog(ELL_WARNING,"a CEjeMovimiento instace couldnt allocate memory");
  }

  std::string l_sType = m.GetPszProperty("type", "");
  if((l_sType.compare("ZOMBIE_BOSS"))&&(l_sType.compare("ZOMBIE_VOMITADOR")))
  {
    if(m_bIsOk)
    {
      m_bIsOk = GeneraManoIzquierda();
    }
  }
  if(l_sType.compare("ZOMBIE_VOMITADOR"))
  {
    if(m_bIsOk)
    {
      m_bIsOk = GeneraManoDerecha ();
    }
  }    
  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CZombie::Init(const CZombie &z)
{
  m_bIsOk = true;//Inherited::Init(m);
  if (!m_bIsOk)
  {
    LOGGER->AddNewLog(ELL_WARNING,"a CCZombie instace couldnt allocate memory");
  }

  SetState( IDLE );
  m_movIA = new CEjeMovimiento();
  assert (m_movIA);
  m_bIsOk = m_movIA->Init();
  if (!m_bIsOk)
  {
    LOGGER->AddNewLog(ELL_WARNING,"a CEjeMovimiento instace couldnt allocate memory");
  }

  if(m_bIsOk)
  {
    m_bIsOk = GeneraManoIzquierda();
  }
  if(m_bIsOk)
  {
    m_bIsOk = GeneraManoDerecha();
  }
  
  return m_bIsOk;
}


bool CZombie::InicializaUserData(std::string name)
{
    m_pUserData = new CZombieUserData();
    m_bIsOk = (m_pUserData != NULL);
    
    if(m_bIsOk)
    {
      m_pUserData->Init(name);
    }
    else
    {
      LOGGER->AddNewLog(ELL_WARNING,"a CZombieUserData instace couldnt allocate memory");
      m_bIsOk = false;
    }

    return m_bIsOk;
}
//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CZombie::Done()
{
  Inherited::Done();
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CZombie::Release(){
//free memory
  CHECKED_DELETE(m_pUserData);
  CHECKED_DELETE(m_actManoDerecha);
  CHECKED_DELETE(m_actManoIzquierda);
  /*CHECKED_DELETE(m_triggerManoDerecha);
  CHECKED_DELETE(m_triggerManoIzquierda);*/
  CHECKED_DELETE(m_movIA);
}

void CZombie::Update(float elapsedTime)
{
}

void CZombie::Render()

{


}

char CZombie::RecalculateIA(Vect3f hisPosition)
{
  Vect3f myPosition( GetInstance()->GetPosition() );

  switch(GetState())
  { 
    case (char)IDLE:
      GoIdle(hisPosition,myPosition);
    break;
    case (char)OLIENDO:
      GoOliendo(hisPosition,myPosition);
    break;
    case (char)MIRANDO:
      GoMirando(hisPosition,myPosition);
    break;
    case (char)ANDANDO:
//      m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->BlendCycle(MIRANDO,elapsedTime);
      GoAndando(hisPosition,myPosition);
    break;
    case (char)ATACANDO:
      GoAtacando(hisPosition,myPosition);
    break;
    case (char)RECIBIENDOGOLPE:
      GoRecibiendoGolpe(hisPosition,myPosition);
    break;
    case (char)VOLVIENDOINICIO:
      GoVolviendoInicio(hisPosition,myPosition);
    break;
    case (char)MURIENDO:
      GoMuriendo();
    break;
  }
  return GetState();
}

//no utilizada
CZombie& CZombie::operator = (CZombie& z)
{
  SetHealth(z.GetHealth());
  SetInstance(z.GetInstance());
  SetModelFilename(z.GetModelFilename());
  SetMoveSpeed(z.GetMoveSpeed());
  SetRotSpeed(z.GetRotSpeed());
  GetInstance()->SetScalar(z.GetInstance()->GetScalarX(),z.GetInstance()->GetScalarX(),z.GetInstance()->GetScalarX());
  SetRadiusSmelling(z.GetRadiusSmelling());
  SetRadiuwViewing(z.GetRadiusViewing());
  GetInstance()->SetName(z.GetInstance()->GetName());
  SetEjeMovimientoIA(z.GetEjeMovimientoIA());

  return *this;
}

void CZombie::GoIdle(Vect3f hisPosition,Vect3f myPosition)
{
/*IDLE:
Init: Generar un tiempo aleat.
entre un valor minimo y maximo
Update: comprobar que la dist.
con el protag. entra en radio olor.
Done: Si finaliza el temporizador
o protag. en radio.
*/

  switch(m_faseEstado)
  {
    case INIT:
      EscondePunyos();
      m_faseEstado = UPDATE;
    break;
    case UPDATE:
      //DENTRO ZONA OLIENDO
      if(hisPosition.SqDistance(myPosition) < (GetRadiusSmelling() * GetRadiusSmelling()))
      {
        SetState( OLIENDO );
        m_faseEstado = INIT;
      }

      //SALUD ACABADA
      if(IsDead())
      {
        SetState( MURIENDO );
        m_faseEstado = INIT;
      }

    break;
    case DONE:
      
    break;
  }
}

void CZombie::GoOliendo(Vect3f hisPosition,Vect3f myPosition)
{
  switch(m_faseEstado)
  {
    case INIT:
      m_faseEstado = UPDATE;
      EscondePunyos();
    break;
    case UPDATE:
      //DENTRO ZONA VISION
      if(hisPosition.SqDistance(myPosition) < (GetRadiusViewing() * GetRadiusViewing()))
      {
        SetState( MIRANDO );
        m_faseEstado = INIT;
      }
      //FUERA ZONA OLIENDO
      if(hisPosition.SqDistance(myPosition) > (GetRadiusSmelling() * GetRadiusSmelling()))
      {
        SetState( VOLVIENDOINICIO );
        m_faseEstado = INIT;
      }
    break;
    case DONE:
    break;
  }
}

void CZombie::GoMirando(Vect3f hisPosition,Vect3f myPosition)
{
/*
MIRANDO:
Init : localizar al contrincante
para atacarle.
Update:comprobar dist. con
personaje para activar accion
de atacar. 
Done: comprobar si se recibe
golpe o si sale del radio de vista.
*/
  switch(m_faseEstado)
  {
    case INIT:
      m_faseEstado = UPDATE;
      EscondePunyos();
    break;
    case UPDATE:
      //FUERA ZONA VISION
      if(hisPosition.SqDistance(myPosition) > (GetRadiusViewing() * GetRadiusViewing()))
      {
        SetState( OLIENDO );
        m_faseEstado = INIT;
      }
      //TOCA AL PERSONAJE Y SE PREPARA PARA ATACAR
      if(GetUserData()->GetIsCollided())
      {
        SetState( ATACANDO );
        m_faseEstado = INIT;
      }
    break;
    case DONE:
    break;
  }
}

void CZombie::GoAndando(Vect3f hisPosition,Vect3f myPosition)
{
/*ANDANDO:
Init : Leer un tiempo aleatorio y un
angulo aleatorio.5
Update: caminar segun su vector
director.
comprobar la dist. con el protag. para
olerlo
Done:Si finaliza el temporizador.
si entra en dist. de olor. 
*/
/*  switch()
  {
    case INIT:
    break;
    case UPDATE:
    break;
    case DONE:
    break;
  }
*/}

void CZombie::GoAtacando(Vect3f hisPosition,Vect3f myPosition)
{
/*
ATACANDO:
Init: Llamar accion de atacar
Update: comprobar si se recibe
golpe o se da.
Done:Si se recibe golpe o se
acaba la acción.
*/
  CAnimatedInstanceModel *model = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel();
//  float duracion = model->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( ATACANDO )->getDuration();

  switch(m_faseEstado)
  {
    case INIT:
      EscondePunyos();
      m_faseEstado = UPDATE;
      model->ExecuteAction(ATACANDO,1.0f/*duracion*/);
      GetUserData()->SetCurrentAction( ATACANDO );
    break;
    case UPDATE:
      //fin de estado de colision para atacar
      if(GetUserData()->GetIsActionFinalized() == ATACANDO)
      {
        GetUserData()->SetIsCollided(false);
        GetUserData()->SetIsActionFinalized(IDLE);
//        GetUserData()->SetCurrentAction( IDLE );
        SetState( IDLE );
        m_faseEstado = INIT;
      }
    break;
    case DONE:
    break;
  }
}

void CZombie::GoRecibiendoGolpe(Vect3f hisPosition,Vect3f myPosition)
{
  CAnimatedInstanceModel *model = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel();
//  float duracion = model->GetModelCal3DHW()->getCoreModel()->getCoreAnimation( RECIBIENDOGOLPE )->getDuration();
  switch(m_faseEstado)
  {
    case INIT:
      EscondePunyos();
      model->ExecuteAction(RECIBIENDOGOLPE,0.2f);
			CORE->GetSoundManager()->PlaySound("zombieMoan");
      GetUserData()->SetCurrentAction( RECIBIENDOGOLPE );
      SetState( INACTIVO );
      m_faseEstado = UPDATE;
    break;
    case UPDATE:
      //fin de estado de colision para atacar
      if(GetUserData()->GetIsActionFinalized() == RECIBIENDOGOLPE)
      {
        GetUserData()->SetIsCollided(false);
        GetUserData()->SetIsActionFinalized(IDLE);
        SetState( IDLE );
        m_faseEstado = INIT;
      }
    break;
    case DONE:
    break;
  }
}

void CZombie::GoVolviendoInicio(Vect3f hisPosition,Vect3f myPosition)
{
/*VOLVIENDOINICIO:Init : Recordar su posicion
inicial.
Update: caminar segun su vector
director al destino.
comprobar si se entra en dist.
de olor.
*/
  switch(m_faseEstado)
  {
    case INIT:
      m_faseEstado = UPDATE;
      EscondePunyos();
    break;
    case UPDATE:
      //DENTRO ZONA OLOR
      if(hisPosition.SqDistance(myPosition) < (GetRadiusSmelling() * GetRadiusSmelling()))
      {
        SetState( OLIENDO );
        m_faseEstado = INIT;
      }
      //DETECTA SU PUNTO DE INICIO PARA PARAR
      if(GetInstance()->GetPosition().SqDistance(GetPosInicial()) < 0.5f)//TODO.. No hardcoded??
      {
        SetState( IDLE );
        m_faseEstado = INIT;
      }
    break;
    case DONE:
    break;
  }
}

void CZombie::GoMuriendo()
{
/*REMURIENDO:
update: animacion de morir
Done : borrar del vector
*/
  switch(m_faseEstado)
  {
    case INIT:
      EscondePunyos();
      CORE->GetSoundManager()->PlaySound("zombieDeath");
      m_faseEstado = UPDATE;
      GetInstance()->GetAnimatedInstanceModel()->ExecuteActionBlocked(MURIENDO, 0.3f);
      GetUserData()->SetCurrentAction( MURIENDO );
    break;
    case UPDATE:
      //fin animacion de morir
      if((GetUserData()->GetIsActionFinalized() == MURIENDO)&&(m_iStepMuerto == 0))
      {
        m_iStepMuerto = 1;
        GetUserData()->SetIsActionFinalized(INACTIVO);
        /*SetState( MUERTO );*/
      } 
      if(m_iStepMuerto == 1)
      {
        //quita el controller
        DestruyeZombie(2);
				//render humo
				PosicionaPieDerecho();
				Vect3f l_v3Pos = GetPosicionPieDerecho();
				HumoMuerte(l_v3Pos);
				//lanzar item
				RandomItem(l_v3Pos);
        m_iStepMuerto = 2;
        LOGGER->AddNewLog(ELL_INFORMATION, "test - Debe soltar 1 cerebro");
      }
      if((m_iStepMuerto == 2)&&(GoMuerto(CORE->GetTimer()->GetElapsedTime())))
      {
        //borrar del mapa mundial, borra todoz
        DestruyeZombie(0);
      }
    break;
    case DONE:
    break;
  }
}

bool CZombie::GoMuerto(float elapsedTime)
{
	m_timeDead += elapsedTime;
  m_bVisible = !m_bVisible;
  GetInstance()->SetVisible(m_bVisible);
	if (m_timeDead > TIEMPO_DESAPARECE )
	{
    //solo lo hace desaparecer y mover el controller
    DestruyeZombie(1);
    return true;
	}
  return false;
}

void CZombie::HumoMuerte(Vect3f pos)
{
	Vect3f l_v3Pos = Vect3f(pos.x, (pos.y+1.0f), pos.z);
	//render
	std::string l_sNameParticle = "pHumo";
	CBillboardAnimation * l_pBillboardAnimation1 = CORE->GetBillboardManager()->GetBAnimationByInstance(l_sNameParticle);
	l_pBillboardAnimation1->SetPos(l_v3Pos);
	l_pBillboardAnimation1->SetEnd(false);
}


//para calcular la posicion del humo en función del pie del zombie cuando muere
void CZombie::PosicionaPieDerecho()
{
  std::string l_sName = GetInstance()->GetName();
  CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(l_sName);
  CalSkeleton* skeleton = model->GetAnimatedInstanceModel()->GetModelCal3DHW()->getSkeleton();
 
  int l_HuesoPieDer = skeleton->getCoreSkeleton()->getCoreBoneId("Bip01 R Foot");
  CalVector l_V3Position = (CalVector)skeleton->getBone(l_HuesoPieDer)->getTranslationAbsolute();

  V3PosicionPieDerecho.x = -l_V3Position.x;
  V3PosicionPieDerecho.y = l_V3Position.y;
  V3PosicionPieDerecho.z = l_V3Position.z;
}

//para devolver la posicion del hueso
Vect3f CZombie::GetPosicionPieDerecho()
{
	Mat44f l_t,l_vMat44;
	l_t.SetIdentity();
	l_t.Translate(V3PosicionPieDerecho);
	l_vMat44.SetIdentity();
	l_vMat44 = GetInstance()->GetMat44();
	Mat44f l_vMatrizPieDerecho =  l_vMat44 * l_t;

	return l_vMatrizPieDerecho.GetPos();
}

//-----------------------------------------
//perseguir por posicion de personaje(sin colisiones)
//-----------------------------------------
//void CZombie::Perseguir(Vect3f hisPosition,float _elapsedtime)
//{
//  Vect3f myPosition = GetInstance()->GetPosition();
//  GetEjeMovimientoIA()->MovAgentIA(hisPosition,myPosition,GetMoveSpeed(),_elapsedtime);
//  
//  //posicion y rotacion del muñeco
//  
////  GetInstance()->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
//  //GetInstance()->SetYaw(-(GetEjeMovimientoIA()->m_fAngleTotalPlayer2));
//
//  CRenderableObject* myinstance = CORE->GetRenderableObjectsManager()->GetInstance(this->GetInstance()->GetName());
//  myinstance->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
//  myinstance->SetYaw(-(GetEjeMovimientoIA()->m_fAngleTotalPlayer2));
//  SetInstance((CRenderableAnimatedInstanceModel*)myinstance);
//}
//-----------------------------------------
//perseguir por vector director
//-----------------------------------------
void CZombie::Perseguir(Vect3f hisPosition,float _elapsedtime, float height)
{
  Vect3f myPosition = GetInstance()->GetPosition();
  float mySpeed = GetMoveSpeed();

  GetEjeMovimientoIA()->MovAgentIA(hisPosition,myPosition,mySpeed,_elapsedtime);
  
  Vect3f aux = GetEjeMovimientoIA()->GetVectorDirector();
  aux.y = 0.0f;
  GetPhysicController()->Move(aux /** _elapsedtime*/ * mySpeed, _elapsedtime);

  Vect3f controllerPosition = GetPhysicController()->GetPosition();
  //controllerPosition.y = 0.0f;
  //myinstance->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
  CRenderableObject* myinstance = GetInstance();
  float l_fPosY = controllerPosition.y - GetPhysicController()->GetRadius()*height ;
  if(height == 1.0f)//boss
    myinstance->SetPosition(Vect3f(controllerPosition.x, l_fPosY -(height/4.0f), controllerPosition.z));
  else              //zombie
    myinstance->SetPosition(Vect3f(controllerPosition.x, l_fPosY, controllerPosition.z));
    
  myinstance->SetYaw(-(GetEjeMovimientoIA()->m_fAngleTotalPlayer2));
  
  SetInstance((CRenderableAnimatedInstanceModel*)myinstance);
}

//-----------------------------------------
//volver inicio por posicion de personaje (sin colisiones)
//-----------------------------------------
//void CZombie::VolviendoInicio(float _elapsedtime)
//{
//  Vect3f myInitialPosition = GetPosInicial();
//  Vect3f myPosition = GetInstance()->GetPosition();
//
//  GetEjeMovimientoIA()->MovAgentIA(myInitialPosition,myPosition,GetMoveSpeed(),_elapsedtime);
//
//  //posicion y rotacion del muñeco
//  
////  GetInstance()->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
//  //GetInstance()->SetYaw(-(GetEjeMovimientoIA()->m_fAngleTotalPlayer2));
//
//  CRenderableObject* myinstance = CORE->GetRenderableObjectsManager()->GetInstance(this->GetInstance()->GetName());
//  m_pPhysicController->Move(this->GetEjeMovimientoIA()->m_V3DirectorPlayer2, _elapsedtime);
//  //myinstance->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
//  myinstance->SetPosition(m_pPhysicController->GetPosition());
//  myinstance->SetYaw(-(GetEjeMovimientoIA()->m_fAngleTotalPlayer2));
//  SetInstance((CRenderableAnimatedInstanceModel*)myinstance);
//}

//-----------------------------------------
// encarando por vector director
//-----------------------------------------
void CZombie::Encarando(Vect3f hisPosition,float _elapsedtime, float height)
{
  Vect3f myPosition = GetInstance()->GetPosition();
  GetEjeMovimientoIA()->MovAgentIA(hisPosition,myPosition,0.0f,_elapsedtime,true);
  
  Vect3f controllerPosition = GetPhysicController()->GetPosition();
  //controllerPosition.y = 0.0f;
  //myinstance->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
  CRenderableObject* myinstance = GetInstance();
  float l_fPosY = controllerPosition.y - GetPhysicController()->GetRadius()*height;
  if(height == 1.0f)//boss
    myinstance->SetPosition(Vect3f(controllerPosition.x, l_fPosY -(height/4.0f), controllerPosition.z));
  else              //zombie
    myinstance->SetPosition(Vect3f(controllerPosition.x, l_fPosY, controllerPosition.z));

  myinstance->SetYaw(-(GetEjeMovimientoIA()->m_fAngleTotalPlayer2));
  SetInstance((CRenderableAnimatedInstanceModel*)myinstance);
}

//-----------------------------------------
//volver inicio por vector director
//-----------------------------------------
void CZombie::VolviendoInicio(float _elapsedtime, float height)
{
  Vect3f myInitialPosition = GetPosInicial();
  Vect3f myPosition = GetInstance()->GetPosition();

  GetEjeMovimientoIA()->MovAgentIA(myInitialPosition,myPosition,GetMoveSpeed(),_elapsedtime);

  //posicion y rotacion del muñeco
  Vect3f aux = GetEjeMovimientoIA()->GetVectorDirector();
  aux.y = 0.0f;
  GetPhysicController()->Move(aux /** _elapsedtime*/ *GetMoveSpeed(), _elapsedtime);
  Vect3f controllerPosition = GetPhysicController()->GetPosition();
  //controllerPosition.y = 0.0f;
  //myinstance->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
  CRenderableObject* myinstance = GetInstance();
  float l_fPosY = controllerPosition.y - GetPhysicController()->GetRadius()*height;
  if(height == 1.0f)//boss
    myinstance->SetPosition(Vect3f(controllerPosition.x, l_fPosY -(height/4.0f), controllerPosition.z));
  else              //zombie
    myinstance->SetPosition(Vect3f(controllerPosition.x, l_fPosY, controllerPosition.z));

  myinstance->SetYaw(-(GetEjeMovimientoIA()->m_fAngleTotalPlayer2));
  SetInstance((CRenderableAnimatedInstanceModel*)myinstance);
}

//-----------------------------------------
//evadir por vector director
//-----------------------------------------
void CZombie::Evadir(Vect3f hisPosition, float _elapsedtime, float _speed, float height)
{
  Vect3f myPosition = GetInstance()->GetPosition();
  //float yy = myPosition.y;
  GetEjeMovimientoIA()->MovAgentIA(hisPosition,myPosition,_speed,_elapsedtime,false);

  Vect3f aux = GetEjeMovimientoIA()->GetVectorDirector();
  aux.y = 0.0f;
  GetPhysicController()->Move(aux /** _elapsedtime*/ * _speed, _elapsedtime);

  Vect3f controllerPosition = GetPhysicController()->GetPosition();
  //controllerPosition.y = 0.0f;
  //myinstance->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
  CRenderableObject* myinstance = GetInstance();
  float l_fPosY = controllerPosition.y - GetPhysicController()->GetRadius()*height;
  if(height == 1.0f)//boss
    myinstance->SetPosition(Vect3f(controllerPosition.x, l_fPosY -(height/4.0f), controllerPosition.z));
  else              //zombie
    myinstance->SetPosition(Vect3f(controllerPosition.x, l_fPosY, controllerPosition.z));

  myinstance->SetYaw(-GetEjeMovimientoIA()->m_fAngleTotalPlayer2);
  SetInstance((CRenderableAnimatedInstanceModel*)myinstance);
}


void CZombie::PosicionaManoIzquierda()
{
//  Mat44f worldmatrix = GetInstance()->GetMat44();
  std::string l_sName = GetInstance()->GetName();
  CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(l_sName);
  CalSkeleton* skeleton = model->GetAnimatedInstanceModel()->GetModelCal3DHW()->getSkeleton();
 
  int l_HuesoManoIzq = skeleton->getCoreSkeleton()->getCoreBoneId("Bip01 L Finger0");
  CalVector l_V3Position = (CalVector)skeleton->getBone(l_HuesoManoIzq)->getTranslationAbsolute();

  V3PosicionManoIzquierda.x = -l_V3Position.x;
  V3PosicionManoIzquierda.y = l_V3Position.y;
  V3PosicionManoIzquierda.z = l_V3Position.z;

  PosicionaTrigger(V3PosicionManoIzquierda, m_actManoIzquierda);
}

void CZombie::PosicionaManoDerecha()
{
//  Mat44f worldmatrix = GetInstance()->GetMat44();
  std::string l_sName = GetInstance()->GetName();
  CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(l_sName);
  CalSkeleton* skeleton = model->GetAnimatedInstanceModel()->GetModelCal3DHW()->getSkeleton();
 
  int l_HuesoManoDer = skeleton->getCoreSkeleton()->getCoreBoneId("Bip01 R Finger0");
  CalVector l_V3Position = (CalVector)skeleton->getBone(l_HuesoManoDer)->getTranslationAbsolute();

  V3PosicionManoDerecha.x = -l_V3Position.x;
  V3PosicionManoDerecha.y = l_V3Position.y;
  V3PosicionManoDerecha.z = l_V3Position.z;

  PosicionaTrigger(V3PosicionManoDerecha, m_actManoDerecha);
  //CTriggerManager* tm = CORE->GetTriggerManager();
  //tm->GetTrigger("z_inst_200 ManoDerecha")->SetPosition(m_actManoDerecha->GetPosition());
}

bool CZombie::GeneraManoDerecha()
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
  m_triggerManoDerecha->SetDimensions(Vect3f(0.15f,0.15f,0.15f));
  
  m_triggerManoDerecha->SetEntity("mi_amigo");
  //m_triggerManoDerecha->SetPosition(Vect3f(0,2,0));
  m_triggerManoDerecha->SetPosition(Vect3f(0,-2,0));
  std::string l_sNameTrigger = GetUserData()->GetName();
  
  l_sNameTrigger += " ManoDerecha";
  std::string l_script;
  baseUtils::FormatSrting( l_script, "golpea_mano_derecha(\"%s\", \"%s\")", l_sNameTrigger.c_str(), m_triggerManoDerecha->GetEntity().c_str());
  m_triggerManoDerecha->SetOnEnterCode(l_script);
  m_actManoDerecha = CORE->GetTriggerManager()->GeneraTrigger(l_sNameTrigger, m_triggerManoDerecha, true);

  return true;
}

bool CZombie::GeneraManoIzquierda()
{
  m_triggerManoIzquierda = new CTrigger();
  assert (m_triggerManoIzquierda);

  m_bIsOk = m_triggerManoIzquierda->Init();
  if(!m_bIsOk)
  {
    LOGGER->AddNewLog(ELL_WARNING,"a CTrigger instace couldnt allocate memory");  
    return false;
  }

  m_triggerManoIzquierda->SetActive(false);
  m_triggerManoIzquierda->SetDimensions(Vect3f(0.15f,0.15f,0.15f));
  m_triggerManoIzquierda->SetEntity("mi_amigo");
  m_triggerManoIzquierda->SetPosition(Vect3f(0,-2,0));
  
  std::string l_sNameTrigger = GetUserData()->GetName();

  l_sNameTrigger += " ManoIzquierda";
  std::string l_script;
  baseUtils::FormatSrting( l_script, "golpea_mano_izquierda(\"%s\", \"%s\")", l_sNameTrigger.c_str(), m_triggerManoIzquierda->GetEntity().c_str());
  m_triggerManoIzquierda->SetOnEnterCode(l_script);
  m_actManoIzquierda = CORE->GetTriggerManager()->GeneraTrigger(l_sNameTrigger, m_triggerManoIzquierda, true);
  return true;
}
void CZombie::InicializaController()
{
  CPhysicUserData *userdata = new CPhysicUserData(GetInstance()->GetName());
  userdata->SetColor(colGREEN);
  userdata->SetPaint(true);
  GetInstance()->SetPosition(Vect3f(GetInstance()->GetPosition().x,GetInstance()->GetPosition().y + 1.0f,GetInstance()->GetPosition().z));
//  m_pPhysicController = new CPhysicController(0.5f,2.0f,2.0f,0.1f,2.0f,1,userdata,GetInstance()->GetPosition());
  m_pPhysicController = new CPhysicController(0.5f, 1.3f, 40.0f, 0.01f, 0.3f, PX_MSK_ENEMY_CONTROLLER, userdata, GetInstance()->GetPosition());
  CORE->GetPhysicsManager()->AddPhysicController(m_pPhysicController);
}

void CZombie::PosicionaTrigger(const Vect3f& _trig, CPhysicActor* _actTrig)
{
  Mat44f l_t,l_vMat44;
  l_t.SetIdentity();
  l_t.Translate( _trig );
  l_vMat44.SetIdentity();
  l_vMat44 = GetInstance()->GetMat44();
  Mat44f l_vMataux =  l_vMat44 * l_t;

  _actTrig->SetMat44( l_vMataux );
}

void CZombie::DestruyeZombie(char fase)
{
  switch (fase)
  {
    case 0:  

    if(m_triggerManoDerecha)
    {
      EscondePunyos();
      m_triggerManoDerecha->SetActive(false);
    } 
    if(m_triggerManoIzquierda)
    {
      EscondePunyos();
      m_triggerManoIzquierda->SetActive(false);
    }

      SetState( MUERTO );
      m_faseEstado = INIT;
    case 1:
      GetInstance()->SetVisible(false);

    case 2:
      GetPhysicController()->GetUserData()->SetPaint(false);
      GetPhysicController()->SetPosition(Vect3f(0,-5,0));
  }
}

void CZombie::EscondePunyos()
{
  m_actManoDerecha->SetGlobalPosition( Vect3f(0.0f,-2.0f,0.0f) );
  m_actManoIzquierda->SetGlobalPosition( Vect3f(0.0f,-2.0f,0.0f) );
  m_triggerManoIzquierda->SetPosition( Vect3f(0.0f,-2.0f,0.0f) );
  m_triggerManoDerecha->SetPosition( Vect3f(0.0f,-2.0f,0.0f) );
  V3PosicionManoIzquierda = Vect3f(0.0f,-2.0f,0.0f);
  V3PosicionManoDerecha = Vect3f(0.0f,-2.0f,0.0f);
  PosicionaTrigger(V3PosicionManoDerecha, m_actManoDerecha);
  PosicionaTrigger(V3PosicionManoIzquierda, m_actManoIzquierda);
}

void CZombie::RandomItem(Vect3f pos)
{
  float l_fNumber;
  float fMin = 1;
	float fMax = 10;
	l_fNumber=((rand()/static_cast<float>(RAND_MAX)) * (fMax - fMin) + fMin);

	if (l_fNumber<=5)
	{
		//lanzar item
    CORE->GetItemManager()->AddItem(pos);
	}
}
