//----------------------------------------------------------------------------------
// CSceneEffect class
// Author: Katherine Capdevilla y Miquel Rojas
// 
//
// Description:
// Esta será la clase base de la que derivarán todas nuestras clases de efectos de
// escena. La clase contiene un vector de CStageTexture que establece una textura
// en una etapa determinada de nuestro shader.
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_SCENE_EFFECT_H_
#define INC_SCENE_EFFECT_H_

#include "Base.h"
#include "Math/Matrix44.h"
#include "Named/Named.h"
#include "Active/Active.h"

#include <string>
#include <dinput.h> 


//------Forward declarations---------
//class CEffect;
class CProcess;
class CTexture;
class CXMLTreeNode;
class CRenderManager;
//-----------------------------------

class CSceneEffect : public CActive, public CNamed
{
	protected:
		class CStageTexture
		{
			public:
			int												m_StageId;
			CTexture										*m_Texture;
			CStageTexture(int StageId, CTexture *Texture);
			void Activate();
		};

		std::vector<CStageTexture>							m_StageTextures;

	public:
		CSceneEffect(CXMLTreeNode &atts);
		virtual ~CSceneEffect();
		void ActivateTextures();
		void AddStageTexture(int StageId, CTexture *Texture);
		virtual void PreRender(CRenderManager *RM, CProcess *Proc);
		virtual void PostRender(CRenderManager *RM);
    virtual void CaptureFrameBuffers(CRenderManager *RM);
};

#endif