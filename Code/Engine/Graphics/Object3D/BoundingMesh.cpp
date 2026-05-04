#include "BoundingMesh.h"
#include "Math/Matrix44.h"
//#include "Core.h"
//#include "PhysicsManager.h"
//#include "CookingMesh/PhysicCookingMesh.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CBoundingMesh::CBoundingMesh()
{
  m_BoundingClass = BOUNDING_MESH;
  m_ASEName = "";
  //m_VertexBufferPhysX(0);
  //m_IndexBufferPhysX(0);
}
///<summary>
/// CBoundingMesh:: Init : Inicializa la mesh de colisión del objeto.
///</summary>
///<param name="&Min" type="Vect3f"></param>
///<param name="&Max" type="Vect3f"></param>
///<param name="BoundingClass" type="int">Mascara del objeto</param>
///<returns name="bool">True si todo se genera exitosamente</returns>
bool CBoundingMesh::Init(std::string ASEName, int BoundingClass)
{
  m_BoundingClass = BoundingClass;
  m_ASEName = ASEName;
  //micocinitademeshes.ReadMeshFromASE("./Data/PhysXASE/suelo.ASE", m_VertexBufferPhysX, m_IndexBufferPhysX);

  return true;
}