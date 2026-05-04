//----------------------------------------------------------------------------------
// Class CZombieOlfateador
// Author: Miquel Rojas
//
// Description:
// Derivada de CZombie, Persigue al jugador mediante algoritmo A*
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CZOMBIEOLFATEADOR_H_
#define CZOMBIEOLFATEADOR_H_

class C_IA;
// Includes
#include "Base.h"
#include "../Zombie/Zombie.h"

class CZombieOlfateador:public CZombie
{
public:
  CZombieOlfateador():CZombie(){}
  virtual ~CZombieOlfateador() { Done(); }

  // Init and End protocols

///<summary>
/// CZombieOlfateador:: Init : Lectura de los XML. Creación y asignación de zombis.
///</summary>
///<param name="m">TreeNode</param>
///<returns name="">bool</returns>
  virtual	bool				              Init								(const CXMLTreeNode &m);

///<summary>
/// CZombieOlfateador:: Init : Recogida de valores de la clase base. Creación y asignación de zombis.
///</summary>
///<param name="z">CZombie</param>
///<returns name="">bool</returns>
  virtual bool                      Init                (CZombie& z);
  virtual void				              Done               	();

  // -----------------------
  // Action functions
  // -----------------------
///<summary>
/// CZombieOlfateador:: RecalculateIA: maquina de estados de zombis olfateadores.
///</summary>
///<param name="">void</param>
///<returns name="">char</returns>
  virtual char                      RecalculateIA       (Vect3f);
  void                              CalculaSiguienteNodo();
  void                              CalculaSiguientePosicionZombie(float);

  virtual void                      Update              (float elapsedTime);
  virtual void                      Render              (CRenderManager *rm);

  // -----------------------
  // Logic functions
  // -----------------------
///<summary>
/// CZombieOlfateador:: GetDummyStart: Obtiene la distancia de disparo de zombis olfateadores.
///</summary>
///<param name="">void</param>
///<returns name="">string</returns>
  std::string                      GetDummyStart      (void)            {return m_sDummyStart;}

///<summary>
/// CZombieOlfateador:: GetAgentIA: Obtiene un agente ia
///</summary>
///<param name="">void</param>
///<returns name="">C_IA</returns>
  C_IA*                             GetAgentIA         (void)            {return m_AgentIA;}

///<summary>
/// CZombieOlfateador:: GetSiguiendoCamino: Obtiene si un zombi esta siguiendo un camino
///</summary>
///<param name="">void</param>
///<returns name="">bool</returns>
  bool                              GetSiguiendoRuta (void)            {return m_bSiguiendoRuta;}

///<summary>
/// CZombieOlfateador:: SetDummyStart: Da la distancia de disparo de zombis olfateadores.
///</summary>
///<param name="_d">string</param>
///<returns name="">void</returns>
  void                              SetDummyStart      (std::string _d) {m_sDummyStart = _d;}

///<summary>
/// CZombieOlfateador:: SetAgentIA: Da IA a zombis olfateadores.
///</summary>
///<param name="">C_IA</param>
///<returns name="">void</returns>
  void                              SetAgent           (C_IA *_a)        {m_AgentIA = _a;}

///<summary>
/// CZombieOlfateador:: SetSiguiendoRuta: Da al zombi la orden de seguir su ruta
///</summary>
///<param name="_s">bool</param>
///<returns name="">void</returns>
  void                              SetSiguiendoRuta   (bool _s)        {m_bSiguiendoRuta = _s;}

  void                              UpdateGoto         (void);

  std::string m_sNodoActual;
  std::string m_sNodoSiguiente;
  Vect3f m_v3PosicionNodoAVisitar;
  size_t m_indiceCamino;
private:
  void        				Release   					();

  //member variables
  std::string m_sDummyStart;
  C_IA *m_AgentIA;
  bool m_bSiguiendoRuta;

  typedef CZombie Inherited;
};
#endif