//----------------------------------------------------------------------------------
// Class CVomito
// Author: Miquel Rojas
//
// Description: el vomito del zombi vomitador
// 
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef VOMITO_H_
#define VOMITO_H_

// Includes
#include "Math/vector3.h"
#include "Active/Active.h"
#include <vector>

//Class
class CRenderManager;
class CPhysicUserData;
class CParticleEmitter;

class CVomito:public CActive
{
public:
  // Init and End protocols
  CVomito() : CActive(), m_bIsOk(false),m_V3PosicionActual(), m_V3Desplazamiento(), m_V3PosicionDestino(), m_V3Trayectoria(){}
  virtual ~CVomito() { Done(); }

	virtual	bool				Init								();
  virtual void				Done               	();
  virtual void        Update              (float, float, float);
  virtual void        Render              (CRenderManager*);

  bool								IsOk               	() const { return m_bIsOk; }
  void                CreaVomito          (Vect3f myPosicion, float height, Vect3f hisPosicion);

//  CPhysicUserData*  AlgunObjetoEsMalla(const std::vector<CPhysicUserData*>&);
  void                ChocaContraObjeto(const std::vector<CPhysicUserData*>&);

  bool                GetChocado            (){return m_bChoque;}
  void                SetChocado            (bool c){m_bChoque = c;}

  // -----------------------
  // Read functions
  // -----------------------
  Vect3f GetPosicionActual(){return m_V3PosicionActual;}
  Vect3f GetDesplazamiento(){return m_V3Desplazamiento;}
  Vect3f GetPosicionDestino(){return m_V3PosicionDestino;}
  Vect3f GetTrayectoria(){return m_V3Trayectoria;}
  //bool GetLanzamientoVomito(){return }

  // -----------------------
  // Write functions
  // -----------------------
  void SetPosicionActual(Vect3f pos){m_V3PosicionActual = pos;}
  void SetDesplazamiento(Vect3f d){m_V3Desplazamiento = d;}
  void SetPosicionDestino(Vect3f pos){m_V3PosicionDestino = pos;}
  void SetTrayectoria(Vect3f d){m_V3Trayectoria = d;}
  //void SetLanzamientoVomito
  CParticleEmitter* GetParticleEmitter(){return m_pEmisor;}
	CParticleEmitter* GetParticleEmitter2(){return m_pEmisor2;}
	CParticleEmitter* GetParticleEmitter3(){return m_pEmisor3;}

private:
  void        				Release   					();

  // member variables
  bool                m_bIsOk;          // Initialization boolean control
  Vect3f              m_V3PosicionActual;
  Vect3f              m_V3Desplazamiento; // movimiento por update
  Vect3f              m_V3PosicionDestino;
  Vect3f              m_V3Trayectoria;  //trayectoria de la sombra del vomito
  bool                m_bChoque;
  CParticleEmitter*   m_pEmisor;
	CParticleEmitter*   m_pEmisor2;
	CParticleEmitter*   m_pEmisor3;

  typedef CActive Inherited;
};


#endif