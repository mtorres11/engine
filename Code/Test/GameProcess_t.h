/*----------------------------------------------------------------------------------*\
 * CGameProcess class                                                               *
 * Author: Marco Torres                                                             *
 * Last Modification: Marco Torres                                                  *
 * Description:                                                                     *
 * Clase GameProcess.                                                              *
 * No se muy bien cual es la funcion asi que si alguien lo sabe lo puede arreglar   *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_TEST_PROCESS_H_
#define INC_TEST_PROCESS_H_

#include "Process.h"
#include "InputManager.h"
//#include "TexturedVertex.h"
#include "Math/MathUtils.h"
#include "StaticMeshes/StaticMeshManager.h"


//-----------FORWARD DECLARATION-----------
class CObject3D;
class CRenderManager;
//class CActionToInput;
//class CRenderableAnimatedInstanceModel;
//class CRenderableObject;
//class CRenderablePhysXObject;
//class CRenderablePhysXManager;
class CPlayer;
class CCoreFIH;
//-----------------------------------------


class CGameProcess_t: public CProcess
{
public:
  //Construct
	CGameProcess_t();
  //Destruct
	virtual ~CGameProcess_t();

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
	void  						InitCameras											();
	Vect3f						GetPositionObj									(CObject3D* p_vObj);
	void 							UpdateLua												();
	void							MovPlayer                       (float elapsedTime);
	void							MovMouse												(); 
	void							Render2D												(CRenderManager* rm);
	void							Render3D												(CRenderManager* rm);
	void							MemoryTest											();
  void              UpdateCamera                    ();
//  void              Register2LUA                    ();

	//variables
	float																							m_fPosX, m_fPosY, m_TimerCounter, m_iDirection, m_fFPS;
	float 																						m_fRotacion;
	float																							m_MaxVision, m_MinVision;
	bool																							m_bDrawCube;
  float                                             m_DeltaFromXML;
	//CObject3D*																				m_pObject3D;
	//CObject3D*  																			m_pObject3D_Pivotal;
  //CActionToInput* m_pAction;
	/*CRenderableAnimatedInstanceModel * 								m_pPlayer;
	CRenderableAnimatedInstanceModel * 								m_pPlayer2;*/
	float																							m_MovementSpeed, m_RunTime, m_RestTime;
	std::string                                       m_ray;
  std::string                                       m_class;
  //CRenderablePhysXManager *                         m_RenderablePhysXManager;
  //CRenderablePhysXObject *													l_PhysXTest;

  CCoreFIH*																					m_pCoregame;
  CPlayer *                                         m_pPlayer;

  /*Pruebas de tiempo*/
  float m_CheckActions, m_MoveMouse, m_UpdateLua, m_MovPlayer, mParticles, m_PhysX, m_IA;

};

#endif

