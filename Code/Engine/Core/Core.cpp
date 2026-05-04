#include "Core.h"
#include "Engine.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "LanguageManager/LanguageManager.h"
#include "Timer/Timer.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/MeshInstance.h"
#include "Lights/LightManager.h"
#include "Texture/TextureManager.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "Shaders/EffectManager.h"
#include "Shaders/SceneEffectManager.h"
#include "ScriptManager.h"
#include "Cameras/CameraManager.h"
#include "GUIManager.h"
#include "PhysicsManager.h"
#include "RenderablePhysX/RenderablePhysXManager.h"
#include "Particles/ParticleManager.h"
#include "SoundManager.h"
#include "Trigger/TriggerManager.h"
#include "ItemManager.h"
#include "BillBoard/BillboardManager.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CCore::~CCore()
{
	CHECKED_DELETE(m_pRenderManager);
  CHECKED_DELETE(m_pFontManager);
  CHECKED_DELETE(m_pLanguageManager);
  CHECKED_DELETE(m_pInputManager);
  CHECKED_DELETE(m_pStaticMeshManager);
  CHECKED_DELETE(m_pRenderableObjectsManager);
  CHECKED_DELETE(m_pLightManager);
  CHECKED_DELETE(m_pAnimatedModelManager);
  CHECKED_DELETE(m_pEffectManager);
  CHECKED_DELETE(m_pTimer);
  CHECKED_DELETE(m_pScriptManager);
	CHECKED_DELETE(m_pCameraManager);
  CHECKED_DELETE(m_pGUIManager);
	CHECKED_DELETE(m_pPhysicsManager);
  CHECKED_DELETE(m_TextureManager);
  CHECKED_DELETE(m_pSceneEffectManager);
	CHECKED_DELETE(m_pParticleManager);
	CHECKED_DELETE(m_pSoundManager);
  CHECKED_DELETE(m_RenderablePhysXManager);
  CHECKED_DELETE(m_pTriggerManager);
	CHECKED_DELETE(m_pItemManager);
	CHECKED_DELETE(m_pBillboardManager);
}

bool CCore::Init(SInitParams _params, HWND hWnd, CEngine* _engine)
{
  bool m_bIsOk = true;
  //bool z = _params.m_bFullScreen;
  //---Init del RenderManager---

  m_pTimer = new CTimer(30);

	if (m_bIsOk)
	{
		m_pEngine = _engine;

    m_pRenderManager = new CRenderManager();
    m_bIsOk = m_pRenderManager->Init(_params, hWnd);
  }

  //---Init del LanguageManager---
  if (m_bIsOk)
  {
    m_pLanguageManager = new CLanguageManager();
    m_bIsOk = m_pLanguageManager->Init();
  }

  //---Init del FontManager---
  if (m_bIsOk)
  {
    m_pFontManager = new CFontManager();
    m_bIsOk = m_pFontManager->Init(m_pRenderManager);
    m_pFontManager->LoadTTFs("./Data/Fonts/fonts.xml");
  }

  //---Init del Input---
  if (m_bIsOk)
  {
    m_pInputManager = new CInputManager();
    m_bIsOk = m_pInputManager->Init(hWnd, Vect2i(_params.m_uWidthScreen, _params.m_uHeightScreen), true);
  }

  //m_TextureManager=new CTextureManager();
  if (m_bIsOk)
  {
    m_TextureManager = new CTextureManager();
    m_TextureManager->Init();
  }

  //---Init del StaticMeshManager---
  if (m_bIsOk)
  {
    m_pStaticMeshManager = new CStaticMeshManager();

  }
  //---Init del RenderableObjectsManager---
  if (m_bIsOk)
  {
    m_pRenderableObjectsManager = new CRenderableObjectsManager();
  }
  if(m_bIsOk)
  {
    m_RenderablePhysXManager = new CRenderablePhysXManager;
  }
  if (m_bIsOk)
  {
    m_pLightManager = new CLightManager();
  }
  if (m_bIsOk)
  {
    m_pAnimatedModelManager = new CAnimatedModelManager();
  }
  if (m_bIsOk)
  {
    m_pEffectManager = new CEffectManager();
  }
  if (m_bIsOk)
  {
    m_pSceneEffectManager = new CSceneEffectManager();
  }
	
	//---Init del SoundManager---
  if (m_bIsOk)
  {
    m_pSoundManager = new CSoundManager();
  }

  if (m_bIsOk)
  {
    m_pScriptManager = new CScriptManager();
    m_pScriptManager->Initialize();
  }
  if (m_bIsOk)
  {
    uint32 w, h;
    m_pRenderManager->GetWandH(w, h);
    m_pGUIManager = new CGUIManager(Vect2i(w,h));
  }
	//---Init del PhysicsManager---
	if (m_bIsOk)
  {
    m_pPhysicsManager= new CPhysicsManager();
	  m_bIsOk = m_pPhysicsManager->Init();
  }

  //---Init del CameraManager---
  if (m_bIsOk)
	{
    m_pCameraManager = new CCameraManager();
    m_bIsOk = m_pCameraManager->Init();
  }

	if (m_bIsOk)
  {
    m_pParticleManager= new CParticleManager();
  }
	if (m_bIsOk)
  {
    m_pTriggerManager= new CTriggerManager();
  }

	if(m_bIsOk)
  {
    m_pItemManager = new CItemManager();
  }

	if(m_bIsOk)
  {
    m_pBillboardManager = new CBillboardManager();
  }

  return m_bIsOk;
}
void CCore::Update(float elapsedTime) 
{
  m_pInputManager->Update();
  CheckActionsFromInputs();

  m_pLightManager->Update(elapsedTime);

  m_pPhysicsManager->Update(elapsedTime);
  m_RenderablePhysXManager->Update(elapsedTime);
}

void CCore::Render() 
{
//	m_pPhysicsManager->DebugRender(m_pRenderManager);
}

void CCore::CheckActionsFromInputs()
{
  float delta = 0.0;

  //logger
  if(GetInputManager()->GetActionToInput()->DoAction("Logger") != 0.0f)
  {
    CLogger *l = LOGGER;
    const std::vector <SLog> &log =  l->GetLogs();
  }

  //console
  if(GetInputManager()->GetActionToInput()->DoAction("Console") != 0.0f)
  {
    //CLogger *l = LOGGER;
    //l->GetLogs();
  }


  //SaveLogsInFile
  if(GetInputManager()->GetActionToInput()->DoAction("SaveLogsInFile") != 0.0f)
  {
    CLogger *l = LOGGER;
    l->SaveLogsInFile();
  }

  //ReloadTTFs
  if(GetInputManager()->GetActionToInput()->DoAction("ReloadTTFs") != 0.0f )
  {
    CFontManager *fm = CORE->GetFontManager();
    fm->ReloadTTFs();
  }

  //ReloadLanguageXMLs
  if(GetInputManager()->GetActionToInput()->DoAction("ReloadLanguageXMLs") != 0.0f )
  {
    CLanguageManager *lm = CORE->GetLanguageManager();
    lm->LoadXMLs();
  }

  //ReloadActions
  if(GetInputManager()->GetActionToInput()->DoAction("ReloadActions") != 0.0f )
  {
    CActionToInput *i = CORE->GetInputManager()->GetActionToInput();
    i->Reload();
  }
}

///<summary>
/// CCore:: SetProcess : Le encarga al engine establecer el proceso dado.
///</summary>
///<param name="_sProcess">String identificador del proceso.</param>
///<returns name=""></returns>
void CCore::SetProcess(const std::string& _sProcess)
{
	m_pEngine->SetActiveProcess(_sProcess);
}

///<summary>
/// CCore:: Exit : Le encarga al engine terminar la ejecucion.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CCore::Exit()
{
	m_pEngine->Exit();
}
