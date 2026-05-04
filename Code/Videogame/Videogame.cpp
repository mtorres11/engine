#include "Core.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "Timer/Timer.h"
#include "Videogame.h"
#include "Math/Matrix44.h"
#include "Cameras/FPSCamera.h"
#include "Cameras/ThPSCamera.h"
#include "Object3D/Object3D.h"
#include "Texture/Texture.h"
#include "Vertex/IndexedVertex.h"
#include "Vertex/VertexType.h"
//#include "StaticMeshes/StaticMesh.h"
//#include "RenderableObjects/RenderableObject.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "Lights/LightManager.h"
#include "ActionToInput.h"
#include "Shaders/EffectManager.h"
#include "Shaders/SceneEffectManager.h"
#include "Cameras/CameraManager.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "AnimationModel/AnimatedCoreModel.h"
#include "AnimationModel/AnimatedModelManager.h"

#include "ScriptManager.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

Videogame::Videogame()
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
  m_MovementSpeed = 5.0f;
  m_RunTime = 2.0f;
  m_RestTime = 3.0f;
  m_pPlayer = 0;
}


void Videogame::Init ()
{
  CalLoader::setLoadingMode(LOADER_ROTATE_X_AXIS);

  m_pObject3D = new CObject3D();
  uint32 w,h;
  CORE->GetRenderManager()->GetWandH(w,h);
  float aspectRatio = (float)w/h;
  

  CORE->GetEffectManager()->Load("Data/Effects.xml");
  CSceneEffectManager * m_SceneEffectManager = CORE->GetSceneEffectManager();
  //m_SceneEffectManager = new CSceneEffectManager();

  CORE->GetStaticMeshManager()->Load("Data/StaticMeshManager.xml");
  CORE->GetLigthManager()->Load("Data/lights.xml");
  CORE->GetAnimatedModelManager()->Load("Data/AnimatedModels.xml");
  CORE->GetRenderableObjectsManager()->Load("Data/RenderableObjects.xml");
  m_SceneEffectManager->LoadXML("Data/scene_effects.xml");

  m_pPlayer = (CRenderableAnimatedInstanceModel*)CORE->GetRenderableObjectsManager()->GetInstance("bot");
}

Videogame::~Videogame()
{
  CHECKED_DELETE(m_pObject3D);
}

void Videogame::Update(float elapsedTime)
{
  CheckActionsFromInputs(elapsedTime);
  Vect3i deltas = CORE->GetInputManager()->GetMouseDelta();
  CORE->GetRenderableObjectsManager()->Update(elapsedTime);
}

void Videogame::Render(CRenderManager* rm) 
{
  CSceneEffectManager * m_SceneEffectManager = CORE->GetSceneEffectManager();
  m_SceneEffectManager->ActivateRenderSceneEffects();

  CEffectManager *l_EffectManager=CORE->GetEffectManager();
  l_EffectManager->SetAnimatedModelTechnique(NULL);
  l_EffectManager->SetStaticMeshTechnique(NULL);

  //Renderizamos la escena
  RenderScene(rm);

  m_SceneEffectManager->CaptureFrameBuffers(rm);
  m_SceneEffectManager->PostRender(rm);
  m_SceneEffectManager->CaptureFrameBuffersAfterPostRender(rm);
}

void Videogame::RenderScene(CRenderManager* rm) 
{
  Mat44f l_Matrix;
  l_Matrix.SetIdentity();
  rm->SetTransform(l_Matrix);
  rm->DrawGrid(10);
  rm->DrawAxis(20);
  l_Matrix.SetIdentity();
  rm->SetTransform(l_Matrix);

  CRenderableObjectsManager *renderObjectsManager = CORE->GetRenderableObjectsManager();
  renderObjectsManager->Render(rm);

  // luces
  CORE->GetLigthManager()->Render(rm);
  RenderDebugInfo();
}


void Videogame::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();

  CFontManager* fm = CORE->GetFontManager();
  CTimer *t = CORE->GetTimer();

  uint32 l_iFontId = fm->GetTTF_Id("arial");

  fm->DrawText(10,75,colWHITE,l_iFontId,"Pitch: %f",m_pObject3D->GetPitch());
  fm->DrawText(10,95,colWHITE,l_iFontId,"Yaw: %f",m_pObject3D->GetYaw());
  fm->DrawText(10,115,colWHITE,l_iFontId,"Position: [%f,%f,%f]",m_pObject3D->GetPosition().x,m_pObject3D->GetPosition().y,m_pObject3D->GetPosition().z);

  CStaticMeshManager* sm= CORE->GetStaticMeshManager();
  fm->DrawTextA(10,135,colYELLOW,l_iFontId,"Num of Faces (Static Meshes): %ld",sm->GetTotalObjectsFaces());
  CAnimatedModelManager* am=CORE->GetAnimatedModelManager();
  fm->DrawTextA(10,155,colYELLOW,l_iFontId,"Num of Faces (Dinamic Meshes): %ld",am->GetTotalNumFaces());

}

void Videogame::CheckActionsFromInputs(float elapsedTime)
{
  Vect3f l_vPos = m_pObject3D->GetPosition();
  float delta = 0.0;
  CCore *l_pCore = CORE;
  Vect3i deltas = CORE->GetInputManager()->GetMouseDelta();

  if((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("ProcessInfo")) != 0.0f )
  {
    //Esto solo se dibuja en tiempo de renderingScene!
    RenderDebugInfo();
  }

  //Reload files
  if((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("ReloadEffects")) != 0.0f )
  {
    CEffectManager* ef=CORE->GetEffectManager();
    ef->Reload();
    CSceneEffectManager *sem = CORE->GetSceneEffectManager();
    sem->Destroy();
    sem->LoadXML("Data/scene_effects.xml");
  }
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("ReloadLua")) != 0.0f )
  {
		CORE->GetScriptManager()->RunFile("./Data/init.lua");
  }

  //MoveXViewerCam
  /*if((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("MoveXViewerCam")) != 0.0f )
  {
    m_pObject3D->SetYaw( m_pObject3D->GetYaw() + delta * -0.01f );
  }

  //MoveZViewerCam
  if((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("MoveZViewerCam")) != 0.0f )
  {
    CThPSCamera* cam = (CThPSCamera*) m_pCamera;
    cam->SetZoom( cam->GetZoom() + delta * -0.1f);
  }*/

  //YawViewerCam
  if((delta = CORE->GetInputManager()->GetActionToInput()->DoAction("YawViewerCam")) != 0.0f )
  {
    m_pObject3D->SetYaw( m_pObject3D->GetYaw() + deltas.x * -0.01f );
  }

  //PitchViewerCam
  if((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("PitchViewerCam")) != 0.0f )
  {
    m_pObject3D->SetPitch( m_pObject3D->GetPitch() + deltas.y * -0.01f );
  }

  //SlowZoomViewerCam
  if((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("SlowZoomViewerCam")) != 0.0f )
  {
    CThPSCamera * cam = (CThPSCamera*)CORE->GetCameraManager()->GetCamera();
    cam->AddZoom(delta * -0.005f);
  }
  else { //ZoomViewerCam
    if((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("ZoomViewerCam")) != 0.0f )
    {
      CThPSCamera * cam = (CThPSCamera*)CORE->GetCameraManager()->GetCamera();
      cam->AddZoom(delta * -0.005f);
    }
  }
  //Movement
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("GoForward")) != 0.0f )
  {
    l_vPos.x = l_vPos.x + cos(m_pObject3D->GetYaw())*elapsedTime*m_MovementSpeed;
    l_vPos.z = l_vPos.z + sin(m_pObject3D->GetYaw())*elapsedTime*m_MovementSpeed;
    m_pObject3D->SetPosition(l_vPos);
    //m_pPlayer->SetPosition(l_vPos);
  }

  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("GoBackwards")) != 0.0f )
  {
    l_vPos.x = l_vPos.x - cos(m_pObject3D->GetYaw())*elapsedTime*m_MovementSpeed;
    l_vPos.z = l_vPos.z - sin(m_pObject3D->GetYaw())*elapsedTime*m_MovementSpeed;
    m_pObject3D->SetPosition(l_vPos);
    //m_pPlayer->SetPosition(l_vPos);
  }

  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("GoSidewardLeft")) != 0.0f )
  {
    l_vPos.x = l_vPos.x + cos(m_pObject3D->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
    l_vPos.z = l_vPos.z + sin(m_pObject3D->GetYaw()+FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
    m_pObject3D->SetPosition(l_vPos);
    //m_pPlayer->SetPosition(l_vPos);
  }

  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("GoSidewardRight")) != 0.0f )
  {
    l_vPos.x = l_vPos.x + cos(m_pObject3D->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
    l_vPos.z = l_vPos.z + sin(m_pObject3D->GetYaw()-FLOAT_PI_VALUE/2)*elapsedTime*m_MovementSpeed;
    m_pObject3D->SetPosition(l_vPos);
    //m_pPlayer->SetPosition(l_vPos);
  }
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("GoUp")) != 0.0f )
  {
    l_vPos.y += elapsedTime*m_MovementSpeed;
    m_pObject3D->SetPosition(l_vPos);
  }
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("GoDown")) != 0.0f )
  {
    l_vPos.y -= elapsedTime*m_MovementSpeed;
    m_pObject3D->SetPosition(l_vPos);
  }
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("PlayerRun")) != 0.0f )
  {
    if(m_RunTime>=0)
    {
      m_MovementSpeed = 10;
      m_RunTime -= elapsedTime;
    }
    else
    {
      m_MovementSpeed = 5;
      if(m_RestTime>=0)
      {
        m_RestTime -= elapsedTime;
      }
      else
      {
        m_RunTime = 2.0f;
        m_RestTime = 3.0f;
      }
    }
  }
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("PlayerQuiet")) != 0.0f )
  {
    m_MovementSpeed = 2.5;
  }
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("PlayerNormalSpeed")) != 0.0f )
  {
    m_MovementSpeed = 5;
  }
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("ReloadRenderableObjects")) != 0.0f )
  {
    CORE->GetRenderableObjectsManager()->Reload();
  }
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("ReloadRenderableObjects")) != 0.0f )
  {
    CORE->GetRenderableObjectsManager()->Reload();
  }
  //Print Screen
  if ((delta = l_pCore->GetInputManager()->GetActionToInput()->DoAction("PrintScreen")) != 0.0f )
  {
    //m_Texture->Save("D:/SHADOWMAP.BMP");
  }
  
  //m_pObject3D->SetPitch( m_pObject3D->GetPitch() + deltas.y * -0.01f );
  //AXIS_MOUSE_Y
}
