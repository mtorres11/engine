#include "BoundingBox.h"
#include "Core.h"
#include "RenderManager.h"
#include "Math/Matrix44.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CBoundingBox::CBoundingBox()
{
  m_BoundingClass = BOUNDING_BOX;
  m_MaxSideLenght = 0.0f;
  m_Dimension = m_MiddlePoint = Vect3f(0.0f);

  for(int i = 0; i < 8; i++)
  {
    m_Box[i] = Vect3f(0.0f);
  }
}

///<summary>
/// CBoundingBox:: Init : Inicializa la caja de colisión del objeto.
///</summary>
///<param name="&Min" type="Vect3f"></param>
///<param name="&Max" type="Vect3f"></param>
///<param name="BoundingClass" type="int">Mascara del objeto</param>
///<returns name="bool">True si todo se genera exitosamente</returns>
bool CBoundingBox::Init(Vect3f& Min, Vect3f& Max, int BoundingClass)
{
  //Asigna la clase del Bounding Object
  m_BoundingClass = BoundingClass;

  /*           4                    5
               _____________________
              /|                  /|
             / |                 / |
            /  |                /  |
           /   |               /   |
          /    |              /    |
         /     |             /     |
      6 /___________________/ 7    |
        |      | 0          |      | 1
        |      |___________ |______|
        |     /             |     / 
        |    /              |    /  
        |   /               |   /   
        |  /                |  /    
        | /                 | /     
      2 |/__________________|/ 3 
  */
  
  //Esta es la caja como la definio el otro grupo.
  //Con la misma numeracion por si hay algun problema
  //Crea la Bounding Box
  //Punto Minimo = m_Box[0]
  //Punto Maximo = m_Box[7]
  //Esquina sobre eje X = m_Box[1]
  //Esquina sobre eje Y = m_Box[4]
  //Esquina sobre eje Z = m_Box[2]
  m_Box[0] = Vect3f(Min.x,Min.y,Min.z);
  m_Box[1] = Vect3f(Max.x,Min.y,Min.z);
  m_Box[2] = Vect3f(Min.x,Min.y,Max.z);
  m_Box[3] = Vect3f(Max.x,Min.y,Max.z);
  m_Box[4] = Vect3f(Min.x,Max.y,Min.z);
  m_Box[5] = Vect3f(Max.x,Max.y,Min.z);
  m_Box[6] = Vect3f(Min.x,Max.y,Max.z);
  m_Box[7] = Vect3f(Max.x,Max.y,Max.z);
  
  //Maximos y minimos
  m_Min = Min;
  m_Max = Max;

  //Calcula Punto Medio
  //Pto Medio = (Pto Max - Pto Min)/2 + Pto Min
  m_MiddlePoint = ((Max + Min)*0.5f);
  
  //Radio Maximo
  m_MaxRadius = m_MiddlePoint.Distance(m_Max);

  //Calcula Dimension
  float l_LengthX = m_Box[0].Distance(m_Box[1]);
  float l_LengthY = m_Box[0].Distance(m_Box[4]);
  float l_LengthZ = m_Box[0].Distance(m_Box[2]);
  m_Dimension = Vect3f(l_LengthX,l_LengthY,l_LengthZ);

  //Calcula el lado maximo
  float l_Max = m_Dimension.x;
  if(l_Max < m_Dimension.y)
  {
     l_Max = m_Dimension.y;
  }
  if(l_Max < m_Dimension.z)
  {
     l_Max = m_Dimension.z;
  }
  m_MaxSideLenght = l_Max;
  return true;
}


void CBoundingBox::DrawBoundingBox()
{
  CRenderManager * RM = CORE->GetRenderManager();
  RM->DrawLine(m_Box[0],m_Box[1],colWHITE);
  RM->DrawLine(m_Box[0],m_Box[2],colWHITE);
  RM->DrawLine(m_Box[0],m_Box[4],colWHITE);
  RM->DrawLine(m_Box[7],m_Box[3],colWHITE);
  RM->DrawLine(m_Box[7],m_Box[5],colWHITE);
  RM->DrawLine(m_Box[7],m_Box[6],colWHITE);
  RM->DrawLine(m_Box[1],m_Box[5],colWHITE);
  RM->DrawLine(m_Box[2],m_Box[6],colWHITE);
  RM->DrawLine(m_Box[4],m_Box[5],colWHITE);
  RM->DrawLine(m_Box[4],m_Box[6],colWHITE);
  RM->DrawLine(m_Box[3],m_Box[2],colWHITE);
  RM->DrawLine(m_Box[3],m_Box[1],colWHITE);
}