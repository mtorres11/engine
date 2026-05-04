#include "Core.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
#include "Viewer.h"
#include "Math/Matrix44.h"
#include "Math/MathUtils.h"
#include "Cameras/FPSCamera.h"
#include "Cameras/ThPSCamera.h"
#include "Object3D/Object3D.h"
#include "Texture/Texture.h"
#include "Vertex/IndexedVertex.h"
#include "Vertex/VertexType.h"
#include "Shaders/EffectManager.h"
#include "Shaders/SceneEffectManager.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "RenderableObjects/MeshInstance.h"
#include "RenderableObjects/RenderableObject.h"
#include "Lights/Light.h"
#include "Lights/LightManager.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "AnimationModel/AnimatedCoreModel.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "texture/Texture.h"
#include "texture/TextureManager.h"
#include "ScriptManager.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraManager.h"
#include "../Engine/GUI/GUIManager.h"
#include "../Engine/PhysX/Actor/PhysicActor.h"
#include "../Engine/PhysX/PhysicsManager.h"
#include "Cameras/Camera.h"
#include "Timer/Timer.h"
#include "Utils/LogRender.h"
#include "Exceptions/Exception.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <direct.h>


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//#pragma warning( disable : 4786 ) 

CSceneEffectManager* g_SceneEffectManager;

CViewer::CViewer()
{
//  m_vRenderObjects.clear();
  m_bShowRenderDebugInfo = false;
  m_bIsScene = false;
  m_iActiveElement = 0;
  m_bSceneCleared = false;
  m_iCurrentCycle = 0;
  m_pDirectionalLight = NULL;
}

CViewer::~CViewer()
{
  //CHECKED_DELETE(m_SceneEffectManager);
}

///<summary>
/// CViewer:: Init : Lectura de los XML. Creación y asignación de las cámaras a los objetos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CViewer::Init ()
{
  //std::string filename = OpenDialogBoxViewer();
  //CreateXMLs(filename);
  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);
  LoadXMLs(); 
  CORE->GetGUIManager()->SetVisiblePointerMouse(true);

  std::vector<CLight*> light = CORE->GetLigthManager()->GetLights();
  std::vector<CLight*>::iterator l_Iter = light.begin();

  //ATENCION... Debe haber una luz direccional minimo en el manager
  for(;l_Iter != light.end();++l_Iter)
  {
    if((*l_Iter)->GetType() == CLight::DIRECTIONAL)
    {
      m_pDirectionalLight = *l_Iter;
      break;
    }
  }
  
  g_SceneEffectManager = CORE->GetSceneEffectManager();
//  InitCameras();
  //vector para objetos individuales
  m_vRenderObjects = CORE->GetRenderableObjectsManager()->GetVector();

  std::vector<CRenderableObject*>::iterator l_IterObjects = m_vRenderObjects.begin();

  //crea el vector con posiciones y orientacion
  for(unsigned int i=0;i < m_vRenderObjects.size();i++)
  {
    SRecordableData l_sData;
    l_sData.m_V3ObjectPos = m_vRenderObjects[i]->GetPosition();
    l_sData.m_V3ObjectYawPitchRoll = Vect3f(m_vRenderObjects[i]->GetYaw(),m_vRenderObjects[i]->GetPitch(),m_vRenderObjects[i]->GetRoll());
    m_vScenePositions.push_back(l_sData);
  }   

  //valores de inicio
  m_bShowRenderDebugInfo = true;
  m_bIsScene = true;
  m_bSceneCleared = true;
  m_iCurrentCycle = 0;
}

std::string CViewer::OpenDialogBoxViewer()
{
  OPENFILENAME ofn;       // common dialog box structure
  char szFile[260]="";       // buffer for file name
  HWND hwnd = NULL;              // owner window
  HANDLE hf = NULL;              // file handle

  // Initialize OPENFILENAME
  ZeroMemory(&ofn, sizeof(ofn));
  ofn.lStructSize = sizeof(ofn);
  ofn.hwndOwner = hwnd;
  ofn.lpstrFile = szFile;
  //
  // Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
  // use the contents of szFile to initialize itself.
  //
  ofn.lpstrFile[0] = '\0';
  ofn.nMaxFile = sizeof(szFile);
  ofn.lpstrFilter = "Mesh Files (*.mesh)\0*.mesh\0";
  ofn.nFilterIndex = 1;
  ofn.lpstrFileTitle = NULL;
  ofn.nMaxFileTitle = 0;
  ofn.lpstrInitialDir = NULL;
  ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

  // Display the Open dialog box. 
  /*if (
  */
  GetOpenFileName(&ofn);
    /*==TRUE) 
      hf = CreateFile(ofn.lpstrFile, GENERIC_READ,
          0, (LPSECURITY_ATTRIBUTES) NULL,
          OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,
          (HANDLE) NULL);
*/
  // returns system of directories
  std::string pathfile(ofn.lpstrFile);
  replaceAll(pathfile, "\\", "/");
  
  if(pathfile != "")
    return pathfile;
  else
  {
    std::string msg_error = "CViewer::OpenDialogBoxViewer->Error al intentar leer un archivo de malla";
    LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
  }
}

std::string& CViewer::replaceAll(std::string& context, const std::string& from, const std::string& to)
{
    size_t lookHere = 0;
    size_t foundHere;
    while((foundHere = context.find(from, lookHere)) != std::string::npos)
    {
          context.replace(foundHere, from.size(), to);
          lookHere = foundHere + to.size();
    }
    return context;
}

///<summary>
/// CViewer:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CViewer::RenderScene(CRenderManager* rm) 
{
	Render3D(rm); 
	Render2D(rm); 
}

///<summary>
/// CViewer:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CViewer::Update(float elapsedTime)
{
  CheckActionsFromInputs(elapsedTime);
	MovMouse(); 
	UpdateLua(); 
	MovPlayer(elapsedTime); 

  CORE->GetRenderableObjectsManager()->Update(elapsedTime);
	// Habilita transparencia
	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("EnableAlphaBlend")) != 0.0f )
	{
		CORE->GetRenderManager()->EnableAlphaBlend();
	}
  
  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("PreviousObject")) != 0.0f )
	{
    if(!m_bIsScene)
    {
      m_vRenderObjects[m_iActiveElement]->SetVisible(false);
      if(m_iActiveElement == 0)
        m_iActiveElement = m_vRenderObjects.size()-1;
      else
        m_iActiveElement --;
    }
	}

  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("NextObject")) != 0.0f )
	{
    if(!m_bIsScene)
    {
      m_vRenderObjects[m_iActiveElement]->SetVisible(false);
      if(m_iActiveElement == m_vRenderObjects.size()-1)
        m_iActiveElement = 0;
      else
        m_iActiveElement ++;
    }
	}

  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ChangeAnimation")) != 0.0f )
	{
    if(!m_bIsScene)
    {
//      std::vector<CRenderableObject*>::iterator l_IterObjects = m_vRenderObjects.begin();
      if(!strcmp( typeid(*m_vRenderObjects[m_iActiveElement]).name(), "class CRenderableAnimatedInstanceModel"))
      {
        CRenderableAnimatedInstanceModel* mymodel = (CRenderableAnimatedInstanceModel*)(m_vRenderObjects[m_iActiveElement]);
        if(mymodel->GetAnimatedInstanceModel()->GetNumAnimations() > m_iCurrentCycle)
          m_iCurrentCycle ++; 
        else
          m_iCurrentCycle = 0;

        mymodel->GetAnimatedInstanceModel()->BlendCycle(m_iCurrentCycle,1.0f);
      }
    }
  }

  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ModelTurnToRight")) != 0.0f )
	{
    if(!m_bIsScene)
    {
      CRenderableObject* mymodel = m_vRenderObjects[m_iActiveElement];
      mymodel->SetYaw(mymodel->GetYaw() + 0.18f);
    }
  }

  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ModelTurnToLeft")) != 0.0f )
	{
    if(!m_bIsScene)
    {
      CRenderableObject* mymodel = m_vRenderObjects[m_iActiveElement];
      mymodel->SetYaw(mymodel->GetYaw() - 0.18f);
    }
  }

  if(m_bSceneCleared)
  {
    m_vRenderObjects[m_iActiveElement]->SetVisible(true);
  }
}

///<summary>
/// CViewer:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CViewer::Render(CRenderManager* rm) 
{
  g_SceneEffectManager->ActivateRenderSceneEffects();

  CEffectManager *l_EffectManager=CORE->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);

  //Renderizamos la escena
  RenderScene(rm);

  g_SceneEffectManager->CaptureFrameBuffers(rm);
  g_SceneEffectManager->PostRender(rm);
  g_SceneEffectManager->CaptureFrameBuffersAfterPostRender(rm);

  if(m_bShowRenderDebugInfo)
    RenderDebugInfo(); 
}

///<summary>
/// CViewer:: LoadXMLs : Carga todos los XML.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CViewer::LoadXMLs()
{
	//Load XML
  CCore * l_pCore = CORE;
  l_pCore->GetEffectManager()->Load("Data/Effects.xml");
  l_pCore->GetStaticMeshManager()->Load("Data/StaticMeshManager.xml");
  l_pCore->GetLigthManager()->Load("Data/lights.xml");
  l_pCore->GetAnimatedModelManager()->Load("Data/AnimatedModels.xml");
  l_pCore->GetRenderableObjectsManager()->Load("Data/RenderableObjects.xml");
  l_pCore->GetCameraManager()->Load("Data/Cameras.xml");
  l_pCore->GetSceneEffectManager()->LoadXML("Data/scene_effects.xml");
//  l_pCore->GetGUIManager()->Init("Data/GUI/initXMLViewer.xml");
}

bool CViewer::CreateXMLs(std::string filename)
{
  if(filename != "")
  {
    //obtiene nombre del mesh
    int posLastBar = filename.find_last_of('/');
    if(posLastBar == -1)
      posLastBar = filename.find_last_of('\\');
    int posLastPoint = filename.find_last_of('.');
    std::string l_sNameCore = filename.substr(posLastBar+1, posLastPoint - (posLastBar + 1));

    int pos = filename.find("Data");
    std::string pathfile = filename.substr(0,pos+5);//"Data/"
    pathfile += "StaticMeshManager.xml";

    //crea static mesh manager.xml
    std::ofstream fsm;
    fsm.open(pathfile.c_str() ,(std::ios::out | std::ios::trunc)); 
    if(fsm.is_open())
    {
      fsm << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << std::endl;
      fsm << "<static_meshes>" << std::endl;
      fsm << "<static_mesh name=\"" << l_sNameCore.c_str() << "\" filename=\"" << filename.c_str() << "\" />" << std::endl;
      fsm << "</static_meshes>" << std::endl;
      fsm.close();  
    }
    else
    {
      std::string msg_error = "CViewer::CreateXMLs->Error al intentar crear staticmeshmanager.xml";
      LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
      throw CException(__FILE__, __LINE__, msg_error);      
      return false;
    }
    //crea renderableobjects.xml
    std::string pathfile2 = filename.substr(0,pos+5);//"Data/"
    pathfile2 += "RenderableObjects.xml";

    std::ofstream fro;
    fro.open(pathfile2.c_str(),(std::ios::out|std::ios::trunc)); 
    if(fro.is_open())
    {
     fro << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << std::endl;
     fro << "<renderable_objs>" << std::endl;
     fro << "<object name=\"" << l_sNameCore.c_str() << "\" core=\"" << l_sNameCore.c_str() << "\" pos=\"0.0 0.0 0.0\" yaw=\"0.0\" pitch=\"0.0\" roll=\"0.0\"  />" << std::endl;
     fro << "</renderable_objs>" << std::endl;

     fro.close();
    }
    else
    {
      std::string msg_error = "CViewer::CreateXMLs->Error al intentar crear staticmeshmanager.xml";
      LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
      throw CException(__FILE__, __LINE__, msg_error);      
      return false;
    }
  }
  else
  {
    return false;
  }
  return true;
}

///<summary>
/// CViewer:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CViewer::Render3D(CRenderManager* rm)
{
	// render manager
//  CRenderableObjectsManager *renderObjectsManager = CORE->GetRenderableObjectsManager();
  //
//  renderObjectsManager->Render(rm);
    std::vector<CRenderableObject*>::iterator l_Iter = m_vRenderObjects.begin();
    for(;l_Iter != m_vRenderObjects.end(); ++l_Iter)
    {
      (*l_Iter)->Render(CORE->GetRenderManager());
    }
}

///<summary>
/// CViewer:: Render2D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CViewer::Render2D(CRenderManager* rm)
{
}

///<summary>
/// CViewer:: PreRender() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CViewer::PreRender(CRenderManager* rm) 
{
	g_SceneEffectManager->PreRender(rm,this);
}

///<summary>
/// CViewer:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CViewer::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();

  CFontManager* fm = CORE->GetFontManager();
  CTimer *t = CORE->GetTimer();

  uint32 l_iFontId = fm->GetTTF_Id("arial");

//  fm->DrawText(10,75,colWHITE,l_iFontId,"Pitch: %f",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPitch());
//  fm->DrawText(10,95,colWHITE,l_iFontId,"Yaw: %f",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw());
//  fm->DrawText(10,115,colWHITE,l_iFontId,"Position: [%f,%f,%f]",CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().x,CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().y,CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetPosition().z);

  if(m_bIsScene)
  {
    CStaticMeshManager* sm= CORE->GetStaticMeshManager();
    fm->DrawTextA(10,135,colYELLOW,l_iFontId,"Num of Faces (Static Meshes): %ld",sm->GetTotalObjectsFaces());
    CAnimatedModelManager* am=CORE->GetAnimatedModelManager();
    fm->DrawTextA(10,155,colYELLOW,l_iFontId,"Num of Faces (Dinamic Meshes): %ld",am->GetTotalNumFaces());
  }
  //opciones
  //carga malla
  //limpia luces (solo queda por defecto)
  //limpia renderable objects
  //carga escena?? desde max
  //carga modelos??
  //carga luces?? desde max

		std::string myInfo;
    CActionToInput* action2Input = CORE->GetInputManager()->GetActionToInput();

    //todo.. screenshot y carga de mallas...
//    action2Input->GetActionConfig(myInfo, "RenderDebugInfoONOFF");
    fm->DrawTextA(10,340,colBLUE,l_iFontId,"INFO ON/OFF: (lShift + I)");//%s",myInfo.c_str());
//    action2Input->GetActionConfig(myInfo, "IsSceneONOFF");
    fm->DrawTextA(10,360,colYELLOW,l_iFontId,"Izq: (A), Der: (D), Avance: (W), Retroc: (S), Arriba: (R), Abajo: (F)");//%s",myInfo.c_str());
    fm->DrawTextA(10,380,colYELLOW,l_iFontId,"Escena/Objetos: (lShift + S), Cambiar Objeto: (lShift + Left / lShift + Right)");//%s",myInfo.c_str());
//    action2Input->GetActionConfig(myInfo, "Screenshot");
    fm->DrawTextA(10,400,colYELLOW,l_iFontId,"Cambio de animacion de modelos: (lShift + Up)");//%s",myInfo.c_str());
//    action2Input->GetActionConfig(myInfo, "ReadMesh");
    fm->DrawTextA(10,420,colYELLOW,l_iFontId,"Limpieza de escena: (lShift + C)");//%s", myInfo.c_str());
    fm->DrawTextA(10,440,colYELLOW,l_iFontId,"Rotar objetos en yaw (lCtrl + Left / l_Ctrl + Right)");
//    action2Input->GetActionConfig(myInfo, "CleanScene");
    fm->DrawTextA(10,460,colYELLOW,l_iFontId,"ScreenShot: (lCtrl + P)");//%s", myInfo.c_str());
    fm->DrawTextA(10,480,colYELLOW,l_iFontId,"TODO..Carga de mallas/modelos desde archivo: (lShift + M)");
    fm->DrawTextA(10,500,colYELLOW,l_iFontId,"Carga de escena y luces (desde MAX)");
//    action2Input->GetActionConfig(myInfo, "CleanLights");
//todo..miguel    fm->DrawTextA(10,520,colYELLOW,l_iFontId,"Limpieza de luces (Direccional por defecto): (lShift + L)");//%s", myInfo.c_str());
}

///<summary>
/// CViewer:: CheckActionsFromInputs : Según una entrada de teclado ejecuta su acción asociada.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CViewer::CheckActionsFromInputs(float elapsedTime)
{
  if((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ReloadEffects")) != 0.0f )
  {
/*    CEffectManager* ef=CORE->GetEffectManager();
    ef->Reload();
    CStaticMeshManager *sm=CORE->GetStaticMeshManager();
    sm->Reload();
    CRenderableObjectsManager *ro = CORE->GetRenderableObjectsManager();
    ro->Reload();
*/    //falta parametros de effects.xml
    CSceneEffectManager *sem = CORE->GetSceneEffectManager();
    sem->ReloadXML();
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
		cam->SetZoom( cam->GetZoom() + m_DeltaFromXML);
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
			cam->AddZoom(m_DeltaFromXML * -0.005f);
		}
	}
}

///<summary>
/// CViewer:: MovPlayer : Calcula los movimientos y la velocidad del player.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CViewer::MovPlayer(float elapsedTime)
{
	Vect3f l_vPos = Vect3f(GetPositionObj(CORE->GetCameraManager()->GetCamera()->GetObject3D())); //posición objeto

  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardLeft")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/4)*elapsedTime;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/4)*elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForwardRight")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/4)*elapsedTime;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/4)*elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsRight")) != 0.0f )
	{
		l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/4)*elapsedTime;
		l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/4)*elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwardsLeft")) != 0.0f )
	{
		l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/4)*elapsedTime;
		l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/4)*elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	//UP DOWN LEFT RIGHT MOVEMENTS
	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoForward")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoBackwards")) != 0.0f )
	{
		l_vPos.x = l_vPos.x - cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime;
		l_vPos.z = l_vPos.z - sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw())*elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardLeft")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	else if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoSidewardRight")) != 0.0f )
	{
		l_vPos.x = l_vPos.x + cos(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime;
		l_vPos.z = l_vPos.z + sin(CORE->GetCameraManager()->GetCamera()->GetObject3D()->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		m_pPlayer->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoUp")) != 0.0f )
	{
		l_vPos.y += elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

	if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("GoDown")) != 0.0f )
	{
		l_vPos.y -= elapsedTime;
		CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPosition(l_vPos);
//		CORE->GetLigthManager()->GetResource("Spot01")->SetPosition(l_vPos);
	}

  //activa/desactiva info
  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("RenderDebugInfoONOFF")) != 0.0f )
	{
		m_bShowRenderDebugInfo = !m_bShowRenderDebugInfo;
	}
  //activa/desactiva info
  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("IsSceneONOFF")) != 0.0f )
	{
		m_bIsScene = !m_bIsScene;
    if(m_bIsScene)
    {
      std::vector<SRecordableData>::iterator l_DataIter = m_vScenePositions.begin();
      std::vector<CRenderableObject*>::iterator l_Iter = m_vRenderObjects.begin();
      for(;l_Iter != m_vRenderObjects.end(); ++l_Iter, ++l_DataIter)
      {
        (*l_Iter)->SetPitch((l_DataIter->m_V3ObjectYawPitchRoll.y));
        (*l_Iter)->SetPosition(Vect3f(l_DataIter->m_V3ObjectPos));
        (*l_Iter)->SetRoll((l_DataIter->m_V3ObjectYawPitchRoll.z));
        (*l_Iter)->SetYaw((l_DataIter->m_V3ObjectYawPitchRoll.x));
        (*l_Iter)->SetVisible(true);
      }
    }
    else
    {
      std::vector<CRenderableObject*>::iterator l_Iter = m_vRenderObjects.begin();
      for(;l_Iter != m_vRenderObjects.end(); ++l_Iter)
      {
        (*l_Iter)->SetPitch(0.0);
        (*l_Iter)->SetPosition(Vect3f(0.0,5.0,0.0));
        (*l_Iter)->SetRoll(0.0);
        (*l_Iter)->SetYaw(0.0);
        (*l_Iter)->SetVisible(false);
      }
//      (*l_Iter)[m_iActiveElement].SetVisible(true);
    }

	}
  //carga mallas
  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("ReadMesh")) != 0.0f )
	{
    std::string filename = OpenDialogBoxViewer();

    //obtiene nombre del mesh
    int posLastBar = filename.find_last_of('/');
    if(posLastBar == -1)
      posLastBar = filename.find_last_of('\\');
    int posLastPoint = filename.find_last_of('.');
    std::string l_sNameCore = filename.substr(posLastBar+1, posLastPoint - (posLastBar + 1));

    CStaticMeshManager* managermallas = CORE->GetStaticMeshManager();
    CRenderableObjectsManager* managerrenderables = CORE->GetRenderableObjectsManager();
    CStaticMesh *mesh = new CStaticMesh();
//    CInstanceMesh *instancia = new CInstanceMesh("Object", mymesh);
//TODO..
    /*
    mesh.Load(filename);
//    managermallas->Destroy();
    managermallas->GetResource("object");
    managermallas->AddResource(mymesh, &mesh);
//    CRenderableObject myObject;
//    managerrenderables->Destroy();
//    managerrenderables->CleanUp();
    managerrenderables->AddMeshInstance(mymesh, mymesh,Vect3f(0.0,0.0,0.0),0.0,0.0,0.0);
//    CORE->GetRenderableObjectsManager()->AddResource(mymesh,"Object",Vect3f(0.0,0.0,0.0),0.0,0.0,0.0);
//    CreateXMLs(mymesh);
//    CORE->GetStaticMeshManager()->Reload();
//    CORE->GetRenderableObjectsManager()->Reload();    
*/  }
  //limpia CleanScene
  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("CleanScene")) != 0.0f )
	{
//    std::string mymesh = OpenDialogBoxViewer();
//    CreateXMLs(mymesh);
/*TODO..    char* buffer;
    if ((buffer = _getcwd (NULL, 0)) == NULL)
        perror ("error _getcwd");
      else 
      {
        std::string buf(buffer);
        std::string pathfile2 = buf;//"Data/"
        pathfile2 += "/Data/RenderableObjects.xml";

        std::ofstream fro;
        fro.open(pathfile2.c_str(),(std::ios::out|std::ios::trunc)); 
        if(fro.is_open())
        {
         fro << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << std::endl;
         fro << "<renderable_objs>" << std::endl;
         fro << "</renderable_objs>" << std::endl;

         fro.close();
        }
        else
        {
          std::string msg_error = "CViewer::CleanScene->Error al intentar crear renderableObjects.xml";
          LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
          throw CException(__FILE__, __LINE__, msg_error);      
        }

      }
    CORE->GetStaticMeshManager()->Reload();
    CORE->GetRenderableObjectsManager()->Reload();    
 */
      m_bSceneCleared = !m_bSceneCleared;

      std::vector<CRenderableObject*>::iterator l_Iter = m_vRenderObjects.begin();
      for(;l_Iter != m_vRenderObjects.end(); ++l_Iter)
      {
        (*l_Iter)->SetVisible(m_bSceneCleared);
      }
      
  }

  //limpia luces
  if ((m_DeltaFromXML = CORE->GetInputManager()->GetActionToInput()->DoAction("CleanLights")) != 0.0f )
	{
    char* buffer;
    if ((buffer = _getcwd (NULL, 0)) == NULL)
        perror ("error _getcwd");
      else 
      {
        std::string buf(buffer);
        std::string pathfile2 = buf;//"Data/"
        pathfile2 += "/Data/lights.xml";

        std::ofstream fro;
        fro.open(pathfile2.c_str(),(std::ios::out|std::ios::trunc)); 
        if(fro.is_open())
        {
         fro << "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>" << std::endl;
         fro << "<lights>" << std::endl;
         fro << "</lights>" << std::endl;

         fro.close();
        }
        else
        {
          std::string msg_error = "CViewer::CleanLights->Error al intentar crear lights.xml";
          LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
          throw CException(__FILE__, __LINE__, msg_error);      
        }

      }
//    CORE->GetStaticMeshManager()->Reload();
    CORE->GetLigthManager()->Load("Data/lights.xml");
//    CORE->GetRenderableObjectsManager()->Reload();    
  }
}

///<summary>
/// CViewer:: GetPositionObj : Nos devuelve un Vect3f con la posición del objeto.
///</summary>
///<param name="p_vObj">Objeto del que vamos a obtener la posición.</param>
///<returns name="posicion">Vector de 3 floats con la posición del objeto.</returns>
Vect3f CViewer::GetPositionObj(CObject3D * p_vObj)
{
  //Moverse a través del escenario. Fijamos la cámara al objeto
	Vect3f l_vPos = p_vObj->GetPosition(); 
	return l_vPos;
}

///<summary>
/// CViewer:: InitCameras : Crea las cámaras y las asocia a un objeto.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CViewer::InitCameras()
{
	//Bots
//	m_pPlayer = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
}

///<summary>
/// CViewer:: MovMouse : Movimiento del ratón asociado a la cámara.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CViewer::MovMouse()
{
}

///<summary>
/// CViewer:: UpdateLua : Scripts de LUA.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CViewer::UpdateLua()
{
}

///<summary>
/// CViewer:: MemoryTest : Pruebas para grabar en RAM o VRAM.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CViewer::MemoryTest()
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
