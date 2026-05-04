//----------------------------------------------------------------------------------
// CEffectManager class
// Author: Andrea Cristobo
//
// Description:
// 
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_RENDER_TEXTURE_SCENE_EFFECT_H_
#define INC_RENDER_TEXTURE_SCENE_EFFECT_H_

#include "Base.h"
#include <string>
#include "Math/Matrix44.h"
#include <d3dx9.h>
#include "xml/XMLTreeNode.h"
//#include "EffectManager.h"
#include "RenderTextureSceneEffect.h"

class CProcess;
class CEffectTechnique;
class CRenderManager;


class CRenderToTextureSceneEffect : public CRenderTextureSceneEffect
{
	protected:
		CEffectTechnique *							m_StaticMeshTechnique;
		CEffectTechnique *							m_AnimatedModelTechnique;
	public:
		CRenderToTextureSceneEffect	(CXMLTreeNode &atts);
		~CRenderToTextureSceneEffect ();
		void	PreRender(CRenderManager *RM, CProcess *Proc);
};
#endif
