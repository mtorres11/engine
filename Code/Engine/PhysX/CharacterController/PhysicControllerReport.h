//----------------------------------------------------------------------------------
// CPhysicControllerReport class
// Author: Miquel Rojas
//
// Description:
// Clase abstracta la cual ofrece la interfaz para poder controlar los eventos de 
// choque entre character controllers
//----------------------------------------------------------------------------------
#pragma once
#ifndef _PHYSIC_CONTROLLER_REPORT_H
#define _PHYSIC_CONTROLLER_REPORT_H


//---Engine Includes---//
#undef min
#undef max
//#include "NxPhysics.h"
#include "NxShape.h"
#include "NxController.h"
//---------------------//

class CPhysicUserData;

class CPhysicControllerReport: public NxUserControllerHitReport    
{    
public:
	CPhysicControllerReport() {;}
	virtual ~CPhysicControllerReport() {/*Nothing*/;}
	//-----------------------------------
	virtual NxControllerAction  onShapeHit(const NxControllerShapeHit& hit);
	virtual NxControllerAction  onControllerHit(const NxControllersHit& hit);
};

#endif