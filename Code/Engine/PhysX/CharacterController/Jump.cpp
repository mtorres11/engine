#include "CharacterController/Jump.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


void CJump::StartJump(float v0, float h0 = 0.0f)
{
	if(m_bJump)
	{
		return;
	}

	m_fJumpTime	= 0.0f;
	m_fV0				= v0;
	m_fH0				= h0;
	m_bJump			= true;
}

void CJump::StopJump()
{
	if (m_bJump)
	{
		m_bJump = false;
	}
}

float CJump::GetHeight(float elapsedTime)
{
	/*
	a(t) = cte = g
	v(t) = g*t + v0
	y(t) = g*t^2 + v0*t + y0
  y(t) = -0.5*a*t^2 + v0*t + y0
	*/

	if(!m_bJump)
	{
		return 0.0f;
	}

	m_fJumpTime += elapsedTime;
	
  return m_fG * m_fJumpTime * m_fJumpTime + m_fV0 * m_fJumpTime + m_fH0;

  /*float h = m_fG*m_fJumpTime*m_fJumpTime + m_fV0*m_fJumpTime;
	return (h - m_fDefaultGravity)*elapsedTime;
  */
  // (Andres) Cambie la función que calcula la altura del salto en cada tic
  // al menos para que vaya bien para el hito
  //return m_fDefaultGravity * elapsedTime * elapsedTime + m_fV0 * elapsedTime;
}

/// <summary>
/// Retorna un booleano que indica si está saltando actualmente
/// </summary>
bool CJump::IsOnJump()
{
  return m_bJump;
}