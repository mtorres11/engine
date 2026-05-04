#include "VideoGame.h"
//#include "Core.h"
#include "CoreFIH.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
//#include "Math/Matrix44.h"
//#include "Math/MathUtils.h"
//#include "Cameras/FPSCamera.h"
#include "Cameras/ThPSCamera.h"
//#include "Object3D/Object3D.h"
//#include "Object3D/BoundingBox.h"
//#include "Texture/Texture.h"
//#include "Vertex/VertexType.h"
//#include "Vertex/IndexedVertex.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
//#include "Lights/Light.h"
#include "Lights/LightManager.h"
#include "Lights/OmniLight.h"
#include "Lights/LightEffects/LightEffect2.h"
#include "ActionToInput.h"
#include "Shaders/EffectManager.h"
#include "Shaders/SceneEffectManager.h"
//#include "RenderableObjects/RenderableObject.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "AnimationModel/AnimatedInstanceModel.h"
//#include "AnimationModel/AnimatedCoreModel.h"
#include "AnimationModel/AnimatedModelManager.h"
//#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "ScriptManager.h"
//#include "Cameras/Camera.h"
#include "Cameras/CameraManager.h"
#include "Cameras/CameraKeyController.h"
#include "GUIManager.h"
//#include "Actor/PhysicActor.h"
//#include "PhysicsManager.h"
#include "InputManager.h"
//#include "Cameras/Camera.h"
//#include "Shaders/SceneEffectManager.h"
//#include "Timer/Timer.h"
//#include "Utils/LogRender.h"
//#include "Particles/ParticleEmitter.h"
//#include "Particles/Particle.h"
#include "Particles/ParticleManager.h"
//#include "PhysicsManager.h"
#include "RenderablePhysX/RenderablePhysXManager.h"
//#include "RenderablePhysX/RenderablePhysXObject.h"
#include "CharacterController/PhysicController.h"
//#include "Joints/Fixed/PhysicFixedJoint.h"
#include "SoundManager.h"
//#include "Trigger/Trigger.h"
#include "Trigger/TriggerManager.h"
#include "ZombieManager/ZombieManager.h"
#include "Player/Player.h"
#include "ItemManager.h"
#include "Spirit/Spirit.h"
#include "BillBoard/BillboardManager.h"
#include "GUI/HealthIndicator.h"
//test
#include "ZombieBoss/ZombieBoss.h"

#include "Utils/BaseUtils.h"
#include <luabind/luabind.hpp>

// includes para pruebas... no deben ir en el proceso del videojuego
//#include "Player/PlayerAnimData.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CVideoGame::CVideoGame()
{
  m_DeltaFromXML = 0.0;
	m_pCoregame = 0;
	m_bDebugRender = false;
  m_ucChangeLevel = 0;
  m_pCameraKeyController = 0;
  m_bSonando = false;
  m_ucGameState = GS_NO_STATE;
  m_bEndAnimation = false;
}

CVideoGame::~CVideoGame()
{
  //IA4
	CHECKED_DELETE(m_pCoregame);
  CHECKED_DELETE(m_pCameraKeyController)
}

///<summary>
/// CVideoGame:: Init : Lectura de los XML. Creación y asignación de las cámaras a los objetos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CVideoGame::Init ()
{
  m_pCore = CORE;

  CRenderManager * rm = m_pCore->GetRenderManager();
  // se liberan las texturas cargadas por la ventana de GUI anterior (cuando viene de la pantalla del titulo)
  m_pCore->GetTextureManager()->Destroy();

  // LUA
  LoadLuaScripts();
  
  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);

  
  //IA4
	m_pCoregame = new CCoreFIH();
  m_pCoregame->Init();

  //espiritu init
  m_pCoregame->InitSpirit();

  m_ucCurrentLevel = 1;

  LoadCommonXMLs();
  LoadLevelXMLs(m_ucCurrentLevel);
  LoadGameConfig();

  m_pCoregame->GetZombieManager()->RegistroAnimacionesCallback();

  m_pCore->GetGUIManager()->Done();
  m_pCore->GetGUIManager()->Init("Data/GUI/inGameGUI.xml");
  m_pCore->GetGUIManager()->SetVisiblePointerMouse(false);
  m_pCore->GetGUIManager()->ActiveWindowsWithEffect("Main.xml", TE_SHADOW_OFF, 1.5f);

  //IA4
	m_pCore->GetTriggerManager()->RegistraTriggers();

  //Partículas
  m_pCore->GetParticleManager()->Init(rm);

	//BillboardAnimado
	m_pCore->GetBillboardManager()->Init(rm);

  //:Player
  m_pCoregame->InitPlayer("Data/Player.xml");
  m_pPlayer = m_pCoregame->GetPlayer();
  m_pCoregame->GetZombieManager()->SetActiveZombies(false);
  //Triggers
	//m_pCore->GetTriggerManager()->Init();

	//Items
	//m_pCore->GetItemManager()->AddItem(Vect3f(17.5f,1.37f,13.6f));

  //espiritu asigna emisor particulas
  if(m_ucCurrentLevel == 1)
  	m_pCoregame->GetSpirit()->SetParticleEmitter("pHitodama1", "pHitodama2");

 //Sonido
  m_pCore->GetSoundManager()->SetListener(m_pCore->GetCameraManager()->GetCamera()->GetObject3D());
  m_pCore->GetSoundManager()->CrossFade("game_music", 3000);

  Register2LUA();

  // iniciar animacion de la camara.
  ((CThPSCamera*)m_pCore->GetCameraManager()->GetCamera())->SetZoom(1.f);
  m_pCameraKeyController = new CCameraKeyController("Data/CameraPath.xml", m_pCore->GetCameraManager()->GetCamera());

  m_ucGameState = GS_CAMERA_CINEMATIC;
  m_pCore->GetTimer()->ResetTimer();
}

///<summary>
/// CVideoGame:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CVideoGame::RenderScene(CRenderManager* rm) 
{
  Render3D(rm);
  Render2D(rm); 
  //RenderDebugInfo(); 
}

///<summary>
/// CVideoGame:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CVideoGame::Update(float elapsedTime)
{
  switch (m_ucGameState)
  {
    case GS_CAMERA_CINEMATIC:
    {
      m_pCameraKeyController->Update(elapsedTime);

      if (m_pCameraKeyController->Finished())
      {
        CThPSCamera* l_pCamera = (CThPSCamera*)m_pCore->GetCameraManager()->GetCamera();
        l_pCamera->SetZoom(7.0f);
        l_pCamera->SetZn(0.1f);
        l_pCamera->SetZf(300.0f);
        l_pCamera->GetObject3D()->SetPitch(-0.39f);
        
        m_ucGameState = GS_LEVEL_UPDATE;
      }
      break;
    }
    
    case GS_LEVEL_UPDATE:
    {
      CActionToInput * l_Action = m_pCore->GetInputManager()->GetActionToInput();
      CheckActionsFromInputs(elapsedTime);

      if ((m_DeltaFromXML = l_Action->DoAction("ReloadRenderableObjects")) != 0.0f )
      {
        m_pCore->GetRenderableObjectsManager()->Reload();
      }

      m_pPlayer->Update(elapsedTime);

      // updates especificos de cada nivel
      switch (m_ucCurrentLevel)
      {
        case 1:
        {
          int l_iTextActual = m_pCore->GetBillboardManager()->GetBAnimationByInstance("pTormenta")->GetTextureAct();
          if ((l_iTextActual == 70 || l_iTextActual == 177) && !m_bSonando)
          {
	          m_pCore->GetSoundManager()->PlaySound("thunder");
	          m_bSonando = true;
          }
          if (l_iTextActual == 71 || l_iTextActual == 178)
          {
	          m_bSonando = false;
          }

          //Items
          m_pCore->GetItemManager()->Update(elapsedTime);

          if(m_pCore->GetItemManager()!=NULL)
          {
	          m_pCore->GetItemManager()->CheckColisions();
          }
          
	        //Animación puerta palanca
	        if (!m_bEndAnimation)
	        {
		        CBillboardManager * m_pBillboardM = m_pCore->GetBillboardManager();
		        int l_iTextAct = 	m_pBillboardM->GetBAnimationByInstance("pPuerta")->GetTextureAct();
		        bool l_bActive = m_pBillboardM->GetActive();
		        if (!l_bActive && l_iTextAct > 28)
		        {
			        std::string l_stBox = "PuertaPalanca";
			        Vect3f l_v3Pos = Vect3f(0.0f, -10.0f, 0.0f);
			        CORE->GetPhysicsManager()->SetInvisbleBoxPosition(l_stBox,l_v3Pos);
			        m_bEndAnimation = true;
		        }
        	}

          //spirit
          if ((m_pCoregame->GetSpirit() != NULL) && (m_ucCurrentLevel == 1))//lo quito despues de las pruebas del boss, sino me peta
            m_pCoregame->GetSpirit()->Update(elapsedTime);
        }
        break;

        case 2:
        {

					//m_pCore->GetSoundManager()->CrossFade("level3", 5);


          COmniLight* l_LightLightning = (COmniLight*)m_pCore->GetLigthManager()->GetResource("relampago");
          CLightEffect2* l_LightningEffect = (CLightEffect2*)l_LightLightning->GetEffect();
          int l_wLightningCurrentKeyframe = l_LightningEffect->GetCurrentKeyframe();
          if (l_wLightningCurrentKeyframe == 2 && !m_bSonando)
          {
	          m_pCore->GetSoundManager()->PlaySoundVolume("thunder", 0.2f);
	          m_bSonando = true;
          }
          if (l_wLightningCurrentKeyframe == 3)
          {
	          m_bSonando = false;
          }
        }
        break;
      }

      UpdateCamera();

      //m_pCore->GetCameraManager()->GetCamera()->InitShake();
      m_pCore->GetCameraManager()->GetCamera()->CameraShake(elapsedTime);

      //IA4
	    m_pCoregame->GetZombieManager()->Update(elapsedTime);

      // Sound
      Vect3f l_PlayerPos = m_pPlayer->GetPhysicController()->GetPosition();
      m_pCore->GetSoundManager()->UpdateSound(l_PlayerPos);
 
	    break;
    }
    
    case GS_PREPARE_CHANGE_LEVEL:
    {
      CheckActionsFromInputs(elapsedTime);
      
      if (m_fTimer < 3.0f)
      {
        m_fTimer += elapsedTime;
      }
      else
      {
        if (m_ucChangeLevel != G_GAME_OVER)
        {
          std::string l_sLoadWindowName;
          baseUtils::FormatSrting(l_sLoadWindowName, "Loading%d.xml", m_ucChangeLevel);
      
          m_pCore->GetGUIManager()->ActiveWindowsWithEffect(l_sLoadWindowName, TE_FADE_TO_BLACK, 2.0f);
          m_pCore->GetSoundManager()->PlaySound("level2");

          m_ucGameState = GS_CHANGE_LEVEL;
        }
        else
        {
          
          m_pCore->GetGUIManager()->ActiveWindowsWithEffect("End.xml", TE_SHADOW, 5.0f);
          m_ucGameState = GS_GAME_OVER_TRANSITION;
        }
      }

      break;
    }
    
    case GS_CHANGE_LEVEL:
    {
      CheckActionsFromInputs(elapsedTime);
      
      std::string l_sLoadWindowName;
      baseUtils::FormatSrting(l_sLoadWindowName, "Loading%d.xml", m_ucChangeLevel);
      
      if (m_pCore->GetGUIManager()->GetCurrentWindow() == l_sLoadWindowName)
      {
        LoadLevel(m_ucChangeLevel);
      }

      break;
    }
    
    case GS_GAME_OVER_TRANSITION:
    {
      if (m_pCore->GetGUIManager()->GetCurrentWindow() != "End.xml")
      {
        CheckActionsFromInputs(elapsedTime);
      }
      else
      {
        m_ucGameState = GS_GAME_OVER;
      }

      break;
    }
    
    case GS_GAME_OVER:
    {
      break;
    }
    
    case GS_LEVEL_SELECTED:
    {
      std::string l_sLoadWindowName;
      baseUtils::FormatSrting(l_sLoadWindowName, "Loading%d.xml", m_ucChangeLevel);
      
      if (m_pCore->GetGUIManager()->GetCurrentWindow() == l_sLoadWindowName)
      {
        LoadLevel(m_ucChangeLevel);
      }

      break;
    }
  }

  m_pCore->GetRenderableObjectsManager()->Update(elapsedTime);
  
  //Partículas
  m_pCore->GetParticleManager()->Update(elapsedTime);

	//Billboard animado
	m_pCore->GetBillboardManager()->Update(elapsedTime);
}

///<summary>
/// CVideoGame:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CVideoGame::Render(CRenderManager* rm) 
{
	////IA

	////eje principal
 // Mat44f matWorld;
 // matWorld.SetIdentity();
 // rm->SetTransform(matWorld);
 // rm->DrawAxis(5);


  CSceneEffectManager *l_pSceneEffectManager = m_pCore->GetSceneEffectManager();
  // Andres: esta linea la comento para no usar efectos de escena
  //l_pSceneEffectManager->ActivateRenderSceneEffects();

  CEffectManager *l_EffectManager=m_pCore->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);

  //Renderizamos la escena
  RenderScene(rm);

  l_pSceneEffectManager->CaptureFrameBuffers(rm);
  l_pSceneEffectManager->PostRender(rm);
  l_pSceneEffectManager->CaptureFrameBuffersAfterPostRender(rm);

	//Triggers
//	m_pCore->GetTriggerManager()->Render();

  //RenderScene(rm);

  //Física
  if(m_bDebugRender)
  {
    CPhysicsManager* l_PhysManager = m_pCore->GetPhysicsManager();
    l_PhysManager->DebugRender(rm);
  }
}


//---------------------------------------------------------------------------------//
//-------------------FUNCIONES PROPIAS DE GAMEPROCESS_TEST-------------------------//
//---------------------------------------------------------------------------------//

///<summary>
/// CVideoGame:: LoadCommonXMLs : Carga los ficheros XML comunes a todo el juego.
///</summary>
void CVideoGame::LoadCommonXMLs()
{
  m_pCore->GetEffectManager()->Load("Data/Effects.xml");
  m_pCore->GetSceneEffectManager()->LoadXML("Data/scene_effects.xml");
  if (!m_pCore->GetSoundManager()->IsOk())
    m_pCore->GetSoundManager()->Init("Data/Sounds.xml");
}

///<summary>
/// CVideoGame:: LoadLevelXMLs : Carga los XML necesarios segun el nivel.
///</summary>
///<param name="_level"></param>
///<returns name=""></returns>
void CVideoGame::LoadLevelXMLs(uint8 _level)
{
  switch (_level)
  {
    case 1:
    {
      m_pCore->GetStaticMeshManager()->Load("Data/Level1/StaticMeshManager.xml");
      m_pCore->GetLigthManager()->Load("Data/Level1/lights.xml");
      m_pCore->GetAnimatedModelManager()->Load("Data/Level1/AnimatedModels.xml");
      m_pCore->GetRenderableObjectsManager()->Load("Data/Level1/RenderableObjects.xml");
      m_pCore->GetPhysicsManager()->LoadInvisibleBlockingBoxes("Data/Level1/InvisibleBoxes.xml");
      m_pCore->GetBillboardManager()->LoadXML("Data/Level1/ParticlesAnimation.xml");
      m_pCore->GetParticleManager()->LoadXML("Data/Level1/Particles.xml");
      m_pCore->GetTriggerManager()->Load("Data/Level1/Triggers.xml");

      m_pCoregame->GetZombieManager()->Load("Data/Level1/ZombieLoader.xml");
      m_pCoregame->GetSpirit()->LoadXML("Data/Level1/spirit.xml"); 
    }
    break;

    case 2:
    {
      m_pCore->GetStaticMeshManager()->Load("Data/Level2/StaticMeshManager.xml");
      m_pCore->GetLigthManager()->Load("Data/Level2/lights.xml");
      m_pCore->GetAnimatedModelManager()->Load("Data/Level2/AnimatedModels.xml");
      m_pCore->GetRenderableObjectsManager()->Load("Data/Level2/RenderableObjects.xml");
    //  l_pCore->GetPhysicsManager()->LoadInvisibleBlockingBoxes("Data/Level1/InvisibleBoxes.xml");
 	    //if(m_pCore->GetBillboardManager())
   	  //  m_pCore->GetBillboardManager()->LoadXML("Data/Level1/ParticlesAnimation.xml");
      m_pCoregame->GetZombieManager()->Load("Data/Level2/ZombieLoader.xml");
    }
    break;
  }
//  LoadGameConfig();
}

///<summary>
/// CVideoGame:: LoadGameConfig : Carga de un XML los parametros de configuracion del juego.
///</summary>
///<param>Puntero a render manager.</param>
///<returns>Bool isOK</returns>
bool CVideoGame::LoadGameConfig()
{
  CXMLTreeNode l_FileXML;
  std::string l_sFileName = "Data/gameConfig.xml";

  if (!l_FileXML.LoadFile(l_sFileName.c_str()))
  {
    // Guarda mensaje de error en el log
		std::string msg_error = "CPlayer::Init->Error al intentar abrir el archivo: ";
		msg_error.append(l_sFileName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
  }
  else
  {
    CXMLTreeNode l_Params = l_FileXML["camera"];
    if (l_Params.Exists())
    {
      m_fYawFactor        = l_Params.GetFloatProperty("yawFactor", 0.18f);
      m_fPitchFactor      = l_Params.GetFloatProperty("pitchFactor", 0.18f);
      m_fPitchUpperLimit  = l_Params.GetFloatProperty("pitchUpperLimit", 0.5f);
      m_fPitchLowerLimit  = l_Params.GetFloatProperty("pitchLowerLimit", -1.5f);

      return true;
    }
  }
  return false;
}

///<summary>
/// CVideoGame:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CVideoGame::Render3D(CRenderManager* rm)
{
  // render manager
  CRenderableObjectsManager *renderObjectsManager = m_pCore->GetRenderableObjectsManager();
  renderObjectsManager->Render(rm);
	//IA
//	m_pCoregame->GetZombieManager()->Render(rm);

  // luces
  //m_pCore->GetLigthManager()->Render(rm);
  //spirit
  //m_pCoregame->GetSpirit()->Render(rm);
  // player
  //m_pPlayer->Render();
}

///<summary>
/// CVideoGame:: Render3D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CVideoGame::Render2D(CRenderManager* rm)
{
  Mat44f l_Matrix;
  l_Matrix.SetIdentity();
  rm->SetTransform(l_Matrix);

  //Partículas
  m_pCore->GetParticleManager()->Render();

	//Billboard Animado
	m_pCore->GetBillboardManager()->Render();
}

///<summary>
/// CVideoGame:: PreRender() : Pre-Render de la escena.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CVideoGame::PreRender(CRenderManager* rm) 
{
  m_pCore->GetSceneEffectManager()->PreRender(rm,this);
}

///<summary>
/// CVideoGame:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CVideoGame::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();
/*
  CFontManager* fm = m_pCore->GetFontManager();
  uint32 l_iFontId = fm->GetTTF_Id("arial");
  CTimer *t = m_pCore->GetTimer();

/*  CStaticMeshManager* sm= m_pCore->GetStaticMeshManager();
  fm->DrawTextA(10,135,colYELLOW,l_iFontId,"Num of Faces (Static Meshes): %ld",sm->GetTotalObjectsFaces());
  CAnimatedModelManager* am=m_pCore->GetAnimatedModelManager();
  fm->DrawTextA(10,155,colYELLOW,l_iFontId,"Num of Faces (Dinamic Meshes): %ld",am->GetTotalNumFaces());
 
  CCamera * l_ActualCamera = m_pCore->GetCameraManager()->GetCamera();
  float l_fCameraYaw = l_ActualCamera->GetObject3D()->GetYaw() - m_DeltaFromXML*0.005f;
  fm->DrawTextA(10, 215, colCYAN, l_iFontId, "camera Yaw: %f", l_fCameraYaw);

  float l_fPlayerYaw = m_pPlayer->GetInstance()->GetYaw();
  fm->DrawTextA(10, 245, colCYAN, l_iFontId, "Player Yaw: %f", l_fPlayerYaw);
  
  fm->DrawTextA(10, 275, colCYAN, l_iFontId, "Player X: %f Y: %f Z: %f", m_pPlayer->GetInstance()->GetPosition().x,m_pPlayer->GetInstance()->GetPosition().y, m_pPlayer->GetInstance()->GetPosition().z);
  Vect3f l_V3fPlayerPos = m_pPlayer->GetPhysicController()->GetPosition();
  fm->DrawTextA(10, 295, colCYAN, l_iFontId, "Player Pos: %f, %f, %f", l_V3fPlayerPos.x, l_V3fPlayerPos.y, l_V3fPlayerPos.z);
*/
/*  Vect3f l_V3fBossInstancePos = m_pCoregame->GetZombieManager()->GetZombieBoss()->GetInstance()->GetPosition();
  fm->DrawTextA(10, 595, colCYAN, l_iFontId, "boss Pos: %f, %f, %f", l_V3fBossInstancePos.x, l_V3fBossInstancePos.y, l_V3fBossInstancePos.z);
*/
}

///<summary>
/// CVideoGame:: CheckActionsFromInputs : Según una entrada de teclado ejecuta su acción asociada.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CVideoGame::CheckActionsFromInputs(float elapsedTime)
{
  CActionToInput * l_Action = m_pCore->GetInputManager()->GetActionToInput();

  #if defined( _DEBUG )

  if((m_DeltaFromXML = l_Action->DoAction("ToogleDebugRender")) != 0.0f )
  {
    m_bDebugRender = !m_bDebugRender;
    m_pCore->GetPhysicsManager()->SetDebugRenderMode(m_bDebugRender);
  }
  
  if((m_DeltaFromXML = l_Action->DoAction("ProcessInfo")) != 0.0f )
  {
    RenderDebugInfo(); //Esto solo se dibuja en tiempo de renderingScene
  }
  
  if((m_DeltaFromXML = l_Action->DoAction("ReloadLua")) != 0.0f )
  {
    LoadLuaScripts();
  }

    if((m_DeltaFromXML = l_Action->DoAction("ReloadEffects")) != 0.0f )
    {
      CEffectManager* ef=m_pCore->GetEffectManager();
      ef->Reload();
      CStaticMeshManager *sm=m_pCore->GetStaticMeshManager();
      sm->Reload();
      CRenderableObjectsManager *ro = m_pCore->GetRenderableObjectsManager();
      ro->Reload();
      //falta parametros de effects.xml
      CSceneEffectManager *sem = m_pCore->GetSceneEffectManager();
      //sem->Destroy();
      sem->LoadXML("Data/scene_effects.xml");
    }

    if((m_DeltaFromXML = l_Action->DoAction("ReloadInvisibleBoxes")) != 0.0f )
    {
      m_pCore->GetPhysicsManager()->ReloadInvisibleBlockingBoxes();
    }
    /*
    if((m_DeltaFromXML = l_Action->DoAction("ReloadTriggers")) != 0.0f )
    {
      m_pCore->GetTriggerManager()->Reload();
    }
    */

  #endif // defined( _DEBUG )

// ACCIONES DE DEBUG HABILITADAS TEMPORALMENTE TAMBIEN EN RELEASE
  

  if((m_DeltaFromXML = l_Action->DoAction("ReloadPlayer")) != 0.0f )
  {
    m_pPlayer->Reload();
  }

  if((m_DeltaFromXML = l_Action->DoAction("ReloadLights")) != 0.0f )
  {
    m_pCore->GetLigthManager()->Reload();
  }

  if((m_DeltaFromXML = l_Action->DoAction("getKey")) != 0.0f )
  {
    m_pCore->GetScriptManager()->RunCode("llave_in()");
  }

  if((m_DeltaFromXML = l_Action->DoAction("GameOver")) != 0.0f )
  {
    //ChangeLevel(2);
    //LoadLevel(2);
    m_pCore->GetGUIManager()->ActiveWindows("End.xml");
    m_ucGameState = GS_GAME_OVER;
  }

// END - ACCIONES DE DEBUG - 


  CCamera * l_ActualCamera = m_pCore->GetCameraManager()->GetCamera();
  
  if(!l_ActualCamera->GetShake())
  {
    //YawViewerCam
    if((m_DeltaFromXML = l_Action->DoAction("YawViewerCam")) != 0.0f )
    {
		  float l_fCameraYaw = l_ActualCamera->GetObject3D()->GetYaw() - m_DeltaFromXML * elapsedTime * m_fYawFactor; //*0.005f;

      // restringiendo el angulo de Yaw de la camara para que siempre esté entre -PI y PI
      while (l_fCameraYaw > FLOAT_PI_VALUE)
        l_fCameraYaw -= FLOAT_2PI_VALUE;
      while (l_fCameraYaw < -FLOAT_PI_VALUE)
        l_fCameraYaw += FLOAT_2PI_VALUE;

      l_ActualCamera->GetObject3D()->SetYaw(l_fCameraYaw);
    }
    //PitchViewerCam
    if((m_DeltaFromXML = l_Action->DoAction("PitchViewerCam")) != 0.0f )
    {
      float l_fPitch = l_ActualCamera->GetObject3D()->GetPitch();

      l_fPitch -= m_DeltaFromXML  * elapsedTime * m_fPitchFactor; //* 0.005f;

      // estableciendo un limite para el angulo
      if (l_fPitch > m_fPitchLowerLimit && l_fPitch < m_fPitchUpperLimit)
        l_ActualCamera->GetObject3D()->SetPitch(l_fPitch);
      else
      {
        if (l_fPitch < m_fPitchLowerLimit)
          l_ActualCamera->GetObject3D()->SetPitch(m_fPitchLowerLimit);
        else if (l_fPitch > m_fPitchUpperLimit)
          l_ActualCamera->GetObject3D()->SetPitch(m_fPitchUpperLimit);
      }
    }
    //SlowZoomViewerCam
    if((m_DeltaFromXML = l_Action->DoAction("SlowZoomViewerCam")) != 0.0f )
    {
      CThPSCamera * cam = (CThPSCamera*)m_pCore->GetCameraManager()->GetCamera();
      cam->AddZoom(m_DeltaFromXML * -0.005f);
    }
    else 
    { //ZoomViewerCam
      if((m_DeltaFromXML = l_Action->DoAction("ZoomViewerCam")) != 0.0f )
      {
        CThPSCamera * cam = (CThPSCamera*)m_pCore->GetCameraManager()->GetCamera();
        cam->AddZoom(m_DeltaFromXML * -0.01f);
      }
    }
  }
}

///<summary>
/// CVideoGame::LoadLuaScripts : Carga y ejecuta todos los scripts LUA del juego
///</summary>
void CVideoGame::LoadLuaScripts()
{
    m_pCore->GetScriptManager()->RunFile("Data/LUAScripts/variables.lua");
	  m_pCore->GetScriptManager()->RunFile("Data/LUAScripts/init.lua");
	  m_pCore->GetScriptManager()->RunFile("Data/LUAScripts/zombis.lua");
	  m_pCore->GetScriptManager()->RunFile("Data/LUAScripts/triggers.lua");
	  m_pCore->GetScriptManager()->RunFile("Data/LUAScripts/zombi_boss.lua");
}

void CVideoGame::UpdateCamera()
{
  //PRUEBA CON RAYOS
  CThPSCamera * l_ActiveCamera = (CThPSCamera*) m_pCore->GetCameraManager()->GetCamera();
  Vect3f l_ActualPosition = l_ActiveCamera->GetEye();
  CPhysicsManager * l_PhysXManager = m_pCore->GetPhysicsManager();
  std::vector<CPhysicUserData*> l_ImpactObjects;
  Vect3f pos = l_ActiveCamera->GetLookAt();
  Vect3f& dir = l_ActiveCamera->GetDirection();
  dir = dir * -1.f;
  dir.Normalize();
  SCollisionInfo info;
  m_ClosestActorCamera = l_PhysXManager->RaycastClosestActor(pos,dir,PX_MSK_ALL_SCENARIO,m_ClosestActorCamera,info);
  if(m_ClosestActorCamera != NULL)
  {
    float l_CollisionDistance = info.m_fDistance;
    if(l_CollisionDistance < CAMERA_ZOOM)
    {
      l_CollisionDistance = l_CollisionDistance - 0.1f;
      l_ActiveCamera->SetZoom(l_CollisionDistance);
      //TODO: Alpha del personaje de acuerdo a la distancia de la camara.
    }
  }
  else
  {
    l_ActiveCamera->SetZoom(4.0f);
  }
  Vect3f l_CameraPosition = m_pPlayer->GetPhysicController()->GetPosition();
  l_CameraPosition.y += 0.6f;
  l_ActiveCamera->GetObject3D()->SetPosition(l_CameraPosition);

  //FIN PRUEBA CON RAYOS

  //PRUEBA CON COLISIONES
  //CThPSCamera * l_ActiveCamera = (CThPSCamera*) m_pCore->GetCameraManager()->GetCamera();
  //Vect3f l_ActualPosition = l_ActiveCamera->GetEye();
  //CPhysicsManager * l_PhysXManager = m_pCore->GetPhysicsManager();
  //std::vector<CPhysicUserData*> l_ImpactObjects;
  //l_PhysXManager->OverlapSphereActor(0.01f,l_ActualPosition, l_ImpactObjects);

  //if(!l_ImpactObjects.empty())
  //{
  //  if(l_ActiveCamera->GetZoom()>2.0f)
  //  {
  //    l_ActiveCamera->AddZoom(-0.2f);
  //  }
  //  else
  //  {
  //    //Mueva arriba a la derecha si la distancia minima es de 2mts
  //    l_ActiveCamera->GetObject3D()->SetYaw(l_ActiveCamera->GetObject3D()->GetYaw()+(FLOAT_PI_VALUE/360));
  //    l_ActiveCamera->GetObject3D()->SetPitch(l_ActiveCamera->GetObject3D()->GetPitch()-(FLOAT_PI_VALUE/360));
  //  }
  //}
  //else
  //{
  //  if(l_ActiveCamera->GetZoom()<=8.0f)
  //  {
  //    l_ActiveCamera->AddZoom(0.05f);
  //  }
  //}
  //Vect3f l_CameraPosition = m_pPlayer->GetPhysicController()->GetPosition();
  //l_CameraPosition.y += 0.6f;
  //l_ActiveCamera->GetObject3D()->SetPosition(l_CameraPosition);
  //FIN PRUEBA CON COLISIONES
}

///<summary>
/// CVideoGame::ReleaseLevel : Libera memoria de los managers para poder cargr un nuevo nivel.
///</summary>
void CVideoGame::ReleaseLevel()
{
  m_pCoregame->ReleaseSpirit();
  m_pCoregame->GetZombieManager()->Done();
    
      // CHECKED_DELETE(m_pRenderManager);
      // CHECKED_DELETE(m_pFontManager);
      // CHECKED_DELETE(m_pLanguageManager);
      // CHECKED_DELETE(m_pInputManager);
  m_pCore->GetStaticMeshManager()->Destroy();
  m_pCore->GetRenderableObjectsManager()->CleanUp();
  m_pCore->GetLigthManager()->Done(); // CHECKED_DELETE(m_pLightManager);
  m_pCore->GetAnimatedModelManager()->Destroy();
      // CHECKED_DELETE(m_pEffectManager);
      // CHECKED_DELETE(m_pTimer);
  // CHECKED_DELETE(m_pScriptManager);
      //CHECKED_DELETE(m_pCameraManager);
  m_pCore->GetGUIManager()->Done();
  m_pCore->GetPhysicsManager()->Done();
  m_pCore->GetTextureManager()->Destroy();
  // CHECKED_DELETE(m_pSceneEffectManager);
  m_pCore->GetParticleManager()->Done();
  //    m_pCore->GetSoundManager()->Done();
  m_pCore->GetRenderablePhysXManager()->Destroy();
  m_pCore->GetTriggerManager()->Done();
  m_pCore->GetItemManager()->Done();
  m_pCore->GetBillboardManager()->Done();
}

///<summary>
/// CVideoGame::LoadLevel : Carga en memoria y establece los parametros necesarios segun cada nivel.
///</summary>
void CVideoGame::LoadLevel(uint8 _level)
{
  if (_level != m_ucCurrentLevel)
  {
    ReleaseLevel();

    m_pCore->GetPhysicsManager()->Init();
    m_pCoregame->GetZombieManager()->Init();

    //espiritu init
    if (_level == 1)
    {
      m_pCoregame->InitSpirit();
    }
    
    LoadLevelXMLs(_level);

    //    m_pCoregame->GetZombieManager()->RegistroAnimacionesCallback();
  

    //Partículas
    //m_pCore->GetParticleManager()->Init(rm);

	  //BillboardAnimado
	  //m_pCore->GetBillboardManager()->Init(rm);

    switch (_level)
    {
      case 1:
      {
        m_pCoregame->GetZombieManager()->RegistroAnimacionesCallback();
        //Partículas
        //m_pCore->GetParticleManager()->Init("");
        m_pCore->GetParticleManager()->Init(m_pCore->GetRenderManager());
        // Animated Billboard
        //m_pCore->GetBillboardManager()->Init("");
        m_pCore->GetBillboardManager()->Init(m_pCore->GetRenderManager());
        
        m_pCoregame->GetZombieManager()->SetActiveZombies(false);
        
        m_pCoregame->GetSpirit()->SetParticleEmitter("pHitodama1", "pHitodama2");
        
        // iniciar animacion de la camara.
        ((CThPSCamera*)m_pCore->GetCameraManager()->GetCamera())->SetZoom(1.f);
        m_pCameraKeyController->ResetTime();// = new CCameraKeyController("Data/CameraPath.xml", m_pCore->GetCameraManager()->GetCamera());
        
        m_ucGameState = GS_CAMERA_CINEMATIC;

        break;
      }

      case 2:
      {
        CObject3D * l_pCameraObject = m_pCore->GetCameraManager()->GetCamera()->GetObject3D();
        l_pCameraObject->SetYaw(0.0f);
        l_pCameraObject->SetPitch(-0.21f);
        l_pCameraObject->SetRoll(0.0f);

        m_ucGameState = GS_LEVEL_UPDATE;

        break;
      }
    }

    m_pCore->GetGUIManager()->Init("Data/GUI/inGameGUI.xml");

    //IA4
	  m_pCore->GetTriggerManager()->RegistraTriggers();

    //:Player
    m_pPlayer->InitCamera();
    m_pPlayer->AssignLights();
    m_pPlayer->AssingAnimation();
    m_pPlayer->InicializaController();
    m_pCoregame->GetHealthIndicator()->AssignGUIElements(); // reestablece las texturas del indicador de salud

    m_ucCurrentLevel = _level;
  }
  else
  {
    switch (m_ucCurrentLevel)
    {
      case 1:
      {
        m_ucGameState = GS_CAMERA_CINEMATIC;
        break;
      }

      case 2:
      {
        m_ucGameState = GS_LEVEL_UPDATE;
        m_pCore->GetScriptManager()->RunCode("revive_boss()");
        break;
      }
    }
  }

  m_pCore->GetSoundManager()->CrossFade("game_music", 1000);
  m_pCore->GetGUIManager()->ActiveWindowsWithEffect("Main.xml", TE_SHADOW_OFF, 2.0f);
  m_pCore->GetGUIManager()->SetVisiblePointerMouse(false);
  m_pPlayer->ReStart();
  /*m_pCoregame->InitPlayer("Data/Player.xml");
    m_pPlayer = m_pCoregame->GetPlayer();
    m_pCoregame->GetZombieManager()->SetActiveZombies(false);*/
   

    //Triggers
//	  m_pCore->GetTriggerManager()->Init();

	  //Items
	  //m_pCore->GetItemManager()->AddItem(Vect3f(17.5f,1.37f,13.6f));

    //espiritu asigna emisor particulas
	  //m_pCoregame->GetSpirit()->SetParticleEmitter("pHitodama1", "pHitodama2");

   //Sonido
    //m_pCore->GetSoundManager()->SetListener(m_pCore->GetCameraManager()->GetCamera()->GetObject3D());
    
//  LoadLuaScripts();
  
  m_ucChangeLevel = 0;
  m_pCore->GetTimer()->ResetTimer();
}

///<summary>
/// CVideoGame::LoadLevel : Establece lso parametros previos a la carga de un nivel (getión de estados, sonido y GUI).
///</summary>
void CVideoGame::ChangeLevel(uint8 _level)
{
  if (_level != m_ucCurrentLevel)
  {
    if (m_ucGameState == GS_GAME_OVER)
    {
      std::string l_sLoadWindowName;
      baseUtils::FormatSrting(l_sLoadWindowName, "Loading%d.xml", _level);
      m_pCore->GetGUIManager()->ActiveWindows(l_sLoadWindowName);
      m_pCore->GetGUIManager()->SetVisiblePointerMouse(false);
      m_ucGameState = GS_LEVEL_SELECTED;
      m_ucChangeLevel = _level;
    }
    else
    {
      m_fTimer = 0.0f;
      m_pPlayer->SetSate(-1);
      m_pPlayer->Update(0.f);
      m_ucChangeLevel = _level;
      m_ucGameState = GS_PREPARE_CHANGE_LEVEL;
    }
  }
  else
  {
    LoadLevel(_level);
  }
}

///<summary>
/// CVideoGame::LoadLevel : Prepara parametros para cambiar de estado y poder mostrar al ventana de GameOVer de seleccion de nivel.
///</summary>
void CVideoGame::GameOver()
{
  m_fTimer = 0.0f;
  m_pPlayer->SetSate(-1);
  m_pPlayer->Update(0.f);
  m_ucChangeLevel = G_GAME_OVER;
  m_ucGameState = GS_PREPARE_CHANGE_LEVEL;
}

///<summary>
/// CVideoGame::ReStart : Reestablece el player a su estado inicial. En el nivel2 también al boss.
///</summary>
void CVideoGame::ReStart()
{
  switch (m_ucCurrentLevel)
  {
    case 1:
    {
      break;
    }

    case 2:
    {
      m_pCore->GetScriptManager()->RunCode("revive_boss()");
      break;
    }
  }
  m_pPlayer->ReStart();
}

void CVideoGame::Register2LUA()
{
  lua_State * l_LS = m_pCore->GetScriptManager()->GetLuaState();
	using namespace luabind;
  
  // Videogame ---------------------------------------------
  module(l_LS)
    [
      // registramos la clase CVideoGame
      class_<CVideoGame>("CVideoGame")
      // registramos su constructor
      .def(constructor<>())
      .def("change_level", &CVideoGame::ChangeLevel)
      .def("load_level", &CVideoGame::LoadLevel)
      .def("game_over", &CVideoGame::GameOver)
      .def("re_start", &CVideoGame::ReStart)
    ];

  globals(l_LS)["g_videogame"] = this;
}

