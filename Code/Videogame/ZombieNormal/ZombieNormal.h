//----------------------------------------------------------------------------------
// Class ZombieNormal
// Author: Miquel Rojas
//
// Description:
// Derivada de CZombie, solo camina y ataca
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CZOMBIENORMAL_H_
#define CZOMBIENORMAL_H_

// Includes
#include "../Zombie/Zombie.h"

class CZombieNormal:public CZombie
{
public:
  CZombieNormal():CZombie(){}
  virtual ~CZombieNormal() { Done(); }
	// Init and End protocols
///<summary>
/// CZombieNormal:: Init : Lectura de los XML. Creación y asignación de zombis.
///</summary>
///<param name="m">TreeNode</param>
///<returns name="">bool</returns>
  virtual	bool				Init								(const CXMLTreeNode &m);

///<summary>
/// CZombieNormal:: Init : Recogida de valores de la clase base. Creación y asignación de zombis.
///</summary>
///<param name="<">CZombie</param>
///<returns name="">bool</returns>
  virtual bool                Init                (CZombie& z);
  virtual void				Done               	();

  // -----------------------
  // Logic functions
  // -----------------------

///<summary>
/// CZombieNormal:: GetAgressivity : Obtiene valor de agresividad de zombis normales.
///</summary>
///<param name="<">void</param>
///<returns name="">unsigned int</returns>
  unsigned int GetAgressivity(void) {return m_uiAgressivity;}

///<summary>
/// CZombieNormal:: SetAgressivity : Da valor de agresividad de zombis normales.
///</summary>
///<param name="<">void</param>
///<returns name="">unsigned int</returns>
  void SetAgressivity(unsigned int _a){ m_uiAgressivity = _a;}
  // -----------------------
  // Action functions
  // -----------------------
  virtual void Update(float elapsedTime);
  virtual void Render(CRenderManager* rm);

///<summary>
/// CZombieNormal:: RecalculateIA: maquina de estados de zombis normales.
///</summary>
///<param name="<">void</param>
///<returns name="">char</returns>
  virtual char RecalculateIA (Vect3f);
//  int          DaBofetadas();

private:
  void        				Release   					();

  // member variables
  unsigned int        m_uiAgressivity;

  typedef CZombie Inherited;
};

#endif