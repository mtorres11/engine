//----------------------------------------------------------------------------------
// Class CZombieRapido
// Author: Miquel Rojas
//
// Description:
// Deriva de CZombie camina mas rapido
//----------------------------------------------------------------------------------
#pragma once

#ifndef CZOMBIERAPIDO_H_
#define CZOMBIERAPIDO_H_

// Includes
#include "../Zombie/Zombie.h"

class CZombieRapido:public CZombie{
public:
  CZombieRapido():CZombie(), m_fExtraSpeed(2.0f){}

  virtual ~CZombieRapido() { Done(); }
	// Init and End protocols

///<summary>
/// CZombieRapido:: Init : Lectura de los XML. Creación y asignación de zombis.
///</summary>
///<param name="m">TreeNode</param>
///<returns name="">bool</returns>
	virtual	bool				Init												(const CXMLTreeNode &m);


///<summary>
/// CZombieRapido:: Init : Recogida de valores de la clase base. Creación y asignación de zombis.
///</summary>
///<param name="z">CZombie</param>
///<returns name="">bool</returns>
  virtual bool                Init(CZombie& z);

  virtual void				Done               					();
  // -----------------------
  // Logic functions
  // -----------------------
  
///<summary>
/// CZombieRapido:: GetExtraSpeed : Obtiene valor de agresividad de zombis rapidos.
///</summary>
///<param name="">void</param>
///<returns name="">float</returns>
  float GetExtraSpeed(void){return m_fExtraSpeed;}

///<summary>
/// CZombieRapido:: SetExtraSpeed : Da valor de agresividad de zombis rapidos.
///</summary>
///<param name="_s">float</param>
///<returns name="">void</returns>
  void SetExtraSpeed(float _s){m_fExtraSpeed = _s;}

  // -----------------------
  // Action functions
  // -----------------------
  int DaBofetadas();

///<summary>
/// CZombieRapido:: RecalculateIA : maquina de estados de zombis rapidos.
///</summary>
///<param name="">void</param>
///<returns name="">char</returns>
  virtual char                      RecalculateIA       (Vect3f);
  virtual void                      Update              (float elapsedTime);
  virtual void                      Render              (CRenderManager* rm);

private:
  float m_fExtraSpeed;
  void        				Release   					();

  typedef CZombie Inherited;
};


#endif