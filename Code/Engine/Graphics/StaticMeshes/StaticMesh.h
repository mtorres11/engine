//----------------------------------------------------------------------------------
// CRenderManager class
// Author: Marco Torres
//
// Description:
// blablabla...
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_STATIC_MESH_H_
#define INC_STATIC_MESH_H_

#include <vector>
#include <string>
#include "Math/MathUtils.h"
#include "Math/Vector3.h"

// forward declarations
class CRenderableVertexs;
class CTexture;
class CRenderManager;
class CEffectTechnique;
// end forward declarations

class CStaticMesh
{
protected:
  std::vector<CRenderableVertexs*> m_RVs;
  std::vector<std::vector<CTexture *>> m_Textures;
  std::string m_FileName;
  std::string m_TechniqueName;
  std::vector<CEffectTechnique *>		m_Techniques;
  unsigned int m_NumVertexs, m_NumFaces;
  std::vector<Vect3f> m_VertexBufferPhysX;
  std::vector<uint32> m_IndexBufferPhysX;

public:
  CStaticMesh();
  ~CStaticMesh();
  bool Load (const std::string &FileName);
  bool ReLoad ();
  void Render (CRenderManager *RM) const;
  void Release ();
  unsigned int GetNumFaces()const {return m_NumFaces;}

  void SetSelfIlluminationColor (Vect3f* _pV3fValue) { m_pV3fSelfIlluminationColor = _pV3fValue; }

  /*Funciones para fisica de mallas*/
  std::vector<Vect3f> GetVertexBufferPhysX  ()  { return m_VertexBufferPhysX; }
  std::vector<uint32> GetIndexBufferPhysX   ()  { return m_IndexBufferPhysX; }

private:

  Vect3f* m_pV3fSelfIlluminationColor;

};

#endif //INC_STATIC_MESH_H_