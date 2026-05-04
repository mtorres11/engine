//----------------------------------------------------------------------------------
// Class TriggerManager
// Author: Miquel Rojas
//
// Description: Manager de triggers
//----------------------------------------------------------------------------------
#pragma once

#ifndef TRIGGERMANAGER_H_
#define TRIGGERMANAGER_H_

// Includes
#include "Base.h"
#include "Trigger.h"
#include "TriggerReport/PhysicTriggerReport.h"
#include <string>
#include <vector>


class CTrigger;
class CPhysicActor;

//Class
class CTriggerManager:public CPhysicTriggerReport, public CMapManager<CTrigger>
{
public:
  CTriggerManager() : CPhysicTriggerReport (), m_bActivo(false),m_bIsOk(false),m_FileName(""){}
  virtual ~CTriggerManager() { Done(); }

	virtual	bool						Init											();
  virtual void						Done               				();
  bool										IsOk               				() const													{ return m_bIsOk; }
  bool										Load											(const std::string &FileName);
//  bool                    Reload                    ();
	void										Render										();
  // -----------------------
  // Read functions
  // -----------------------
  std::vector<CTrigger*>  GetTriggersVector					()																{return m_vTriggers;}
  std::string             GetFileName								()																{return m_FileName;}
	bool										GetTriggerActivo					()																{return m_bActivo;}
	CTrigger*								GetTrigger								(std::string name);
  // -----------------------
  // Write functions
  // -----------------------

  // -----------------------
  // Logic functions
  // -----------------------
  CPhysicActor*           GeneraTrigger							(const std::string& _name, CTrigger* _trigger, bool _esPunyo);
  void                    RegistraTriggers					();
  virtual void						OnEnter										(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape);
	virtual	void						OnLeave										(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape);

private:
  void        				    Release   								();

  // member variables
  bool																m_bIsOk;          // Initialization boolean control
  std::string													m_FileName;
  std::vector <CTrigger*>							m_vTriggers;
  std::vector <CPhysicUserData*>			m_vUserDatas;
  std::vector <CPhysicActor*>					m_vTriggerActors;
	bool																m_bActivo;
};


#endif

