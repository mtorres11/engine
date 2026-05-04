//----------------------------------------------------------------------------------
// CBoundingBox class
// Author: Enric Vergara
// Modificada: Marco
// Description:
// Esta clase será la base de toda instancia fisica que pueda existir en la escena
// Pertenece al CObject3D
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_BOUNDING_OBJECT_H_
#define INC_BOUNDING_OBJECT_H_

//NOTA IMPORTANTE:
//No sirve para terminos reales dentro del videojuego, solo para debug;
//porque los objetos NUNCA colisionan contra los planos.

#define BOUNDING_BOX 0x0001
#define BOUNDING_SPHERE 0x0002
#define BOUNDING_CAPSULE 0x0004
#define BOUNDING_MESH 0x0008
#define BOUNDING_PLANE 0x0010
#define NO_BOUNDING_OBJECT 0x0020

#include "Math\MathTypes.h"
#include "Math\Vector3.h"

class CBoundingObject
{
public:
  CBoundingObject(){}
  virtual ~CBoundingObject() {}

  //virtual bool Init(Vect3f& Min, Vect3f& Max, int BoundingClass){ return false; }
  //virtual bool  Init(int BoundingClass) = 0;
  Vect3f&       GetDimension      () { return m_Dimension; }
  Vect3f&       GetMiddlePoint    () { return m_MiddlePoint; }
  float         GetMaxSideLength  () { return m_MaxSideLenght; }
  int           GetBoundingClass  () { return m_BoundingClass; }
  Vect3f&       GetMin            () { return m_Min; }
  Vect3f&       GetMax            () { return m_Max; }
  float         GetMaxRadius      () { return m_MaxRadius; }
  
protected:
  Vect3f        m_MiddlePoint;
  float         m_MaxSideLenght;
  float         m_MaxRadius;
  Vect3f        m_Dimension;
  int           m_BoundingClass;
  Vect3f        m_Min;
  Vect3f        m_Max;
};

#endif