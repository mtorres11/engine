/*----------------------------------------------------------------------------------*\
 * GUIProcess class                                                                *
 * Author: Andres                                                                   *
 * Last Modification: Andres                                                        *
 * Description:                                                                     *
 * Proceso para pruebas de GUI                                                      *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_GUI_PROCESS_H_
#define INC_GUI_PROCESS_H_

#include "Process.h"


//-----------FORWARD DECLARATION-----------
class CRenderManager;
class CHealthIndicator;
//-----------------------------------------

class CGUIProcess: public CProcess
{
public:
  //Construct
	CGUIProcess();
  //Destruct
	virtual ~CGUIProcess();

  //functions
	void							Init						      					();
	void							Update					      					(float elapsedTime);
	void							Render					      					(CRenderManager * rm);
	void							RenderScene											(CRenderManager * rm);
  void							RenderDebugInfo									();
  void              PreRender                       (CRenderManager* rm);

private:

	//functions
  void              LoadXMLs                        ();
	void  						CheckActionsFromInputs					(float elapsedTime);
	void 							UpdateLua												();
	void							Render2D												(CRenderManager* rm);
	void							Render3D												(CRenderManager* rm);
  void              Register2LUA                    ();

  CHealthIndicator* m_pHealthIndicator;
};

#endif

