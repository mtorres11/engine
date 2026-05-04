//----------------------------------------------------------------------------------
// CDrawQuadSceneEffect class
// Author: Katherine Capdevilla
//
// Description:
// Esta clase nos permitirá hacer efectos de PostProcesado
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_DRAW_QUAD_SCENE_EFFECT_H_
#define INC_DRAW_QUAD_SCENE_EFFECT_H_

#include "Base.h"
#include <string>
#include "Object3D/Object3D.h"
#include "Math/Matrix44.h"
#include <d3dx9.h> 
#include "SceneEffect.h"
#include "RenderManager.h"
#include "EffectTechnique.h"
#include "XML/XMLTreeNode.h"


class CDrawQuadSceneEffect : public CSceneEffect
{
	protected:
		CEffectTechnique *m_Technique;
		CColor m_Color;

	public:
		CDrawQuadSceneEffect(CXMLTreeNode &atts);
		~CDrawQuadSceneEffect();
		void PostRender(CRenderManager *RM);
};

#endif