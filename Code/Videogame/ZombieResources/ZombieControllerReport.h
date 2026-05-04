//----------------------------------------------------------------------------------
// CZombieControllerReport class
// Author: Miquel Rojas
//
// Description:
// Clase abstracta la cual ofrece la interfaz para poder controlar los eventos de 
// choque entre character controllers para zombies
//----------------------------------------------------------------------------------
#pragma once
#ifndef _ZOMBIE_CONTROLLER_REPORT_H
#define _ZOMBIE_CONTROLLER_REPORT_H


//---Engine Includes---//
#undef min
#undef max
//#include "NxPhysics.h"
#include "NxShape.h"
#include "NxController.h"
//#include "CharacterController/PhysicControllerReport.h"
#include <string>
//---------------------//

class CZombieUserData;

class CZombieControllerReport
{    
public:
	CZombieControllerReport() {;}
	virtual ~CZombieControllerReport() {/*Nothing*/;}
	//-----------------------------------
	void onShapeHit(const NxControllerShapeHit& hit);
	void onControllerHit(const std::string &controller, const std::string &other);
};

#endif