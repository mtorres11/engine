/*----------------------------------------------------------------------------------*\
 * CProcessParticles class                                                          *
 * Author: Andrea Cristobo                                                          *
 * Last Modification:																															  *
 * Description:                                                                     *                                                            *
 * Clase para hacer pruebas con las partículas		   															  *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_PARTICLE_PROCESS_H_
#define INC_PARTICLE_PROCESS_H_

#include "Core.h"
#include "Process.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleManager.h"
#include "Vertex/VertexType.h"
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


class CProcessParticles: public CProcess
{
public:
	CProcessParticles();
	virtual ~CProcessParticles();

  //functions
	void							Init						      					();
	void							Update					      					(float elapsedTime);
	void							Render					      					(CRenderManager * rm);
	void							RenderScene											(CRenderManager * rm);
  void							RenderDebugInfo									();
  void              PreRender                       (CRenderManager* rm);

private:

		//functions
	void  						LoadXMLs												();
	void  						InitCameras											();
	void							Render2D												(CRenderManager* rm);
	void							Render3D												(CRenderManager* rm);
	void  						CheckActionsFromInputs					(float elapsedTime);
	Vect3f						GetPositionObj									(CObject3D* p_vObj);
	void 							UpdateLua												();
	void							MovPlayer                       (float elapsedTime);
	void							MovMouse												(); 


	//variables
	float																							m_fPosX, m_fPosY, m_TimerCounter, m_iDirection, m_fFPS;
	float 																						m_fRotacion;
	float																							m_MaxVision, m_MinVision;
	bool																							m_bDrawCube;
  float                                             m_DeltaFromXML;
	CObject3D*  																			m_pObject3D_Pivotal;
  CActionToInput*																		m_pAction;
	CRenderableAnimatedInstanceModel * 								m_pPlayer;
	CRenderableAnimatedInstanceModel * 								m_pPlayer2;
	float																							m_MovementSpeed, m_RunTime, m_RestTime;
  CSceneEffectManager*                              m_SceneEffectManager;
	//CParticleEmitter																	m_ParticleEmitter;
	//CParticleManager																	m_ParticleManager;
};

#endif

