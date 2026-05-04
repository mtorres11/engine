/*----------------------------------------------------------------------------------*\
 * CEngine class                                                                    *
 * Author: Marco Torres                                                             *
 * Last Modification: Marco Torres                                                  *
 * Description:                                                                     *
 * Clase Engine.                                                                    *
 * Motor de videojuego                                                              *
\*----------------------------------------------------------------------------------*/

#pragma once
#ifndef INC_ENGINE_H_
#define INC_ENGINE_H_


#include "Timer/Timer.h"
#include "Math/MathTypes.h"
#include "Base.h"
#include <string>
#include <Windows.h>
#include "Core.h"//class CCore;
using namespace std;

#include "Process.h"
#include "MapManager/MapManager.h"



//--- Forward Declaration---
class CRenderManager;
class CLogRender;
class CConsole;
//--------------------------

//-- Parametros de configuracion por defecto --
#define P_FULLSCREEN    false  
#define P_WIDTHSCREEN   800
#define P_HEIGHTSCREEN  600
#define P_CONSOLE       false
#define P_LOGRENDER     false
#define P_MOUSE_ENABLED false
#define P_GUI_ENABLED   true
//---------------------------------------------

class CEngine : CMapManager<CProcess>
{
public:
  //Construct
	CEngine(): m_pProcess(0), m_pNextProcess(0), m_pCore(0), m_Timer(30), m_pLogRender(0), m_pConsole(0), m_bChangeProcess(0), m_bExit(0){}
  //Destruct
	virtual ~CEngine();
  //functions
  void				Init						(HWND hWnd);
  void        LoadConfig  		(const std::string & pathXML);
	void				Update					();
	void				Render					();
  void				SetProcess			(CProcess * process){m_pProcess = process;}
  CProcess *	GetProcess			(){return m_pProcess;}

	void				AddProcess			(const std::string & _sProcessId, CProcess* _pProcess);
	void				SetActiveProcess(const std::string & _sProcess);
	void				SetGameProcess	();
	void				Exit						();

	void				RenderScene			(CRenderManager* rm);
	void				Release					();
	CTimer			GetTimer				(){return m_Timer;}
  SInitParams GetInitParams		(){return m_ConfigParams;}

private:
  CProcess *	m_pProcess;
	CProcess *	m_pNextProcess;
  CCore *			m_pCore;
	CTimer			m_Timer;
  SInitParams m_ConfigParams;
  CLogRender*	m_pLogRender;
  CConsole *  m_pConsole;

	bool				m_bChangeProcess;
	bool				m_bExit;
};

#endif /* INC_ENGINE_H_ */