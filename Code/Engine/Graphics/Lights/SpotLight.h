//----------------------------------------------------------------------------------
// CSpotLight class
// Author: Katherine Capdevilla
// Última modificación: Andrea Cristobo 10/01/11
// Description:
// Otro tipo de luz
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_SPOT_LIGHT_H_
#define INC_SPOT_LIGHT_H_

#include "Base.h"
#include "MapManager/MapManager.h"
#include <string>
#include "Object3D/Object3D.h"
#include "Light.h"
#include "DirectionalLight.h"

class CSpotLight : public CDirectionalLight
{
	protected:
		float		m_fAngle;
		float		m_fFallOff;

	public:
    CSpotLight();
		 ~CSpotLight();
		void SetAngle(float _fAngle);
		float GetAngle() const;
		void SetFallOff(const float _fFallOff);
		float GetFallOff() const;
};

#endif //INC_SPOT_LIGHT_H_