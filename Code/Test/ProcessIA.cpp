#include "Core.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
#include "ProcessIA.h"
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

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

#define RADIANS 0.018f

CProcessIA::CProcessIA()
{
  m_fSpeed1 = 0.0;
  m_fSpeed2 = 0.0;
  m_bChaser = false;
  m_V3DirectorPlayer2.SetZero();
  m_pPlayer1=NULL;
  m_pPlayer2=NULL;
  m_fAngleParcialPlayer2=0.0;
  m_fAngleTotalPlayer2=0.0;
  m_V3RightPlayer2.SetZero();
  m_V3UpPlayer2.SetZero();
  m_SceneEffectManager=0;
}

CProcessIA::~CProcessIA()
{
  //CHECKED_DELETE(m_pPlayer1);
  //CHECKED_DELETE(m_pPlayer2);
}

///<summary>
/// CProcessIA:: Init : Lectura de los XML. Creación y asignación de las cámaras a los objetos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessIA::Init ()
{
  
  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
  m_SceneEffectManager = CORE->GetSceneEffectManager();//new CSceneEffectManager();
	LoadXMLs(); 

  CORE->GetCameraManager()->SetCamera(CORE->GetCameraManager()->GetCameraByName("Camera0"));
  //Bots
	m_pPlayer1 = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
  m_pPlayer2 = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo2");
 
  m_bChaser = true;
}

///<summary>
/// CProcessIA:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessIA::RenderScene(CRenderManager* rm) 
{
  /*PhysX test*/
	//CORE->Render();
	Render3D(rm); 
	Render2D(rm); 
}

///<summary>
/// CProcessIA:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CProcessIA::Update(float elapsedTime)
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

  MovPlayerIA(elapsedTime);
}

///<summary>
/// CProcessIA:: PreRender : Prerenderización de efectos.
///</summary>
///<param name="rm">RenderManager.</param>
///<returns name=""></returns>

void CProcessIA::PreRender(CRenderManager* rm) 
{
	m_SceneEffectManager->PreRender(rm,this);
}

///<summary>
/// CProcessIA:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessIA::Render(CRenderManager* rm) 
{
  //eje principal
  Mat44f matWorld;
  matWorld.SetIdentity();
  rm->SetTransform(matWorld);
  rm->DrawAxis(10);

  //mueve eje de IA
  RenderMovIA(rm);

  m_SceneEffectManager->ActivateRenderSceneEffects();

  CEffectManager *l_EffectManager=CORE->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);
  
  //m_SceneEffectManager->PreRender(rm,this);
  //Renderizamos la escena
  RenderScene(rm);

  m_SceneEffectManager->CaptureFrameBuffers(rm);
  m_SceneEffectManager->PostRender(rm);
  m_SceneEffectManager->CaptureFrameBuffersAfterPostRender(rm);

  RenderDebugInfo(); 
}
//---------------------------------------------------------------------------------//
//-------------------FUNCIONES PROPIAS DE GAMEPROCESS_TEST-------------------------//
//---------------------------------------------------------------------------------//

///<summary>
/// CProcessIA:: LoadXMLs : Carga todos los XML.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessIA::LoadXMLs()
{
	//Load XML
  CCore * l_pCore = CORE;
  l_pCore->GetEffectManager()->Load("Data/Effects.xml");
  l_pCore->GetStaticMeshManager()->Load("Data/StaticMeshManager.xml");
  l_pCore->GetLigthManager()->Load("Data/lights.xml");
  l_pCore->GetAnimatedModelManager()->Load("Data/AnimatedModels.xml");
  l_pCore->GetRenderableObjectsManager()->Load("Data/RenderableObjects.xml");
  //l_pCore->GetCameraManager()->Load("Data/Cameras.xml");
  m_SceneEffectManager->LoadXML("Data/scene_effects.xml");
  //l_pCore->GetGUIManager()->Init("Data/GUI/initGui_laberynth.xml");
}

///<summary>
/// CProcessIA:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessIA::Render3D(CRenderManager* rm)
{
	// render manager
  CRenderableObjectsManager *renderObjectsManager = CORE->GetRenderableObjectsManager();
  renderObjectsManager->Render(rm);

  // luces
  CORE->GetLigthManager()->Render(rm);

	// cámaras
//  CORE->GetCameraManager()->DrawCameras();
	//CORE->GetRenderManager()->DrawCamera(CORE->GetCameraManager()->GetCamera());
}

///<summary>
/// CProcessIA:: Render3D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessIA::Render2D(CRenderManager* rm)
{
}

///<summary>
/// CProcessIA:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessIA::RenderDebugInfo()
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
  fm->DrawTextA(10,175,colYELLOW,l_iFontId,"radians player2: %f degrees player2: %f", m_fAngleTotalPlayer2, m_fAngleTotalPlayer2*(180/ePIf));
}

///<summary>
/// CProcessIA:: CheckActionsFromInputs : Según una entrada de teclado ejecuta su acción asociada.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CProcessIA::CheckActionsFromInputs(float elapsedTime)
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

///<summary>
/// CProcessIA:: MovPlayer : Calcula los movimientos y la velocidad del player.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CProcessIA::MovPlayer(float elapsedTime)
{
  Vect3f l_vPos = CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetPosition(); //posición objeto

	//DIAGONAL MOVEMENTS
	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardLeft")) != 0.0f )
	{
    l_vPos.x = l_vPos.x - /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		l_vPos.z = l_vPos.z + /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		m_pPlayer1->SetPosition(l_vPos);
    m_pPlayer1->SetYaw(0.8f);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardRight")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		l_vPos.z = l_vPos.z + /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		m_pPlayer1->SetPosition(l_vPos);
		m_pPlayer1->SetYaw(-0.5f);
    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsRight")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		l_vPos.z = l_vPos.z - /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		m_pPlayer1->SetPosition(l_vPos);
		m_pPlayer1->SetYaw(-2.5f);
    //CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsLeft")) != 0.0f )
	{
		l_vPos.x = l_vPos.x - /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		l_vPos.z = l_vPos.z - /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		m_pPlayer1->SetPosition(l_vPos);
    m_pPlayer1->SetYaw(2.5f);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	//UP DOWN LEFT RIGHT MOVEMENTS
	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForward")) != 0.0f )
	{
		l_vPos.z = l_vPos.z + /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		m_pPlayer1->SetPosition(l_vPos);
    m_pPlayer1->SetYaw(0.0f);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwards")) != 0.0f )
	{
		l_vPos.z = l_vPos.z - /*sin(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		m_pPlayer1->SetPosition(l_vPos);
    m_pPlayer1->SetYaw(3.0f);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardLeft")) != 0.0f )
	{
		l_vPos.x = l_vPos.x - /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		m_pPlayer1->SetPosition(l_vPos);
    m_pPlayer1->SetYaw(1.3f);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardRight")) != 0.0f )
	{
    l_vPos.x = l_vPos.x + /*cos(CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetYaw())*/elapsedTime*m_fSpeed1;
		//CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
		m_pPlayer1->SetPosition(l_vPos);
    m_pPlayer1->SetYaw(-1.5f);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
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
//		m_MovementSpeed = 15;
		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}
}
  
void CProcessIA::MovCamera()
{
  //CAMERAS
  CCamera * l_CameraTemp;
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
	}
}

void CProcessIA::MovPlayerIA(float _elapsedtime)
{
  Vect3f l_V3PosPlayer1 = m_pPlayer1->GetPosition();
  Vect3f l_V3PosPlayer2 = m_pPlayer2->GetPosition();

  //se obtiene el vector director optimo normalizado
  m_V3DirPlayer2Optimo = l_V3PosPlayer1 - l_V3PosPlayer2;
  m_V3DirPlayer2Optimo = m_V3DirPlayer2Optimo.GetNormalized();

  //en el primer scan la direccion es la optima
//  float m_fAnglePlayer2 = 0.0f;
  if(_elapsedtime == 0.0)
  {
     m_V3DirectorPlayer2 = m_V3DirPlayer2Optimo;
     m_fAngleParcialPlayer2 = m_V3DirPlayer2Optimo.GetAngleY();
     m_fAngleTotalPlayer2 = m_V3DirPlayer2Optimo.GetAngleY();
  }

  //eje del player2
  //obtiene el vector director real
  m_fAngleParcialPlayer2 = AjusteDireccion();
  m_fAngleTotalPlayer2 += m_fAngleParcialPlayer2;

  m_V3DirectorPlayer2.RotateY(m_fAngleParcialPlayer2);
  m_V3UpPlayer2(l_V3PosPlayer2.x, 1.0f, l_V3PosPlayer2.z);
  m_V3RightPlayer2 =  m_V3DirectorPlayer2.GetRotatedY( ePI2f );
  m_V3RightPlayer2.y = 0.0;

  if(m_bChaser)
  {
    m_fSpeed1 = 2.0f;
    m_fSpeed2 = 1.8f;
    
    l_V3PosPlayer2.x += m_V3DirectorPlayer2.x * _elapsedtime * m_fSpeed2;
    l_V3PosPlayer2.z += m_V3DirectorPlayer2.z * _elapsedtime * m_fSpeed2;
  }
  else
  {
    m_fSpeed1 = 2.5f;
    m_fSpeed2 = 1.0f;
    
    l_V3PosPlayer2.x -= m_V3DirectorPlayer2.x * _elapsedtime * m_fSpeed2;
    l_V3PosPlayer2.z -= m_V3DirectorPlayer2.z * _elapsedtime * m_fSpeed2;
  }
  //posicion y rotacion del muñeco
  m_pPlayer2->SetPosition(Vect3f(l_V3PosPlayer2.x,0,l_V3PosPlayer2.z));
  m_pPlayer2->SetYaw(-m_fAngleTotalPlayer2);

  //separacion al colisionar (sin PhysX)
  if((abs(l_V3PosPlayer1.x - l_V3PosPlayer2.x) < 0.1f) && (abs(l_V3PosPlayer1.z - l_V3PosPlayer2.z) < 0.1f))
  {
    m_bChaser = !m_bChaser;
    l_V3PosPlayer1.x -= (float)0.2;
    l_V3PosPlayer1.z -= (float)0.2;
    l_V3PosPlayer2.x += (float)0.2;
    l_V3PosPlayer2.z += (float)0.2;
  }
}

float CProcessIA::AjusteDireccion()
{
  Vect3f myDir = m_V3DirectorPlayer2;
  Vect3f myRight = m_V3RightPlayer2;

  if( (m_V3DirPlayer2Optimo * myDir) < 0 )//player esta detras, GIRARA MAS RAPIDO si el producto escalar es negativo.
  {
    if(m_V3DirPlayer2Optimo * myRight < 0)//player esta a la izquierda
      return -3*RADIANS;
    else    //player esta a la derecha
      return +3*RADIANS;
  }
  else  //player esta delante
  {
    if(m_V3DirPlayer2Optimo * myRight < 0)//player esta a la izquierda
      return -RADIANS;
    else    //player esta a la derecha
	  return +RADIANS;
  }
  
}

void CProcessIA::RenderMovIA(CRenderManager *rm)
{
  Vect3f l_V3PosPlayer1 = m_pPlayer1->GetPosition();
  Vect3f l_V3PosPlayer2 = m_pPlayer2->GetPosition();

  Mat44f vecOptimo;
  vecOptimo.SetIdentity();
  rm->SetTransform(vecOptimo);
  rm->DrawLine(l_V3PosPlayer2, l_V3PosPlayer1,colWHITE);
  
  Mat44f AxisPlayer2;
  AxisPlayer2.SetIdentity();
  rm->SetTransform(AxisPlayer2);
  rm->DrawLine(l_V3PosPlayer2, l_V3PosPlayer2 + m_V3DirectorPlayer2, colRED);
  rm->DrawLine(l_V3PosPlayer2, l_V3PosPlayer2 + m_V3RightPlayer2, colYELLOW);
  rm->DrawLine(l_V3PosPlayer2, m_V3UpPlayer2, colGREEN);
}

///<summary>
/// CProcessIA:: GetPositionObj : Nos devuelve un Vect3f con la posición del objeto.
///</summary>
///<param name="p_vObj">Objeto del que vamos a obtener la posición.</param>
///<returns name="posicion">Vector de 3 floats con la posición del objeto.</returns>
Vect3f CProcessIA::GetPositionObj(CObject3D * p_vObj)
{
  //Moverse a través del escenario. Fijamos la cámara al objeto
	Vect3f l_vPos = p_vObj->GetPosition(); 
	return l_vPos;
}

///<summary>
/// CProcessIA:: InitCameras : Crea las cámaras y las asocia a un objeto.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessIA::InitCameras()
{
}

///<summary>
/// CProcessIA:: MovMouse : Movimiento del ratón asociado a la cámara.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessIA::MovMouse()
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

///<summary>
/// CProcessIA:: UpdateLua : Scripts de LUA.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessIA::UpdateLua()
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

///<summary>
/// CProcessIA:: MemoryTest : Pruebas para grabar en RAM o VRAM.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessIA::MemoryTest()
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