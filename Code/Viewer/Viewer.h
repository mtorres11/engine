/*----------------------------------------------------------------------------------*\
 * CGameProcess class                                                               *
 * Author: Marco Torres                                                             *
 * Last Modification: Miquel Rojas                                                  *
 * Description:                                                                     *
 * Clase GameProcess.                                                              *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef VIEWER_PROCESS_H_
#define VIEWER_PROCESS_H_

#include "Process.h"
#include "InputManager.h"
#include "StaticMeshes/StaticMeshManager.h"
#include <vector>
#include "RenderableObjects/RenderableObject.h"

//-----------FORWARD DECLARATION-----------
class CObject3D;
class CRenderManager;
class CActionToInput;
class CRenderableAnimatedInstanceModel;
class CLight;
class CSceneEffectManager;
//-----------------------------------------

struct SRecordableData
{
  Vect3f                                      m_V3ObjectPos;
  Vect3f                                      m_V3ObjectYawPitchRoll;
};

class CViewer: public CProcess
{
public:
  //Construct
	CViewer();
  //Destruct
	virtual ~CViewer();
  //functions
	void	Init																();
	void	Update					      							(float elapsedTime);
	void	Render					      							(CRenderManager * rm);
	void	RenderScene													(CRenderManager * rm);
	void	RenderDebugInfo											();
  void              PreRender                       (CRenderManager* rm);
  void  CheckActionsFromInputs							(float elapsedTime);
  void  LoadXMLs                            ();
  void  InitCameras                         ();
	Vect3f GetPositionObj								    	(CObject3D* p_vObj);
	void 	UpdateLua											    	();
	void	MovPlayer                           (float elapsedTime);
	void	MovMouse											    	(); 
  void	Render2D											    	(CRenderManager* rm);
	void	Render3D											    	(CRenderManager* rm);
	void	MemoryTest										    	();
	std::string OpenDialogBoxViewer           ();
  bool  CreateXMLs                          (std::string);
  std::string& replaceAll(std::string& context, const std::string& from, const std::string& to);
  CLight* GetDirecionalLight (){return m_pDirectionalLight;}

  //variables
  bool m_bShowRenderDebugInfo;
  bool m_bIsScene;
  bool m_bSceneCleared;
  int  m_iCurrentCycle;
private:
	//functions

	//variables
	float																			m_fPosX, m_fPosY, m_TimerCounter, m_iDirection, m_fFPS;
	float 																		m_fRotacion;
	float																			m_MaxVision, m_MinVision;
	bool																							m_bDrawCube;
	float       															m_DeltaFromXML;
	CRenderableAnimatedInstanceModel * 				m_pPlayer;
	CObject3D*  																			m_pObject3D_Pivotal;
  CActionToInput*														m_pAction;
 // CSceneEffectManager* m_SceneEffectManager;
  float                                     m_MovementSpeed;
  std::vector<CRenderableObject*>            m_vRenderObjects;
  //std::vector<CRenderableObject*>            m_vRenderMeshes;
  //std::vector<CRenderableObject*>            m_vRenderModels;
  int  m_iActiveElement;
  std::vector<SRecordableData>                  m_vScenePositions;
  CLight* m_pDirectionalLight;  
};

#endif

