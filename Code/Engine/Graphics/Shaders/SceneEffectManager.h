//----------------------------------------------------------------------------------
// CTechnique class
// Author: Miquel Rojas
//
// Description:
// Carga un archivo xml con la información de efectos de escena y recorre todos los vectores segun la fase
// de efecto en la que se encuentre.
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_SCENE_EFFECT_MANAGER_H_
#define INC_SCENE_EFFECT_MANAGER_H_

#include "Base.h"
#include <vector>
#include <string>
#include <d3dx9.h> 

class CSceneEffect;
class CRenderManager;
class CProcess;


class CSceneEffectManager
{
	public:
		std::vector<CSceneEffect *>				m_PreRenderSceneEffects;
		std::vector<CSceneEffect *>				m_RenderSceneEffects;
		std::vector<CSceneEffect *>				m_CaptureFrameBufferSceneEffects;
		std::vector<CSceneEffect *>				m_CaptureFrameBufferSceneEffectsAfterPostRender;
		std::vector<CSceneEffect *>				m_PostRenderSceneEffects;

	public:
		CSceneEffectManager();
		~CSceneEffectManager();

		void								Destroy																();
		void								LoadXML																(const std::string &FileName);
    void                ReloadXML                             (); 
		void								PreRender															(CRenderManager *RM, CProcess *Proc);
    void  Reload(std::vector<CSceneEffect *>&,std::vector<CSceneEffect *>&);
		void								ActivateRenderSceneEffects						();
		void								CaptureFrameBuffers										(CRenderManager *RM);
		void								PostRender														(CRenderManager *RM);
		void								CaptureFrameBuffersAfterPostRender		(CRenderManager *RM);
private:
  std::string           m_Filename;
};

#endif
