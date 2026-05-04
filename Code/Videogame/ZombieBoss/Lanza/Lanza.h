//----------------------------------------------------------------------------------
// Class
// Author: Miquel Rojas
//
// Description:
// objetos que salen del suelo
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef LANZA_H_
#define LANZA_H_

class CRenderManager;
class CRenderableAnimatedInstanceModel;
// Includes
#include "Math/Vector3.h"
#include <string>

//Class
class CLanza
{
public:
  // Init and End protocols
  CLanza() : m_bIsOk(false), m_fElapsedTimeAnimacion(0.0f), m_fTiempoAnimacion(0.0f), m_pRenderableAnimatedInstanceModel(0)
          ,m_bIniciadaAnimacion(false),V3PosicionPuntaLanza(),m_bActiva(false) {}
          
  virtual ~CLanza()                                                               { Done(); }

	virtual	bool				                Init								(int i);
  virtual void				                Done               	();
  bool								                IsOk               	() const                { return m_bIsOk; }

  virtual void                        Update              (float _elapsedTime);
  virtual void                        Render              (CRenderManager* rm);

  // -----------------------
  // Read functions
  // -----------------------
    CRenderableAnimatedInstanceModel* GetInstance         (void)                  {return m_pRenderableAnimatedInstanceModel;}
    float                             GetTiempoAnimacion  (void);  
    void                             GetSalidaLanza      (Vect3f &_pos, const std::string &_collidemesh);
  // -----------------------
  // Write functions
  // -----------------------

  // -----------------------
  // Logic functions
  // -----------------------
  void                                ResetTiempoAnimacion  ()                    {m_fElapsedTimeAnimacion = 0.0f;}
  void                                AcumulaTiempoAnimacion(float _elapsedtime)  {m_fElapsedTimeAnimacion += _elapsedtime;}
  bool                                FinAnimacionLanza     ()                    {return m_fElapsedTimeAnimacion >= m_fTiempoAnimacion + 0.3f;}
  void                                PosicionaPuntaLanza   ();
  

private:
  void        				                Release   					  ();

  // member variables
  bool                                        m_bIsOk;          // Initialization boolean control
  CRenderableAnimatedInstanceModel*           m_pRenderableAnimatedInstanceModel;
  float                                       m_fElapsedTimeAnimacion;
  float                                       m_fTiempoAnimacion;
  bool                                        m_bIniciadaAnimacion;
  Vect3f                                      V3PosicionPuntaLanza;
  bool                                        m_bActiva;
};


#endif