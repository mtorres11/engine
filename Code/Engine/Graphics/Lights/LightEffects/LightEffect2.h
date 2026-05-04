//----------------------------------------------------------------------------------
// CLightEffect2 class
// Author: Andres
//
// Description:
// Efecto 2: Destellos, usados para los relampagos
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_LIGHT_EFFECT2_H_
#define INC_LIGHT_EFFECT2_H_

#include "LightEffect.h"
#include "Utils/LerpAnimator1D.h"
#include <vector>

class CLightEffect2 : public CLightEffect
{
public:
  CLightEffect2(CLight * _pParentLight);
  ~CLightEffect2();   

  void Init();
  void Update(float elapsedTime);

  void    SetFPS              (float _fValue) { m_fFPS = _fValue; }
  uint16  GetCurrentKeyframe  () { return m_wCurrentKeyframe; }
  void    AddKeyFrame         (float _fFinalMultiplier, uint16 _wLength);

private:
  struct SKeyframe
  {
    float   m_fFinalMultiplier;
    uint16  m_wLength;
  };

  float                     m_fFPS;
  bool                      m_bIncrement;
  bool                      m_bLightIsOn;
  float                     m_fCurrentMultiplier;
  uint16                    m_wCurrentKeyframe;
  std::vector<SKeyframe>    m_vKeyframes;

	CLerpAnimator1D			m_LerpAnimator1D;
};

#endif //INC_LIGHT_EFFECT2_H_
