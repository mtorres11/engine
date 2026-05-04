#include "ProcessParticles.h"
#include "Core.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/Particle.h"
#include "Particles/ParticleManager.h"
#include "RenderManager.h"
#include "Texture/Texture.h"
#include <string>
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
#include "Math/Matrix44.h"
#include "Math/MathUtils.h"
#include "Cameras/Camera.h"
#include "Cameras/FPSCamera.h"
#include "Cameras/ThPSCamera.h"
#include "Cameras/CameraManager.h"
#include "Object3D/Object3D.h"
#include "Vertex/IndexedVertex.h"
#include "Vertex/VertexType.h"
#include "ActionToInput.h"
#include "RenderableObjects/RenderableObject.h"
#include "InputManager.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "Lights/LightManager.h"
#include "Shaders/EffectManager.h"
#include "Shaders/SceneEffectManager.h"
#include "RenderableObjects/RenderableObject.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "AnimationModel/AnimatedCoreModel.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "RecyclingArray/RecyclingArray.h"
#include "Logger/Logger.h"
#include "ScriptManager.h"
#include "Lights/Light.h"
#include "GUIManager.h"
#include "SoundManager.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CProcessParticles::CProcessParticles()
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
	m_MovementSpeed = 4.0f;
  m_RunTime = 2.0f;
  m_RestTime = 3.0f;
  m_DeltaFromXML = 0.0;
  m_SceneEffectManager=0;
}

CProcessParticles::~CProcessParticles()
{

}

///<summary>
/// CProcessParticles:: Init : Lectura del XML. Creación y asignación de las partículas.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CProcessParticles::Init ()
{
	CORE->GetScriptManager()->RunFile("Data/LUAScripts/variables.lua");
	CORE->GetScriptManager()->RunFile("Data/LUAScripts/init.lua");
  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
	LoadXMLs(); 
	InitCameras();

	//Sin XML
	//m_ParticleEmitter.Init(CORE->GetRenderManager(), "./Data/Textures/Punto_Alfa.dds");
	//CORE->GetGUIManager()->ActiveWindowsWithEffect("Game.xml", TE_SHADOW, 10.0f);
	//Con XML
	CORE->GetParticleManager()->Init(CORE->GetRenderManager());
}

///<summary>
/// CProcessParticles:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name="void"></returns>
void CProcessParticles::RenderScene(CRenderManager* rm) 
{
	Render3D(rm);
	CORE->GetParticleManager()->Render();
	Render2D(rm);
	RenderDebugInfo();
}

void CProcessParticles::RenderDebugInfo()
{
	//Mostramos la información sobre las partículas:
  CProcess::RenderDebugInfo();

  CFontManager* fm = CORE->GetFontManager();
  CTimer *t = CORE->GetTimer();
  uint32 l_iFontId = fm->GetTTF_Id("arial");

	//fm->DrawTextA (550,60,colWHITE,l_iFontId,"Tiempo transcurrido: %f",t->GetTotalTime());
	//fm->DrawTextA (550,80,colWHITE,l_iFontId,"Número máximo partículas: %i",m_ParticleEmitter.GetTotalElements());
  //fm->DrawTextA (550,100,colWHITE,l_iFontId,"Partículas vivas: %i",m_ParticleEmitter.GetUsedElements());
  //fm->DrawTextA (550,120,colWHITE,l_iFontId,"Partículas muertas: %i",m_ParticleEmitter.GetFreeElements());
}

void CProcessParticles::PreRender(CRenderManager* rm)
{

}

///<summary>
/// CProcessParticles:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name="void"></returns>
void CProcessParticles::Update(float elapsedTime)
{
	CheckActionsFromInputs(elapsedTime);
	MovMouse(); 
	UpdateLua(); 
	MovPlayer(elapsedTime); 

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
  //sin XML
	//m_ParticleEmitter.Update(elapsedTime);

	//con XML
	CORE->GetParticleManager()->Update(elapsedTime);
}

///<summary>
/// CProcessParticles:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name="void"></returns>
void CProcessParticles::Render(CRenderManager* rm) 
{
  CEffectManager *l_EffectManager=CORE->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);

	//Render escena
  RenderScene(rm);

	//Render partículas (con XML)
}





//-------------------FUNCIONES PROPIAS DE PROCESS PARTICLES------------------------//

///<summary>
/// CProcessParticles:: LoadXMLs : Carga todos los XML.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessParticles::LoadXMLs()
{
  CORE->GetEffectManager()->Load("Data/Effects.xml");
  CORE->GetStaticMeshManager()->Load("Data/StaticMeshManager.xml");
  CORE->GetLigthManager()->Load("Data/Lights.xml");
  CORE->GetAnimatedModelManager()->Load("Data/AnimatedModels.xml");
  CORE->GetRenderableObjectsManager()->Load("Data/RenderableObjects.xml");
	CORE->GetParticleManager()->LoadXML("Data/Particles.xml");
	//CORE->GetGUIManager()->Init("Data/GUI/initParticles.xml");
}

///<summary>
/// CProcessIA:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessParticles::Render3D(CRenderManager* rm)
{
	// render manager
  CRenderableObjectsManager *renderObjectsManager = CORE->GetRenderableObjectsManager();
  renderObjectsManager->Render(rm);
  Mat44f matWorld;
  matWorld.SetIdentity();
  rm->SetTransform(matWorld);
  rm->DrawAxis(10);

	// luces
  CORE->GetLigthManager()->Render(rm);

	// cámaras
  CORE->GetCameraManager()->DrawCameras();
}

///<summary>
/// CGameProcess_t:: Render2D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessParticles::Render2D(CRenderManager* rm)
{
	Mat44f l_Matrix;
  l_Matrix.SetIdentity();
  rm->SetTransform(l_Matrix);
  l_Matrix.SetIdentity();
  rm->SetTransform(l_Matrix);
}

void CProcessParticles::CheckActionsFromInputs(float elapsedTime)
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
    CSceneEffectManager *sem = m_SceneEffectManager;
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
	//Particles
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ReloadXMLParticles")) != 0.0f )
  {
		CParticleManager * l_pManag = CORE->GetParticleManager();
    l_pManag->Release();
		l_pManag->LoadXML("Data/Particles.xml");
		l_pManag->Init(CORE->GetRenderManager());
  }
	if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("StopParticles")) != 0.0f )
  {
		CParticleManager * l_pManag = CORE->GetParticleManager();
		l_pManag->Release();
  }
}

void CProcessParticles::MovPlayer(float elapsedTime)
{
	Vect3f l_vPos = GetPositionObj(CORE->GetCameraManager()->GetCamera()->GetObject3D()); //posición objeto

	//DIAGONAL MOVEMENTS
	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardLeft")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/4)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/4)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		//m_pPlayer->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardRight")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/4)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/4)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		//m_pPlayer->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsRight")) != 0.0f )
	{
		l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/4)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/4)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		//m_pPlayer->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsLeft")) != 0.0f )
	{
		l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/4)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/4)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		//m_pPlayer->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	//UP DOWN LEFT RIGHT MOVEMENTS
	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForward")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		//m_pPlayer->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwards")) != 0.0f )
	{
		l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		//m_pPlayer->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardLeft")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		//m_pPlayer->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardRight")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		////m_pPlayer->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoUp")) != 0.0f )
	{
		l_vPos.y += elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoDown")) != 0.0f )
	{
		l_vPos.y -= elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("PlayerRun")) != 0.0f )
	{
		m_MovementSpeed = 15;
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("PlayerQuiet")) != 0.0f )
	{
		m_MovementSpeed = 2.5;
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("PlayerNormalSpeed")) != 0.0f )
	{
		m_MovementSpeed = 5;
	}
}

Vect3f CProcessParticles::GetPositionObj(CObject3D * p_vObj)
{
  //Moverse a través del escenario. Fijamos la cámara al objeto
	Vect3f l_vPos = p_vObj->GetPosition(); 
	return l_vPos;
}


void CProcessParticles::InitCameras()
{
	//Bots
	m_pPlayer = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
}

void CProcessParticles::MovMouse()
{
	//Vect3i deltas = CORE->GetInputManager()->GetMouseDelta();
	//CThPSCamera * cam = (CThPSCamera*) CORE->GetCameraManager()->GetCamera();;
  //cam->SetZoom( cam->GetZoom() + deltas.z * -0.02f);
}

void CProcessParticles::UpdateLua()
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
}

