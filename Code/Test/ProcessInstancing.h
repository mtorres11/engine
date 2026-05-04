/*----------------------------------------------------------------------------------*\
 * CProcessInstancing class                                                          *
 * Author: Miquel Rojas                                                             *
 * Last Modification:																															  *
 * Description:                                                                     *                                                            *
 * Clase para probar instancing               		   															  *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_INSTANCING_PROCESS_H_
#define INC_INSTANCING_PROCESS_H_

#include "Core.h"
#include "Process.h"
#include "Vertex/VertexType.h"
#include "InputManager.h"
#include "Math/MathUtils.h"
#include "StaticMeshes/StaticMeshManager.h"


//-----------FORWARD DECLARATION-----------
class CObject3D;
class CRenderManager;
class CActionToInput;
class CRenderableAnimatedInstanceModel;
class CRenderableObject;
//-----------------------------------------


class CProcessInstancing: public CProcess
{
public:
	CProcessInstancing();
	virtual ~CProcessInstancing();

  //functions
	void							Init						      					();
	void							Update					      					(float elapsedTime);
	void							Render					      					(CRenderManager * rm);
	void							RenderScene											(CRenderManager * rm);
  void							RenderDebugInfo									();
  void              PreRender                       (CRenderManager* rm);
  void              RenderInstancing                (CRenderManager* rm);

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

  float m_MovementSpeed;
  float m_DeltaFromXML;
  CActionToInput*																		m_pAction;
  CSceneEffectManager*                              m_SceneEffectManager;
};

#endif

