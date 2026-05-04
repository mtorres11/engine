/*----------------------------------------------------------------------------------*\
 * MAinProcess class                                                                *
 * Author: Andres                                                                   *
 * Last Modification: Andres                                                        *
 * Description:                                                                     *
 * Proceso de inicio, se carga al iniciar la aplicacion.                            *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_MAIN_PROCESS_H_
#define INC_MAIN_PROCESS_H_

#include "Process.h"
/*#include "InputManager.h"
#include "TexturedVertex.h"
#include "Math/MathUtils.h"
#include "StaticMeshes/StaticMeshManager.h"*/


//-----------FORWARD DECLARATION-----------
class CRenderManager;
/*class CObject3D;
class CActionToInput;
class CRenderableAnimatedInstanceModel;
class CRenderableObject;*/
//-----------------------------------------

class CMainProcess: public CProcess
{
public:
  //Construct
	CMainProcess();
  //Destruct
	virtual ~CMainProcess();

  //functions
	void							Init						      					();
	void							Update					      					(float elapsedTime);
	void							Render					      					(CRenderManager * rm);
	void							RenderScene											(CRenderManager * rm);
  void							RenderDebugInfo									();
  void              PreRender                       (CRenderManager* rm);

private:

	//functions
	void  						CheckActionsFromInputs					(float elapsedTime);
	void  						LoadXMLs												();
	//void 							UpdateLua												();
	void							Render2D												(CRenderManager* rm);
	void							Render3D												(CRenderManager* rm);
};

#endif

