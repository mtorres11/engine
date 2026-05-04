#include "ZombieVomitador.h"
#include "Vomito.h"

#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "../IA/EjeMovimientoIA.h"

#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "AnimationModel/AnimatedInstanceModel.h"

#include "CharacterController/PhysicController.h"
#include "SoundManager.h"
#include "Timer/Timer.h"
#include "BillBoard/BillboardManager.h"
#include "ItemManager.h"

#define TIEMPO_IDLE_TRAS_VOMITO 5.0f
#define TIEMPO_SALIDA 20.0f

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CZombieVomitador::Init(const CXMLTreeNode &m)
{
	bool bIsOk = Inherited::Init(m);
  if (!bIsOk)
  {
		printf("a CZombieVomitador instace couldnt allocate memory");
    Done();								//We call Done()  to release before the parent class
	}
  else
  {
    //tienen que haber alternativas como campos de alt1,alt2,alt3,alt4 
    m_uiNumAlternativas = m.GetIntProperty("num_alternativas");
    switch(m_uiNumAlternativas)
    {
    case 4:
      m_v3AlternativasPosicion.push_back(m.GetVect3fProperty("alt4",Vect3f(0.0f,0.0f,0.0f)));
    case 3:
      m_v3AlternativasPosicion.push_back(m.GetVect3fProperty("alt3",Vect3f(0.0f,0.0f,0.0f)));
    case 2:
      m_v3AlternativasPosicion.push_back(m.GetVect3fProperty("alt2",Vect3f(0.0f,0.0f,0.0f)));
    case 1:
      m_v3AlternativasPosicion.push_back(m.GetVect3fProperty("alt1",Vect3f(0.0f,0.0f,0.0f)));
    case 0:
      //entra la posicion original del zombi en el vector para tambien hacer random
      m_v3AlternativasPosicion.push_back(m.GetVect3fProperty("pos",Vect3f(0.0f,0.0f,0.0f)));
    break;
    default:
      //entra la posicion original del zombi en el vector para tambien hacer random
      m_v3AlternativasPosicion.push_back(m.GetVect3fProperty("pos",Vect3f(0.0f,0.0f,0.0f)));

      LOGGER->AddNewLog(ELL_WARNING, "No se ha introducido el numero correcto de alternativas, se guarda solo la posicion original");
    }
    m_uiAimDistance = m.GetIntProperty("aim_distance");
    m_fTamanyoVomito = m.GetFloatProperty("size", 0.2f);
    m_fSpeedVomito = m.GetFloatProperty("speed_vomito", 2.0f);
    m_fPausaVomito = m.GetFloatKeyword("pause_vomito",3.0f);
    std::string l_sName = m.GetPszProperty("name");

    if(!CreaCrater(l_sName))
    {
      LOGGER->AddNewLog(ELL_ERROR, "CZombieVomitador->Init: El zombi vomitador no tiene nombre");
      return false;
    }  

  }
  
  //test_my_id = test_id++;
  m_pVomito = 0;

  return bIsOk;
}

bool CZombieVomitador::Init(const CZombie &z)
{
	bool bIsOk = Inherited::Init(z);
  if (!bIsOk)
  {
		printf("a CZombieNormal instace couldnt allocate memory");
    Done();								//We call Done()  to release before the parent class
	}

  std::string l_sName = Inherited::GetInstance()->GetName();
  
  if(!CreaCrater(l_sName))
  {
    LOGGER->AddNewLog(ELL_ERROR, "CZombieVomitador->Init: El zombi vomitador no tiene nombre");
    return false;
  }  

  //test_my_id = test_id++;
  m_pVomito = 0;
  return bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CZombieVomitador::Done()
{
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
void CZombieVomitador::Release()
{
//free memory
    CHECKED_DELETE(m_movIA);
    CHECKED_DELETE(m_pVomito);
//    CHECKED_DELETE(m_pArena);
}	


//----------------------------------------------------------------------------
// Recalculate IA
//----------------------------------------------------------------------------
char CZombieVomitador::RecalculateIA(Vect3f hisPosition)
{
  Vect3f myPosition( GetInstance()->GetPosition() );

  switch(GetState())
  { 
    case (char)IDLE:
      GoIdle(hisPosition,myPosition);
    break;
    case (char)RECIBIENDOGOLPE:
      SetState(CZombieVomitador::VOM_RECIBEGOLPE);
      break;
    case (char)CZombieVomitador::VOM_APARECIENDO:
      GoApareciendo(hisPosition);
    break;
    case (char)CZombieVomitador::VOM_DISPARANDO:
      GoVomitando(hisPosition,myPosition);
    break;
    case (char)CZombieVomitador::VOM_RECIBEGOLPE:
      GoRecibiendoGolpe(hisPosition,myPosition);
    break;
    case (char)CZombieVomitador::VOM_IDLETRASVOMITO:
      GoIdleTrasVomito(hisPosition,myPosition);
    break;
    case (char)CZombieVomitador::VOM_SALIDA:
      GoSalida(hisPosition,myPosition);
    break;
    case (char)INACTIVO:
//      GoIdle(hisPosition,myPosition);
    break;
    case (char)CZombieVomitador::VOM_MURIENDO:
      GoMuriendo();
    break;
  }
  return GetState();
}


//----------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------
void CZombieVomitador::Update(float elapsedTime)
{    
    Vect3f hisPosition( CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetPosition() );
    
    //dibujar cuando la distancia es la distancia smelling
    if((hisPosition.SqDistance(GetInstance()->GetPosition()) < GetRadiusSmelling() * GetRadiusSmelling()))
    {
      if(GetUserData()->GetCurrentAction() >= CZombieVomitador::VOM_APARECIENDO)
      {
        GetInstance()->SetVisible(true);
        m_pArena->SetVisible(true);
      }
    }
    else
    {
      GetInstance()->SetVisible(false);
      m_pArena->SetVisible(false);

    }

    //update vomito
    if(m_pVomito)
    {
      m_pVomito->Update(elapsedTime, GetTamanyoVomito(), GetSpeedVomito());
    }

    SetState( RecalculateIA(hisPosition) );
    char s = 0;

    if(GetState() == CZombieVomitador::VOM_IDLETRASVOMITO)
    { 
      s = CZombieVomitador::VOM_IDLETRASVOMITO - OFFSET_ANIMATIONS;
      m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->BlendCycle(s,elapsedTime);
    }

    //SALUD ACABADA
    if((IsDead()) && (GetState() != CZombieVomitador::VOM_MURIENDO)&& (GetState() != MUERTO))
    {
      SetState( CZombieVomitador::VOM_MURIENDO );
      m_faseEstado = INIT;
    }
  //}
}


//------------------------------------------------------------m_----------------
// Render
//----------------------------------------------------------------------------
void CZombieVomitador::Render(CRenderManager *rm)
{//TODO..
  Mat44f l_t;
  l_t.SetIdentity();
  rm->SetTransform(l_t);
  rm->DrawCircleH(GetInstance()->GetPosition().x, GetInstance()->GetPosition().z, GetRadiusSmelling(),colYELLOW);

  l_t.SetIdentity();
  rm->SetTransform(l_t);
  rm->DrawCircleH(GetInstance()->GetPosition().x, GetInstance()->GetPosition().z, GetRadiusViewing(),colRED);

  //render vomito
  if(m_pVomito)
  {
    m_pVomito->Render(rm);
  }
}

void CZombieVomitador::GoIdle(Vect3f hisPosition,Vect3f myPosition)
{
/*
IDLE:
*/
  switch(m_faseEstado)
  {
    case INIT:
      GetInstance()->SetVisible(false);
      m_pArena->SetVisible(false);
      GetPhysicController()->SetPosition(Vect3f(0.f,-5.f,0.f));
      m_faseEstado = UPDATE;
      m_dTiempoSalida = CORE->GetTimer()->GetTotalTime();

    	m_dDiffTiempoSalidaAleat  =((rand()/static_cast<double>(RAND_MAX)) * TIEMPO_SALIDA);

      //m_dDiffTiempoSalidaAleat = (double)rand() * TIEMPO_SALIDA;
      SetState(IDLE);
      GetUserData()->SetCurrentAction( IDLE ); 
    break;
    case UPDATE:
      GetInstance()->SetVisible(false);
      m_pArena->SetVisible(false);
      GetPhysicController()->SetPosition(Vect3f(0,-5,0));
        
      if((m_dTiempoSalida + m_dDiffTiempoSalidaAleat) < CORE->GetTimer()->GetTotalTime())
      {
        SetState(CZombieVomitador::VOM_APARECIENDO);
        m_faseEstado = INIT;
      }
    break;
    case DONE:
    break;
  }
}

void CZombieVomitador::GoApareciendo(Vect3f hisPosition)
{
/*
...
*/
  char s = 0;
  char 
  val = 0;
  Vect3f m_V3Dir(0);
  float m_fAngle = 0.0f;
  Vect3f myPosition(0.0f,0.0f,0.f);
  switch(m_faseEstado)
  {
    case INIT:
      if(m_uiNumAlternativas > 0)
      {
        val = rand() % m_uiNumAlternativas;
      }
      GetPhysicController()->SetPosition(m_v3AlternativasPosicion[val]);

      myPosition = GetPhysicController()->GetPosition();
      GetInstance()->SetPosition(myPosition);
      m_V3Dir = hisPosition - myPosition;
      m_fAngle = m_V3Dir.GetAngleY();

      GetInstance()->SetYaw( -m_fAngle);

      //animacion aparecer
      s = CZombieVomitador::VOM_APARECIENDO - OFFSET_ANIMATIONS;
      m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->ExecuteAction(s,0.1f);
      GetUserData()->SetCurrentAction(CZombieVomitador::VOM_APARECIENDO);
      m_faseEstado = UPDATE;
      //CORE->GetSoundManager()->PlaySound("zombieVomit");
      m_pArena->GetAnimatedInstanceModel()->ClearCycle(1.0f);
      m_pArena->GetAnimatedInstanceModel()->RemoveAction(1);
      m_pArena->GetAnimatedInstanceModel()->ExecuteActionBlocked(1, 0.0f);
      m_pArena->SetPosition(Vect3f(myPosition.x, myPosition.y - 0.2f, myPosition.z));

      m_pArena->SetVisible(true);
      
    break;
    case UPDATE:
      ////pone el crater visible cuando empieza la animacion
      //if(GetUserData()->GetIsCollided())
      //{
      //  m_pArena->SetVisible(true);
      //}
      //comprueba que acaba la animacion de aparecer para disparar
      if(GetUserData()->GetIsActionFinalized() == GetUserData()->GetCurrentAction())
      {
        SetState(CZombieVomitador::VOM_DISPARANDO);
        GetUserData()->SetCurrentAction(CZombieVomitador::VOM_DISPARANDO);
        m_faseEstado = INIT;
      }
     
    break;
    case DONE:
    break;
  }
}


void CZombieVomitador::GoVomitando(Vect3f hisPosition, Vect3f myPosition)
{
/*
DISPARANDO:
Init : calcular posicion del contrincante
Animacion de disparo y disparar con physX
Update: el muñeco se puede recolocar
mientras que el disparo se convierte en 
nuevo elemento ObjectPhysX.
Done: comprueba si recibe golpe, o
si pasa a estado de mirando o oliendo.
*/

  Vect3f l_V3DirectorNorm;
  Vect3f l_V3Encarar;
  char s = 0;

  Vect3f m_V3Dir(0);
  float m_fAngle=0.0f;
  switch(m_faseEstado)
  {
    case INIT:
      m_V3Dir = hisPosition - myPosition;
      m_fAngle = m_V3Dir.GetAngleY();

      GetInstance()->SetYaw( -m_fAngle);
      if(m_pVomito)
      {
        m_dTiempoInicioVomito = 0.0;
        CHECKED_DELETE(m_pVomito);
      }
      else
      {
        m_dTiempoInicioVomito = CORE->GetTimer()->GetTotalTime();

        m_pVomito = new CVomito();
        if(m_pVomito)
        {
          m_faseEstado = UPDATE;

          //animacion disparar vomito
          s = CZombieVomitador::VOM_DISPARANDO - OFFSET_ANIMATIONS;
          m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->ExecuteAction(s,0.1f);

					CORE->GetSoundManager()->PlaySound("zombieVomit");
          m_pVomito->Init();

          l_V3DirectorNorm = m_V3Dir.GetNormalized();
          
          //encarar a personaje
          l_V3Encarar( 
            l_V3DirectorNorm.x * (GetPhysicController()->GetRadius() + GetTamanyoVomito() + 0.1f )
            , 0.0f
            , l_V3DirectorNorm.z * (GetPhysicController()->GetRadius() + GetTamanyoVomito() + 0.1f)
          );

          //crea vomito
          m_pVomito->CreaVomito(
            GetInstance()->GetPosition() + l_V3Encarar
            , GetPhysicController()->GetHeight()
            , GetPositionHero()
          );
        }  
      }


    break;
    case UPDATE:
      //activa vomito
      m_pVomito->SetActive(true);
      
      //if(m_pVomito->GetChocado())
      //{
        if(GetUserData()->GetIsActionFinalized() == GetUserData()->GetCurrentAction())
        {
          CHECKED_DELETE(m_pVomito);
          SetState( CZombieVomitador::VOM_IDLETRASVOMITO );
          GetUserData()->SetCurrentAction( CZombieVomitador::VOM_IDLETRASVOMITO );
          m_faseEstado = INIT;
          m_dTiempoInicioVomito = 0.0;
        }
//      }
    break;
    case DONE:
      
    break;
  }
  
}

void CZombieVomitador::GoIdleTrasVomito(Vect3f hisPosition,Vect3f myPosition)
{
  switch(m_faseEstado)
  {
    case INIT:
      m_faseEstado = UPDATE;
      m_dTiempoIdleTrasVomito = CORE->GetTimer()->GetTotalTime();
    break;
    case UPDATE:
      if((m_dTiempoIdleTrasVomito + TIEMPO_IDLE_TRAS_VOMITO) < CORE->GetTimer()->GetTotalTime())
      {
        SetState( CZombieVomitador::VOM_SALIDA );
        GetUserData()->SetCurrentAction( CZombieVomitador::VOM_SALIDA );
        m_faseEstado = INIT;
      }
    break;
    case DONE:
    break;
  }
}

void CZombieVomitador::GoSalida(Vect3f hisPosition,Vect3f myPosition)
{
  char s = '0';

  switch(m_faseEstado)
  {
    case INIT:
      m_faseEstado = UPDATE;
      //m_dTiempoSalida = CORE->GetTimer()->GetTotalTime();

      //animacion salida
      //if(GetInstance()->GetVisible())
      //{
        s = CZombieVomitador::VOM_SALIDA - OFFSET_ANIMATIONS;
        m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel()->ExecuteAction(s,0.1f);
        GetUserData()->SetCurrentAction( CZombieVomitador::VOM_SALIDA );
        //m_pArena->GetAnimatedInstanceModel()->ClearCycle(1.0f);
        //m_pArena->GetAnimatedInstanceModel()->RemoveAction(1);
        m_pArena->GetAnimatedInstanceModel()->ExecuteAction(0, 0.1f);

      //}

    break;
    case UPDATE:
      if(GetUserData()->GetIsActionFinalized() == GetUserData()->GetCurrentAction())
      {
        GetInstance()->SetVisible(false);
        GetPhysicController()->SetPosition(Vect3f(0,-5,0));
        SetState(IDLE);
        GetUserData()->SetCurrentAction( IDLE ); 
        m_faseEstado = INIT;
        m_pArena->SetVisible(false);
      }
    break;
    case DONE:
    break;
  }
}

void CZombieVomitador::GoRecibiendoGolpe(Vect3f hisPosition,Vect3f myPosition)
{
  CAnimatedInstanceModel *model = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel();

  char s = 0;
  switch(m_faseEstado)
  {
    case INIT:
      s = CZombieVomitador::VOM_RECIBEGOLPE - OFFSET_ANIMATIONS;
      model->ExecuteAction(s,0.1f);

			CORE->GetSoundManager()->PlaySound("zombieMoan");
      GetUserData()->SetCurrentAction( CZombieVomitador::VOM_RECIBEGOLPE);
      SetState( CZombieVomitador::VOM_RECIBEGOLPE);
      m_faseEstado = UPDATE;
    break;
    case UPDATE:
      //fin de estado de colision para atacar
      if(GetUserData()->GetIsActionFinalized() == CZombieVomitador::VOM_RECIBEGOLPE)
      {
        GetUserData()->SetIsActionFinalized(CZombieVomitador::VOM_IDLETRASVOMITO);
        SetState( CZombieVomitador::VOM_IDLETRASVOMITO );
        m_faseEstado = INIT;
      }
    break;
    case DONE:
    break;
  }
}

//para calcular la posicion del humo en función de la cabeza del zombie cuando muere
void CZombieVomitador::PosicionaCabeza()
{
  std::string l_sName = GetInstance()->GetName();
  CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(l_sName);
  CalSkeleton* skeleton = model->GetAnimatedInstanceModel()->GetModelCal3DHW()->getSkeleton();
 
  int l_HuesoCabeza = skeleton->getCoreSkeleton()->getCoreBoneId("Bip01 Head");
  CalVector l_V3Position = (CalVector)skeleton->getBone(l_HuesoCabeza)->getTranslationAbsolute();

  V3PosicionCabeza.x = -l_V3Position.x;
  V3PosicionCabeza.y = l_V3Position.y;
  V3PosicionCabeza.z = l_V3Position.z;
}

//para devolver la posicion del hueso
Vect3f CZombieVomitador::GetPosicionCabeza()
{
	Mat44f l_t,l_vMat44;
	l_t.SetIdentity();
	l_t.Translate(V3PosicionCabeza);
	l_vMat44.SetIdentity();
	l_vMat44 = GetInstance()->GetMat44();
	Mat44f l_vMatrizHuesoCabeza =  l_vMat44 * l_t;

	return l_vMatrizHuesoCabeza.GetPos();
}

void CZombieVomitador::GoMuriendo()
{
/*REMURIENDO:
update: animacion de morir
Done : borrar del vector
*/
  char s = 0;
  switch(m_faseEstado)
  {
    case INIT:
      CORE->GetSoundManager()->PlaySound("zombieDeath");
      m_faseEstado = UPDATE;
      s = CZombieVomitador::VOM_MURIENDO - OFFSET_ANIMATIONS;
      GetInstance()->GetAnimatedInstanceModel()->ExecuteActionBlocked(s,0.1f);
      GetUserData()->SetCurrentAction( CZombieVomitador::VOM_MURIENDO );
    break;
    case UPDATE:
      //fin animacion de morir
      if(GetUserData()->GetIsActionFinalized() == CZombieVomitador::VOM_MURIENDO &&m_iStepMuerto == 0)
      {
        m_iStepMuerto = 1;
        GetUserData()->SetIsActionFinalized(INACTIVO);
      } 
      if(m_iStepMuerto == 1)
      {
        //quita el controller
        DestruyeZombie(2);
				//render humo
				PosicionaCabeza();
				Vect3f l_v3Pos = GetPosicionCabeza();
				HumoMuerte(l_v3Pos);
				//lanzar item
				RandomItem(l_v3Pos);
        m_iStepMuerto = 2;
      }
      if((m_iStepMuerto == 2)&&(GoMuerto(CORE->GetTimer()->GetElapsedTime())))
      {
        //borrar del mapa mundial, borra todo
        DestruyeZombie(0);
        m_pArena->SetVisible(false);
      }

    break;
    case DONE:
    break;
  }
}

void CZombieVomitador::HumoMuerte(Vect3f pos)
{
	Vect3f l_v3Pos = Vect3f (pos.x, (pos.y+1.0f), pos.z);
	//render
	std::string l_sNameParticle = "pHumo";
	CBillboardAnimation * l_pBillboardAnimation1 = CORE->GetBillboardManager()->GetBAnimationByInstance(l_sNameParticle);
	l_pBillboardAnimation1->SetPos(l_v3Pos);
	l_pBillboardAnimation1->SetEnd(false);
}

void CZombieVomitador::InicializaController()
{
  CPhysicUserData *userdata = new CPhysicUserData(GetInstance()->GetName());
  userdata->SetColor(colGREEN);
  userdata->SetPaint(true);
  GetInstance()->SetPosition(Vect3f(GetInstance()->GetPosition().x,GetInstance()->GetPosition().y ,GetInstance()->GetPosition().z));
//  m_pPhysicController = new CPhysicController(0.5f,2.0f,2.0f,0.1f,2.0f,1,userdata,GetInstance()->GetPosition());
  m_pPhysicController = new CPhysicController(0.5f, 0.6f, 40.0f, 0.01f, 0.3f, PX_MSK_ENEMY_CONTROLLER, userdata, GetInstance()->GetPosition());
  CORE->GetPhysicsManager()->AddPhysicController(m_pPhysicController);
}

bool CZombieVomitador::CreaCrater(std::string _sName)
{
  if(!_sName.compare(""))
  {
    return false;
  }
  _sName += " crater";

  CRenderableObject* l_pObject = CORE->GetRenderableObjectsManager()->AddAnimatedModelInstance("arenazombievomitador", _sName, Vect3f(1.f,-10.f,-1.f), 180.f,0.f,0.f);
  //CORE->GetRenderableObjectsManager()->AddResource(_sName, l_pObject);
  m_pArena = CORE->GetRenderableObjectsManager()->GetInstanceAsAnimatedInstanceModel(_sName);


  if(!m_pArena)
  {
    return false;
  }
  return true;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
