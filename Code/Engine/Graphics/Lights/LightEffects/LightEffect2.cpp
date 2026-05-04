#include "LightEffect2.h"
#include "Lights/Light.h"

CLightEffect2::CLightEffect2(CLight * _pParentLight)
{
  m_iType = 2;
  m_pParentLight = _pParentLight;
}

CLightEffect2::~CLightEffect2()
{
}

void CLightEffect2::Init()
{
  m_pParentLight->SetEnable(true);
  m_wCurrentKeyframe = 0;
  m_LerpAnimator1D.SetValues(m_pParentLight->GetMultiplier(), m_vKeyframes[0].m_fFinalMultiplier, m_vKeyframes[0].m_wLength / m_fFPS, FUNC_INCREMENT);
  m_bIncrement = true;
  m_bLightIsOn = true;
}

void CLightEffect2::Update(float elapsedTime)
{
  if (m_LerpAnimator1D.Update(elapsedTime, m_fCurrentMultiplier))
	{
    ++m_wCurrentKeyframe;

    if (m_wCurrentKeyframe == m_vKeyframes.size())
      m_wCurrentKeyframe = 0;

		if (m_bIncrement)
    {
      m_LerpAnimator1D.SetValues(m_fCurrentMultiplier, m_vKeyframes[m_wCurrentKeyframe].m_fFinalMultiplier, m_vKeyframes[m_wCurrentKeyframe].m_wLength / m_fFPS, FUNC_DECREMENT);
      m_bIncrement = false;
    }
    else
    {
      m_LerpAnimator1D.SetValues(m_fCurrentMultiplier, m_vKeyframes[m_wCurrentKeyframe].m_fFinalMultiplier, m_vKeyframes[m_wCurrentKeyframe].m_wLength / m_fFPS, FUNC_INCREMENT);
      m_bIncrement = true;
    }
	}

  if (m_fCurrentMultiplier == 0.f)
  {
    if (m_bLightIsOn)
    {
      m_pParentLight->SetEnable(false);
      m_bLightIsOn = false;
    }
  }
  else
  {
    if (!m_bLightIsOn)
    {
      m_pParentLight->SetEnable(true);
      m_bLightIsOn = true;
    }
  }
  
  m_pParentLight->SetMultiplier(m_fCurrentMultiplier);
}

void CLightEffect2::AddKeyFrame(float _fFinalMultiplier, uint16 _wLength)
{
  SKeyframe l_KeyFrame;
  l_KeyFrame.m_fFinalMultiplier = _fFinalMultiplier;
  l_KeyFrame.m_wLength = _wLength;
  m_vKeyframes.push_back(l_KeyFrame);
}
