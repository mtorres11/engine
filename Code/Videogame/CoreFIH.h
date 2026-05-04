//----------------------------------------------------------------------------------
// Class CoreFIH
// Author: Miquel Rojas
//
// Description:
// Core del juego para acceder a los diferentes managers
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CoreFIH_H_
#define CoreFIH_H_

// Includes
#include "Singleton.h"
#include <string>

class CPlayer;
class CHealthIndicator;
class CZombieManager;
class CRenderManager;
class CSpirit;

//Class
class CCoreFIH: public CSingleton<CCoreFIH>
{
public:
  // Init and End protocols
  CCoreFIH () : m_bIsOk(false), m_pZombieManager(0), m_pPlayer(0), m_pHealthIndicator(0), m_pSpirit(0), m_bBossRegistered(false) {}
  virtual ~CCoreFIH () { Done(); }
  virtual	bool				Init								();
  virtual void				Done               	();
  bool								IsOk               	() const { return m_bIsOk; }

  // -----------------------
  // Read functions
  // -----------------------
  CPlayer*            GetPlayer           ()        {return m_pPlayer;}
  CZombieManager*     GetZombieManager    ()        {return m_pZombieManager;}
  CSpirit*            GetSpirit           ()        {return m_pSpirit;}
  CHealthIndicator*   GetHealthIndicator  ()        {return m_pHealthIndicator; }
  void                Register2LUA        ();
  bool                InitPlayer          (std::string _file);
  bool                InitSpirit          ();
  void                ReleaseSpirit       ();
  bool                GetBossRegistered   ()        { return m_bBossRegistered; }
  
  // -----------------------
  // Write functions
  // -----------------------
  void                SetBossRegistered   (bool _bFlag) { m_bBossRegistered = _bFlag; }

private:
  void        				Release   					();

  // member variables
  bool                m_bIsOk;          // Initialization boolean control

  CZombieManager*     m_pZombieManager;
  CPlayer*            m_pPlayer;
  CHealthIndicator*   m_pHealthIndicator;
  CSpirit*            m_pSpirit;
  bool                m_bBossRegistered;
	// Types 
	//uncomment to derived classes
  //typedef CProcess Inherited;
};


#endif