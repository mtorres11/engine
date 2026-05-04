#include "LightEffect1.h"

#include "Lights/Light.h"

CLightEffect1::CLightEffect1(CLight * _pParentLight)
{
  m_iType = 1;
  
  m_pParentLight = _pParentLight;
}

CLightEffect1::~CLightEffect1()
{
}

void CLightEffect1::Init()
{
  m_LerpAnimator1D.SetValues(m_fMaxMultiplier, m_fMinMultiplier, m_fLapDuration, FUNC_DECREMENT);
  m_bIncrement = false;
}

void CLightEffect1::Update(float elapsedTime)
{
  if(m_LerpAnimator1D.Update(elapsedTime, m_fCurrentMultiplier))
	{
		if (m_bIncrement)
    {
      m_LerpAnimator1D.SetValues(m_fMaxMultiplier, m_fMinMultiplier, m_fLapDuration, FUNC_DECREMENT);
      m_bIncrement = false;
    }
    else
    {
      m_LerpAnimator1D.SetValues(m_fMinMultiplier, m_fMaxMultiplier, m_fLapDuration, FUNC_INCREMENT);
      m_bIncrement = true;
    }
	}
  
  m_pParentLight->SetMultiplier(m_fCurrentMultiplier);
}