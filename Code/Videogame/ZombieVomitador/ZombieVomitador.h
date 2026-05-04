//----------------------------------------------------------------------------------
// Class CZombieVomitador
// Author: Miquel Rojas
//
// Description:
// Derivada de CZombie, lanza proyectiles
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CZOMBIEVOMITADOR_H_
#define CZOMBIEVOMITADOR_H_

#define OFFSET_ANIMATIONS 20

class CVomito;
// Includes
#include "../Zombie/Zombie.h"


class CZombieVomitador:public CZombie
{
public:
  enum
  {
    VOM_APARECIENDO = OFFSET_ANIMATIONS,
    VOM_DISPARANDO,
    VOM_IDLETRASVOMITO,
    VOM_RECIBEGOLPE,
    VOM_MURIENDO,
    VOM_SALIDA,
  };
  CZombieVomitador():
      CZombie(), m_pVomito(0), m_uiAimDistance(0), m_fTamanyoVomito(0.2f), m_fSpeedVomito(2.0f),m_dTiempoInicioVomito(0.0),m_fPausaVomito(0.0f),m_dTiempoIdleTrasVomito(0.0), m_dTiempoSalida(0.0)
      , m_uiNumAlternativas(0), m_pArena(0), m_dDiffTiempoSalidaAleat(0.0){}
  virtual ~CZombieVomitador() { Done(); }

  // Init and End protocols

///<summary>
/// CZombieVomitador:: Init : Lectura de los XML. Creación y asignación de zombis.
///</summary>
///<param name="m">TreeNode</param>
///<returns name="">bool</returns>
  virtual	bool				              Init								(const CXMLTreeNode &m);

///<summary>
/// CZombieVomitador:: Init : Recogida de valores de la clase base. Creación y asignación de zombis.
///</summary>
///<param name="z">CZombie</param>
///<returns name="">bool</returns>
  virtual bool                      Init                (const CZombie& z);
  virtual void				              Done               	();

  // -----------------------
  // Action functions
  // -----------------------
///<summary>
/// CZombieVomitador:: RecalculateIA: maquina de estados de zombis vomitadores.
///</summary>
///<param name="">void</param>
///<returns name="">char</returns>
  virtual char                      RecalculateIA       (Vect3f);

  virtual void                      Update              (float elapsedTime);
  virtual void                      Render              (CRenderManager *rm);

  // -----------------------
  // Logic functions
  // -----------------------
///<summary>
/// CZombieVomitador:: GetAimDistance: Obtiene la distancia de disparo de zombis vomitadores.
///</summary>
///<param name="">void</param>
///<returns name="">unsigned ing</returns>
  unsigned int                      GetAimDistance      (void)            {return m_uiAimDistance;}

///<summary>
/// CZombieVomitador:: SetAimDistance: Da la distancia de disparo de zombis vomitadores.
///</summary>
///<param name="_d">unsigned int</param>
///<returns name="">char</returns>
  void                              SetAimDistance      (unsigned int _d) {m_uiAimDistance = _d;}

  float                             GetSpeedVomito()         {return m_fSpeedVomito;}
  void                              SetSpeedVomito(float s)  {m_fSpeedVomito = s;}

  float                             GetTamanyoVomito()         {return m_fTamanyoVomito;}
  void                              SetTamanyoVomito(float t)  {m_fTamanyoVomito = t;}

  virtual void                      InicializaController();

  void                              GoIdle             (Vect3f,Vect3f);
  void                              GoIdleTrasVomito   (Vect3f hisPosition,Vect3f myPosition);
  void                              GoSalida           (Vect3f hisPosition,Vect3f myPosition);
//  void                              GoMirando          (Vect3f hisPosition,Vect3f myPosition);
  void                              GoApareciendo      (Vect3f);
  void                              GoVomitando        (Vect3f,Vect3f);
  void                              GoRecibiendoGolpe  (Vect3f hisPosition,Vect3f myPosition);
  void                              GoMuriendo         ();
	void                              HumoMuerte         (Vect3f pos);
	void															PosicionaCabeza    ();
	Vect3f                            GetPosicionCabeza  ();
  
private:
  void        				Release   					();
  bool                CreaCrater          (std::string);

  CVomito* m_pVomito;
  //member variables
  unsigned int m_uiAimDistance;
  float m_fTamanyoVomito;
  float m_fSpeedVomito;
  float m_fPausaVomito;
  
  double m_dTiempoInicioVomito;
  double m_dTiempoIdleTrasVomito;
  double m_dTiempoSalida;
  double m_dDiffTiempoSalidaAleat;
  
  char m_uiNumAlternativas;
  std::vector<Vect3f> m_v3AlternativasPosicion;
  
  CRenderableAnimatedInstanceModel* m_pArena;
	Vect3f V3PosicionCabeza;
  
  typedef CZombie Inherited;


};
#endif