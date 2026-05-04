//----------------------------------------------------------------------------------
// CDirectionalLight class
// Author: Katherine Capdevilla
// Last modification: Andrea Cristobo - 10/01/11
// Description:
// Otro tipo de luz
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_DIRECTIONAL_LIGHT_H_
#define INC_DIRECTIONAL_LIGHT_H_

#include "Base.h"
#include "Light.h"
#include "Object3D/Object3D.h"
#include "Math/Matrix44.h"
#include "MapManager/MapManager.h"
#include <string>


class CDirectionalLight : public CLight
{
	protected:
		Vect3f			m_Direction;

	public:
    CDirectionalLight():CLight(){}
		void SetDirection(const Vect3f &Direction);
		Vect3f GetDirection() const;
		virtual void Render(CRenderManager *RM);
		Mat44f CDirectionalLight::GetLightViewMatrix()  const;
		Mat44f CDirectionalLight::GetLightProjectionMatrix() const;
};

#endif //INC_DIRECTIONAL_LIGHT_H_