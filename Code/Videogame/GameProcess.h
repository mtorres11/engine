/*----------------------------------------------------------------------------------*\
 * CGameProcess class                                                               *
 * Author: Marco Torres                                                             *
 * Last Modification: Miquel Rojas                                                  *
 * Description:                                                                     *
 * Clase GameProcess.                                                              *
 * Proceso del juego                                                                *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_TEST_PROCESS_H_
#define INC_TEST_PROCESS_H_

#include "Process.h"
#include "InputManager.h"
//#include "TexturedVertex.h"
#include "StaticMeshes/StaticMesh.h"


//-----------FORWARD DECLARATION-----------
class CObject3D;
class CRenderManager;
class CActionToInput;
//-----------------------------------------

class CGameProcess: public CProcess
{
public:
  //Construct
	CGameProcess();
  //Destruct
	virtual ~CGameProcess();

  //functions
	void	Init						      ();
	void	Update					      (float elapsedTime);
	void	Render					      (CRenderManager * rm);
  void	RenderDebugInfo	      (CRenderManager * rm, CFontManager* fm, float fps);
  void  CheckActionsFromInputs(float elapsedTime);
private:
};

#endif

