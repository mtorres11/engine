//----------------------------------------------------------------------------------
// CLight class
// Author: Andres
//
// Description:
// Clase base para crear efectos de luces
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_LIGHT_EFFECT_H_
#define INC_LIGHT_EFFECT_H_

class CLight;

class CLightEffect
{
public:
  CLightEffect() : m_iType(0), m_pParentLight(0) {}
  virtual ~CLightEffect();

  int GetType() { return m_iType; }

protected:
  int       m_iType;
  CLight *  m_pParentLight;
};

#endif //INC_LIGHT_EFFECT_H_
