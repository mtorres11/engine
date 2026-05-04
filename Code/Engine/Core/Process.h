/*----------------------------------------------------------------------------------*\
* CProcess class                                                                   *
* Author: Marco Torres                                                             *
* Last Modification: Marco Torres                                                  *
* Description:                                                                     *
* Clase Engine.                                                                    *
* Descripcion aqui                                                                 *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_PROCESS_H_
#define INC_PROCESS_H_

#include "Base.h"

//---Forward Declarations---
class CCamera;
class CRenderManager;
class CFontManager;
class CObject3D;
class CSceneEffectManager;
class CLogRender;
//--------------------------

class CProcess{
public:
  //Consruct
  CProcess(){ /*m_SceneEffectManager=0;*/};
  //Destruct
  virtual ~CProcess(){;};

  //functions
  virtual void	Update					(float elapsedTime) = 0;
  virtual void	Render					(CRenderManager * rm) = 0;
  virtual void	Init						() = 0;
  virtual void	PreRender				(CRenderManager * rm);
  virtual void	RenderDebugInfo	();
  //CCamera *			GetCamera				() {return m_pCamera;}
  virtual void	RenderScene(CRenderManager * rm) = 0;
  virtual CLogRender*   GetLogRender(){return m_pLogRender;}
protected:
  //CCamera* m_pCamera;
  //CSceneEffectManager* m_SceneEffectManager;
  CLogRender* m_pLogRender;	
};

#endif