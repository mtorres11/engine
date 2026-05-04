//----------------------------------------------------------------------------------
// Class CSpirit
// Author: Miquel Rojas
//
// Description: Espiritu japones, sigue al personaje y si se asusta si este se acerca
// 
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef SPIRIT_H_
#define SPIRIT_H_

// Includes
#include <string>
#include "Base.h"
#include "Math/Vector3.h"

class CParticleEmitter;
class CEjeMovimiento;
class CRenderManager;
class CRenderableObject;
class CLight;
//Class
class CSpirit
{
public:
  enum
  {
    IDLE = 0,
    PERSIGUIENDO,
    EVADIENDO,
    CONTENTO,
    MOVSEXY
  };
  // Init and End protocols
  CSpirit() : m_bIsOk(false),m_cState(IDLE),m_fSpeed(0.0f),m_fSpeedMax(0.f),m_fSpeedMin(0.f),m_fSpeed_YUp(0.f),m_fSpeed_YDown(0.f)
    ,m_fPosYMax(0.f),m_fPosYMin(0.f),m_V3Pos(0),m_eje(0),m_emitter(0),m_pFileName(""), m_bLevita(false),m_fAperturaEmisor(0.f)
    ,m_fDistNear(0.f), m_fDistFar(0.f), m_bInitState(false), m_dTimeIdle(0.0), m_dTimeChase(0.0),m_fDelayTimeIdle(0.f),m_fDelayTimeChase(0.f)
    ,m_fAngleTurn(0.0f),m_uiNumeroVueltasContento(0),m_uiNumeroVueltasContentoActual(0), m_fSpeedRotationContento(0.f), m_pLight(0), m_V3PosIni(0) {}

  virtual ~CSpirit() { Done(); }

	virtual	bool				Init								();
  virtual void				Done               	();
  virtual void        Update              (float);
  void                Render              (CRenderManager*);
  bool								IsOk               	() const { return m_bIsOk; }


  void                UpdateLevitation    (float);
  void                UpdateIAMovement    (float);
  // -----------------------
  // Read functions
  // -----------------------
  bool LoadXML(std::string);
  void Reload();
  CEjeMovimiento*     GetEjeMovimiento(void){return m_eje;}
  void                RecalculateIA   ();

  // -----------------------
  // Write functions
  // -----------------------
	void SetParticleEmitter(std::string name, std::string name2);
  bool GoContento(float elapsedTime);
  bool GoMovimientoSexy(float elapsedTime);


private:
  void        				Release   					    ();
  void                ObtieneVectoresDireccion(CRenderableObject*,float&,float&);
  // member variables
  bool                m_bIsOk;          // Initialization boolean control

  char                m_cState;
  float               m_fSpeed;
  float               m_fSpeedMax;
  float               m_fSpeedMin;
  float               m_fSpeed_YUp;
  float               m_fSpeed_YDown;
  float               m_fPosYMax;
  float               m_fPosYMin;
  Vect3f              m_V3Pos;
  Vect3f              m_V3PosIni;
  CEjeMovimiento*     m_eje;
  CParticleEmitter*   m_emitter;
	CParticleEmitter*   m_emitter2;
  std::string         m_pFileName;
  bool                m_bLevita;
  float               m_fAperturaEmisor;
  float               m_fDistNear;
  float               m_fDistFar;
  bool                m_bInitState;
  float               m_fDelayTimeIdle;
  float               m_fDelayTimeChase;
  double              m_dTimeIdle;
  double              m_dTimeChase;
  float               m_fAngleTurn;
  unsigned int        m_uiNumeroVueltasContento;
  unsigned int        m_uiNumeroVueltasContentoActual;
  float               m_fSpeedRotationContento;
  CLight *            m_pLight;
//pruebas
  Vect3f m_vecSpirit;
  Vect3f m_vecPlayer;

};


#endif