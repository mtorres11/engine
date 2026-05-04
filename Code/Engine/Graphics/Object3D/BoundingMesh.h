//----------------------------------------------------------------------------------
// CBoundingMesh class
// Author: Marco Torres
// Modificada: Marco
// Description:
// Esta clase es el boundingo object identico a una malla renderizada
// Pertenece al CObject3D
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_BOUNDING_MESH_H_
#define INC_BOUNDING_MESH_H_

#include "Math/MathTypes.h"
#include "Math/Vector3.h"
#include "Math/MathUtils.h"
#include "BoundingObject.h"
#include <string>
#include <vector>

class CBoundingMesh : public CBoundingObject
{
public:
  CBoundingMesh();
  ~CBoundingMesh() {};

  bool                        Init              (std::string ASEName, int BoundingClass);
  //void                        Release           ();
  std::vector<Vect3f>         GetVertexBuffer   (){ return m_VertexBufferPhysX; }
  std::vector<unsigned int>   GetIndexBuffer    (){ return m_IndexBufferPhysX; }
  std::string                 GetName           (){ return m_ASEName; }

  void                        SetVertexBuffer   ( std::vector<Vect3f> VertexBuffer )      { m_VertexBufferPhysX = VertexBuffer; }
  void                        SetIndexBuffer    ( std::vector<unsigned int> IndexBuffer ) { m_IndexBufferPhysX = IndexBuffer; }
  
private:
  std::string m_ASEName;
  std::vector<Vect3f> m_VertexBufferPhysX;
  std::vector<unsigned int> m_IndexBufferPhysX;
};

#endif