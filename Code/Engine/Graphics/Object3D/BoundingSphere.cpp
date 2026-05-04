#include "BoundingSphere.h"
#include "Math/Matrix44.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CBoundingSphere::CBoundingSphere()
{
	m_BoundingClass = BOUNDING_SPHERE;
	m_MaxSideLenght = 0.0;
	m_Dimension = m_MiddlePoint = Vect3f(0.0f);
}
///<summary>
/// CBoundingSphere:: Init : Inicializa la esfera de colisión del objeto.
///</summary>
///<param name="&Min" type="Vect3f"></param>
///<param name="&Max" type="Vect3f"></param>
///<param name="BoundingClass" type="int">Mascara del objeto</param>
///<returns name="bool">True si todo se genera exitosamente</returns>
bool CBoundingSphere::Init(Vect3f& Min, Vect3f& Max, int BoundingClass)
{
	//Asigna la clase del Bounding Object
	m_BoundingClass = BoundingClass;

	//Calcula Punto Medio
	//Pto Medio = (Pto Max - Pto Min)/2 + Pto Min
	m_MiddlePoint = ((Max + Min)*0.5f);
  
  //Maximos y minimos
  m_Min = Min;
  m_Max = Max;
	
  //Radio Maximo
  m_MaxRadius = m_MiddlePoint.Distance(m_Max);

  //radio
	m_Radius = (Min.Distance(Max))/1.68f;
  //m_Radius = (Max.x - Min.x) * 0.5f;

	return true;
}