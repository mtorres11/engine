//----------------------------------------------------------------------------------
// CZombieUserData class
// Author: Miquel Rojas
//
// Description:
// Clase derivada de CPhysicUserData para tratar datos de usuario para los zombies
//----------------------------------------------------------------------------------

#pragma once
#ifndef CZOMBIERESOURCES_H
#define CZOMBIERESOURCES_H

#include <string>
#include "PhysicsManager.h"

class CScriptManager;

class CZombieUserData
{
public:
  CZombieUserData():  m_sUserData(""),m_bIsCollided(false),m_uiActionFinalized(0) {}
  
  virtual bool Init(std::string Name);
  virtual void Done(); 

	virtual   ~CZombieUserData(){/*Nothing*/}

  void          SetName(std::string n)                          {m_sUserData = n;}
  std::string   GetName()                                       {return m_sUserData;}
  void          SetIsCollided(bool collision)                   {m_bIsCollided = collision;}
  bool          GetIsCollided()                                 {return m_bIsCollided;}
  void          SetIsActionFinalized(unsigned int act)          {m_uiActionFinalized = act;}
  unsigned int  GetIsActionFinalized()                          {return m_uiActionFinalized;}
  void          SetCurrentAction(unsigned int act)              {m_uiCurrentAction = act;}
  unsigned int  GetCurrentAction()                              {return m_uiCurrentAction;}


  bool          IsOk         ()                   {return m_bIsOk;}
private:
  void Release();
  std::string       m_sUserData;
  bool              m_bIsCollided;
  unsigned int      m_uiActionFinalized;
  unsigned int      m_uiCurrentAction;
  bool              m_bIsOk;
};

//----------------------------------------------------------------------------------
// CZombieResources class
// Author: Miquel Rojas
//
// Description:
// Clase recursos 
//----------------------------------------------------------------------------------
class CZombieResources{
public:
	CZombieResources(std::string id, std::string mesh){
		this->m_szId = id;
		this->m_szMesh = mesh;
	}

//	virtual ~CEnemyResources();

  std::string m_szId;
  std::string m_szMesh;	  
};


#endif