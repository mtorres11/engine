//----------------------------------------------------------------------------------
// Class CEjeMovimiento
// Author: Miquel Rojas
//
// Description: Contiene los vectores y algoritmos para decidir movimientos de IA
//----------------------------------------------------------------------------------
#pragma once

#define RADIANS 0.018f

#ifndef CEJEMOVIMIENTO_H_
#define CEJEMOVIMIENTO_H_

// Includes
#include "Math/Vector3.h"
#include "RenderManager.h"
//Class
class CEjeMovimiento
{
public:
  // Init and End protocols
  CEjeMovimiento() : m_bDatosInicializacion(false), m_fAngleParcialPlayer2(0.0f), m_fAngleTotalPlayer2(0.0f),m_bIsOk(false){}
  virtual ~CEjeMovimiento() { Done(); }

	virtual	bool				Init								();
  virtual void				Done               	();
  bool								IsOk               	() const { return m_bIsOk; }

  // -----------------------
  // Read functions
  // -----------------------
void MovAgentIA(Vect3f hisPosition, Vect3f &myPosition,float myspeed,float _elapsedtime, bool _chase=true);
bool CompruebaPlayerDentroAreaVision(const float &visionDistance, const float &visionAngle=45.0f);
//void MovAgentIA(Vect3f hisPosition, Vect3f &myPosition,float &myspeed);
void RenderMovIA(CRenderManager *rm, Vect3f pos_player, Vect3f mypos, float _visionAngle=45.0f, float alturaEje = 0.5f, float largoEje = 4.0f);
Vect3f GetVectorDirector(){return m_V3DirectorPlayer2;}
Vect3f GetVectorDirectorOptimo(){return m_V3DirPlayer2Optimo;}
void ResetDatosInicializacion(){m_bDatosInicializacion = false;}
  // -----------------------
  // Write functions
  // -----------------------
  float m_fAngleTotalPlayer2;

private:
  void        				Release   					();
  float AjusteDireccion();
  void BuscaEncarar();
  bool m_bDatosInicializacion;
  // member variables
  float m_fAngleParcialPlayer2;
  Vect3f m_V3DirPlayer2Optimo;
  Vect3f m_V3DirDistanciaOptimo; //guarda el vector distancia sin normalizar.
  Vect3f m_V3DirectorPlayer2;
  Vect3f m_V3RightPlayer2;
  Vect3f m_V3UpPlayer2;
  
  bool m_bChaser; //false=cazador player 1, true=cazador player 2
  

  bool                m_bIsOk;          // Initialization boolean control
};


#endif