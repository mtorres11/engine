//----------------------------------------------------------------------------------
// CBoundingBox class
// Author: Enric Vergara
// Modificada: Marco
// Description:
// Esta clase será la base de toda instancia fisica que pueda existir en la escena
// Pertenece al CObject3D
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_BOUNDING_BOX_H_
#define INC_BOUNDING_BOX_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "BoundingObject.h"

class CBoundingBox : public CBoundingObject
{
public:
  CBoundingBox();
  ~CBoundingBox() {};

  bool        Init              (Vect3f& Min, Vect3f& Max, int BoundingClass);
  Vect3f*     GetBox            () { return m_Box; };
  void        DrawBoundingBox   ();
  //void        Release           ();
  
private:
  Vect3f    m_Box[8];
};

#endif