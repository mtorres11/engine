#include "Core.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
#include "ProcessIA2.h"
#include "Math/Matrix44.h"
#include "Math/MathUtils.h"
#include "Cameras/FPSCamera.h"
#include "Cameras/ThPSCamera.h"
#include "Object3D/Object3D.h"
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
#include "CharacterController/PhysicController.h"
#include "../Videogame/ZombieManager/ZombieManager.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

#define RADIANS 0.018f

CZombieManager* g_ZombieManager = NULL;
CPhysicController* gController1 = 0;
CPhysicUserData* userdata = 0;

CProcessIA2::CProcessIA2()
{
  m_fSpeed1 = 3.0;
  //m_fSpeedZombie = 0.0;
  //m_bChaser = false;
  //m_V3DirectorPlayer2.SetZero();
  m_pPlayer1=NULL;
  //m_pPlayer1Zombie=NULL;
  //m_fAngleParcialZombie=0.0;
  //m_fAngleTotalZombie=0.0;
  //m_V3RightZombie.SetZero();
  //m_V3UpZombie.SetZero();
//  m_SceneEffectManager=0;
  //m_MovementSpeed = 3.0;
}

CProcessIA2::~CProcessIA2()
{
  //CHECKED_DELETE(m_pPlayer1);
  //CHECKED_DELETE(m_pPlayer12);
 CHECKED_DELETE(gController1);
//  CHECKED_DELETE(userdata);
  CHECKED_DELETE(g_ZombieManager);
}


void CProcessIA2::Init ()
{
  g_ZombieManager = new CZombieManager();

  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
// m_SceneEffectManager = CORE->GetSceneEffectManager();//new CSceneEffectManager();
	LoadXMLs(); 

  CORE->GetCameraManager()->SetCamera(CORE->GetCameraManager()->GetCameraByName("Camera0"));
  //Bots
	m_pPlayer1 = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
  //crea controller
  CRenderableObject* controller = CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
  userdata = new CPhysicUserData(controller->GetName());
  userdata->SetPaint(true);
  gController1 = new CPhysicController(0.5f, 1.0f, 0.5f, 0.1f, 0.5f, 1, userdata, m_pPlayer1->GetPosition()/*controller->GetPosition()*/);
  CORE->GetPhysicsManager()->AddPhysicController(gController1);
  CORE->GetPhysicsManager()->SetDebugRenderMode(true);
}

void CProcessIA2::RenderScene(CRenderManager* rm) 
{
  /*PhysX test*/
	//CORE->Render();
	Render3D(rm); 
	Render2D(rm); 
}


void CProcessIA2::Update(float elapsedTime)
{
  CheckActionsFromInputs(elapsedTime);
//	MovMouse(); 
//	UpdateLua(); 
  if(CORE->GetInputManager()->IsAnyKeyPressed())
  {
    MovPlayer(elapsedTime); 
    MovCamera();
  }
  if(CORE->GetInputManager()->GetMouseDelta() != v3iZERO)
  {
    MovMouse();
  }
  g_ZombieManager->Update(elapsedTime);
  MovPlayerIA(elapsedTime);
}


void CProcessIA2::PreRender(CRenderManager* rm) 
{
//	m_SceneEffectManager->PreRender(rm,this);
}

void CProcessIA2::Render(CRenderManager* rm) 
{
  //eje principal
  Mat44f matWorld;
  matWorld.SetIdentity();
  rm->SetTransform(matWorld);
  rm->DrawAxis(5);

  //mueve eje de IA
  RenderMovIA(rm);

// m_SceneEffectManager->ActivateRenderSceneEffects();

  CEffectManager *l_EffectManager=CORE->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);
  
  //m_SceneEffectManager->PreRender(rm,this);
  //Renderizamos la escena
  RenderScene(rm);
  CORE->GetPhysicsManager()->DebugRender(rm);
  
/*  m_SceneEffectManager->CaptureFrameBuffers(rm);
  m_SceneEffectManager->PostRender(rm);
  m_SceneEffectManager->CaptureFrameBuffersAfterPostRender(rm);
*/

  RenderDebugInfo(); 
}
//---------------------------------------------------------------------------------//
//-------------------FUNCIONES PROPIAS DE GAMEPROCESS_TEST-------------------------//
//---------------------------------------------------------------------------------//

void CProcessIA2::LoadXMLs()
{
	//Load XML
  CCore * l_pCore = CORE;
  l_pCore->GetEffectManager()->Load("Data/Effects.xml");
  l_pCore->GetStaticMeshManager()->Load("Data/StaticMeshManager - ia.xml");
  l_pCore->GetLigthManager()->Load("Data/lights.xml");
  l_pCore->GetAnimatedModelManager()->Load("Data/AnimatedModels.xml");
  l_pCore->GetRenderableObjectsManager()->Load("Data/RenderableObjects - ia.xml");
//  l_pCore->GetCameraManager()->Load("Data/Cameras.xml");
// m_SceneEffectManager->LoadXML("Data/scene_effects.xml"
//  l_pCore->GetGUIManager()->Init("Data/GUI/initGui_laberynth.xml");
  g_ZombieManager->Init("Data/ZombieLoader.xml");
}


void CProcessIA2::Render3D(CRenderManager* rm)
{
	// render manager
  CRenderableObjectsManager *renderObjectsManager = CORE->GetRenderableObjectsManager();
  renderObjectsManager->Render(rm);
  
  g_ZombieManager->Render(rm);
  // luces
  CORE->GetLigthManager()->Render(rm);

	// cámaras
//  CORE->GetCameraManager()->DrawCameras();
	//CORE->GetRenderManager()->DrawCamera(CORE->GetCameraManager()->GetCamera());
}

void CProcessIA2::Render2D(CRenderManager* rm)
{
}

void CProcessIA2::RenderDebugInfo()
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
}

void CProcessIA2::CheckActionsFromInputs(float elapsedTime)
{
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ProcessInfo")) != 0.0f )
  {
    RenderDebugInfo(); //Esto solo se dibuja en tiempo de renderingScene
  }
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ReloadEffects")) != 0.0f )
  {
    CEffectManager* ef=CORE->GetEffectManager();
    ef->Reload();
    CStaticMeshManager *sm=CORE->GetStaticMeshManager();
    sm->Reload();
    CRenderableObjectsManager *ro = CORE->GetRenderableObjectsManager();
    ro->Reload();
    //falta parametros de effects.xml
//   CSceneEffectManager *sem = m_SceneEffectManager;
		//sem->Destroy();
//   sem->LoadXML("Data/scene_effects.xml");
  }
	////ScreenShot
  //if(GetInputManager()->GetActionToInput()->DoAction("SaveLogsInFile") != 0.0f)
  //{
  //  CRenderManager *rm = CORE->GetRenderManager();
  //  rm->GetWindowRect()
  //}
	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ReloadRenderableObjects")) != 0.0f )
	{
		CORE->GetRenderableObjectsManager()->Reload();
	}
  CORE->GetRenderableObjectsManager()->Update(elapsedTime);
	// Habilita transparencia
	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("EnableAlphaBlend")) != 0.0f )
	{
		CORE->GetRenderManager()->EnableAlphaBlend();
	}
}

void CProcessIA2::MovPlayer(float elapsedTime)
{
  //Vect3f l_vPos = CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetPosition(); //posición objeto
  //l_vPos.y = 0.f;

  	Vect3f l_vPos = GetPositionObj(CORE->GetCameraManager()->GetCamera()->GetObject3D()); //posición objeto

	//Física
	Vect3f l_VectorDirector;
  CCamera * l_ActualCamera = CORE->GetCameraManager()->GetCamera();
  CObject3D * l_CameraObject = l_ActualCamera->GetObject3D();
  float l_ActualYaw = l_CameraObject->GetYaw();

  CActionToInput* l_Action = CORE->GetInputManager()->GetActionToInput();
	if ((m_DeltaFromXML = l_Action->DoAction("GoForwardLeft")) != 0.0f )
	{
    l_VectorDirector.x = cos(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw+FLOAT_PI_VALUE/4);
    gController1->Move(l_VectorDirector*1.01f, elapsedTime);
    //l_CameraObject->SetPosition(gController1->GetPosition());
		//m_pPlayer1->SetPosition(gController1->GetPosition());
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = l_Action->DoAction("GoForwardRight")) != 0.0f )
	{

    Vect3f l_VectorDirector;
   l_VectorDirector.x = cos(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw-FLOAT_PI_VALUE/4);
    gController1->Move(l_VectorDirector*1.01f, elapsedTime);
    //l_CameraObject->SetPosition(gController1->GetPosition());
		//m_pPlayer1->SetPosition(gController1->GetPosition());
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}


  else if ((m_DeltaFromXML = l_Action->DoAction("GoBackwardsRight")) != 0.0f )
	{
    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw+FLOAT_PI_VALUE/4);
    gController1->Move((l_VectorDirector*-1.01f), elapsedTime);
    //l_CameraObject->SetPosition(gController1->GetPosition());
		//m_pPlayer1->SetPosition(gController1->GetPosition());
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = l_Action->DoAction("GoBackwardsLeft")) != 0.0f )
	{
    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw-FLOAT_PI_VALUE/4);
    gController1->Move((l_VectorDirector*-1.01f), elapsedTime);
    //l_CameraObject->SetPosition(gController1->GetPosition());
		//m_pPlayer1->SetPosition(gController1->GetPosition());
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	//UP DOWN LEFT RIGHT MOVEMENTS
	else if ((m_DeltaFromXML = l_Action->DoAction("GoForward")) != 0.0f )
	{
		/*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);*/
    
    Vect3f l_VectorDirector = l_ActualCamera->GetDirection().GetNormalized();
    l_VectorDirector.y = 0.0f;
    gController1->Move(l_VectorDirector*1.01f, elapsedTime);
    //l_CameraObject->SetPosition(gController1->GetPosition());
		//m_pPlayer1->SetPosition(gController1->GetPosition());
	}

	else if ((m_DeltaFromXML = l_Action->DoAction("GoBackwards")) != 0.0f )
	{
		/*l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);*/
    
    Vect3f l_VectorDirector = l_ActualCamera->GetDirection().GetNormalized();
    l_VectorDirector.y = 0.0f;
    gController1->Move((l_VectorDirector*-1.01f), elapsedTime);
    //l_CameraObject->SetPosition(gController1->GetPosition());
		//m_pPlayer1->SetPosition(gController1->GetPosition());
	}

	else if ((m_DeltaFromXML = l_Action->DoAction("GoSidewardLeft")) != 0.0f )
	{
		/*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);*/
    
    Vect3f l_VectorStrafe = (l_ActualCamera->GetDirection() ^ l_ActualCamera->GetVecUp()).Normalize();
    gController1->Move(l_VectorStrafe*1.01f, elapsedTime);
    //l_CameraObject->SetPosition(gController1->GetPosition());
		//m_pPlayer1->SetPosition(gController1->GetPosition());
	}

	else if ((m_DeltaFromXML = l_Action->DoAction("GoSidewardRight")) != 0.0f )
	{
		/*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);*/

    Vect3f l_VectorStrafe = (l_ActualCamera->GetVecUp() ^ l_ActualCamera->GetDirection()).Normalize();
    gController1->Move(l_VectorStrafe*1.01f, elapsedTime);
    //l_CameraObject->SetPosition(gController1->GetPosition());
		//m_pPlayer1->SetPosition(gController1->GetPosition());
	}
	if ((m_DeltaFromXML = l_Action->DoAction("Jump")) != 0.0f )
	{
    gController1->Jump(1); //Testing purposes ony
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
		m_MovementSpeed = 15;
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = l_Action->DoAction("PlayerQuiet")) != 0.0f )
	{
		m_MovementSpeed = 2.5;
	}

	if ((m_DeltaFromXML = l_Action->DoAction("PlayerNormalSpeed")) != 0.0f )
	{
		m_MovementSpeed = 5;
	}

  
  
//  	//DIAGONAL MOVEMENTS
//	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardLeft")) != 0.0f )
//	{
//    l_vPos.z = l_vPos.z + /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		l_vPos.x = l_vPos.x + /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		m_pPlayer1->SetPosition(l_vPos);
//    m_pPlayer1->SetYaw(0.8f);
//		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardRight")) != 0.0f )
//	{
//		l_vPos.z = l_vPos.z - /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		l_vPos.x = l_vPos.x + /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer1->SetPosition(l_vPos);
//		m_pPlayer1->SetYaw(-0.5f);
//    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsRight")) != 0.0f )
//	{
//		l_vPos.z = l_vPos.z - /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		l_vPos.x = l_vPos.x - /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer1->SetPosition(l_vPos);
//		m_pPlayer1->SetYaw(-2.5f);
//    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsLeft")) != 0.0f )
//	{
//		l_vPos.z = l_vPos.z + /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		l_vPos.x = l_vPos.x - /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer1->SetPosition(l_vPos);
//    m_pPlayer1->SetYaw(2.5f);
//		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	//UP DOWN LEFT RIGHT MOVEMENTS
//	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForward")) != 0.0f )
//	{
//		l_vPos.x = l_vPos.x + /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer1->SetPosition(l_vPos);
//    m_pPlayer1->SetYaw(0.0f);
//		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwards")) != 0.0f )
//	{
//		l_vPos.x = l_vPos.x - /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer1->SetPosition(l_vPos);
//    m_pPlayer1->SetYaw(3.0f);
//		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardLeft")) != 0.0f )
//	{
//		l_vPos.z = l_vPos.z + /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer1->SetPosition(l_vPos);
//    m_pPlayer1->SetYaw(1.3f);
//		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardRight")) != 0.0f )
//	{
//    l_vPos.z = l_vPos.z - /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
//		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		
//		m_pPlayer1->SetPosition(l_vPos);
//    m_pPlayer1->SetYaw(-1.5f);
//		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoUp")) != 0.0f )
//	{
//		l_vPos.y += elapsedTime*m_MovementSpeed;
//		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoDown")) != 0.0f )
//	{
//		l_vPos.y -= elapsedTime*m_MovementSpeed;
//		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}
//
//	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("PlayerRun")) != 0.0f )
//	{
//    //m_pPlayer1->SetPosition(gController1->GetPosition());
////		m_MovementSpeed = 15;
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
//	}

  l_CameraObject->SetPosition(gController1->GetPosition());
  m_pPlayer1->SetPosition(gController1->GetPosition());
}
  
void CProcessIA2::MovCamera()
{
  //CAMERAS
  CCamera * l_CameraTemp;
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera0")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera0");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
}

void CProcessIA2::MovPlayerIA(float _elapsedtime)
{

//  Vect3f l_V3PosPlayer1 = m_pPlayer1->GetPosition();
//
//  //se obtiene el vector director optimo normalizado
//  m_V3DirPlayer2Optimo = l_V3PosPlayer1 - l_V3PosPlayer2;
//  m_V3DirPlayer2Optimo = m_V3DirPlayer2Optimo.GetNormalized();
//
//  //en el primer scan la direccion es la optima
////  float m_fAnglePlayer2 = 0.0f;
//  if(_elapsedtime == 0.0)
//  {
//     m_V3DirectorPlayer2 = m_V3DirPlayer2Optimo;
//     m_fAngleParcialPlayer2 = m_V3DirPlayer2Optimo.GetAngleY();
//     m_fAngleTotalPlayer2 = m_V3DirPlayer2Optimo.GetAngleY();
//  }
//
//  //eje del player2
//  //obtiene el vector director real
//  m_fAngleParcialPlayer2 = AjusteDireccion();
//  m_fAngleTotalPlayer2 += m_fAngleParcialPlayer2;
//
//  m_V3DirectorPlayer2.RotateY(m_fAngleParcialPlayer2);
//  m_V3UpPlayer2(l_V3PosPlayer2.x, 1.0f, l_V3PosPlayer2.z);
//  m_V3RightPlayer2 =  m_V3DirectorPlayer2.GetRotatedY( ePI2f );
//  m_V3RightPlayer2.y = 0.0;
//
//  if(m_bChaser)
//  {
//    m_fSpeed1 = 2.0f;
//    m_fSpeed2 = 1.8f;
//    
//    l_V3PosPlayer2.x += m_V3DirectorPlayer2.x * _elapsedtime * m_fSpeed2;
//    l_V3PosPlayer2.z += m_V3DirectorPlayer2.z * _elapsedtime * m_fSpeed2;
//  }
//  else
//  {
//    m_fSpeed1 = 2.5f;
//    m_fSpeed2 = 1.0f;
//    
//    l_V3PosPlayer2.x -= m_V3DirectorPlayer2.x * _elapsedtime * m_fSpeed2;
//    l_V3PosPlayer2.z -= m_V3DirectorPlayer2.z * _elapsedtime * m_fSpeed2;
//  }
//  //posicion y rotacion del muñeco
//  m_pPlayer12->SetPosition(Vect3f(l_V3PosPlayer2.x,0,l_V3PosPlayer2.z));
//  m_pPlayer12->SetYaw(-m_fAngleTotalPlayer2);
//
//  //separacion al colisionar (sin PhysX)
//  if((abs(l_V3PosPlayer1.x - l_V3PosPlayer2.x) < 0.1f) && (abs(l_V3PosPlayer1.z - l_V3PosPlayer2.z) < 0.1f))
//  {
//    m_bChaser = !m_bChaser;
//    l_V3PosPlayer1.x -= (float)0.2;
//    l_V3PosPlayer1.z -= (float)0.2;
//    l_V3PosPlayer2.x += (float)0.2;
//    l_V3PosPlayer2.z += (float)0.2;
//  }
}
//
//float CProcessIA2::AjusteDireccion()
//{
  //Vect3f myDir = m_V3DirectorPlayer2;
  //Vect3f myRight = m_V3RightPlayer2;

  //if( (m_V3DirPlayer2Optimo * myDir) < 0 )//player esta detras, GIRARA MAS RAPIDO si el producto escalar es negativo.
  //{
  //  if(m_V3DirPlayer2Optimo * myRight < 0)//player esta a la izquierda
  //    return -3*RADIANS;
  //  else    //player esta a la derecha
  //    return +3*RADIANS;
  //}
  //else  //player esta delante
  //{
  //  if(m_V3DirPlayer2Optimo * myRight < 0)//player esta a la izquierda
  //    return -RADIANS;
  //  else    //player esta a la derecha
	 // return +RADIANS;
  //}
  //
//}

void CProcessIA2::RenderMovIA(CRenderManager *rm)
{
  //Vect3f l_V3PosPlayer1 = m_pPlayer1->GetPosition();
  //Vect3f l_V3PosPlayer2 = m_pPlayer12->GetPosition();

  //Mat44f vecOptimo;
  //vecOptimo.SetIdentity();
  //rm->SetTransform(vecOptimo);
  //rm->DrawLine(l_V3PosPlayer2, l_V3PosPlayer1,colWHITE);
  //
  //Mat44f AxisPlayer2;
  //AxisPlayer2.SetIdentity();
  //rm->SetTransform(AxisPlayer2);
  //rm->DrawLine(l_V3PosPlayer2, l_V3PosPlayer2 + m_V3DirectorPlayer2, colRED);
  //rm->DrawLine(l_V3PosPlayer2, l_V3PosPlayer2 + m_V3RightPlayer2, colYELLOW);
  //rm->DrawLine(l_V3PosPlayer2, m_V3UpPlayer2, colGREEN);
}

Vect3f CProcessIA2::GetPositionObj(CObject3D * p_vObj)
{
  //Moverse a través del escenario. Fijamos la cámara al objeto
	Vect3f l_vPos = p_vObj->GetPosition(); 
	return l_vPos;
}

void CProcessIA2::InitCameras()
{
}

void CProcessIA2::MovMouse()
{
	//Vect3i deltas = CORE->GetInputManager()->GetMouseDelta();
	//CThPSCamera * cam = (CThPSCamera*) CORE->GetCameraManager()->GetCamera();;
  //cam->SetZoom( cam->GetZoom() + deltas.z * -0.02f);
  //MoveXViewerCam
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("MoveXViewerCam")) != 0.0f )
	{
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetYaw( CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw() + m_DeltaFromXML * -0.01f );
	}
	//MoveZViewerCam
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("MoveZViewerCam")) != 0.0f )
	{
		CThPSCamera* cam = (CThPSCamera*) CORE->GetCameraManager()->GetCamera();
		cam->SetZoom( cam->GetZoom() + m_DeltaFromXML * -0.1f);
	}
	//YawViewerCam
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("YawViewerCam")) != 0.0f )
	{
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetYaw( CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw() + m_DeltaFromXML*0.05f );
	}
	//PitchViewerCam
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("PitchViewerCam")) != 0.0f )
	{
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPitch(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPitch() + (m_DeltaFromXML * 0.05f) );
	}
	//SlowZoomViewerCam
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("SlowZoomViewerCam")) != 0.0f )
	{
		CThPSCamera * cam = (CThPSCamera*)CORE->GetCameraManager()->GetCamera();
		cam->AddZoom(m_DeltaFromXML * -0.005f);
	}
	else 
	{ //ZoomViewerCam
		if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ZoomViewerCam")) != 0.0f )
		{
			CThPSCamera * cam = (CThPSCamera*)CORE->GetCameraManager()->GetCamera();
			cam->AddZoom(m_DeltaFromXML * -0.05f);
		}
	}
}

void CProcessIA2::UpdateLua()
{
	// Carga de script
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("LoadLuaFile")) != 0.0f )
	{
		//TODO: Cargar un XML con los path de los lua files y cargar cada archivo del xml
		CORE->GetScriptManager()->LoadFile("./Data/LUAScripts/variables.lua");
		CORE->GetScriptManager()->LoadFile("./Data/LUAScripts/init.lua");
	}
	// Carga del script LUA
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("RunLuaFile")) != 0.0f )
	{
		CORE->GetScriptManager()->RunFile("./Data/LUAScripts/variables.lua");
		CORE->GetScriptManager()->RunFile("./Data/LUAScripts/init.lua");
	}
  // Ejecucion de una funcion del script LUA
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("RunLuaFunction")) != 0.0f )
	{
    CORE->GetScriptManager()->RunCode("tumba()");
	}
	// Ejecución de script de lua
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("CallChunck")) != 0.0f )
	{
		CORE->GetScriptManager()->CallChunk();
	}
	// TODO
	/*if(CORE->GetInputManager()->IsUpDown(IDV_KEYBOARD,KEY_K)) 
	{
		CORE->GetScriptManager()->RunCode("tumbaAlta()");
	}*/
}

void CProcessIA2::MemoryTest()
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