//----------------------------------------------------------------------------------
// COmniLight class
// Author: Katherine Capdevilla
//
// Description:
// Un tipo de luz
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_OMNILIGHT_H_
#define INC_OMNILIGHT_H_

#include "Base.h"
#include "MapManager/MapManager.h"
#include <string>
#include "Object3D/Object3D.h"
#include "Light.h"

class COmniLight : public CLight
{
	public:
		COmniLight() : CLight() {}
};

#endif //INC_OMNI_LIGHT_H_