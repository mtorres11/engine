//----------------------------------------------------------------------------------
// CLightEffect1 class
// Author: Andres
//
// Description:
// Efecto 1: Intensidad parpadeante (Efecto de loop en la intensidad de la luz)
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_LIGHT_EFFECT1_H_
#define INC_LIGHT_EFFECT1_H_

#include "LightEffect.h"
#include "Utils/LerpAnimator1D.h"

class CLightEffect1 : public CLightEffect
{
public:
  CLightEffect1(CLight * _pParentLight);
  ~CLightEffect1();

  void SetMaxMultiplier  (float value) { m_fMaxMultiplier = value; }
  void SetMinMultiplier  (float value) { m_fMinMultiplier = value; }
  void SetLapDuration    (float _fFramesPerLap, float _fFPS)   { m_fLapDuration = _fFramesPerLap / _fFPS; }

  void Init();
  void Update(float elapsedTime);

private:
  float     m_fMaxMultiplier;
  float     m_fMinMultiplier;
  float     m_fLapDuration;

  bool      m_bIncrement;
  float     m_fCurrentMultiplier;

	CLerpAnimator1D			m_LerpAnimator1D;
};

#endif //INC_LIGHT_EFFECT1_H_
