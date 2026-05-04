#include "MainProcess.h"
#include "ScriptManager.h"
#include "GUIManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
/*#include "Core.h"
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
#include "GameProcess_t.h"
#include "Math/Matrix44.h"
#include "Math/MathUtils.h"
#include "Cameras/FPSCamera.h"wWw
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

#include "Actor/PhysicActor.h"
#include "PhysicsManager.h"
#include "InputManager.h"
#include "Cameras/Camera.h"
#include "Shaders/SceneEffectManager.h"
#include "Timer/Timer.h"
#include "Utils/LogRender.h"
#include "SoundManager.h"*/

#if defined( _DEBUG )
  #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CMainProcess::CMainProcess()
{
}

CMainProcess::~CMainProcess()
{
}

///<summary>
/// CMainProcess:: Init : Lectura de los XML. Creación y asignación de las cámaras a los objetos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CMainProcess::Init ()
{
  LoadXMLs(); 
	CORE->GetScriptManager()->RunFile("Data/LUAScripts/variables.lua");
	CORE->GetScriptManager()->RunFile("Data/LUAScripts/init.lua");
  CORE->GetRenderManager()->EnableAlphaBlend();
  //CORE->GetGUIManager()->SetVisiblePointerMouse(true);
}

///<summary>
/// CMainProcess:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CMainProcess::RenderScene(CRenderManager* rm) 
{
	Render3D(rm); 
	Render2D(rm); 
  RenderDebugInfo(); 
}

///<summary>
/// CMainProcess:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CMainProcess::Update(float elapsedTime)
{
  CheckActionsFromInputs(elapsedTime);
//	UpdateLua(); 
  
/*	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ReloadRenderableObjects")) != 0.0f )
	{
		CORE->GetRenderableObjectsManager()->Reload();
	}
  CORE->GetRenderableObjectsManager()->Update(elapsedTime);
	// Habilita transparencia
	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("EnableAlphaBlend")) != 0.0f )
	{
		CORE->GetRenderManager()->EnableAlphaBlend();
	}*/
}

///<summary>
/// CMainProcess:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CMainProcess::Render(CRenderManager* rm) 
{
  /*CSceneEffectManager *l_pSceneEffectManager = CORE->GetSceneEffectManager();
  // Andres: esta linea la comento para no usar efectos de escena
  //l_pSceneEffectManager->ActivateRenderSceneEffects();

  CEffectManager *l_EffectManager=CORE->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);
*/
  //Renderizamos la escena
  RenderScene(rm);
/*
  l_pSceneEffectManager->CaptureFrameBuffers(rm);
  l_pSceneEffectManager->PostRender(rm);
  l_pSceneEffectManager->CaptureFrameBuffersAfterPostRender(rm);


	// Sound Manager
	CSoundManager *l_SoundManager=CORE->GetSoundManager();*/

  /*CGUI *l_GUI=GetCurrentGUI();
  if(l_GUI)
  {
  l_GUI->Render(PSParams);
  }*/
  //RenderScene(rm);

  
}


//---------------------------------------------------------------------------------//
//-------------------FUNCIONES PROPIAS DE GAMEPROCESS_TEST-------------------------//
//---------------------------------------------------------------------------------//

///<summary>
/// CMainProcess:: LoadXMLs : Carga todos los XML.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CMainProcess::LoadXMLs()
{
	//Load XML
  CCore * l_pCore = CORE;
  l_pCore->GetGUIManager()->Init("Data/GUI/initGUI.xml");
	l_pCore->GetSoundManager()->Init("Data/Sounds.xml");
	/*l_pCore->GetEffectManager()->Load("Data/Effects.xml");
  l_pCore->GetStaticMeshManager()->Load("Data/StaticMeshManager.xml");
  l_pCore->GetLigthManager()->Load("Data/lights.xml");
  l_pCore->GetAnimatedModelManager()->Load("Data/AnimatedModels.xml");
  l_pCore->GetRenderableObjectsManager()->Load("Data/RenderableObjects.xml");
  l_pCore->GetCameraManager()->Load("Data/Cameras.xml");
  l_pCore->GetSceneEffectManager()->LoadXML("Data/scene_effects.xml");
  l_pCore->GetSoundManager()->Init("Data/Sounds.xml");*/
}

///<summary>
/// CMainProcess:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CMainProcess::Render3D(CRenderManager* rm)
{
	// render manager
  /*CRenderableObjectsManager *renderObjectsManager = CORE->GetRenderableObjectsManager();
  renderObjectsManager->Render(rm);

  // luces
  CORE->GetLigthManager()->Render(rm);

	// cámaras
  CORE->GetCameraManager()->DrawCameras();*/
	//CORE->GetRenderManager()->DrawCamera(CORE->GetCameraManager()->GetCamera());

}

///<summary>
/// CMainProcess:: Render3D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CMainProcess::Render2D(CRenderManager* rm)
{
	/*Mat44f l_Matrix;
  l_Matrix.SetIdentity();
  rm->SetTransform(l_Matrix);
  l_Matrix.SetIdentity();
  rm->SetTransform(l_Matrix);*/

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
/// CMainProcess:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CMainProcess::PreRender(CRenderManager* rm) 
{
  //CORE->GetSceneEffectManager()->PreRender(rm,this);
}

///<summary>
/// CMainProcess:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CMainProcess::RenderDebugInfo()
{
  //CProcess::RenderDebugInfo();

  /*CFontManager* fm = CORE->GetFontManager();
  CTimer *t = CORE->GetTimer();
  uint32 l_iFontId = fm->GetTTF_Id("arial");

  //fm->DrawText(10,75,colWHITE,l_iFontId,"Pitch: %f",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPitch());
  //fm->DrawText(10,95,colWHITE,l_iFontId,"Yaw: %f",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw());
  //fm->DrawText(10,115,colWHITE,l_iFontId,"Position: [%f,%f,%f]",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().x,CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().y,CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().z);

  CStaticMeshManager* sm= CORE->GetStaticMeshManager();
  fm->DrawTextA(10,135,colYELLOW,l_iFontId,"Num of Faces (Static Meshes): %ld",sm->GetTotalObjectsFaces());
  CAnimatedModelManager* am=CORE->GetAnimatedModelManager();
  fm->DrawTextA(10,155,colYELLOW,l_iFontId,"Num of Faces (Dinamic Meshes): %ld",am->GetTotalNumFaces());*/

}

///<summary>
/// CMainProcess:: CheckActionsFromInputs : Según una entrada de teclado ejecuta su acción asociada.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CMainProcess::CheckActionsFromInputs(float elapsedTime)
{
/*  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ProcessInfo")) != 0.0f )
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
    CSceneEffectManager *sem = CORE->GetSceneEffectManager();
		//sem->Destroy();
    sem->LoadXML("Data/scene_effects.xml");
  }
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

	//////////////////////////////////////////////////////////////////////////////
	// Sound-related actions

	//TestSound
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("TestSound")) != 0.0f )
	{
		CORE->GetSoundManager()->TestSound();
	}
	//////////////////////////////////////////////////////////////////////////////
*/





	////ScreenShot
  //if(GetInputManager()->GetActionToInput()->DoAction("SaveLogsInFile") != 0.0f)
  //{
  //  CRenderManager *rm = CORE->GetRenderManager();
  //  rm->GetWindowRect()
  //}
  //CAMERAS
  /*CCamera * l_CameraTemp;
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera0")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera0");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera1")) != 0.0f )
	{

    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera1");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera2")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera2");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera3")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera3");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera4")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera4");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera5")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera5");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera6")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera6");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera7")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera7");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera8")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera8");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Camera9")) != 0.0f )
	{
    l_CameraTemp = CORE->GetCameraManager()->GetCameraByName("Camera9");
    CORE->GetCameraManager()->SetCamera(l_CameraTemp);
	}*/
}

