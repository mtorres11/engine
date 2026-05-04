//----------------------------------------------------------------------------------
// CBoundingBox class
// Author: Enric Vergara
// Modificada: Marco
// Description:
// Esta clase será la base de toda instancia fisica que pueda existir en la escena
// Pertenece al CObject3D
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_BOUNDING_SPHERE_H_
#define INC_BOUNDING_SPHERE_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "BoundingObject.h"

class CBoundingSphere : public CBoundingObject
{
public:
  CBoundingSphere();
  ~CBoundingSphere() {};

  bool        Init              (Vect3f& Min, Vect3f& Max, int BoundingClass);
  float       GetRadius         () { return m_Radius; };
  //void        Release           ();
  
private:
  float       m_Radius;
};

#endif