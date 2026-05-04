//----------------------------------------------------------------------------------
// Class CHealthIndicator
// Author: Andres
//
// Description:
// Componente de GUI para el indicador de salud
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CHEALTH_INDICATOR_H_
#define CHEALTH_INDICATOR_H_

#include "Base.h"

class CAnimatedImage;
class CImage;

class CHealthIndicator
{
public:
  CHealthIndicator() : m_bIsOk(false), m_fAnimSpeed(0.5f), m_usMaxHealthFrame(0), m_pHealthAnimation(0), m_pHealthBG(0) {}
  ~CHealthIndicator() {}
  
  bool Init                                 (float _fMaxHealth);
  //void Done            											();
  void Update                               ();
  //void Render                               ();

  void Hit                                  (float health, float damage);
  void HealthUp                             (float health, float healthUp);
  void MaxHealth                            ();
  bool AssignGUIElements                    ();

private:
  
  bool              m_bIsOk;
  float             m_fAnimSpeed;
  CAnimatedImage *  m_pHealthAnimation;
  CImage *          m_pHealthBG;

  uint16            m_usMaxHealthFrame;
  float             m_fMaxHealth;
};

#endif
