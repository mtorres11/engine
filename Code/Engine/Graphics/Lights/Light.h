//----------------------------------------------------------------------------------
// CLight class
// Author: Katherine Capdevilla
//
// Description:
// Crear Luces
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_LIGHT_H_
#define INC_LIGHT_H_

#include "Base.h"
#include "MapManager/MapManager.h"
#include <string>
#include "Object3D/Object3D.h"
#include "Math/Matrix44.h"
#include "Math/Color.h"

class CLightEffect;

class CLight : public CObject3D
{
public:
	enum TLightType
	{
		OMNI=0,
		DIRECTIONAL,
		SPOT
	};

	protected:
		CColor										m_Color;
    float                     m_fMultiplier; //intensidad de la luz
		TLightType								m_Type;
		std::string								m_sName;
		bool											m_bRenderShadows;//si proyecta sombras
		float											m_fStartRangeAttenuation;//comienzo de rango de la atenuacion
		float											m_fEndRangeAttenuation;//fin de rango de la atenuacion
		bool											m_bEnable;//si la luz esta activa o no
    CLightEffect *            m_pLightEffect;
    
		static TLightType					GetLightTypeByName(const std::string &StrLightType);

	public:
		CLight();
		virtual ~CLight();
		void SetName(const std::string &_sName);
		const std::string &GetName();
		void SetColor(const CColor &Color);
    void SetMultiplier(float _fMultiplier);
		const CColor & GetColor() const;
		float GetMultiplier();
		void SetStartRangeAttenuation(const float _fStartRangeAttenuation);
		float GetStartRangeAttenuation() const;
		void SetEndRangeAttenuation(const float f_EndRangeAttenuation);
		float GetEndRangeAttenuation() const;
		bool RenderShadows() const;
		void SetType(const TLightType _Type);
		TLightType GetType() const;
		virtual void      Render(CRenderManager *_pRM);

		void              SetEnable(bool _bEnable);
		bool              GetEnable();
    void              SetEffect(CLightEffect* _pEffect = NULL) { m_pLightEffect = _pEffect; }
    CLightEffect *    GetEffect() { return m_pLightEffect; }

};

#endif //INC_LIGHT_H_