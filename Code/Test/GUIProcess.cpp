#include "GUIProcess.h"

#include "ScriptManager.h"
#include "GUIManager.h"
#include "RenderManager.h"
#include "SoundManager.h"
#include "ActionToInput.h"
#include "InputManager.h"

#include "GUI/HealthIndicator.h"

#include <luabind/luabind.hpp>

#if defined( _DEBUG )
  #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

using namespace luabind;

CGUIProcess::CGUIProcess()
: m_pHealthIndicator(0)
{
}

CGUIProcess::~CGUIProcess()
{
  CHECKED_DELETE(m_pHealthIndicator)
}

///<summary>
/// CGUIProcess:: Init : Lectura de los XML. Creación y asignación de las cámaras a los objetos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGUIProcess::Init ()
{
  LoadXMLs(); 
	CORE->GetScriptManager()->RunFile("Data/LUAScripts/variables.lua");
	CORE->GetScriptManager()->RunFile("Data/LUAScripts/init.lua");
  CORE->GetRenderManager()->EnableAlphaBlend();

  CORE->GetGUIManager()->ActiveWindows("Game.xml");

  m_pHealthIndicator = new CHealthIndicator();
  m_pHealthIndicator->Init(10.0f);

  //CORE->GetGUIManager()->SetVisiblePointerMouse(true);
  Register2LUA();
}

///<summary>
/// CGUIProcess:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CGUIProcess::RenderScene(CRenderManager* rm) 
{
	Render3D(rm); 
	Render2D(rm); 
  RenderDebugInfo(); 
}

///<summary>
/// CGUIProcess:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CGUIProcess::Update(float elapsedTime)
{
  CheckActionsFromInputs(elapsedTime);
}

///<summary>
/// CGUIProcess:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CGUIProcess::Render(CRenderManager* rm) 
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
/// CGUIProcess:: LoadXMLs : Carga todos los XML.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGUIProcess::LoadXMLs()
{
	//Load XML
  CCore * l_pCore = CORE;
  l_pCore->GetGUIManager()->Init("Data/GUI/initGUI.xml");
	l_pCore->GetSoundManager()->Init("Data/Sounds.xml");
}

///<summary>
/// CGUIProcess:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CGUIProcess::Render3D(CRenderManager* rm)
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
/// CGUIProcess:: Render3D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
void CGUIProcess::Render2D(CRenderManager* rm)
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
/// CGUIProcess:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGUIProcess::PreRender(CRenderManager* rm) 
{
  //CORE->GetSceneEffectManager()->PreRender(rm,this);
}

///<summary>
/// CGUIProcess:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CGUIProcess::RenderDebugInfo()
{
  CProcess::RenderDebugInfo();

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
/// CGUIProcess:: CheckActionsFromInputs : Según una entrada de teclado ejecuta su acción asociada.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
void CGUIProcess::CheckActionsFromInputs(float elapsedTime)
{
  CActionToInput * l_Action = CORE->GetInputManager()->GetActionToInput();
	
  if (l_Action->DoAction("ReloadGUI") != 0.0f ) // L_SHIFT + G
  {
    CORE->GetGUIManager()->ReloadGuiFiles();
    CORE->GetGUIManager()->ActiveWindows("Game.xml");
  }
  if (l_Action->DoAction("RunLuaFile") != 0.0f ) // R_SHIFT + R
	{
    //TODO: Cargar un XML con los path de los lua files y cargar cada archivo del xml
		CORE->GetScriptManager()->RunFile("Data/LUAScripts/variables.lua");
	  CORE->GetScriptManager()->RunFile("Data/LUAScripts/init.lua");
	}
	
}

// registro las clases del juego y sus instancias
void CGUIProcess::Register2LUA()
{
  lua_State * l_LS = CORE->GetScriptManager()->GetLuaState();
  

  module(l_LS)
    [
      // registramos la clase CGUIManager
      class_<CHealthIndicator>("CHealthIndicator")
      // registramos su constructor
      .def(constructor<>())
      .def("hit", &CHealthIndicator::Hit)
      .def("health_up", &CHealthIndicator::HealthUp)
    ];

  globals(l_LS)["gui_HI"] = m_pHealthIndicator;
}