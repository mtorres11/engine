/*----------------------------------------------------------------------------------*\
 * CGameProcess class                                                               *
 * Author: Marco Torres                                                             *
 * Last Modification: Marco Torres                                                  *
 * Description:                                                                     *
 * Clase GameProcess.                                                              *
 * No se muy bien cual es la funcion asi que si alguien lo sabe lo puede arreglar   *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef VIDEOGAME_PROCESS_H_
#define VIDEOGAME_PROCESS_H_

#include "Process.h"
#include "InputManager.h"
#include "StaticMeshes/StaticMeshManager.h"


//-----------FORWARD DECLARATION-----------
class CObject3D;
class CRenderManager;
class CActionToInput;
class CRenderableAnimatedInstanceModel;
//-----------------------------------------

class Videogame: public CProcess
{
public:
  //Construct
	Videogame();
  //Destruct
	virtual ~Videogame();

  //functions
	void	Init						      ();
	void	Update					      (float elapsedTime);
	void	Render					      (CRenderManager * rm);
	void	RenderScene(CRenderManager * rm);
  void	RenderDebugInfo	      ();
  void  CheckActionsFromInputs(float elapsedTime);
private:
	float				m_fPosX, m_fPosY, m_TimerCounter, m_iDirection, m_fFPS;
	float 			m_fRotacion;
	float				m_MaxVision, m_MinVision;
	bool				m_bDrawCube;
	CObject3D*	m_pObject3D;
  CActionToInput* m_pAction;
  float m_MovementSpeed, m_RunTime, m_RestTime;
  CRenderableAnimatedInstanceModel * m_pPlayer;
};

#endif

