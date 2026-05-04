#include "Player.h"

#include "Core.h"
#include "CoreFIH.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "Math/MathUtils.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "AnimationModel/AnimatedCoreModel.h"
#include "XML/XMLTreeNode.h"
#include "Exceptions/Exception.h"
#include "BillBoard/BillboardManager.h"
#include "ZombieManager/ZombieManager.h"
#include "Zombie/Zombie.h"
#include "ZombieVomitador/ZombieVomitador.h"
#include "ZombieBoss/ZombieBoss.h"

#include "Cameras/Camera.h"
#include "Cameras/CameraManager.h"
#include "PhysicsManager.h"
#include "CharacterController/PhysicController.h"
#include "SoundManager.h"
#include "ScriptManager.h"
#include "Lights/Light.h"
#include "Lights/LightManager.h"

#include "GUIManager.h"

#include "PlayerAnimData.h"
#include "PlayerAnimCallback.h"

#include "RenderManager.h"

//TODO: solo para debug, eliminar
#include "Utils/BaseUtils.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

#include <math.h>


#define asec(x) acos(1.0f / x)
#define UPPER_LIMIT FLOAT_PI_VALUE

CPlayer::CPlayer()
: m_bIsOk(false)
, m_pAnimation(0)
, m_pAnimCallback(0)
, m_pPlayerCamera(0)
, m_iPlayerState(S_IDLE)
, m_bAttackLightIsOn(false)
, m_bJumpTiming(false)
, m_bOnJumpTrigger(false)
, m_fJumpTimeCounter(0.0f)
, m_fAngleRotation(0.0f)
, m_fCameraRotation(0.0f)
{
  m_pAnimData = new CPlayerAnimData();

  #if defined( _DEBUG )
    m_sSetupFileName = "";
  #endif // defined( _DEBUG )
}


CPlayer::~CPlayer()
{
  CHECKED_DELETE(m_pPhysicController)
  CHECKED_DELETE(m_pAnimData)
  CHECKED_DELETE(m_pAnimCallback)
}

bool CPlayer::Init(std::string _file)
{
  m_bIsOk = true;

  CXMLTreeNode l_FileXML;
  
  if (!l_FileXML.LoadFile(_file.c_str()))
  {
    // Guarda mensaje de error en el log
		std::string msg_error = "CPlayer::Init->Error al intentar abrir el archivo: ";
		msg_error.append(_file.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
    m_bIsOk = false;
  }
  else
  {
    CXMLTreeNode l_Params = l_FileXML["Parameters"];
    if (l_Params.Exists())
    {
      m_sInstanceName         = l_Params.GetPszProperty("Instance");
      m_fMaxHealth            = l_Params.GetFloatProperty("MaxHealth", 100.f);
      m_fDefaultMoveSpeed     = l_Params.GetFloatProperty("MoveSpeed", 3.0f);
      m_fJumpVerticalSpeed    = l_Params.GetFloatProperty("JumpVerticalSpeed", 2.f);
      m_fJumpSpeed            = l_Params.GetFloatProperty("JumpMoveSpeed", 100.f);
      m_fLightAttackDamage    = l_Params.GetFloatProperty("LightAttackDamage", 30.0f);
      m_fStrongAttackDamage   = l_Params.GetFloatProperty("StrongAttackDamage", 150.f);
      m_fJumpTimeToStart      = l_Params.GetFloatProperty("JumpTimeToStart", 2.f);
      m_fJumpDiretionCosAngle = l_Params.GetFloatProperty("JumpDiretionCosAngle", 0.9f);
      m_sAttackLightName      = l_Params.GetPszProperty("AttackLight", "");
    }

    l_Params = l_FileXML["Controller"];
    if (l_Params.Exists())
    {
      m_fControllerRadius     = l_Params.GetFloatProperty("radius", 0.5f);
      m_fControllerHeight     = l_Params.GetFloatProperty("height", 1.0f);
      m_fControllerSlope      = l_Params.GetFloatProperty("slope", 40.0f);
      m_fControllerSkinWidth  = l_Params.GetFloatProperty("skinwidth", 0.1f);
      m_fControllerStepOffset = l_Params.GetFloatProperty("stepOffset", 0.3f);
    }

    l_Params = l_FileXML["ToFace"];
    if (l_Params.Exists())
    {
      m_fAngleRotation        = l_Params.GetFloatProperty("angle_rotation", 0.018f);
      m_fCameraRotation       = l_Params.GetFloatProperty("camera_rotation", 0.009f);
      
      m_fCameraRotation = (m_fCameraRotation > m_fAngleRotation)? m_fAngleRotation : m_fCameraRotation;
    }

    #if defined( _DEBUG )
    #endif // defined( _DEBUG )
    
    l_Params = l_FileXML["Debug"];
    if (l_Params.Exists())
    {
      m_f_dJumpVerticalSpeed = l_Params.GetFloatProperty("JumpVerticalSpeed", 20.0f);
      m_f_dJumpMoveSpeed     = l_Params.GetFloatProperty("JumpMoveSpeed", 0.0f);
      m_f_dHitDamage         = l_Params.GetFloatProperty("HitDamage", 10.f);
      m_f_dRunSpeedFactor    = l_Params.GetFloatProperty("RunSpeedFactor", 0.5f);
    }
    
  }
          
  
  if (m_bIsOk)
  {
    AssignLights();
    AssingAnimation();

    if (m_pPhysicController == NULL)
      InicializaController();
    //InicializaController("mi_amigo",2.5f, 3.0f, 40.0f, 0.01f, 0.3f);
    m_fMoveSpeed = m_fDefaultMoveSpeed;
    m_fRotSpeedFactor = 6.0f;

    m_fCurrentAttackDamage = 0.0f;

    m_pAction2Input = CORE->GetInputManager()->GetActionToInput();

    m_fHealth = m_fMaxHealth;

    if (m_pPlayerCamera == NULL)
    {
      m_pPlayerCamera = CORE->GetCameraManager()->GetCamera();
      InitCamera();
    }

    /*if (m_pAnimCallback == NULL)
      RegAnimCallback();*/
  }

  #if defined( _DEBUG )
  #endif // defined( _DEBUG )
    m_sSetupFileName = _file;

  return m_bIsOk;
}

void CPlayer::InicializaController()
{
  CHECKED_DELETE(m_pPhysicController)
  
  CPhysicUserData *userdata = new CPhysicUserData(m_sInstanceName);
  userdata->SetColor(colCYAN);
  userdata->SetPaint(true);
  
  m_V3IniPos = m_pRenderableAnimatedInstanceModel->GetPosition();

  m_pPhysicController = new CPhysicController(m_fControllerRadius, m_fControllerHeight, m_fControllerSlope, m_fControllerSkinWidth, m_fControllerStepOffset, PX_MSK_ALL, userdata, m_V3IniPos);
  CORE->GetPhysicsManager()->AddPhysicController(m_pPhysicController);
}

/// <summary>
/// Establece ls valores inciales de la camara que sigue al player. Util tras haber liberado memoria.
/// </summary>
void CPlayer::InitCamera()
{
  m_fIniCameraYaw = m_pPlayerCamera->GetObject3D()->GetYaw();
  m_fIniCameraPitch = m_pPlayerCamera->GetObject3D()->GetPitch();
}

/// <summary>
/// Establece la animación del player. Util tras haber liberado memoria.
/// </summary>
void CPlayer::AssingAnimation()
{
  m_pRenderableAnimatedInstanceModel = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(m_sInstanceName);

  m_pAnimation = m_pRenderableAnimatedInstanceModel->GetAnimatedInstanceModel();

  m_pAnimData->Init();
  m_pAnimation->GetModelCal3DHW()->setUserData(m_pAnimData);

  RegAnimCallback();
}

/// <summary>
/// Establece las luces del player. Util tras haber liberado memoria.
/// </summary>
void CPlayer::AssignLights()
{
  m_pAttackLight = CORE->GetLigthManager()->GetResource(m_sAttackLightName);
}

/// <summary>
/// Reestablece los parametros del player para reiniciar el nivel.
/// </summary>
void CPlayer::ReStart()
{
  m_pPhysicController->SetPosition(m_V3IniPos);
  m_pPlayerCamera->GetObject3D()->SetYaw(m_fIniCameraYaw);
  m_pPlayerCamera->GetObject3D()->SetPitch(m_fIniCameraPitch);
  m_pRenderableAnimatedInstanceModel->SetYaw(m_fIniCameraYaw - FLOAT_HALF_PI_VALUE);

  CORE->GetGUIManager()->SetVisibleGuiElement("gui_blood", false);
  CORE->GetGUIManager()->SetVisibleGuiElement("gui_dialog", false);

  // asignaciones del revive
  CORE->GetScriptManager()->RunCode("gui_HI:max_health()");

  m_fHealth = m_fMaxHealth;
  m_bCollideEnemies = true;

  m_pAnimation->RemoveAction(S_DEATH);
  m_iPlayerState = S_IDLE;
  m_iPlayerActionState = S_NO_ACTION;
}

void CPlayer::Done()
{
}

void CPlayer::Update(float elapsedTime)
{
  m_pPhysicController->Move(Vect3f(0.001f, 0.0f, 0.001f), elapsedTime);

  switch(m_iPlayerState) {
    // se usa para dejar inactivo al player al temrinar el nivel
    case S_NO_ACTION: 
      m_pAnimation->BlendCycle(S_IDLE, 0.2f);
      break;

    case S_DEATH:
      break;

    case S_JUMP:
      if (m_pAnimData->GetIsJumping())
      {
        if (!m_bIsJumping)
        {
          m_pPhysicController->Jump(m_fJumpVerticalSpeed, m_V3fJumpDirection.x, m_V3fJumpDirection.z);
          m_bIsJumping = true;
        }

        m_pPhysicController->Move(m_V3fJumpDirection, elapsedTime);
      }
      else
      {
        m_pPhysicController->Move(Vect3f(MOVE_EPSILON, MOVE_EPSILON, MOVE_EPSILON), elapsedTime);
      }

      ActionStateMachine();
      break;

    case S_WALK:
      m_pAnimation->BlendCycle(S_WALK, 0.2f);
      
      ActionStateMachine();

      if (MovPlayer(elapsedTime))
      {
        if (m_bJumpTiming)
        {
          if (IsFacingPlatform())
            m_fJumpTimeCounter += elapsedTime;
          else
            m_fJumpTimeCounter = 0.0f;

          if (m_fJumpTimeCounter >= m_fJumpTimeToStart)
          {
            DoJump();
          }
        }
        else if (m_bOnJumpTrigger)
        {
          Jump();
        }
      }
      else
      {
        m_pPhysicController->Move(Vect3f(MOVE_EPSILON, MOVE_EPSILON, MOVE_EPSILON), elapsedTime);
        m_iPlayerState = S_IDLE;
        if (m_bJumpTiming)
        {
          m_bJumpTiming = false;
        }
      }

      break;

    case S_IDLE:
    default:
      m_pAnimation->BlendCycle(S_IDLE, 0.2f);

      ActionStateMachine();
      
      if ((m_iPlayerActionState != S_S_ATTACK) && MovPlayer(elapsedTime))
      {
        m_iPlayerState = S_WALK;
      }
      else
      {
        m_pPhysicController->Move(Vect3f(MOVE_EPSILON, MOVE_EPSILON, MOVE_EPSILON), elapsedTime);
      }

      break;
  }
  
  #if defined( _DEBUG )
  #endif
  MovPlayerDebug(elapsedTime);
  
  // actualizando la posicion del modelo animado
  Vect3f l_pos = m_pPhysicController->GetPosition();
  l_pos.y -= 1.1f;
  m_pRenderableAnimatedInstanceModel->SetPosition(l_pos);


  //CORE->GetLigthManager()->GetResource("Player")->SetPosition(l_pos);
  //UpdateAttackCollisionBox();
  /*bool xxx = m_pAnimData->GetCheckAttackCollision();
  //baseUtils::FormatSrting(m_sDebug, "Player> checkAttackCollision: %d", xxx);
  */
//  baseUtils::FormatSrting(m_sDebug, "%s", m_pAnimData->m_sDebug.c_str());
  //baseUtils::FormatSrting(m_sDebug, "Player>  health: %f", m_fHealth);
}

// sólo para debug
void CPlayer::Render()
{
  CORE->GetRenderManager()->SetTransform(m_AttackCollisionBox);
  CORE->GetRenderManager()->DrawAxis(0.5f);
  CORE->GetRenderManager()->DrawSphere(0.1f, colCYAN, 4);
  if (m_pAnimData->GetCheckAttackCollision())
  {
    CORE->GetRenderManager()->DrawCube(Vect3f(0.8f, 0.2f, 2.4f), colBLUE);
  }
  else
  {
    CORE->GetRenderManager()->DrawCube(Vect3f(0.8f, 0.2f, 2.4f), colYELLOW);
  }
  
  //encarar
  Vect3f a = GetInstance()->GetPosition(); 

  Mat44f vecOptimo;
  vecOptimo.SetIdentity();
  CORE->GetRenderManager()->SetTransform(vecOptimo);

  CORE->GetRenderManager()->DrawLine(a,a + m_v3Player,colWHITE);
  CORE->GetRenderManager()->DrawLine(a,a + m_v3Enemy,colRED);
  CORE->GetRenderManager()->DrawLine(a,a + m_v3PlayerRight,colYELLOW);
}

void CPlayer::HealthUp(float _fHealtPoints)
{
  std::string l_sLUACode;
  baseUtils::FormatSrting(l_sLUACode, "gui_HI:health_up(%f,%f)", m_fHealth, _fHealtPoints);
  CORE->GetScriptManager()->RunCode(l_sLUACode);

  m_fHealth += _fHealtPoints;

  if (m_fHealth > m_fMaxHealth)
  {
    m_fHealth = m_fMaxHealth;
  }
}

// se ejecuta cuando el player es golpeado
void CPlayer::Hit(float damage)
{
  if (m_bCollideEnemies)
  {
    std::string l_sLUACode;
    baseUtils::FormatSrting(l_sLUACode, "gui_HI:hit(%f,%f)", m_fHealth, damage);
    CORE->GetScriptManager()->RunCode(l_sLUACode);

    m_pAnimData->SetCheckAttackCollision(false);
    m_fHealth -= damage;
    if (m_fHealth <= 0) {
      Die();
    }
    else
    {
      // codigo para la sangre
      CORE->GetScriptManager()->RunCode("gui_show_blood()");
      
      m_pAttackLight->SetEnable(false);
      m_pAnimation->RemoveAction(m_iPlayerActionState);
      m_pAnimation->ExecuteAction(S_HIT, 0.2f, 0.2f);
      m_pAnimData->SetCurrentAction(S_HIT);
      m_iPlayerActionState = S_HIT;
    }
  }
  m_bCollideEnemies = false;
}
void CPlayer::Revive()
{
  std::string l_sLUACode;
  baseUtils::FormatSrting(l_sLUACode, "gui_HI:health_up(%f,%f)", m_fHealth, m_fMaxHealth);
  CORE->GetScriptManager()->RunCode(l_sLUACode);

  m_fHealth         = m_fMaxHealth;
  m_bCollideEnemies = true;

  m_pAnimation->RemoveAction(S_DEATH);
  m_iPlayerState = S_IDLE;
  m_iPlayerActionState = S_NO_ACTION;
}

void CPlayer::ActionStateMachine()
{
  switch(m_iPlayerActionState)
  {
    case S_JUMP:
      if (!m_pAnimData->IsPlaying(S_JUMP))
      {
        m_bJumpTiming = false;
        m_iPlayerActionState = S_NO_ACTION;
        m_iPlayerState = S_IDLE;
      }
      break;
    
    case S_HIT:
      if (!m_pAnimData->IsPlaying(S_HIT))
      {
        m_bCollideEnemies = true;
        m_iPlayerActionState = S_NO_ACTION;
      }
      break;
    
    case S_S_ATTACK:
      StrongAttack();
      break;
    
    case S_L_ATTACK:
      LightAttack();
      break;
    
    default:
      if (m_pAction2Input->DoAction("PlayerLightAttack") != 0.0f )
      {
        m_fCurrentAttackDamage = m_fLightAttackDamage;
        m_pAnimation->ExecuteAction(S_L_ATTACK, 0.2f, 0.2f);
        m_pAnimData->SetCurrentAction(S_L_ATTACK);
        
        m_pAttackLight->SetEnable(true);
        m_bAttackLightIsOn = true;
        
        CORE->GetSoundManager()->PlaySound("swordSwing1");
        m_pAnimData->SetIsPlayingSound(true);

        m_iPlayerActionState = S_L_ATTACK;
      }
      else if (m_pAction2Input->DoAction("PlayerStrongAttack") != 0.0f )
      {
        m_fCurrentAttackDamage = m_fStrongAttackDamage;
        m_pAnimation->ExecuteAction(S_S_ATTACK, 0.1f, 0.2f);
        m_pAnimData->SetCurrentAction(S_S_ATTACK);
        
        m_pAttackLight->SetEnable(true);
        m_bAttackLightIsOn = true;
        
        m_pAnimData->SetIsPlayingSound(false);
        
        m_iPlayerActionState = S_S_ATTACK;
        m_iPlayerState = S_IDLE;
      }
   }
}

bool CPlayer::MovPlayer(float elapsedTime)
{
  //Física
  Vect3f l_VectorDirector;
  m_pPlayerCamera = CORE->GetCameraManager()->GetCamera();
  CObject3D * l_CameraObject = m_pPlayerCamera->GetObject3D();
  float l_ActualYaw = l_CameraObject->GetYaw();

  if (m_pAction2Input->DoAction("GoForwardLeft") != 0.0f )
  {
    l_VectorDirector.x = cos(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector *= m_fMoveSpeed;
    m_pPhysicController->Move(l_VectorDirector, elapsedTime);

    RotPlayer(elapsedTime);
    
    return true;
  }
  else if (m_pAction2Input->DoAction("GoForwardRight") != 0.0f )
  {

    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector *= m_fMoveSpeed;
    m_pPhysicController->Move(l_VectorDirector, elapsedTime);	

    RotPlayer(elapsedTime);
    
    return true;
  }

  else if (m_pAction2Input->DoAction("GoBackwardsRight") != 0.0f )
  {
    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector *= m_fMoveSpeed;
    m_pPhysicController->Move((l_VectorDirector*-1), elapsedTime);

    RotPlayer(elapsedTime);
    
    return true;
  }

  else if (m_pAction2Input->DoAction("GoBackwardsLeft") != 0.0f )
  {
    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector *= m_fMoveSpeed;
    m_pPhysicController->Move((l_VectorDirector*-1), elapsedTime);

    RotPlayer(elapsedTime);
    
    return true;
  }

  //UP DOWN LEFT RIGHT MOVEMENTS
  else if (m_pAction2Input->DoAction("GoForward") != 0.0f )
  {
    //if(!m_pPlayerCamera->GetShake())
    //{
       Vect3f l_VectorDirector = m_pPlayerCamera->GetDirection();
    //}
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.Normalize();
    l_VectorDirector *= m_fMoveSpeed;
    m_pPhysicController->Move(l_VectorDirector, elapsedTime);

    RotPlayer(elapsedTime);
    
    return true;
  }

  else if (m_pAction2Input->DoAction("GoBackwards") != 0.0f )
  {
    //if(!m_pPlayerCamera->GetShake())
    //{
      Vect3f l_VectorDirector = m_pPlayerCamera->GetDirection();
    //}
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.Normalize();
    l_VectorDirector *= m_fMoveSpeed;
    m_pPhysicController->Move((l_VectorDirector*-1), elapsedTime);	

    RotPlayer(elapsedTime);
    
    return true;
  }

  else if (m_pAction2Input->DoAction("GoSidewardLeft") != 0.0f )
  {
    //if(!m_pPlayerCamera->GetShake())
    //{
      Vect3f l_VectorStrafe = (m_pPlayerCamera->GetDirection() ^ m_pPlayerCamera->GetVecUp()).Normalize(); 
      l_VectorStrafe.y = 0.0f;
      l_VectorStrafe *= m_fMoveSpeed;
      m_pPhysicController->Move(l_VectorStrafe, elapsedTime);
    //}
    RotPlayer(elapsedTime);
    
    return true;
  }

  else if (m_pAction2Input->DoAction("GoSidewardRight") != 0.0f )
  {
    //if(!m_pPlayerCamera->GetShake())
    //{
      Vect3f l_VectorStrafe = (m_pPlayerCamera->GetVecUp() ^ m_pPlayerCamera->GetDirection()).Normalize();
      l_VectorStrafe.y = 0.0f;
      l_VectorStrafe *= m_fMoveSpeed;
      m_pPhysicController->Move(l_VectorStrafe, elapsedTime);	
    //}
    RotPlayer(elapsedTime);
    
    return true;
  }
  
  else if (m_pAction2Input->DoAction("PlayerTurnToEnemy") != 0.0f )
  {
    if(CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss())
    {
      TurnToEnemy(CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss()->GetPhysicController()->GetPosition());
    }
  }
  
  return false;
}

void CPlayer::GoBack(float _dt)
{
  float l_fYawPlayer = - (GetInstance()->GetYaw() + FLOAT_HALF_PI_VALUE);

  m_pPhysicController->Move( Vect3f (cos(l_fYawPlayer)*10.0f, 0.0f, sin(l_fYawPlayer)*10.0f), _dt);

}

void CPlayer::TurnToEnemy(Vect3f _hisPos)
{
  float l_fYawPlayer = - (GetInstance()->GetYaw() + FLOAT_HALF_PI_VALUE);

  Vect3f l_V3DirOptimo =  _hisPos - GetInstance()->GetPosition();

  float dir = l_V3DirOptimo.GetAngleY() - FLOAT_HALF_PI_VALUE;
  fmod(dir, FLOAT_PI_VALUE);

  m_v3Player = Vect3f(10.f,1.0f,0.0f);
  m_v3Player.RotateY(l_fYawPlayer);

  m_v3PlayerRight = Vect3f(10.f,1.0f,0.0f);
  m_v3PlayerRight = m_v3Player.GetRotatedY(FLOAT_HALF_PI_VALUE);
  
  m_v3Enemy = Vect3f(10.f,1.0f,0.0f);
  m_v3Enemy.RotateY(dir);

  if(m_v3Enemy * m_v3PlayerRight > 0)
  {  
    GetInstance()->SetYaw( GetInstance()->GetYaw() - m_fAngleRotation);
//    if(!m_pPlayerCamera->GetShake())
//    {
      m_pPlayerCamera->GetObject3D()->SetYaw(m_pPlayerCamera->GetObject3D()->GetYaw() - m_fCameraRotation);
//    }
  }
  else
  {
    GetInstance()->SetYaw( GetInstance()->GetYaw() + m_fAngleRotation);
    //if(!m_pPlayerCamera->GetShake())
    //{
      m_pPlayerCamera->GetObject3D()->SetYaw(m_pPlayerCamera->GetObject3D()->GetYaw() + m_fCameraRotation);
    //}
  }
}

void CPlayer::RotPlayer(float elapsedTime)
{
  Mat44f l_mModelMatrix = GetInstance()->GetMat44();

  float l_fCamYaw     = m_pPlayerCamera->GetObject3D()->GetYaw();
  float l_fPlayerYaw  = m_pRenderableAnimatedInstanceModel->GetYaw() + FLOAT_HALF_PI_VALUE;

  // convierto el angulo a un valor equivalente entre -PI y PI
  if (l_fPlayerYaw > FLOAT_PI_VALUE)
    l_fPlayerYaw -= FLOAT_2PI_VALUE;
  else if (l_fPlayerYaw < -FLOAT_PI_VALUE)
    l_fPlayerYaw += FLOAT_2PI_VALUE;

  float l_fAngle = l_fPlayerYaw - l_fCamYaw;

  // convierto el angulo a un valor equivalente entre -PI y PI
  if (l_fAngle > FLOAT_PI_VALUE)
    l_fAngle -= FLOAT_2PI_VALUE;
  else if (l_fAngle < -FLOAT_PI_VALUE)
    l_fAngle += FLOAT_2PI_VALUE;

  float l_fAbsAngle = fabs(l_fAngle);

  if (l_fAbsAngle != 0.0f && l_fAbsAngle <= UPPER_LIMIT)
  {
    l_fAbsAngle = sqrt(pow(l_fAbsAngle, 3)) + 1.0f;
    
    float l_fRotSpeed = asec(l_fAbsAngle) * m_fRotSpeedFactor;

    if (l_fAngle > 0)
    {
      l_fPlayerYaw = l_fPlayerYaw - (l_fRotSpeed * elapsedTime) - FLOAT_HALF_PI_VALUE;
    }
    else
    {
      l_fPlayerYaw = l_fPlayerYaw + (l_fRotSpeed * elapsedTime) - FLOAT_HALF_PI_VALUE;
    }
  
    // convierto el angulo a un valor equivalente entre -PI y PI
    if (l_fPlayerYaw > FLOAT_PI_VALUE)
      l_fPlayerYaw -= FLOAT_2PI_VALUE;
    else if (l_fPlayerYaw < -FLOAT_PI_VALUE)
      l_fPlayerYaw += FLOAT_2PI_VALUE;

    m_pRenderableAnimatedInstanceModel->SetYaw(l_fPlayerYaw);  
  }
}

void CPlayer::Die() 
{
  m_fHealth = 0;
  m_pAnimation->ExecuteActionBlocked(S_DEATH, 0.3f);
  m_pAnimData->SetCurrentAction(S_DEATH);
  CORE->GetSoundManager()->PlaySound("PlayerDeath");
  m_pAnimData->SetIsPlayingSound(true);
  m_iPlayerState = S_DEATH;
  CORE->GetGUIManager()->ActiveWindowsWithEffect("Main.xml", TE_SHADOW, 10.f);
}

// se registran las animaciones que tendran callbacks (update y AnimComplete)
void CPlayer::RegAnimCallback()
{
  CHECKED_DELETE(m_pAnimCallback)
  m_pAnimCallback = new CPlayerAnimCallback();

  CalCoreAnimation* l_pCoreAnimation = CORE->GetAnimatedModelManager()->GetResource("bot")->GetCoreModel()->getCoreAnimation(S_S_ATTACK);
  l_pCoreAnimation->registerCallback(m_pAnimCallback, 0.0f);
  
  l_pCoreAnimation = CORE->GetAnimatedModelManager()->GetResource("bot")->GetCoreModel()->getCoreAnimation(S_L_ATTACK);
  l_pCoreAnimation->registerCallback(m_pAnimCallback, 0.0f);
  
  l_pCoreAnimation = CORE->GetAnimatedModelManager()->GetResource("bot")->GetCoreModel()->getCoreAnimation(S_HIT);
  l_pCoreAnimation->registerCallback(m_pAnimCallback, 0.0f);
  
  l_pCoreAnimation = CORE->GetAnimatedModelManager()->GetResource("bot")->GetCoreModel()->getCoreAnimation(S_DEATH);
  l_pCoreAnimation->registerCallback(m_pAnimCallback, 0.0f);
  
  l_pCoreAnimation = CORE->GetAnimatedModelManager()->GetResource("bot")->GetCoreModel()->getCoreAnimation(S_JUMP);
  l_pCoreAnimation->registerCallback(m_pAnimCallback, 0.0f);
}

void CPlayer::UpdateAttackCollisionBox()
{
  CalSkeleton* l_Skeleton = m_pAnimation->GetModelCal3DHW()->getSkeleton();

  int l_iScytheBone = l_Skeleton->getCoreSkeleton()->getCoreBoneId("ARMA");
  CalVector l_V3Position = l_Skeleton->getBone(l_iScytheBone)->getTranslationAbsolute();
  //CalQuaternion l_CalRotation = l_Skeleton->getBone(l_iScytheBone)->getRotationAbsolute();
  //CalVector l_V3Position = l_Skeleton->getBone(l_iScytheBone)->getTranslation();
//  CalQuaternion l_CalRotation = l_Skeleton->getBone(l_iScytheBone)->getRotationAbsolute();
//  CalMatrix l_CalBoneMatrix = l_Skeleton->getBone(l_iScytheBone)->getTransformMatrix();
  
//  CalQuaternion l_CalRotMatrix = l_Skeleton->getBone(l_iScytheBone)->getRotationBoneSpace();

//  CalMatrix l_CalTMatrix = CalMatrix(l_CalRotation);
  //CalMatrix l_CalTMatrix = CalMatrix(l_CalRotMatrix);

  Vect3f V3ScythePos;
  V3ScythePos.x = -l_V3Position.x;
  V3ScythePos.y = l_V3Position.y;
  V3ScythePos.z = l_V3Position.z;

//  Vect3f V3ScytheBladePos;
  //V3ScytheBladePos.x += 0.041569f;
  //V3ScytheBladePos.y += -0.161193f;
  //V3ScytheBladePos.z += 0.615405f;

/*  V3ScytheBladePos.x = 0.0f;
  V3ScytheBladePos.y = 1.0f;
  V3ScytheBladePos.z = 0.0f;
*/
//  Mat33f boneMatrix;
  /*boneMatrix.m00 = l_CalTMatrix.dxdx;  boneMatrix.m01 = l_CalTMatrix.dydx;  boneMatrix.m02 = l_CalTMatrix.dzdx;
  boneMatrix.m10 = l_CalTMatrix.dxdy;  boneMatrix.m11 = l_CalTMatrix.dydy;  boneMatrix.m12 = l_CalTMatrix.dzdy;
  boneMatrix.m20 = l_CalTMatrix.dxdz;  boneMatrix.m21 = l_CalTMatrix.dydz;  boneMatrix.m22 = l_CalTMatrix.dzdz;
  */
/*  boneMatrix.m00 = l_CalTMatrix.dxdx;  boneMatrix.m01 = l_CalTMatrix.dxdy;  boneMatrix.m02 = l_CalTMatrix.dxdz;
  boneMatrix.m10 = l_CalTMatrix.dydx;  boneMatrix.m11 = l_CalTMatrix.dydy;  boneMatrix.m12 = l_CalTMatrix.dydz;
  boneMatrix.m20 = l_CalTMatrix.dzdx;  boneMatrix.m21 = l_CalTMatrix.dzdy;  boneMatrix.m22 = l_CalTMatrix.dzdz;
*/  
  
  Mat44f l_Mat44Local, l_Mat44Instance;
//  Mat44f l_Blade;


  /*l_Mat44Local.m00 = l_CalTMatrix.dxdx;  l_Mat44Local.m01 = l_CalTMatrix.dydx;  l_Mat44Local.m02 = l_CalTMatrix.dzdx;  l_Mat44Local.m03 = 0.0f;
  l_Mat44Local.m10 = l_CalTMatrix.dxdy;  l_Mat44Local.m11 = l_CalTMatrix.dydy;  l_Mat44Local.m12 = l_CalTMatrix.dzdy;  l_Mat44Local.m13 = 0.0f;
  l_Mat44Local.m20 = l_CalTMatrix.dxdz;  l_Mat44Local.m21 = l_CalTMatrix.dydz;  l_Mat44Local.m22 = l_CalTMatrix.dzdz;  l_Mat44Local.m23 = 0.0f;
  l_Mat44Local.m30 = 0.0f;               l_Mat44Local.m31 = 0.0f;               l_Mat44Local.m32 = 0.0f;               l_Mat44Local.m33 = 1.0f;
*/
/*    l_Mat44Local.m00 = l_CalTMatrix.dxdx;  l_Mat44Local.m01 = l_CalTMatrix.dxdy;  l_Mat44Local.m02 = l_CalTMatrix.dxdz;  l_Mat44Local.m03 = 0.0f;
  l_Mat44Local.m10 = l_CalTMatrix.dydx;  l_Mat44Local.m11 = l_CalTMatrix.dydy;  l_Mat44Local.m12 = l_CalTMatrix.dydz;  l_Mat44Local.m13 = 0.0f;
  l_Mat44Local.m20 = l_CalTMatrix.dzdx;  l_Mat44Local.m21 = l_CalTMatrix.dzdy;  l_Mat44Local.m22 = l_CalTMatrix.dzdz;  l_Mat44Local.m23 = 0.0f;
  l_Mat44Local.m30 = 0.0f;               l_Mat44Local.m31 = 0.0f;               l_Mat44Local.m32 = 0.0f;               l_Mat44Local.m33 = 1.0f;
*/

  
  
//  float l_fAngleX = mathUtils::ATan2(-l_CalTMatrix.dzdy, l_CalTMatrix.dydy);
//  float l_fAngleY = mathUtils::ATan2(l_CalTMatrix.dxdz, l_CalTMatrix.dxdx);
//  float l_fAngleZ = mathUtils::ASin(-l_CalTMatrix.dxdy);
 /* float l_fAngleX = mathUtils::ASin(l_CalTMatrix.dydz);
  float l_fAngleY = mathUtils::ASin(l_CalTMatrix.dzdx);
  float l_fAngleZ = mathUtils::ASin(l_CalTMatrix.dxdy);
*/

 
  /*
  l_Mat44Local.RotByAngleX(-l_fAngleX);
  l_Mat44Local.RotByAngleY(-l_fAngleY);
  l_Mat44Local.RotByAngleZ(l_fAngleZ);
  
  */


  /*l_Mat44Local.RotByAngleZ(-l_fAngleZ);
  l_Mat44Local.RotByAngleY(-l_fAngleY);
  l_Mat44Local.RotByAngleX(l_fAngleX);
  */
  

  //l_Mat44Local.Translate( V3ScythePos );
  

 /* l_Mat44Local.RotByAngleX(l_fAngleX);
  l_Mat44Local.RotByAngleY(l_fAngleY);
  l_Mat44Local.RotByAngleZ(l_fAngleZ);*/

//  l_Blade.SetIdentity();
  //l_Blade.Translate(V3ScytheBladePos);
/*Mat33f subMatrixBlade = boneMatrix * l_Blade.GetSubMatrix33();//
  l_Blade.SetSubMatrix33(subMatrixBlade);*/
  

  // matrix del hueso
  l_Mat44Local.SetIdentity();
  l_Mat44Local.Translate(V3ScythePos);

//boneMatrix //Mat33 mat33 = matriz de CAL3d
//l_Mat44Local //Mat44f = mimatrizdetransformacion
//Si es necesario
//mimatrizdetransformacion.LHtoRH
//
//float boneyaw = boneMatrix.GetYaw();
//float bonepitch = boneMatrix.GetPitch();
//float boneroll = boneMatrix.GetRoll();
//V3ScythePos //posicion = huesodecal3d.getPOsition
//l_Mat44Local.SetRotByAngleZ(boneyaw);//.setyaw(yaw)
//l_Mat44Local.SetRotByAngleY(bonepitch);//.setpitch(pitch)
//l_Mat44Local.SetRotByAngleZ(boneroll);//.setroll(roll)
/*Mat44f r, r2, r3, t, l_Scale;
t.SetIdentity();
r.SetIdentity();
r2.SetIdentity();
r3.SetIdentity();
t.Translate(V3ScythePos);
r.SetFromAngleY(-boneyaw);
r2.SetFromAngleZ(bonepitch);
r3.SetFromAngleX(boneroll);*/
//l_Scale.SetFromScale(m_ScalarX, m_ScalarY, m_ScalarZ);
//l_Mat44Local = t*r*r2*r3;//*l_Scale;   
//l_Mat44Local = t;

//l_Mat44Local.SetRotByAngleZ(boneyaw);//.setyaw(yaw)
//l_Mat44Local.SetRotByAngleY(boneroll);//.setpitch(pitch)
//l_Mat44Local.SetRotByAngleZ(bonepitch);//.setroll(roll)
//l_Mat44Local.SetPos(V3ScythePos);//.setposition(posicion)

  //Mat33f subMatrixScythe = boneMatrix * l_Mat44Local.GetSubMatrix33();//
  
/*l_Mat44Local.RotByAngleZ(l_fAngleZ);
l_Mat44Local.RotByAngleY(l_fAngleY);
l_Mat44Local.RotByAngleX(l_fAngleX);*/

  //l_Mat44Local.SetSubMatrix33(subMatrixScythe);
  //l_Mat44Local.Translate(V3ScythePos);
  
  
  

  //l_Mat44Instance.SetIdentity();
  l_Mat44Instance = GetInstance()->GetMat44();
  

  /*Mat33f subMatrix = l_Mat44Instance.GetSubMatrix33() * boneMatrix;
  l_Mat44Instance.SetSubMatrix33(subMatrix);
*/
    //l_Mat44Local = l_Mat44Local * l_Blade;
  //l_Mat44Instance = l_Mat44Instance * (l_Mat44Local * l_Blade);
  //l_Mat44Instance = l_Blade * l_Mat44Local * l_Mat44Instance;

  //m_AttackCollisionBox =  l_Mat44Instance * l_Mat44Local * l_Blade;
  //m_AttackCollisionBox =  l_Mat44Instance;// * (l_Mat44Local.Translate(V3ScythePos));// * l_Blade;
  m_AttackCollisionBox =  l_Mat44Instance * l_Mat44Local;
  
  // posición de la luz
  if (m_pAttackLight != NULL)
    m_pAttackLight->SetPosition(m_AttackCollisionBox.GetPos());
}

void CPlayer::CheckAttackCollision()
{
  Vect3f posSphere = m_AttackCollisionBox.GetPos();
  std::vector<CPhysicUserData*>	l_ImpactObjects;

  // dimensiones de la caja (0.8f, 0.2f, 2.4f)
  CORE->GetPhysicsManager()->OverlapBoxActor(Vect3f(0.8f, 0.2f, 2.4f), m_AttackCollisionBox, l_ImpactObjects);
  //CORE->GetPhysicsManager()->OverlapSphereActor(1.0f, posSphere, l_ImpactObjects);

  if(!l_ImpactObjects.empty())
  {
    std::vector<CPhysicUserData*>::iterator itVEnd = l_ImpactObjects.end();
    for(std::vector<CPhysicUserData*>::iterator it = l_ImpactObjects.begin(); it != itVEnd; ++it)
    {
      // si es una instancia de zombie
      if (IsZombie(*it))
      {
				CZombie * l_pZombie = CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetResource((*it)->GetName().c_str());
				if (l_pZombie->GetHealth() > 0)
				{
					SalpicarSangre((*it)->GetName().c_str());
				}
        // codigo para hacerle daño al zombie
        std::string l_sScript;
        baseUtils::FormatSrting(l_sScript, "z_recibe_golpe('%s', %f)",(*it)->GetName().c_str(), m_fCurrentAttackDamage);
        CORE->GetScriptManager()->RunCode(l_sScript);

				//CORE->GetSoundManager()->PlaySound("zombieAttack");
        // codigo para la sangre
        //CORE->GetScriptManager()->RunCode("gui_show_blood()");
      }
    }
  }
}
//para calcular la posicion de la sangre en el hueso del cuello (en realidad la pos a la altura del pecho)
void CPlayer::PosicionaTorso(std::string nombre)
{
  CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(nombre);
  CalSkeleton* skeleton = model->GetAnimatedInstanceModel()->GetModelCal3DHW()->getSkeleton();
 
  std::string l_sHueso = "";
  
  l_sHueso = !nombre.compare("z_inst_200")? "Bip01 Spine1": "Bip01 Spine4";
 
  int l_iHuesoTorso = skeleton->getCoreSkeleton()->getCoreBoneId(l_sHueso);
  CalVector l_v3Position;
  
  if(l_iHuesoTorso != -1)
    l_v3Position = (CalVector)skeleton->getBone(l_iHuesoTorso)->getTranslationAbsolute();
  else
  {
    // Guarda mensaje de error en el log
		std::string msg_error = "CPlayer::PosicionaTorso->Error hueso no encontrado: ";
		msg_error.append(l_sHueso);
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
    m_bIsOk = false;
  }    
    
  m_v3PosicionTorso.x = -l_v3Position.x;
  m_v3PosicionTorso.y = l_v3Position.y;
  m_v3PosicionTorso.z = l_v3Position.z;
}

//para devolver la posicion del hueso
Vect3f CPlayer::GetPosicionTorso(std::string nombre)
{
	Mat44f l_t,l_vMat44;
	l_t.SetIdentity();
	l_t.Translate(m_v3PosicionTorso);
	l_vMat44.SetIdentity();
	CRenderableAnimatedInstanceModel* model = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance(nombre);
	l_vMat44 = model->GetMat44();
	Mat44f l_vMatrizTorso =  l_vMat44 * l_t;

	return l_vMatrizTorso.GetPos();
}

void CPlayer::SalpicarSangre(std::string name)
{
	std::string l_sName = "pSangre1";
	//si es ataque suave, la sangre es de tipo 1, sino tipo 2
	if (m_fCurrentAttackDamage <= m_fLightAttackDamage)
	{
		l_sName = "pSangre2";
	}
	//render
	CBillboardAnimation * l_pBillboardAnimation = CORE->GetBillboardManager()->GetBAnimationByInstance(l_sName);
	//calculamos la pos
	if(l_pBillboardAnimation)
	{
	  PosicionaTorso(name);
	  l_pBillboardAnimation->SetPos(GetPosicionTorso(name));
	  l_pBillboardAnimation->SetEnd(false);
	}
}

// retorna bool: true si el PhysicUserData dado pertenece a un zombie
bool CPlayer::IsZombie(CPhysicUserData * _UserData)
{
  if (_UserData->GetName().find("z_inst_") != std::string::npos)
  {
    if (_UserData->GetName().find("Mano") == std::string::npos)
    {
      return true;
    }
  }
  return false;
}

/*** ACCIONES ***************************************************/

void CPlayer::LightAttack()
{
  if (!m_pAnimData->IsPlaying(S_L_ATTACK))
  {
    m_iPlayerActionState = S_NO_ACTION;
    return;
  }
    
  if (m_pAnimData->GetCheckAttackCollision())
  {
    UpdateAttackCollisionBox();
    CheckAttackCollision();
  }
  else if (m_bAttackLightIsOn) // Si termina el ataque (el swing mas no la animacion) apagar la luz si esta encendida
  {
    m_pAttackLight->SetEnable(false);
    m_bAttackLightIsOn = false;
  }
}

void CPlayer::StrongAttack()
{
  if (!m_pAnimData->IsPlaying(S_S_ATTACK))
  {
    m_iPlayerActionState = S_NO_ACTION;
    return;
  }
    
  if (m_pAnimData->GetCheckAttackCollision())
  {
    UpdateAttackCollisionBox();
    CheckAttackCollision();
  }
  else if (m_bAttackLightIsOn) // Si termina el ataque (el swing mas no la animacion) apagar la luz si esta encendida
  {
    m_pAttackLight->SetEnable(false);
    m_bAttackLightIsOn = false;
  }
}

/// <summary>
/// Establece los parametros para realizar el salto
/// </summary>
void CPlayer::Jump(std::string _sPlatformId, Vect3f _V3fDirection)
{
  m_bOnJumpTrigger = true;

  if (_sPlatformId != "")
    m_MapJumpDirections[_sPlatformId] = _V3fDirection;

  if (!m_bJumpTiming)
  {
    m_bJumpTiming = true;
    m_fJumpTimeCounter = 0.0f;
  }
}

/// <summary>
/// Cancela los parametros establecidos para el salto
/// </summary>
void CPlayer::CancelJump(std::string _sPlatformId)
{
  m_MapJumpDirections.erase(_sPlatformId);

  if (m_MapJumpDirections.size() == 0)
  {
    m_bJumpTiming = false;
    m_bOnJumpTrigger = false;
  }
}

/// <summary>
/// Realiza el salto
/// </summary>
void CPlayer::DoJump()
{
  m_V3fJumpDirection = m_pPlayerCamera->GetDirection();
  m_V3fJumpDirection.y = 0.0f;
  m_V3fJumpDirection.Normalize();
  m_V3fJumpDirection *= m_fJumpSpeed;
  
  //m_pPhysicController->Jump(m_fJumpVerticalSpeed, m_V3fJumpDirection.x, m_V3fJumpDirection.z);

  m_pAnimation->ExecuteAction(S_JUMP, 0.1f, 0.2f);
  m_pAnimData->SetCurrentAction(S_JUMP);
  m_pAnimData->SetIsPlayingSound(false);
  m_pAnimData->SetIsJumping(false);
  m_bIsJumping = false;
  m_MapJumpDirections.clear();
  
  m_iPlayerActionState = S_JUMP;
  m_iPlayerState = S_JUMP;
}

/// <summary>
/// Muerte del personaje por caida de precipicios y plataformas
/// </summary>
void CPlayer::Fall()
{
  CORE->GetSoundManager()->PlaySound("PlayerFall");
  m_bCollideEnemies = false;
  m_pAnimation->RemoveAction(m_iPlayerActionState);
  CORE->GetGUIManager()->ActiveWindowsWithEffect("Main.xml", TE_SHADOW, 1.f);
}


/// <summary>
/// Retorna true si el player está mirando hacia la dirección del trigger de salto
/// </summary>
bool CPlayer::IsFacingPlatform()
{
  Vect3f l_fCamDirection = m_pPlayerCamera->GetDirection();
  l_fCamDirection.y = 0.f;
  l_fCamDirection.Normalize();

  std::map<std::string, Vect3f>::iterator it = m_MapJumpDirections.begin();
  std::map<std::string, Vect3f>::iterator itEnd = m_MapJumpDirections.end();
  
  for (; it != itEnd; ++it)
  {
    float l_fCosAngle = l_fCamDirection * (*it).second;

    if (l_fCosAngle > m_fJumpDiretionCosAngle)
      return true;
  }

  return false;
}
Vect3f CPlayer::GetPosition ()
{
  return GetPhysicController()->GetPosition();
}

void CPlayer::SetPosition(Vect3f pos) 
{ 
  //GetPhysicController()->SetPosition(Vect3f(3.0f,2.5f,-3.8f));
  GetPhysicController()->SetPosition(pos);
}

void CPlayer::RestartPosition() 
{ 
  //GetPhysicController()->SetPosition(Vect3f(3.0f,2.5f,-3.8f));
  GetPhysicController()->SetPosition(Vect3f(3.0f,15.5f,-3.8f));
}

// FUNCIONES SOLO EN MODO DEBUG

bool CPlayer::Reload()
{
  if (Init(m_sSetupFileName))
  {
    ReStart();
    return true;
  }
  
  return false;
}

void CPlayer::MovPlayerDebug(float elapsedTime)
{
  #if defined( _DEBUG )
    if (m_pAction2Input->DoAction("Jump") != 0.0f)
    {
      m_V3fJumpDirection = m_pPlayerCamera->GetDirection();
      m_V3fJumpDirection.y = 0.0f;
      m_V3fJumpDirection.Normalize();
      m_V3fJumpDirection *= m_f_dJumpMoveSpeed;
    
      m_pPhysicController->Jump(m_f_dJumpVerticalSpeed, m_V3fJumpDirection.x, m_V3fJumpDirection.z);
    }

    if (m_pAction2Input->DoAction("PlatformJump") != 0.0f)
    {
      m_V3fJumpDirection = m_pPlayerCamera->GetDirection();
      m_V3fJumpDirection.y = 0.0f;
      m_V3fJumpDirection.Normalize();
     /* if (m_V3fJumpDirection.x > 0)
        m_V3fJumpDirection = Vect3f(1.0f, 0.0f, 0.0f); 
      else
        m_V3fJumpDirection = Vect3f(-1.0f, 0.0f, 0.0f); 
  */
      m_V3fJumpDirection *= m_fJumpSpeed;
    
      //m_pPhysicController->Jump(m_fJumpVerticalSpeed, m_V3fJumpDirection.x, m_V3fJumpDirection.z);

      m_pAnimation->ExecuteAction(S_JUMP, 0.1f, 0.2f);
      m_pAnimData->SetCurrentAction(S_JUMP);
      m_pAnimData->SetIsPlayingSound(false);
      m_pAnimData->SetIsJumping(false);
      m_bIsJumping = false;
      
      m_iPlayerActionState = S_JUMP;
      m_iPlayerState = S_JUMP;
    }

    if (m_pAction2Input->DoAction("Hit") != 0.0f )
    {
      if (m_bCollideEnemies)
        Hit(m_f_dHitDamage); //Testing purposes ony
    }
  #endif // defined( _DEBUG )

  if (m_pAction2Input->DoAction("Revive") != 0.0f )
  {
    if (IsDead())
      Revive();
  }

  if (m_pAction2Input->DoAction("PlayerRun") != 0.0f )
  {
    m_fMoveSpeed = m_fDefaultMoveSpeed * m_f_dRunSpeedFactor;
  }
  else
  {
    m_fMoveSpeed = m_fDefaultMoveSpeed;
  }
}

