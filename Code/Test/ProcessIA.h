/*----------------------------------------------------------------------------------*\
 * CProcessIA class                                                               *
 * Author: Miquel Rojas                                                             *
 * Last Modification: Miquel Rojas                                                  *
 * Description:                                                                     *
 * Clase ProcessIA.                                                              *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_IA_PROCESS_H_
#define INC_IA_PROCESS_H_

#include "Process.h"
#include "InputManager.h"
//#include "TexturedVertex.h"
#include "Math/MathUtils.h"
#include "StaticMeshes/StaticMeshManager.h"


//-----------FORWARD DECLARATION-----------
class CObject3D;
class CRenderManager;
class CActionToInput;
class CRenderableAnimatedInstanceModel;
class CRenderableObject;
//-----------------------------------------

class CProcessIA: public CProcess
{
public:
  //Construct
	CProcessIA();
  //Destruct
	virtual ~CProcessIA();

  //functions
	void							Init						      					();
	void							Update					      					(float elapsedTime);
  void              PreRender                       (CRenderManager * rm);
  void							Render					      					(CRenderManager * rm);
	void							RenderScene											(CRenderManager * rm);
  void							RenderDebugInfo									();

private:

	//functions
	void  						CheckActionsFromInputs					(float elapsedTime);
	void  						LoadXMLs												();
	void  						InitCameras											();
	Vect3f						GetPositionObj									(CObject3D* p_vObj);
	void 							UpdateLua												();
	void							MovPlayer                       (float elapsedTime);
  void              MovCamera                       ();
	void							MovPlayerIA                     (float elapsedTime);
  void							MovMouse												(); 
	void							Render2D												(CRenderManager* rm);
	void							Render3D												(CRenderManager* rm);
  void							RenderMovIA 										(CRenderManager* rm);
  float             AjusteDireccion                 ();
  void							MemoryTest											();

	//variables
	float																							m_fPosX, m_fPosY, m_TimerCounter, m_iDirection, m_fFPS;
	float 																						m_fRotacion;
	float																							m_MaxVision, m_MinVision;
	bool																							m_bDrawCube;
  float                                             m_DeltaFromXML;
	//CObject3D*																				m_pObject3D;
	//CObject3D*  																			m_pObject3D_Pivotal;
  CActionToInput* m_pAction;
	CRenderableAnimatedInstanceModel * 								m_pPlayer1;
	CRenderableAnimatedInstanceModel * 								m_pPlayer2;
  float m_fSpeed1;
  float m_fSpeed2;
  float m_fAngleParcialPlayer2;
  float m_fAngleTotalPlayer2;
  Vect3f m_V3DirPlayer2Optimo;
  Vect3f m_V3DirectorPlayer2;
  Vect3f m_V3RightPlayer2;
  Vect3f m_V3UpPlayer2;
  
  bool m_bChaser; //false=cazador player 1, true=cazador player 2
	float																							m_MovementSpeed, m_RunTime, m_RestTime;
  CSceneEffectManager* m_SceneEffectManager;
};

#endif

