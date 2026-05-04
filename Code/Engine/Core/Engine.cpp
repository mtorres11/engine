#include "Engine.h"
#include "Process.h"
#include "RenderManager.h"
#include "Core.h"
#include "Logger/Logger.h"
#include "Timer/Timer.h"
#include "XML/XMLTreeNode.h"
#include "Exceptions/Exception.h"
#include "Base.h"
#include "Utils/LogRender.h"
#include "Console/Console.h"
#include "../Input/InputManager.h"
#include "../Input/InputType/Mouse/Mouse.h"
#include "ActionToInput.h"
#include "Cameras/CameraManager.h"
#include "GUIManager.h"
#include "Texture/Texture.h"
#include "SoundManager.h"


#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CEngine::~CEngine(void)
{
  //CHECKED_DELETE(m_pProcess);
	Destroy(); // este destroy recorre el mapa de procesos haciendo CHECKED_DELETE
  
	CHECKED_DELETE(m_pCore);

	CLogger * p = LOGGER;
	CHECKED_DELETE(p);
  
  #if defined( _DEBUG )
  #endif // defined( _DEBUG )
	  CHECKED_DELETE(m_pLogRender);
    CHECKED_DELETE(m_pConsole);
}

void CEngine::Init(HWND hWnd)
{
	new CLogger(); //Creacion por primera vez del LOGGER

  m_pCore = new CCore();

  //le tenemos que pasar los parametros de inicio
  m_pCore->Init(m_ConfigParams, hWnd, this);
	
	m_pProcess->Init();
	
  #if defined( _DEBUG )
  #endif
	  if (m_ConfigParams.m_bLogRender)
    {
      m_pLogRender = new CLogRender();
      m_pLogRender->SetLinePerPage(20);
    }
    if (m_ConfigParams.m_bConsoleEnabled)
    {
      m_pConsole = new CConsole();
      m_pConsole->Init(m_pCore->GetScriptManager());
    }
}


/// <summary>
/// Carga del .xml con los parametros de configuración de inicio y asigna los valores 
/// </summary>
/// <param name="pathXML">Ruta del archivo .xml de configuración</param>
void CEngine::LoadConfig(const std::string & pathXML)
{
 
  CXMLTreeNode parser;

	try
	{

		// Checkea si carga el archivo .xml de configuración
		if (parser.LoadFile(pathXML.c_str())) 
		{
			CXMLTreeNode  p = parser["Parameters"];

      if (p.Exists())
      {
        int numNodes = p.GetNumChildren();		
				for(int i=0; i < numNodes; ++i)
				{
					if (!p(i).IsComment())
					{
            CXMLTreeNode grupo = p(i);
            
            if (strcmp(grupo.GetName(),"config") == 0)
            {
              m_ConfigParams.m_bFullScreen = grupo.GetBoolProperty("fullscreen", P_FULLSCREEN);
              m_ConfigParams.m_uWidthScreen = grupo.GetIntProperty("widthscreen", P_WIDTHSCREEN);
              m_ConfigParams.m_uHeightScreen = grupo.GetIntProperty("heightscreen", P_HEIGHTSCREEN);

              if (m_ConfigParams.m_uWidthScreen < 300 || m_ConfigParams.m_uHeightScreen < 300)
              {
                m_ConfigParams.m_uWidthScreen = P_WIDTHSCREEN;
		            m_ConfigParams.m_uHeightScreen = P_HEIGHTSCREEN;
				        LOGGER->AddNewLog(ELL_WARNING, "CEngine::Init->Valores de resolución de pantalla muy bajo. Valores por defecto (800x600) establecidos");
			        }
            }
            #if defined( _DEBUG )
            #endif
            else if (strcmp(grupo.GetName(), "debug") == 0)
            {
               m_ConfigParams.m_bConsoleEnabled = grupo.GetBoolProperty("consoleEnabled", P_CONSOLE);
               m_ConfigParams.m_bLogRender = grupo.GetBoolProperty("logRender", P_LOGRENDER);
               m_ConfigParams.m_bMouseEnabled = grupo.GetBoolProperty("mouseEnabled", P_MOUSE_ENABLED);
               m_ConfigParams.m_bGuiEnabled = grupo.GetBoolProperty("guiEnabled", P_GUI_ENABLED);
            }
          }
        }
      }
      else
      {
        m_ConfigParams.m_bFullScreen    = P_FULLSCREEN;
        m_ConfigParams.m_uWidthScreen   = P_WIDTHSCREEN;
        m_ConfigParams.m_uHeightScreen  = P_HEIGHTSCREEN;	  

        #if defined( _DEBUG )
        #endif
          m_ConfigParams.m_bConsoleEnabled = P_CONSOLE;
          m_ConfigParams.m_bLogRender = P_LOGRENDER;
          m_ConfigParams.m_bMouseEnabled = P_MOUSE_ENABLED;
          m_ConfigParams.m_bGuiEnabled = P_GUI_ENABLED;

        //string msg_error = "CEngine::Init->Error al intentar leer el archivo de configuracion: " + pathXML;
        LOGGER->AddNewLog(ELL_WARNING, "CEngine::Init->Se establecieron los parametros de configuracion por defecto porque no existe el nodo 'Parameters' en el archivo: %s", pathXML.c_str());
      }
    } // Si no pudo cargar el archivo XML
    else 
	  { 
      m_ConfigParams.m_bFullScreen    = P_FULLSCREEN;
      m_ConfigParams.m_uWidthScreen   = P_WIDTHSCREEN;
      m_ConfigParams.m_uHeightScreen  = P_HEIGHTSCREEN;	  

      #if defined( _DEBUG )
      #endif
        m_ConfigParams.m_bConsoleEnabled = P_CONSOLE;
        m_ConfigParams.m_bLogRender = P_LOGRENDER;
        m_ConfigParams.m_bMouseEnabled = P_MOUSE_ENABLED;
        m_ConfigParams.m_bGuiEnabled = P_GUI_ENABLED;

      //string msg_error = "CEngine::Init->Error al intentar leer el archivo de configuracion: " + pathXML;
      LOGGER->AddNewLog(ELL_WARNING, "CEngine::Init->Error al intentar leer el archivo de configuracion: %s", pathXML.c_str());
    }
    //-- END XML

	}
	catch (...)
	{
		LOGGER->AddNewLog(ELL_WARNING, "CEngine::Init->Error al intentar leer el archivo de configuracion: %s", pathXML.c_str());
	}


}

void CEngine::Update() 
{
	if (m_bExit) {
		PostQuitMessage( 0 );
		return;
	}
	if (m_bChangeProcess)
	{
		m_pProcess = m_pNextProcess;
		m_pNextProcess = NULL;
		m_pProcess->Init();
		m_bChangeProcess = false;
	}

  CTimer *t = m_pCore->GetTimer();
	t->Update();
	float elapsedTime = t->GetElapsedTime();
	m_pCore->Update(elapsedTime);
	m_pProcess->Update(elapsedTime);
	
  #if defined( _DEBUG )
  #endif
  if (m_ConfigParams.m_bGuiEnabled)
  {
    m_pCore->GetGUIManager()->Update(elapsedTime);

    if(m_pCore->GetInputManager()->GetActionToInput()->DoAction("ReloadGUI") != 0.0f)
    {
      m_pCore->GetGUIManager()->ReloadGuiFiles();
    }
  }
  #if defined( _DEBUG )
  #endif
    if (m_ConfigParams.m_bLogRender)
    {
      m_pLogRender->Update(elapsedTime);

      if(m_pCore->GetInputManager()->GetActionToInput()->DoAction("Logger") != 0.0f)
      {
        m_pLogRender->SetVisible(!m_pLogRender->GetVisible());
      }
      if(m_pCore->GetInputManager()->GetActionToInput()->DoAction("LogRender_PageDown") != 0.0f)
      {
        m_pLogRender->PageDown();
      }
      if(m_pCore->GetInputManager()->GetActionToInput()->DoAction("LogRender_PageUp") != 0.0f)
      {
        m_pLogRender->PageUp();
      }
      if(m_pCore->GetInputManager()->GetActionToInput()->DoAction("LogRender_PrevLine") != 0.0f)
      {
        m_pLogRender->PrevLine();
      }
      if(m_pCore->GetInputManager()->GetActionToInput()->DoAction("LogRender_NextLine") != 0.0f)
      {
        m_pLogRender->NextLine();
      }
    }
    if (m_ConfigParams.m_bConsoleEnabled)
    {
      m_pConsole->Update(elapsedTime);

      if(m_pCore->GetInputManager()->GetActionToInput()->DoAction("Console") != 0.0f)
      {
        m_pConsole->SetActive(!m_pConsole->IsActive());
      }
    }
}

void CEngine::Render() {
 	CRenderManager * rm = m_pCore->GetRenderManager();
	m_pProcess->PreRender(rm);


	rm->BeginRendering();

	CCamera* camera = CORE->GetCameraManager()->GetCamera();
	rm->SetupMatrices(camera);
  //Actualiza el Frustum para optimizar renderizado de objetos
  CORE->GetCameraManager()->UpdateFrustumCamera();

	{
		RenderScene(rm);
	}
	rm->EndRendering();
	rm->Present();
}


void CEngine::RenderScene (CRenderManager* rm)
{
	if(m_pProcess != NULL)
	{
		m_pProcess->Render(rm);
  //  m_Timer = CORE->GetTimer();
		//float elapsedTime = m_Timer.GetElapsedTime();
		//m_pProcess->RenderDebugInfo(rm, fm, elapsedTime);
		m_pCore->Render();
  
    #if defined( _DEBUG )
    #endif
    if (m_ConfigParams.m_bGuiEnabled)
    {
      m_pCore->GetGUIManager()->Render(rm, m_pCore->GetFontManager());
    }
    #if defined( _DEBUG )
    #endif
      m_pCore->GetRenderManager()->EnableAlphaBlend();
      if (m_ConfigParams.m_bLogRender)
      {
        m_pLogRender->Render(rm, m_pCore->GetFontManager(),colYELLOW);
      }
      if (m_ConfigParams.m_bConsoleEnabled)
      {
        m_pConsole->Render(rm, m_pCore->GetFontManager());
      }
      m_pCore->GetRenderManager()->DisableAlphaBlend();
  }

  if(m_pCore->GetInputManager()->GetActionToInput()->DoAction("Screenshot") != 0.0f)
  {
    /*    //crea prerender en textura
    CTexture* l_Texture = new CTexture();  
    LPDIRECT3DSURFACE9 l_Surface;
  
    uint32 l_Width, l_Height;
    CORE->GetRenderManager()->GetWandH(l_Width, l_Height);

    l_Texture->Create("screenshot", l_Width, l_Height, 0,CTexture::RENDERTARGET,CTexture::DEFAULT,CTexture::A8R8G8B8);

//    l_pTexture->GetD3DTexture()->GetSurfaceLevel(0, );
    rm->DrawTexturedQuad2D(Vect2i(0,0), m_ConfigParams.m_uWidthScreen, m_ConfigParams.m_uHeightScreen, CRenderManager::UPPER_LEFT,l_Texture, colWHITE);
    l_Texture->GetD3DTexture()->GetSurfaceLevel(0, &l_Surface);  
    CORE->GetRenderManager()->GetDevice()->GetRenderTarget(0,&l_Surface);
    std::string pathFoto = "./Data/Screenshots/screenshot.bmp";
    l_Texture->Save(pathFoto);
    l_Surface->Release();
    delete l_Texture;
*/	}
}

void CEngine::Release()
{
	CHECKED_DELETE(m_pCore);
	CLogger* logger = LOGGER;
	delete logger;
}

///<summary>
/// CEngine:: AddProcess : Agrega un proceso al mapa
///</summary>
///<param name="_sProcessId">Nombre identificador del proceso a agregar</param>
///<param name="_pProcess">Puntero al proceso a agregar</param>
///<returns name=""></returns>
void CEngine::AddProcess(const std::string & _sProcessId, CProcess* _pProcess)
{
	AddResource(_sProcessId, _pProcess);
}

///<summary>
/// CEngine:: SetActiveProcess : Cambia el proceso activo, en el siguiente ciclo de Update
///</summary>
///<param name="_sProcess">Nombre identificador del proceso a asignar</param>
///<returns name=""></returns>
void CEngine::SetActiveProcess(const std::string & _sProcess)
{
	m_pNextProcess = GetResource(_sProcess);
	if (m_pNextProcess == NULL)
	{
		std::string msg_error = "ENgine::ActiveProcess->No existe el proceso: " + _sProcess;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
	}
	else
	{
		m_bChangeProcess = true;
	}
}

///<summary>
/// CEngine:: Exit : Termina la ejecución durante el siguiente ciclo de Update
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CEngine::Exit()
{
	m_bExit = true;
}