//----------------------------------------------------------------------------------
// CBoundingBox class
// Author: Enric Vergara
// Modificada: Marco
// Description:
// Esta clase será la base de toda instancia fisica que pueda existir en la escena
// Pertenece al CObject3D
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_BOUNDING_CAPSULE_H_
#define INC_BOUNDING_CAPSULE_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "Object3d/BoundingObject.h"

class CBoundingCapsule : CBoundingObject
{
public:
  CBoundingCapsule();
  ~CBoundingCapsule() {};

  bool        Init              (Vect3f& Min, Vect3f& Max, int BoundingClass);
	float       GetRadius         () { return m_Radius; };
	float				GetHeight					() { return m_Height; };
  //void        Release           ();
  
private:
  float m_Radius, m_heigth;
	float				m_Height;
};

#endif