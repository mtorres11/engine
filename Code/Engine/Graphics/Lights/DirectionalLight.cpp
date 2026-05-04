#include "Base.h"
#include "MapManager/MapManager.h"
#include "Vertex/VertexType.h"
#include "RenderManager.h"
#include "Object3D/Object3D.h"
#include "Light.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "assert.h"
#include <string>
#include "Math/Matrix44.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

void CDirectionalLight::SetDirection(const Vect3f &Direction)
{
		m_Direction=Direction;
}

Vect3f CDirectionalLight::GetDirection() const
{
    return m_Direction;
}

void CDirectionalLight::Render(CRenderManager *RM)
{
    Mat44f l_t;
	  l_t.SetIdentity();
    l_t.Translate(m_Position);
    RM->SetTransform(l_t);
    RM->DrawAxis(3);
    Vect3f l_PosB=m_Position+m_Direction*20.0f;

	  l_t.SetIdentity();
    l_t.Translate(l_PosB);
    RM->SetTransform(l_t);
    RM->DrawAxis(1);

    l_t.SetIdentity();
	  RM->SetTransform(l_t);
    RM->DrawLine(m_Position, l_PosB, colYELLOW);
}

Mat44f CDirectionalLight::GetLightViewMatrix()  const
{
  D3DXMATRIX l_LightViewMatrix;
  D3DXVECTOR3 l_Eye(m_Position.x, m_Position.y, m_Position.z), 
              l_LookAt(m_Position.x + m_Direction.x*10, m_Position.y + m_Direction.y*10, m_Position.z + m_Direction.z*10), 
							l_VUP(0.0f,1.0f,0.0f);
  D3DXMatrixLookAtLH( &l_LightViewMatrix, &l_Eye, &l_LookAt, &l_VUP);
  return Mat44f(l_LightViewMatrix);
}

Mat44f CDirectionalLight::GetLightProjectionMatrix() const
{
  D3DXMATRIX l_LightProjectionMatrix;
	D3DXMatrixPerspectiveFovLH( &l_LightProjectionMatrix, 60.0f*D3DX_PI/180.0f, 1.0f, 1.0f, 1000.0f );

  return Mat44f(l_LightProjectionMatrix);
}
