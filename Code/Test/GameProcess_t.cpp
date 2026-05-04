#include "Core.h"
#include "CoreFIH.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
#include "GameProcess_t.h"
#include "Math/Matrix44.h"
#include "Math/MathUtils.h"
#include "Math/MathTypes.h"
#include "Cameras/FPSCamera.h"
#include "Cameras/ThPSCamera.h"
#include "Object3D/Object3D.h"
#include "Object3D/BoundingBox.h"
#include "Texture/Texture.h"
#include "Vertex/IndexedVertex.h"
#include "Vertex/VertexType.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "Lights/Light.h"
#include "Lights/LightManager.h"
#include "ActionToInput.h"
#include "Shaders/EffectManager.h"
#include "Shaders/SceneEffectManager.h"
#include "RenderableObjects/RenderableObject.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "AnimationModel/AnimatedCoreModel.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "texture/Texture.h"
#include "texture/TextureManager.h"
#include "ScriptManager.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraManager.h"
#include "GUIManager.h"
#include "Actor/PhysicActor.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "Cameras/Camera.h"
#include "Shaders/SceneEffectManager.h"
#include "Timer/Timer.h"
#include "Utils/LogRender.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/Particle.h"
#include "Particles/ParticleManager.h"
#include "PhysicsManager.h"
#include "RenderablePhysX/RenderablePhysXManager.h"
#include "RenderablePhysX/RenderablePhysXObject.h"
#include "CharacterController/PhysicController.h"
#include "Joints/Fixed/PhysicFixedJoint.h"
#include "SoundManager.h"
#include "Trigger/TriggerManager.h"
#include "Trigger/Trigger.h"
#include "../Videogame/ZombieManager/ZombieManager.h"
#include "IA.h"
#include "Player/Player.h"
#include "ItemManager.h"

#include <luabind/luabind.hpp>


// includes para pruebas... no deben ir en el proceso del videojuego
#include "Player/PlayerAnimData.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

using namespace luabind;

CPhysicUserData * g_ClosestActorPX = 0;
CPhysicUserData * g_ClosestActor = 0;
//std::string l_InstanceName = "Box01";
//CPhysicController * g_PhysXController = 0;
//CPhysicUserData * g_UserDataController;
CPhysicFixedJoint* g_Joint;
std::string g_CollitionObject = "";

//CZombieManager* g_ZombieManagerGP = NULL;
//C_IA* g_AgenteIAGP = NULL;


CGameProcess_t::CGameProcess_t()
{
  m_fPosX = 0;
  m_fPosY = 100;
  m_TimerCounter = 0;
  m_iDirection = 100; 
  m_fFPS = 0.f;
  m_fRotacion = 0.f;
  m_bDrawCube = true;
  m_MaxVision = 42.0f;
  m_MinVision = 42.0f;
  m_MovementSpeed = 0.04f;//TODO: La velocidad de caminata normal de una persona es de 0,92 mts por segundo. Estamos moviendonos a 4. Hay que cambiarlo
  m_RunTime = 2.0f;
  m_RestTime = 3.0f;
  m_DeltaFromXML = 0.0;
  m_ray = "";
  m_class = "";
  m_CheckActions = 0.0f;
  m_MoveMouse = 0.0f;
  m_UpdateLua = 0.0f;
  m_MovPlayer = 0.0f;
  mParticles = 0.0f;
  m_PhysX = 0.0f;
  m_IA = 0.0f;

  
	m_pCoregame = 0;
  m_pPlayer = 0;
}

CGameProcess_t::~CGameProcess_t()
{
  //CHECKED_DELETE(g_PhysXController)
  //CHECKED_DELETE(g_UserDataController)
  CHECKED_DELETE(g_Joint)
  //CHECKED_DELETE(g_ZombieManagerGP)
  //CHECKED_DELETE(g_AgenteIAGP)
  CHECKED_DELETE(m_pCoregame)
}

///<summary>
/// CGameProcess_t:: Init : Lectura de los XML. Creación y asignación de las cámaras a los objetos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGameProcess_t::Init ()
{
  CORE->GetPhysicsManager()->SetDebugRenderMode(true);
  //g_ZombieManagerGP = new CZombieManager();

  //g_AgenteIAGP = new C_IA();
  //g_AgenteIAGP->CargaGrafo("./data/paths.dat");  
  //  g_AgenteIAGP->Dijsktra("\"8\"","\"6\"");

  CORE->GetScriptManager()->RunFile("./Data/LUAScripts/variables.lua");
  CORE->GetScriptManager()->RunFile("./Data/LUAScripts/init.lua");
  CORE->GetScriptManager()->RunFile("./Data/LUAScripts/zombis.lua");
  
  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
  
  CORE->GetGUIManager()->Init("Data/GUI/initGUI.xml");
  
  m_pCoregame = new CCoreFIH();
  m_pCoregame->Init();
  
  LoadXMLs(); 
  
  
  //Partículas
  CORE->GetParticleManager()->Init(CORE->GetRenderManager());
  
  
  InitCameras();
  /*
  //CORE->GetGUIManager()->ActiveWindows("Game.xml");
  //CORE->GetGUIManager()->SetVisiblePointerMouse(true);

  //Partículas
  CORE->GetParticleManager()->Init(CORE->GetRenderManager());

  //Controller
  /*g_UserDataController = new CPhysicUserData("Controller");
  g_UserDataController->SetPaint(true);
  g_UserDataController->SetColor(colWHITE);*/

  //g_PhysXController = new CPhysicController(0.8f,1.8f,60.0f,0.1f,2.0f,1,g_UserDataController,Vect3f(20.0f,2.7f,0.0f));
  //g_PhysXController->SetYaw(-90);
  //g_PhysXController->SetYaw(-FLOAT_PI_VALUE/4);
  //CORE->GetPhysicsManager()->AddPhysicController(g_PhysXController);
  //CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(g_PhysXController->GetPosition());

  //CPhysicActor * g_Box01 = CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject("Box01")->GetPhysXActor();
  //CPhysicActor * g_Box02 = CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject("Box04")->GetPhysXActor();
  //g_Joint = new CPhysicFixedJoint();
  //g_Joint->SetInfo(g_Box01,g_Box02);
  //CORE->GetPhysicsManager()->AddPhysicFixedJoint(g_Joint);

  //CORE->GetTriggerManager()->Init();
  
  //:Player
  m_pCoregame->InitPlayer();
  m_pPlayer = m_pCoregame->GetPlayer();

  //Triggers
	CORE->GetTriggerManager()->Init();
  CORE->GetTriggerManager()->RegistraTriggers();
  
  //CORE->GetSoundManager()->SetListener(CORE->GetCameraManager()->GetCamera()->GetObject3D());
  CORE->GetSoundManager()->SetListener(CORE->GetCameraManager()->GetCamera()->GetObject3D());

  
  CORE->GetGUIManager()->SetVisiblePointerMouse(false);
  CORE->GetGUIManager()->ActiveWindows("Game.xml");

  //Items
	CORE->GetItemManager()->AddItem(Vect3f(34.0f,5.0f,2.0f));
}

///<summary>
/// CGameProcess_t:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CGameProcess_t::RenderScene(CRenderManager* rm) 
{
  Render3D(rm);  
  Render2D(rm); 
  RenderDebugInfo(); 

  //CORE->GetScriptManager()->RunCode("get_core():get_font_manager():lua_draw_literal(100, 100, 'Exit')");
}

///<summary>
/// CGameProcess_t:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CGameProcess_t::Update(float elapsedTime)
{
  CActionToInput * l_Action = CORE->GetInputManager()->GetActionToInput();
  CheckActionsFromInputs(elapsedTime);
  UpdateLua();
  //MovMouse();
  //m_pCoregame->GetPlayer()->Update(elapsedTime);
  m_pPlayer->Update(elapsedTime);
  m_pCoregame->GetZombieManager()->Update(elapsedTime);
  
  /*int x = 0;
  // pseudo maquina de estados (solo para pruebas... Andres)
  CRenderableAnimatedInstanceModel* l_pRenderableInstance = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
  CAnimatedInstanceModel* l_pAnimation = l_pRenderableInstance->GetAnimatedInstanceModel();
  */
  if ((m_DeltaFromXML = l_Action->DoAction("ReloadRenderableObjects")) != 0.0f )
  {
    CORE->GetRenderableObjectsManager()->Reload();
  }
  CORE->GetRenderableObjectsManager()->Update(elapsedTime);
  // Habilita transparencia
  if ((m_DeltaFromXML = l_Action->DoAction("EnableAlphaBlend")) != 0.0f )
  {
    CORE->GetRenderManager()->EnableAlphaBlend();
  }

  //Partículas
  CORE->GetParticleManager()->Update(elapsedTime);

  // Sound
  Vect3f l_PlayerPos = m_pPlayer->GetPhysicController()->GetPosition();
  //Vect3f l_PlayerPos = m_pCoregame->GetPlayer()->GetPhysicController()->GetPosition();
  CORE->GetSoundManager()->UpdateSound(l_PlayerPos);

  //PhysXController
  //g_PhysXController->Move(Vect3f(0.0f,0.0f,0.0f),elapsedTime);
  //CORE->GetRenderablePhysXManager()->Update(elapsedTime);

  /*CRenderableObject* l_Character = CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
  Vect3f l_ControllerPosition = g_PhysXController->GetPosition();
  l_ControllerPosition.y = l_ControllerPosition.y - 1.5f;
  l_Character->SetPosition(l_ControllerPosition);*/

  //IA
  //g_ZombieManagerGP->Update(elapsedTime);

  // Sound
  //CORE->GetSoundManager()->UpdateSound(elapsedTime);

  //Check Cameras Position

  //Items
	CORE->GetItemManager()->Update(elapsedTime);

  UpdateCamera();
}

///<summary>
/// CGameProcess_t:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CGameProcess_t::Render(CRenderManager* rm) 
{
  CSceneEffectManager *l_pSceneEffectManager = CORE->GetSceneEffectManager();
  // Andres: esta linea la comento para no usar efectos de escena
  //l_pSceneEffectManager->ActivateRenderSceneEffects();

  CEffectManager *l_EffectManager=CORE->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);

  //Renderizamos la escena
  RenderScene(rm);

  l_pSceneEffectManager->CaptureFrameBuffers(rm);
  l_pSceneEffectManager->PostRender(rm);
  l_pSceneEffectManager->CaptureFrameBuffersAfterPostRender(rm);


  // Sound Manager
  //CSoundManager *l_SoundManager=CORE->GetSoundManager();

  /*CGUI *l_GUI=GetCurrentGUI();
  if(l_GUI)
  {
  l_GUI->Render(PSParams);
  }*/
  //RenderScene(rm);

  
  /*Colisiones de camara*/
  //CCamera * camera = CORE->GetCameraManager()->GetCamera();
  //camera->GetPhysXActor()->SetMat44(camera->GetObject3D()->GetMat44());
}


//---------------------------------------------------------------------------------//
//-------------------FUNCIONES PROPIAS DE GAMEPROCESS_TEST-------------------------//
//---------------------------------------------------------------------------------//

///<summary>
/// CGameProcess_t:: LoadXMLs : Carga todos los XML.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGameProcess_t::LoadXMLs()
{
  //Load XML
  CCore * l_pCore = CORE;
  l_pCore->GetEffectManager()->Load("Data/Effects.xml");
  l_pCore->GetLigthManager()->Load("Data/lights.xml");
  l_pCore->GetStaticMeshManager()->Load("Data/StaticMeshManager.xml");
  l_pCore->GetAnimatedModelManager()->Load("Data/AnimatedModels.xml");
  l_pCore->GetRenderableObjectsManager()->Load("Data/RenderableObjects.xml");
  
  //l_pCore->GetCameraManager()->Load("Data/Cameras.xml");
  l_pCore->GetSceneEffectManager()->LoadXML("Data/scene_effects.xml");
  //CORE->GetParticleManager()->LoadXMLStruct("Data/Particles/ParticlesStruct.xml");
  //CORE->GetParticleManager()->LoadXMLInstance("Data/Particles/ParticlesInstance.xml");
  l_pCore->GetParticleManager()->LoadXML("Data/Particles.xml");

  m_pCoregame->GetZombieManager()->Init("Data/ZombieLoader.xml");
  l_pCore->GetTriggerManager()->Load("Data/Triggers.xml");
  //l_pCore->GetGUIManager()->Init("Data/GUI/initGUI.xml");
	l_pCore->GetSoundManager()->Init("Data/Sounds.xml");
	//g_ZombieManagerGP->Init("Data/ZombieLoader.xml");
}

///<summary>
/// CGameProcess_t:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CGameProcess_t::Render3D(CRenderManager* rm)
{

  // render manager
  CRenderableObjectsManager *renderObjectsManager = CORE->GetRenderableObjectsManager();
  renderObjectsManager->Render(rm);


  m_pCoregame->GetZombieManager()->Render(rm);

  //player
  //m_pCoregame->GetPlayer()->Render();
  m_pPlayer->Render();

  // luces
  CORE->GetLigthManager()->Render(rm);

  // cámaras
  CORE->GetCameraManager()->DrawCameras();
  //CORE->GetRenderManager()->DrawCamera(CORE->GetCameraManager()->GetCamera());

  //Física
  CPhysicsManager* l_PhysManager = CORE->GetPhysicsManager();
  l_PhysManager->DebugRender(rm);

  //IA
  //g_ZombieManagerGP->Render(rm);
  //g_AgenteIAGP->DibujaDummies(rm);

}

///<summary>
/// CGameProcess_t:: Render3D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CGameProcess_t::Render2D(CRenderManager* rm)
{
  Mat44f l_Matrix;
  l_Matrix.SetIdentity();
  rm->SetTransform(l_Matrix);
  
  //Partículas
  CORE->GetParticleManager()->Render();


  //Hello world!
  //CFontManager *  fm = CORE->GetFontManager();
  //uint32 l_iFontId = fm->GetTTF_Id("arial");
  //fm->DrawLiteral(300,10,"HiWorld");
  //fm->DrawLiteral(300,25,"Name");
  //fm->DrawText((uint32)m_fPosX,(uint32)m_fPosY,colWHITE,l_iFontId,"Hello World!");

  //-------------------DRAWQUAD2D---------------------
  //Vect2i pos=(300,300);
  //rm->DrawQuad2D(pos,200,200,rm->UPPER_LEFT,colBLUE); //DrawQuad2D No.3

  //CTexture* texture= CORE->GetTextureManager()->GetTexture("Data/Textures/cara.png");
  //SRectangle2D coordText={(0.0f,0.0f),(1.0f,1.0f)}; 
  //rm->DrawQuad2D(pos,200,200,rm->UPPER_LEFT, texture, coordText); //DrawQuad2D No.2

  //rm->DrawQuad2D(pos,200,200,rm->UPPER_LEFT, texture,NONE_FLIP); //DrawQuad2D No.3
  //rm->EnableAlphaBlend();
}

///<summary>
/// CGameProcess_t:: PreRender() : Prerenderiza los efectos
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGameProcess_t::PreRender(CRenderManager* rm) 
{
  CORE->GetSceneEffectManager()->PreRender(rm,this);
}

///<summary>
/// CGameProcess_t:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGameProcess_t::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();

  CFontManager* fm = CORE->GetFontManager();
  CTimer *t = CORE->GetTimer();
  uint32 l_iFontId = fm->GetTTF_Id("arial");

  //fm->DrawText(10,75,colWHITE,l_iFontId,"Pitch: %f",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPitch());
  //fm->DrawText(10,95,colWHITE,l_iFontId,"Yaw: %f",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw());
  //fm->DrawText(10,115,colWHITE,l_iFontId,"Position: [%f,%f,%f]",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().x,CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().y,CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().z);

  CStaticMeshManager* sm= CORE->GetStaticMeshManager();
  fm->DrawTextA(10,135,colYELLOW,l_iFontId,"Num of Faces (Static Meshes): %ld",sm->GetTotalObjectsFaces());
  CAnimatedModelManager* am=CORE->GetAnimatedModelManager();
  fm->DrawTextA(10,155,colYELLOW,l_iFontId,"Num of Faces (Dinamic Meshes): %ld",am->GetTotalNumFaces());
  fm->DrawTextA(10,185,colYELLOW,l_iFontId,g_CollitionObject.c_str());
  std::string temp = CORE->GetCameraManager()->GetCamera()->GetName();
  fm->DrawTextA(10,205,colYELLOW,l_iFontId,temp.c_str());

  fm->DrawTextA(10, 215, colCYAN, l_iFontId, m_pPlayer->m_sDebug.c_str());

  /*fm->DrawTextA(10,300,colBLUE,l_iFontId,"A* - Ctrl izq.+ (1-9)");
  fm->DrawTextA(10,320,colBLUE,l_iFontId,"Dijsktra - Ctrl der.+ (1-9)");

  fm->DrawTextA(10,360,colWHITE,l_iFontId,"Check Actions: %ld",m_CheckActions);
  fm->DrawTextA(10,380,colWHITE,l_iFontId,"Move Mouse: %ld",m_MoveMouse);
  fm->DrawTextA(10,400,colWHITE,l_iFontId,"LUA: %ld",m_UpdateLua);
  fm->DrawTextA(10,420,colWHITE,l_iFontId,"Mov Player: %ld",m_MovPlayer);
  fm->DrawTextA(10,440,colWHITE,l_iFontId,"Particles: %ld",mParticles);
  fm->DrawTextA(10,460,colWHITE,l_iFontId,"PhysX: %ld",m_PhysX);
  fm->DrawTextA(10,480,colWHITE,l_iFontId,"IA: %ld",m_IA);*/
}

///<summary>
/// CGameProcess_t:: CheckActionsFromInputs : Según una entrada de teclado ejecuta su acción asociada.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CGameProcess_t::CheckActionsFromInputs(float elapsedTime)
{
  CActionToInput * l_Action = CORE->GetInputManager()->GetActionToInput();
  if((m_DeltaFromXML = l_Action->DoAction("ProcessInfo")) != 0.0f )
  {
    RenderDebugInfo(); //Esto solo se dibuja en tiempo de renderingScene
  }
  if((m_DeltaFromXML = l_Action->DoAction("ReloadEffects")) != 0.0f )
  {
    CEffectManager* ef=CORE->GetEffectManager();
    ef->Reload();
    CStaticMeshManager *sm=CORE->GetStaticMeshManager();
    sm->Reload();
    CRenderableObjectsManager *ro = CORE->GetRenderableObjectsManager();
    ro->Reload();
    //falta parametros de effects.xml
    CSceneEffectManager *sem = CORE->GetSceneEffectManager();
    //sem->Destroy();
    sem->LoadXML("Data/scene_effects.xml");
  }

  CCamera * l_ActualCamera = CORE->GetCameraManager()->GetCamera();
  
  //YawViewerCam
  if((m_DeltaFromXML = l_Action->DoAction("YawViewerCam")) != 0.0f )
  {
    //Vect3f pos = l_ActualCamera->GetEye();
    //const Vect3f& dir = l_ActualCamera->GetDirection();
    //SCollisionInfo info;
    //g_ClosestActor = CORE->GetPhysicsManager()->RaycastClosestActor(dir,pos,1,g_ClosestActor,info);
    //m_ray = g_ClosestActor->GetName();
    //if(m_ray == "Controller"){
    float l_fCameraYaw = l_ActualCamera->GetObject3D()->GetYaw() - m_DeltaFromXML*0.002f;
    // restringiendo el angulo de Yaw de la camara para que siempre esté entre -PI y PI
    if (l_fCameraYaw > FLOAT_PI_VALUE)
      l_fCameraYaw -= FLOAT_2PI_VALUE;
    else if (l_fCameraYaw < -FLOAT_PI_VALUE)
      l_fCameraYaw += FLOAT_2PI_VALUE;

    l_ActualCamera->GetObject3D()->SetYaw(l_fCameraYaw);
    //}
  }
  //PitchViewerCam
  if((m_DeltaFromXML = l_Action->DoAction("PitchViewerCam")) != 0.0f )
  {
    float l_fPitch = l_ActualCamera->GetObject3D()->GetPitch();

    l_fPitch -= m_DeltaFromXML * 0.005f;

    // estableciendo un limite para el angulo
    if (l_fPitch > -1.5f && l_fPitch < 0.5f)
      l_ActualCamera->GetObject3D()->SetPitch(l_fPitch);
  }
  //SlowZoomViewerCam
  if((m_DeltaFromXML = l_Action->DoAction("SlowZoomViewerCam")) != 0.0f )
  {
    CThPSCamera * cam = (CThPSCamera*)CORE->GetCameraManager()->GetCamera();
    cam->AddZoom(m_DeltaFromXML * -0.005f);
  }
  else 
  { //ZoomViewerCam
    if((m_DeltaFromXML = l_Action->DoAction("ZoomViewerCam")) != 0.0f )
    {
      CThPSCamera * cam = (CThPSCamera*)CORE->GetCameraManager()->GetCamera();
      cam->AddZoom(m_DeltaFromXML * -0.01f);
    }
  }

  //////////////////////////////////////////////////////////////////////////////
  // Sound-related actions
  //PlaySample
  //if((m_DeltaFromXML = l_Action->DoAction("PlaySample")) != 0.0f )
  //{
  //	CORE->GetSoundManager()->PlaySample("growl");
  //}

  //PlayMusic
  //if((m_DeltaFromXML = l_Action->DoAction("PlayMusic")) != 0.0f )
  //{
  //	CORE->GetSoundManager()->PlayMusic("stillDre");
  //}

  //PlaySound
  if((m_DeltaFromXML = l_Action->DoAction("PlaySound")) != 0.0f )
  {
    CORE->GetSoundManager()->PlaySound("swordSwing1");
    //CORE->GetSoundManager()->PlaySound("stillDre");
  }

  //TestSound
  if((m_DeltaFromXML = l_Action->DoAction("TestSound")) != 0.0f )
  {
    CORE->GetSoundManager()->TestSound();
  }

  //CrossFade
  if((m_DeltaFromXML = l_Action->DoAction("CrossFade")) != 0.0f )
  {
    CORE->GetSoundManager()->CrossFade("99");
  }

  //FadeOut
  if((m_DeltaFromXML = l_Action->DoAction("FadeOut")) != 0.0f )
  {
    CORE->GetSoundManager()->FadeOut();
  }


  //FadeIn
  if((m_DeltaFromXML = l_Action->DoAction("FadeIn")) != 0.0f )
  {
    CORE->GetSoundManager()->FadeIn("99");
  }


  //Pause
  if((m_DeltaFromXML = l_Action->DoAction("Pause")) != 0.0f )
  {
    CORE->GetSoundManager()->Pause();
  }


  //Resume
  if((m_DeltaFromXML = l_Action->DoAction("Resume")) != 0.0f )
  {
    CORE->GetSoundManager()->Resume();
  }


  //////////////////////////////////////////////////////////////////////////////

  ////ScreenShot
  //if(GetInputManager()->GetActionToInput()->DoAction("SaveLogsInFile") != 0.0f)
  //{
  //  CRenderManager *rm = CORE->GetRenderManager();
  //  rm->GetWindowRect()
  //}
  //CAMERAS
  CCamera * l_CameraTemp;
  if((m_DeltaFromXML = l_Action->DoAction("Camera0")) != 0.0f )
  {
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera0");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  if((m_DeltaFromXML = l_Action->DoAction("Camera1")) != 0.0f )
  {
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera1");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  /*if((m_DeltaFromXML = l_Action->DoAction("Camera2")) != 0.0f )
  {
  l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera2");
  CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  if((m_DeltaFromXML = l_Action->DoAction("Camera3")) != 0.0f )
  {
  l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera3");
  CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  if((m_DeltaFromXML = l_Action->DoAction("Camera4")) != 0.0f )
  {
  l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera4");
  CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  if((m_DeltaFromXML = l_Action->DoAction("Camera5")) != 0.0f )
  {
  l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera5");
  CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  if((m_DeltaFromXML = l_Action->DoAction("Camera6")) != 0.0f )
  {
  l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera6");
  CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  if((m_DeltaFromXML = l_Action->DoAction("Camera7")) != 0.0f )
  {
  l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera7");
  CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  if((m_DeltaFromXML = l_Action->DoAction("Camera8")) != 0.0f )
  {
  l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera8");
  CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }
  if((m_DeltaFromXML = l_Action->DoAction("Camera9")) != 0.0f )
  {
  l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera9");
  CORE->GetCameraManager()->SetCamera(l_CameraTemp);
  }*/

  //Física
  if((m_DeltaFromXML = l_Action->DoAction("Ray")) != 0.0f )
  {
    const Vect3f pos = CORE->GetCameraManager()->GetCamera()->GetEye();
    Vect3f& dir = CORE->GetCameraManager()->GetCamera()->GetDirection();
    SCollisionInfo info;
    g_ClosestActor = CORE->GetPhysicsManager()->RaycastClosestActor(pos,dir,1,g_ClosestActor,info);
    m_ray = g_ClosestActor->GetName();
  }
}

///<summary>
/// CGameProcess_t:: MovPlayer : Calcula los movimientos y la velocidad del player.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CGameProcess_t::MovPlayer(float elapsedTime)
{
  /*Vect3f l_vPos = GetPositionObj(CORE->GetCameraManager()->GetCamera()->GetObject3D()); //posición objeto
  CActionToInput * l_Action = CORE->GetInputManager()->GetActionToInput();


  bool l_bIsMoving = false;



  //Física
  Vect3f l_VectorDirector;
  CCamera * l_ActualCamera = CORE->GetCameraManager()->GetCamera();
  CObject3D * l_CameraObject = l_ActualCamera->GetObject3D();
  float l_ActualYaw = l_CameraObject->GetYaw();

  if ((m_DeltaFromXML = l_Action->DoAction("GoForwardLeft")) != 0.0f )
  {
    l_VectorDirector.x = cos(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector *= m_MovementSpeed;
    g_PhysXController->Move(l_VectorDirector, elapsedTime);
    //l_CameraObject->SetPosition(g_PhysXController->GetPosition());
    //m_pPlayer->SetPosition(l_vPos);
    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);	
    l_bIsMoving = true;
  }
  else if ((m_DeltaFromXML = l_Action->DoAction("GoForwardRight")) != 0.0f )
  {

    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector *= m_MovementSpeed;
    g_PhysXController->Move(l_VectorDirector, elapsedTime);	
    l_bIsMoving = true;
    //l_CameraObject->SetPosition(g_PhysXController->GetPosition());
    //m_pPlayer->SetPosition(l_vPos);
    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
  }

  else if ((m_DeltaFromXML = l_Action->DoAction("GoBackwardsRight")) != 0.0f )
  {
    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector *= m_MovementSpeed;
    g_PhysXController->Move((l_VectorDirector*-1), elapsedTime);	
    l_bIsMoving = true;
    //l_CameraObject->SetPosition(g_PhysXController->GetPosition());
    //m_pPlayer->SetPosition(l_vPos);
    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos); 
  }

  else if ((m_DeltaFromXML = l_Action->DoAction("GoBackwardsLeft")) != 0.0f )
  {
    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector *= m_MovementSpeed;
    g_PhysXController->Move((l_VectorDirector*-1), elapsedTime);	
    l_bIsMoving = true;
    //l_CameraObject->SetPosition(g_PhysXController->GetPosition());
    //m_pPlayer->SetPosition(l_vPos);
    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
  }

  //UP DOWN LEFT RIGHT MOVEMENTS
  else if ((m_DeltaFromXML = l_Action->DoAction("GoForward")) != 0.0f )
  {
    /*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
    l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
    CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);/

    Vect3f l_VectorDirector = l_ActualCamera->GetDirection();
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.Normalize();
    l_VectorDirector *= m_MovementSpeed;
    g_PhysXController->Move(l_VectorDirector, elapsedTime);	
    l_bIsMoving = true;
    //l_CameraObject->SetPosition(g_PhysXController->GetPosition());

  }

  else if ((m_DeltaFromXML = l_Action->DoAction("GoBackwards")) != 0.0f )
  {
    /*l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
    l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
    CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);/

    Vect3f l_VectorDirector = l_ActualCamera->GetDirection();
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.Normalize();
    l_VectorDirector *= m_MovementSpeed;
    g_PhysXController->Move((l_VectorDirector*-1), elapsedTime);	
    l_bIsMoving = true;
    //l_CameraObject->SetPosition(g_PhysXController->GetPosition());

  }

  else if ((m_DeltaFromXML = l_Action->DoAction("GoSidewardLeft")) != 0.0f )
  {
    /*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
    l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
    CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);/

    Vect3f l_VectorStrafe = (l_ActualCamera->GetDirection() ^ l_ActualCamera->GetVecUp()).Normalize();
    l_VectorStrafe *= m_MovementSpeed;
    g_PhysXController->Move(l_VectorStrafe, elapsedTime);
    Vect3f test = g_PhysXController->GetPosition();
    l_CameraObject->SetPosition(test);	
    l_bIsMoving = true;

  }

  else if ((m_DeltaFromXML = l_Action->DoAction("GoSidewardRight")) != 0.0f )
  {
    /*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
    l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
    CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);/

    Vect3f l_VectorStrafe = (l_ActualCamera->GetVecUp() ^ l_ActualCamera->GetDirection()).Normalize();
    l_VectorStrafe *= m_MovementSpeed;
    g_PhysXController->Move(l_VectorStrafe, elapsedTime);	
    l_bIsMoving = true;
    //l_CameraObject->SetPosition(g_PhysXController->GetPosition());

  }
  if ((m_DeltaFromXML = l_Action->DoAction("Jump")) != 0.0f )
  {
    g_PhysXController->Jump(60); //Testing purposes ony
  }

  if ((m_DeltaFromXML = l_Action->DoAction("GoUp")) != 0.0f )
  {
    l_vPos.y += elapsedTime*m_MovementSpeed;
    l_CameraObject->SetPosition(l_vPos);
    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);

  }

  if ((m_DeltaFromXML = l_Action->DoAction("GoDown")) != 0.0f )
  {
    l_vPos.y -= elapsedTime*m_MovementSpeed;
    l_CameraObject->SetPosition(l_vPos);
    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);

  }

  if ((m_DeltaFromXML = l_Action->DoAction("PlayerRun")) != 0.0f )
  {
    m_MovementSpeed = 0.92f;
  }

  if ((m_DeltaFromXML = l_Action->DoAction("PlayerQuiet")) != 0.0f )
  {
    m_MovementSpeed = 0.25f;
  }

  if ((m_DeltaFromXML = l_Action->DoAction("PlayerNormalSpeed")) != 0.0f )
  {
    m_MovementSpeed = 0.03f;
  }
  /*CAMERA MOVEMENTS*/
  /*if ((m_DeltaFromXML = l_Action->DoAction("MoveCameraForward")) != 0.0f )
  {
  l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
  l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
  CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
  }/

  //Actualizacion de la camara
  l_CameraObject->SetPosition(g_PhysXController->GetPosition());
*/


}

///<summary>
/// CGameProcess_t:: GetPositionObj : Nos devuelve un Vect3f con la posición del objeto.
///</summary>
///<param name="p_vObj">Objeto del que vamos a obtener la posición.</param>
///<returns name="posicion">Vector de 3 floats con la posición del objeto.</returns>
Vect3f CGameProcess_t::GetPositionObj(CObject3D * p_vObj)
{
  //Moverse a través del escenario. Fijamos la cámara al objeto
  Vect3f l_vPos = p_vObj->GetPosition(); 
  return l_vPos;
}

///<summary>
/// CGameProcess_t:: InitCameras : Crea las cámaras y las asocia a un objeto.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGameProcess_t::InitCameras()
{
  //Bots
  //m_pPlayer = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
}

///<summary>
/// CGameProcess_t:: MovMouse : Movimiento del ratón asociado a la cámara.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGameProcess_t::MovMouse()
{
  //Vect3i deltas = CORE->GetInputManager()->GetMouseDelta();
  //CThPSCamera * cam = (CThPSCamera*) CORE->GetCameraManager()->GetCamera();;
  //cam->SetZoom( cam->GetZoom() + deltas.z * -0.02f);
}

///<summary>
/// CGameProcess_t:: UpdateLua : Scripts de LUA.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGameProcess_t::UpdateLua()
{
  CActionToInput * l_Action = CORE->GetInputManager()->GetActionToInput();
  // Carga de script
  if((m_DeltaFromXML = l_Action->DoAction("LoadLuaFile")) != 0.0f )
  {
    //TODO: Cargar un XML con los path de los lua files y cargar cada archivo del xml
    CORE->GetScriptManager()->LoadFile("./Data/LUAScripts/variables.lua");
    CORE->GetScriptManager()->LoadFile("./Data/LUAScripts/init.lua");
  }
  // Carga del script LUA
  if((m_DeltaFromXML = l_Action->DoAction("RunLuaFile")) != 0.0f )
  {
    CORE->GetScriptManager()->RunFile("./Data/LUAScripts/variables.lua");
    CORE->GetScriptManager()->RunFile("./Data/LUAScripts/init.lua");
    CORE->GetScriptManager()->RunFile("./Data/LUAScripts/zombis.lua");
  }
  // Ejecucion de una funcion del script LUA
  if((m_DeltaFromXML = l_Action->DoAction("RunLuaFunction")) != 0.0f )
  {
    CORE->GetScriptManager()->RunCode("tumba()");
  }
  // Ejecución de script de lua
  if((m_DeltaFromXML = l_Action->DoAction("CallChunck")) != 0.0f )
  {
    CORE->GetScriptManager()->CallChunk();
  }
  // TODO
  /*if(CORE->GetInputManager()->IsUpDown(IDV_KEYBOARD,KEY_K)) 
  {
  CORE->GetScriptManager()->RunCode("tumbaAlta()");
  }*/
}

///<summary>
/// CGameProcess_t:: MemoryTest : Pruebas para grabar en RAM o VRAM.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGameProcess_t::MemoryTest()
{
  //Para probarlo hacer una llamada desde el RenderScene()
  //CORE->GetRenderManager()->GetDevice()->SetFVF(TTEXTURE1_VERTEX::GetFVF());
  //CORE->GetRenderManager()->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLELIST, 2, m_vTexturasABorrar, sizeof(TTEXTURE1_VERTEX));
  //CORE->GetRenderManager()->GetDevice()->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0, 4, 2, g_index, D3DFMT_INDEX16, g_Textures4, sizeof(TTEXTURE1_VERTEX)); 

  //Usando memoria de video
  //CORE->GetRenderManager()->GetDevice()->SetStreamSource(0,vertexBuffer,0,sizeof(TTEXTURE1_VERTEX));
  //CORE->GetRenderManager()->GetDevice()->SetIndices(indexBuffer);
  //CORE->GetRenderManager()->GetDevice()->SetFVF(TTEXTURE1_VERTEX::GetFVF());
  //CORE->GetRenderManager()->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void CGameProcess_t::UpdateCamera()
{
  //PRUEBA CON RAYOS
  CThPSCamera * l_ActiveCamera = (CThPSCamera*) CORE->GetCameraManager()->GetCamera();
  Vect3f l_ActualPosition = l_ActiveCamera->GetEye();
  CPhysicsManager * l_PhysXManager = CORE->GetPhysicsManager();
  std::vector<CPhysicUserData*> l_ImpactObjects;
  Vect3f pos = l_ActiveCamera->GetLookAt();
  Vect3f& dir = l_ActiveCamera->GetDirection();
  dir = dir * -1.f;
  dir.Normalize();
  SCollisionInfo info;
  g_ClosestActorPX = l_PhysXManager->RaycastClosestActor(pos,dir,PX_MSK_ALL_SCENARIO,g_ClosestActorPX,info);
  if(g_ClosestActorPX != NULL)
  {
    g_CollitionObject = g_ClosestActorPX->GetName();
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
  //CThPSCamera * l_ActiveCamera = (CThPSCamera*) CORE->GetCameraManager()->GetCamera();
  //Vect3f l_ActualPosition = l_ActiveCamera->GetEye();
  //CPhysicsManager * l_PhysXManager = CORE->GetPhysicsManager();
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