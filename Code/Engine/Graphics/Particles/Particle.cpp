#include "Particle.h"
#include <vector>
#include "Vertex/VertexType.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CParticle:: Constructor : Inicialización de las variables miembro.
///</summary>
CParticle::CParticle()
: m_fSize				(0.0f),
	m_fLifetime   (0.0f),
	m_fAge				(0.0f),
	m_Color				(1.0f,1.0f,1.0f,1.0f),
	m_vPos				(D3DXVECTOR3(1.0f,1.0f,1.0f)),
	m_vDir				(D3DXVECTOR3(1.0f,1.0f,1.0f)),
	m_vVelocity   (D3DXVECTOR3(1.0f,1.0f,1.0f)),
	m_bGravity    (false),
	m_fAngle      (0.0f),
	m_fAngleTotal (0.0f)
{}	

///<summary>
/// CParticle:: Init : Inicialización.
///</summary>
///<param name="void"></param>
///<returns name="result">Booleano que indica si la inicialización se ha realizado con éxito.</returns>
bool CParticle::Init ()
{
	m_bIsOk = true;
	return m_bIsOk;
}

///<summary>
/// CParticle:: Done : Finalize data.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CParticle::Done()
{
	m_ColorVect.clear();
	m_DirVect.clear();
	m_TimeColor.clear();
	m_TimeDir.clear();
	m_SizeVect.clear();
	m_TimeSize.clear();
  if (IsOk())
  {
    m_bIsOk = false;
  }
}

///<summary>
/// CParticle:: Update : Actualización de la posición y vida de las partículas.
///</summary>
///<param name="fTimeDelta">Tiempo transcurrido.</param>
///<returns name="result">Booleano que indica si se ha actualizado con éxito.</returns>
bool CParticle::Update(float fTimeDelta)
{
	m_fAge += fTimeDelta;
	//si el tiempo de vida de la partícula todavía no se ha agotado
	if (m_fAge < m_fLifetime)
	{
		for (unsigned int j=0; j<m_TimeColor.size(); j++)
		{
			if (m_TimeColor[j]<m_fAge)
				m_Color = m_ColorVect[j];
		}
		for (unsigned int i=0; i<m_TimeDir.size(); i++)
		{
			if (m_TimeDir[i]<m_fAge)
				m_vDir = m_DirVect[i];
		}
		for (unsigned int i=0; i<m_TimeSize.size(); i++)
		{
			if (m_TimeSize[i]<m_fAge)
				m_fSize = m_SizeVect[i];
		}
		for (unsigned int i=0; i<m_TimeVelocity.size(); i++)
		{
			if (m_TimeVelocity[i]<m_fAge)
				m_vVelocity = m_VelocityVect[i];
		}
		for (unsigned int i=0; i<m_TimeAngle.size(); i++)
		{
			if (m_TimeAngle[i]<m_fAge)
				m_fAngle = m_AngleVect[i];
		}
		if (!m_bGravity)
		{
			m_vPos.x = m_vPos.x + (m_vVelocity.x*m_vDir.x*fTimeDelta);
			m_vPos.y = m_vPos.y + (m_vVelocity.y*m_vDir.y*fTimeDelta);
			m_vPos.z = m_vPos.z + (m_vVelocity.z*m_vDir.z*fTimeDelta);
		}
		else
		{
			m_vPos.x = m_vPos.x + (m_vVelocity.x*m_vDir.x*fTimeDelta);
			m_vPos.y = m_vPos.y + (m_vVelocity.y*m_vDir.y*fTimeDelta);
			m_vPos.z = m_vPos.z + (m_vVelocity.z*m_vDir.z*fTimeDelta);
			//m_vVelocity.x = m_vVelocity.x*fTimeDelta;
			m_vVelocity.y = m_vVelocity.y-9.8f*fTimeDelta;
			//m_vVelocity.z = m_vVelocity.z*fTimeDelta;
		}
		//con Billboard
		m_Position = Vect3f(m_vPos.x, m_vPos.y, m_vPos.z);
		m_Billboard.SetPos(m_Position);
		m_Billboard.SetSize(m_fSize, m_fSize);
		//rotación
		m_fAngleTotal = m_fAngleTotal + (m_fAngle*fTimeDelta);
		m_Billboard.Update(fTimeDelta, m_fAngleTotal);
		//puntos
		m_PointA = m_Billboard.GetPointA(); 
		m_PointB = m_Billboard.GetPointB();
		m_PointC = m_Billboard.GetPointC();
		m_PointD = m_Billboard.GetPointD();

		return true;
	}
	//sino la partícula muere
	return false;
}





