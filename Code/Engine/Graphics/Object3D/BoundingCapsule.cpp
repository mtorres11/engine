#include "BoundingCapsule.h"
#include "Math/Matrix44.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CBoundingCapsule::CBoundingCapsule()
{
  m_BoundingClass = BOUNDING_CAPSULE;
  m_MaxSideLenght = m_Radius = m_Height = 0.0f;
  m_Dimension = m_MiddlePoint = Vect3f(0.0f);
}
///<summary>
/// CBoundingCapsule:: Init : Inicializa la capsula de colisión del objeto.
///</summary>
///<param name="&Min" type="Vect3f"></param>
///<param name="&Max" type="Vect3f"></param>
///<param name="BoundingClass" type="int">Mascara del objeto</param>
///<returns name="bool">True si todo se genera exitosamente</returns>
bool CBoundingCapsule::Init(Vect3f& Min, Vect3f& Max, int BoundingClass)
{
	//Asigna la clase del Bounding Object
  m_BoundingClass = BoundingClass;

  //Calcula Punto Medio
	//Pto Medio = (Pto Max - Pto Min)/2 + Pto Min
	m_MiddlePoint = ((Max + Min)*0.5f);

	//Calcula Altura
	Vect3f l_min = Vect3f(Min.x,Min.y,Min.z);
	Vect3f l_max = Vect3f(Min.x,Max.y,Min.z);
	m_Height = l_min.Distance(l_max);

  //Maximos y minimos
  m_Min = Min;
  m_Max = Max;

  //Radio Maximo
  m_MaxRadius = m_MiddlePoint.Distance(m_Max);

  //Calcula Dimension
	m_Radius = (Min.Distance(Max))/m_Height;

	return true;
}