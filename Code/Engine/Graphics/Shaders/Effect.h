//----------------------------------------------------------------------------------
// CEffect class
// Author: Andrea Cristobo
// Modificaciones : Katherine Capdevilla - 13-01-2011
//
// Description:
// Clase en la que encapsularemos la clase LPD3DXEFFECT de DirectX.
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_EFFECT_H_
#define INC_EFFECT_H_

#include "Base.h"
#include "MapManager/MapManager.h"
#include <string>
#include "Object3D/Object3D.h"
#include "Math/Matrix44.h"
#include <d3dx9.h> 

#define MAX_LIGHTS_BY_SHADER 4
#define MAX_INSTANCES_BY_SHADER 10

class CEffect
{
	public:
		std::string										m_FileName;
		LPD3DXEFFECT									m_Effect;
		BOOL													m_LightsEnabled[MAX_LIGHTS_BY_SHADER];
		int														m_LightsType[MAX_LIGHTS_BY_SHADER];
		float													m_LightsAngle[MAX_LIGHTS_BY_SHADER];
		float													m_LightsFallOff[MAX_LIGHTS_BY_SHADER];
		float													m_LightsStartRangeAttenuation[MAX_LIGHTS_BY_SHADER];
		float													m_LightsEndRangeAttenuation[MAX_LIGHTS_BY_SHADER];
		Vect3f												m_LightsPosition[MAX_LIGHTS_BY_SHADER];
		Vect3f												m_LightsDirection[MAX_LIGHTS_BY_SHADER];
		Vect3f												m_LightsColor[MAX_LIGHTS_BY_SHADER];
		D3DXHANDLE										m_WorldMatrixParameter;
		D3DXHANDLE										m_ViewMatrixParameter;
		D3DXHANDLE										m_ProjectionMatrixParameter;
		D3DXHANDLE										m_WorldViewMatrixParameter;
		D3DXHANDLE										m_ViewProjectionMatrixParameter;
		D3DXHANDLE										m_WorldViewProjectionMatrixParameter;
		D3DXHANDLE										m_ViewToLightProjectionMatrixParameter;
		D3DXHANDLE										m_LightEnabledParameter;
		D3DXHANDLE										m_LightsTypeParameter;
		D3DXHANDLE										m_LightsPositionParameter;
		D3DXHANDLE										m_LightsDirectionParameter;
		D3DXHANDLE										m_LightsAngleParameter;
		D3DXHANDLE										m_LightsColorParameter;
		D3DXHANDLE										m_LightsFallOffParameter;
		D3DXHANDLE										m_LightsStartRangeAttenuationParameter;
		D3DXHANDLE										m_LightsEndRangeAttenuationParameter;
		D3DXHANDLE										m_CameraPositionParameter;
		D3DXHANDLE										m_BonesParameter;
		D3DXHANDLE										m_TimeParameter;

		Vect3f												m_InstancesPosition[MAX_INSTANCES_BY_SHADER];//Instancing
		D3DXHANDLE										m_InstancesPositionParameter; //Instancing

    Vect4f                        m_AmbientLight;												
    D3DXHANDLE                    m_AmbientLightParameter;
    D3DXHANDLE                    m_SelfIlluminationColorParameter;
		
		CEffect();
		~CEffect();
		bool											Load																			(const std::string &FileName);
		bool											Reload																		();
		bool											LoadEffect																();
		bool											SetLights																	(size_t NumOfLights);
		
		void											Release																		();
		void											GetParameterBySemantic										(const std::string &SemanticName, D3DXHANDLE	&l_Handle);
		void											Unload																		();
		void											SetNullParameters													();

		//DirectX Methods Interface
		LPD3DXEFFECT									GetD3DEffect															()const;
		D3DXHANDLE										GetTechniqueByName												(const std::string &TechniqueName);
};

#endif //INC_EFFECT_H_