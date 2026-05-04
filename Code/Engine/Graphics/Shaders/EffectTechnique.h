//----------------------------------------------------------------------------------
// CTechnique class
// Author: Andrea Cristobo
//
// Description:
// Clase que encapsula la información que utilizaremos para cada una de las techniques 
// de nuestro Effect.
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_EFFECTTECHNIQUE_H_
#define INC_EFFECTTECHNIQUE_H_

#include "Base.h"
#include "MapManager/MapManager.h"
#include <string>
#include "Object3D/Object3D.h"
#include "Math/Matrix44.h"
//#include "Effect.h"
#include <d3dx9.h> 
#include "xml/XMLTreeNode.h"

class CEffect;

class CEffectTechnique
{
	private:
		bool										m_UseCameraPosition;
		bool										m_UseInverseProjMatrix;
		bool										m_UseInverseViewMatrix;
		bool										m_UseInverseWorldMatrix;
		bool										m_UseLights;
		int											m_NumOfLights;
		bool										m_UseLightAmbientColor;
		bool										m_UseProjMatrix;
		bool										m_UseViewMatrix;
		bool										m_UseWorldMatrix;
		bool										m_UseWorldViewMatrix;
		bool										m_UseWorldViewProjectionMatrix;
		bool										m_UseViewProjectionMatrix;
		bool										m_UseViewToLightProjectionMatrix;
		bool										m_UseTime;
		bool										m_UseLightsDirection;
		bool										m_UseLightsColor;
		bool										m_UseLightsPosition;
		bool										m_UseLightsEnabled;
		bool										m_UseLightsType;
		bool										m_UseLightsStartRange;
		bool										m_UseLightsEndRange;
    bool                    m_UseSelfIlluminationColor;
		bool										m_UseInstancingPos;
		float                   m_TimeParameter;
    CEffect*								m_Effect;
		D3DXHANDLE							m_D3DTechnique;
		std::string							m_TechniqueName;
		std::string							m_EffectName;

	public:
    CEffectTechnique ();

		void                    Init                (CXMLTreeNode &TreeNode);
		~CEffectTechnique();

		inline CEffect*					GetEffect						() const {return m_Effect;}
		bool										BeginRender					(Vect3f* _pV3fSelfIlluminationColor = NULL);
		bool										Refresh							();
		//DirectX Methods Interface
		D3DXHANDLE							GetD3DTechnique			();
};

#endif