#include "Core.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
#include "ProcessPhysX.h"
#include "Math/Matrix44.h"
#include "Math/MathUtils.h"
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
#include "RenderablePhysX/RenderablePhysXManager.h"
#include "RenderablePhysX/RenderablePhysXObject.h"
#include "CharacterController/PhysicController.h"
#include "Joints/Fixed/PhysicFixedJoint.h"
#include "GUIManager.h"
#include "SoundManager.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CPhysicUserData * g_ClosestActorPX = 0;
std::string l_InstanceNamePX = "Box01";
CPhysicController * g_PhysXControllerPX = 0;
CPhysicUserData * g_UserDataControllerPX;
CPhysicFixedJoint* g_JointPX;


CProcessPhysX::CProcessPhysX()
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
	m_MovementSpeed = 4.0f;//TODO: La velocidad de caminata normal de una persona es de 0,92 mts por segundo. Estamos moviendonos a 4. Hay que cambiarlo
  m_RunTime = 2.0f;
  m_RestTime = 3.0f;
  m_DeltaFromXML = 0.0;
  m_ray = "";
  m_class = "";
}

CProcessPhysX::~CProcessPhysX()
{
  //CHECKED_DELETE(m_SceneEffectManager);
  CHECKED_DELETE(g_PhysXControllerPX);
  CHECKED_DELETE(g_UserDataControllerPX);
  CHECKED_DELETE(g_JointPX)
}

///<summary>
/// CGameProcess_t:: Init : Lectura de los XML. Creación y asignación de las cámaras a los objetos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessPhysX::Init ()
{
  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
	LoadXMLs(); 
	InitCameras();
	CORE->GetScriptManager()->LoadFile("./Data/LUAScripts/variables.lua");
	CORE->GetScriptManager()->LoadFile("./Data/LUAScripts/init.lua");
	CORE->GetGUIManager()->ActiveWindowsWithEffect("Game.xml", TE_SHADOW, 10.0f);
  CORE->GetGUIManager()->ActiveWindowsWithEffect("Game.xml", TE_SHADOW, 10.0f);

  CORE->GetGUIManager()->SetVisiblePointerMouse(true);
  CORE->GetScriptManager()->RunFile("Data/LUAScripts/init.lua");

	/*PhysX test*/
  //Plano del piso
	/*CPhysicUserData * plano = new CPhysicUserData("plano");
	CPhysicActor * l_plano_test = new CPhysicActor(plano);
	l_plano_test->AddPlaneShape(Vect3f(0,1,0), 0.0f, 0);
	CORE->GetPhysicsManager()->AddPhysicActor(l_plano_test);*/
  //End Plano

  //Funciona hasta (*)
  /*CRenderableObject * l_Object3D = CORE->GetRenderableObjectsManager()->GetInstance(l_InstanceNamePX);

  Vect3f l_Size = l_Object3D->GetBoundingObject()->GetDimension();
  Vect3f l_positionObject3D = l_Object3D->GetPosition();
  Vect3f l_position = l_Object3D->GetBoundingObject()->GetMiddlePoint();
  l_position.x = 0.0f;
  l_position.z = 0.0f;

  l_PhysXTest = new CRenderablePhysXObject(l_InstanceNamePX);
  l_PhysXTest->GetPhysXActor()->AddBoxSphape(l_Size,l_position,0,0);
  l_PhysXTest->GetPhysXActor()->SetGlobalPosition(l_positionObject3D);
  l_PhysXTest->GetPhysXActor()->CreateBody(0.5f,0.0);
  CORE->GetPhysicsManager()->AddPhysicActor(l_PhysXTest->GetPhysXActor());
  
  Mat44f l_MatrixToPhysX = l_Object3D->GetMat44();
  l_PhysXTest->GetPhysXActor()->SetMat44(l_MatrixToPhysX);
  */
  //(*)

  //Mat44f l_MatrixToPhysX, l_Rotation, l_Translation;
  //l_Translation.SetIdentity();
  //l_Rotation.SetIdentity();
  //l_Rotation.SetFromPitchRollYaw(Vect3f(m_fPitch, m_fYaw, m_fRoll)); //Revisar porque el orden de los parametros
  //l_Rotation.SetFromPitchRollYaw(Vect3f(l_Object3D->GetPitch(), l_Object3D->GetYaw(), l_Object3D->GetRoll()));
  //l_Rotation.SetFromPitchRollYaw(Vect3f(l_Object3D->GetPitch(), l_Object3D->GetRoll(), l_Object3D->GetYaw()));
  //l_Rotation.SetFromPitchRollYaw(Vect3f(l_Object3D->GetYaw(), l_Object3D->GetPitch(), l_Object3D->GetRoll()));
  //l_Rotation.SetFromPitchRollYaw(Vect3f(l_Object3D->GetYaw(), l_Object3D->GetRoll(), l_Object3D->GetPitch()));
  //l_Rotation.SetFromPitchRollYaw(Vect3f(l_Object3D->GetRoll(), l_Object3D->GetPitch(), l_Object3D->GetYaw()));
  //l_Rotation.SetFromPitchRollYaw(Vect3f(l_Object3D->GetRoll(), l_Object3D->GetYaw(), l_Object3D->GetPitch()));
  //l_Translation.SetIdentity();
  //l_Translation.Translate(l_PhysXTest->GetPhysXActor()->GetPosition());
  //l_MatrixToPhysX = l_Translation * l_Rotation;

  //l_PhysXTest->GetPhysXActor()->SetMat44(l_translation);

	//CRenderableObject * l_object = CORE->GetRenderableObjectsManager()->GetInstance("BoxBumpMap");

  //asociarle física a un renderable object
	//CPhysicUserData * box = new CPhysicUserData("box");
	//CPhysicActor * l_box_test = new CPhysicActor(box);
  //l_box_test->AddBoxSphape(10.0f,l_object->GetPosition());
	////creamos la mat44f con los valores del renderable object
	///*CRenderableObject * */l_object = CORE->GetRenderableObjectsManager()->GetInstance("BoxBumpMap");
	////Mat44f& matrix = Mat44f(l_object->GetPitch(),l_object->GetYaw(),l_object->GetRoll(),Vect3f(l_object->GetPosition()));
	////l_box_test->GetMat44(matrix);
	//l_box_test->CreateBody(0.5,0.0);
	//CORE->GetPhysicsManager()->AddPhysicActor(l_box_test);
	//
	//CPhysicUserData * esfera = new CPhysicUserData("esfera");
	//CPhysicActor * l_esfera_test = new CPhysicActor(esfera);
	//l_esfera_test->AddSphereShape(0.5,Vect3f(0,50,0));
 // l_esfera_test->AddSphereShape(0.5,Vect3f(1,50,1));
 // l_esfera_test->AddSphereShape(0.5,Vect3f(1,50,0));
 // l_esfera_test->AddSphereShape(0.5,Vect3f(0,50,1));
 // //l_esfera_test->AddBoxSphape(Vect3f(1.0f,1.0f,1.0f),Vect3f(0.5,50,0.5));

	//l_esfera_test->CreateBody(0.5,0);
	//CORE->GetPhysicsManager()->AddPhysicActor(l_esfera_test);

  //Controller
  g_UserDataControllerPX = new CPhysicUserData("Controller");
  g_UserDataControllerPX->SetPaint(true);
  g_UserDataControllerPX->SetColor(colWHITE);

  g_PhysXControllerPX = new CPhysicController(0.5f,1.8f,2.0f,0.1f,2.0f,1,g_UserDataControllerPX,Vect3f(5.0f,50.0f,5.0f));
  g_PhysXControllerPX->SetYaw(-90);
  CORE->GetPhysicsManager()->AddPhysicController(g_PhysXControllerPX);

  CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(g_PhysXControllerPX->GetPosition());

  //CPhysicActor * g_Box01 = CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject("Box01")->GetPhysXActor();
  //CPhysicActor * g_Box02 = CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject("Box04")->GetPhysXActor();
  //g_Joint = new CPhysicFixedJoint();
  //g_Joint->SetInfo(g_Box01,g_Box02);
  //CORE->GetPhysicsManager()->AddPhysicFixedJoint(g_Joint);


}

///<summary>
/// CGameProcess_t:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessPhysX::RenderScene(CRenderManager* rm) 
{
  CORE->Render();
	Render3D(rm); 
	Render2D(rm); 
  RenderDebugInfo(); 

	//prueba
  //CORE->GetScriptManager()->RunCode("lua_draw_literal_test()");
	CORE->GetScriptManager()->RunCode("get_core():get_font_manager():lua_draw_literal(100, 100, 'Exit')");
}

///<summary>
/// CGameProcess_t:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CProcessPhysX::Update(float elapsedTime)
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
  CORE->GetRenderablePhysXManager()->Update(elapsedTime);

  /*CRenderableObject * l_Object3D = CORE->GetRenderableObjectsManager()->GetInstance(l_InstanceNamePX);
  Vect3f l_PhysXPosition = l_PhysXTest->GetPhysXActor()->GetPosition();

  Mat44f l_MatrixPhysX;
  l_PhysXTest->GetPhysXActor()->GetMat44(l_MatrixPhysX);
  l_Object3D->SetMat44(l_MatrixPhysX);*/

  //l_PhysXTest->GetPhysXActor()->SetMat44(l_Translation);

  //l_PhysXTest->Update(CORE->GetRenderManager());

  //PhysXController
  g_PhysXControllerPX->Move(Vect3f(0.0f,0.0f,0.0f),elapsedTime);
}

///<summary>
/// CGameProcess_t:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessPhysX::Render(CRenderManager* rm) 
{
  //m_SceneEffectManager->ActivateRenderSceneEffects();

  CEffectManager *l_EffectManager=CORE->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);

  //Renderizamos la escena
  RenderScene(rm);
  rm->DrawAxis(10);

  CPhysicsManager* l_PhysManager = CORE->GetPhysicsManager();
   l_PhysManager->DebugRender(rm);

  //m_SceneEffectManager->CaptureFrameBuffers(rm);
  //m_SceneEffectManager->PostRender(rm);
  //m_SceneEffectManager->CaptureFrameBuffersAfterPostRender(rm);

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
/// CGameProcess_t:: LoadXMLs : Carga todos los XML.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessPhysX::LoadXMLs()
{
	//Load XML
  CCore * l_pCore = CORE;
  l_pCore->GetEffectManager()->Load("Data/Effects.xml");
  l_pCore->GetStaticMeshManager()->Load("Data/StaticMeshManager.xml");
  l_pCore->GetLigthManager()->Load("Data/lights.xml");
  l_pCore->GetAnimatedModelManager()->Load("Data/AnimatedModels.xml");
  l_pCore->GetRenderableObjectsManager()->Load("Data/RenderableObjects.xml");
  l_pCore->GetCameraManager()->Load("Data/Cameras.xml");
  //m_SceneEffectManager = new CSceneEffectManager();
  //m_SceneEffectManager->LoadXML("Data/scene_effects.xml");
  //l_pCore->GetGUIManager()->Init("Data/GUI/initGui_laberynth.xml");
}

///<summary>
/// CGameProcess_t:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessPhysX::Render3D(CRenderManager* rm)
{
	// render manager
  CRenderableObjectsManager *renderObjectsManager = CORE->GetRenderableObjectsManager();
  renderObjectsManager->Render(rm);

  // luces
  CORE->GetLigthManager()->Render(rm);

	// cámaras
  CORE->GetCameraManager()->DrawCameras();
	//CORE->GetRenderManager()->DrawCamera(CORE->GetCameraManager()->GetCamera());

}

///<summary>
/// CGameProcess_t:: Render3D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CProcessPhysX::Render2D(CRenderManager* rm)
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
/// CGameProcess_t:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessPhysX::RenderDebugInfo()
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
  //fm->DrawTextA(10,185,colYELLOW,l_iFontId,"Disparando un Rayo a: %s de clase %s",m_ray,m_class);
  fm->DrawTextA(10,185,colYELLOW,l_iFontId,m_ray.c_str());
}

///<summary>
/// CGameProcess_t:: CheckActionsFromInputs : Según una entrada de teclado ejecuta su acción asociada.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CProcessPhysX::CheckActionsFromInputs(float elapsedTime)
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
    //CSceneEffectManager *sem = m_SceneEffectManager;
		//sem->Destroy();
    //sem->LoadXML("Data/scene_effects.xml");
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
	////ScreenShot
  //if(GetInputManager()->GetActionToInput()->DoAction("SaveLogsInFile") != 0.0f)
  //{
  //  CRenderManager *rm = CORE->GetRenderManager();
  //  rm->GetWindowRect()
  //}
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
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ShutDeBolTuMi")) != 0.0f )
	{
		/*CPhysicUserData * esfera = new CPhysicUserData("clon");
    esfera->SetPaint(true);
    esfera->SetColor(colYELLOW);
    CPhysicActor * l_esfera_test = new CPhysicActor(esfera);
    Vect3f l_position = CORE->GetCameraManager()->GetCamera()->GetEye();
    l_esfera_test->AddSphereShape(0.5,l_position);
    l_esfera_test->CreateBody(300.0f,0);
    CORE->GetPhysicsManager()->AddPhysicActor(l_esfera_test);
    Vect3f l_VectorDirector = CORE->GetCameraManager()->GetCamera()->GetDirection();
    l_esfera_test->SetLinearVelocity(l_VectorDirector*20);*/
	}
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Ray")) != 0.0f )
	{
    const Vect3f pos = CORE->GetCameraManager()->GetCamera()->GetEye();
    Vect3f& dir = CORE->GetCameraManager()->GetCamera()->GetDirection();
    SCollisionInfo info;
    g_ClosestActorPX = CORE->GetPhysicsManager()->RaycastClosestActor(pos,dir,1,g_ClosestActorPX,info);
    m_ray = g_ClosestActorPX->GetName();
  }
}

///<summary>
/// CGameProcess_t:: MovPlayer : Calcula los movimientos y la velocidad del player.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CProcessPhysX::MovPlayer(float elapsedTime)
{
	Vect3f l_vPos = GetPositionObj(CORE->GetCameraManager()->GetCamera()->GetObject3D()); //posición objeto

	//DIAGONAL MOVEMENTS

  /*
    Hay que pedirle al object3d el yaw y el vector director es DIR(cos(yaw), 0, sen(yaw))
    IZQ(cos(Yaw-PI/2), 0, sen(Yaw-PI/2))
    IZQ(cos(Yaw-PI/4), 0, sen(Yaw-PI/4))
    En este momento le pasamos la posicion del object3d, Para pasarle el vector director hacemos lo mismo pero con el yaw del object3d
    y no con la pos

    Attachar el object3d al controlador.
    Hacer calculo de posicion con el vector director al controlador y luego asignar la pos del controlador al object3d de la camara.
  */
  Vect3f l_VectorDirector;
  CCamera * l_ActualCamera = CORE->GetCameraManager()->GetCamera();
  CObject3D * l_CameraObject = l_ActualCamera->GetObject3D();
  float l_ActualYaw = l_CameraObject->GetYaw();
  

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardLeft")) != 0.0f )
	{
    l_VectorDirector.x = cos(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw+FLOAT_PI_VALUE/4);
    g_PhysXControllerPX->Move(l_VectorDirector, elapsedTime);
    l_CameraObject->SetPosition(g_PhysXControllerPX->GetPosition());
		//m_pPlayer->SetPosition(l_vPos);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardRight")) != 0.0f )
	{

    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw-FLOAT_PI_VALUE/4);
    g_PhysXControllerPX->Move(l_VectorDirector, elapsedTime);
    l_CameraObject->SetPosition(g_PhysXControllerPX->GetPosition());
		//m_pPlayer->SetPosition(l_vPos);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsRight")) != 0.0f )
	{
    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw+FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw+FLOAT_PI_VALUE/4);
    g_PhysXControllerPX->Move((l_VectorDirector*-1), elapsedTime);
    l_CameraObject->SetPosition(g_PhysXControllerPX->GetPosition());
		//m_pPlayer->SetPosition(l_vPos);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsLeft")) != 0.0f )
	{
    Vect3f l_VectorDirector;
    l_VectorDirector.x = cos(l_ActualYaw-FLOAT_PI_VALUE/4);
    l_VectorDirector.y = 0.0f;
    l_VectorDirector.z = sin(l_ActualYaw-FLOAT_PI_VALUE/4);
    g_PhysXControllerPX->Move((l_VectorDirector*-1), elapsedTime);
    l_CameraObject->SetPosition(g_PhysXControllerPX->GetPosition());
		//m_pPlayer->SetPosition(l_vPos);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	//UP DOWN LEFT RIGHT MOVEMENTS
	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForward")) != 0.0f )
	{
		/*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);*/
    
    Vect3f l_VectorDirector = l_ActualCamera->GetDirection().GetNormalized();
    l_VectorDirector.y = 0.0f;
    g_PhysXControllerPX->Move(l_VectorDirector, elapsedTime);
    l_CameraObject->SetPosition(g_PhysXControllerPX->GetPosition());
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwards")) != 0.0f )
	{
		/*l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);*/
    
    Vect3f l_VectorDirector = l_ActualCamera->GetDirection().GetNormalized();
    l_VectorDirector.y = 0.0f;
    g_PhysXControllerPX->Move((l_VectorDirector*-1), elapsedTime);
    l_CameraObject->SetPosition(g_PhysXControllerPX->GetPosition());
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardLeft")) != 0.0f )
	{
		/*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);*/
    
    Vect3f l_VectorStrafe = (l_ActualCamera->GetDirection() ^ l_ActualCamera->GetVecUp()).Normalize();
    g_PhysXControllerPX->Move(l_VectorStrafe, elapsedTime);
    l_CameraObject->SetPosition(g_PhysXControllerPX->GetPosition());
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardRight")) != 0.0f )
	{
		/*l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);*/

    Vect3f l_VectorStrafe = (l_ActualCamera->GetVecUp() ^ l_ActualCamera->GetDirection()).Normalize();
    g_PhysXControllerPX->Move(l_VectorStrafe, elapsedTime);
    l_CameraObject->SetPosition(g_PhysXControllerPX->GetPosition());
	}
	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("Jump")) != 0.0f )
	{
    g_PhysXControllerPX->Jump(1); //Testing purposes ony
  }

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoUp")) != 0.0f )
	{
		l_vPos.y += elapsedTime*m_MovementSpeed;
		l_CameraObject->SetPosition(l_vPos);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoDown")) != 0.0f )
	{
		l_vPos.y -= elapsedTime*m_MovementSpeed;
		l_CameraObject->SetPosition(l_vPos);
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("PlayerRun")) != 0.0f )
	{
		m_MovementSpeed = 15;
		//CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
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

///<summary>
/// CGameProcess_t:: GetPositionObj : Nos devuelve un Vect3f con la posición del objeto.
///</summary>
///<param name="p_vObj">Objeto del que vamos a obtener la posición.</param>
///<returns name="posicion">Vector de 3 floats con la posición del objeto.</returns>
Vect3f CProcessPhysX::GetPositionObj(CObject3D * p_vObj)
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
void CProcessPhysX::InitCameras()
{
	//Bots
	m_pPlayer = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
}

///<summary>
/// CGameProcess_t:: MovMouse : Movimiento del ratón asociado a la cámara.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessPhysX::MovMouse()
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
void CProcessPhysX::UpdateLua()
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
/// CGameProcess_t:: MemoryTest : Pruebas para grabar en RAM o VRAM.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CProcessPhysX::MemoryTest()
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