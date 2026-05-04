#include <Windows.h>
#include "XML/XMLTreeNode.h"
#include "Engine.h"
#include "Exceptions/Exception.h"

//---INCLUDE PROCESS------
#include "MainProcess.h"
//#include "GameProcess_t.h"
//#include "GameProcess.h"
#include "VideoGame.h"
//#include "ProcessPhysX.h"
//#include "ProcessIA.h"
//#include "ProcessIA2.h"
//#include "ProcessIA3.h"
//#include "ProcessIA4.h"
//#include "ProcessParticles.h"
//#include "ProcessTriggers.h"
//#include "GUIProcess.h"
//-------------------------

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


#define APPLICATION_NAME	"FEAR IN HELL"

	
//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

  switch( msg )
  {
  case WM_DESTROY:
    {
      PostQuitMessage( 0 );
      return 0;
    }
    break;
  case WM_KEYDOWN:
    {
      switch( wParam )
      {
      case VK_ESCAPE:
        //Cleanup();
        //LOGGER->SaveLogsInFile();
        PostQuitMessage( 0 );
        return 0;
        break;
      }
    }
    break;
  }//end switch( msg )

  return DefWindowProc( hWnd, msg, wParam, lParam );
}

  void ShowErrorMessage (const std::string& message)
  {
		bool logSaved = false;
    //logSaved = LOGGER->SaveLogsInFile();
    std::string end_message = "";
		if (logSaved)    
		{
			end_message += "Sorry, Application failed. Logs saved\n";}
		else
		{    
			end_message += "Sorry, Application failed. Logs could not be saved\n";
		}
    end_message += message;
    MessageBox(0, end_message.c_str(), "FlostiProject Report", MB_OK | MB_ICONERROR);
		}

//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{
	#if defined( _DEBUG )
		MemLeaks::MemoryBegin();
	#endif //defined(_DEBUG)

  CEngine engine;

  CProcess * mainProcess = new CMainProcess();
  //CProcess * gameProcess = new CGameProcess_t();
	//CProcess * gameProcess = new CGameProcess();
	CProcess * gameProcess = new CVideoGame();
  
	//CProcess * gameProcess = new CExamen_Test3D();
	//CProcess * gameProcess = new CExamen_Snake();
  //CProcess * gameProcess = new CProcessIA(); //ejercicio caza-evasion
  //CProcess * gameProcess = new CProcessIA2(); //comportamiento tres tipos de zombis TODO..
  //CProcess * gameProcess = new CProcessIA3(); //comportamiento zombie olfateador - A*
  //CProcess * gameProcess = new CProcessIA4(); //comportamiento tres tipos de zombis + physics + maquinas estados + callbacks
  //CProcess * gameProcess = new CProcessParticles();
	//CProcess * gameProcess = new CProcessPhysX();
  //CProcess * gameProcess = new CProcessTriggers(); //utilizacion basica de triggers
  //CProcess * gameProcess = new CGUIProcess(); //utilizacion basica de triggers

  std::string l_XMLPath = "Data/init.xml";
  engine.AddProcess("main", mainProcess);
	engine.AddProcess("game", gameProcess);
	engine.SetProcess(mainProcess);
  //engine.SetProcess(gameProcess);//For debug only
  //Mat33f m;
  // Register the window class
  WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };
 
  RegisterClassEx( &wc );
	try{
		// Create the application's window
		/*
			TODO
			Aqui va el Engine::LoadParameters
			Aqui se setea manualmente el valor de la altura y anchura de la ventana.
			Esto hay que cargarlo directamente del XML y no setearlo manualmente de aqui.
		*/
    engine.LoadConfig(l_XMLPath);
		uint32 w = engine.GetInitParams().m_uWidthScreen;
		uint32 h = engine.GetInitParams().m_uHeightScreen;
		
		HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 0, 0, w, h, NULL, NULL, wc.hInstance, NULL );
    engine.Init(hWnd);

		//CXMLTreeNode NewXML;


		// Añadir aquí el Init de la applicación

		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );

		// Añadir en el while la condición de salida del programa de la aplicación

		while( msg.message != WM_QUIT )
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				// Main loop: Añadir aquí el Update y Render de la aplicación principal
				engine.Update();
				engine.Render();
			}
		}
	}
	catch(CException& e)
  {
    ShowErrorMessage(e.GetDescription());
  }
	catch (...)
  { 
    ShowErrorMessage("Exception Occured");
  }

  UnregisterClass( APPLICATION_NAME, wc.hInstance );
	//CHECKED_DELETE(g_FlostEngine);



  // Añadir una llamada a la alicación para finalizar/liberar memoria de todos sus datos

  return 0;
}

