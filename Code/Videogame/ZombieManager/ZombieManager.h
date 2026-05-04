//----------------------------------------------------------------------------------
// CZombieManager Class
// Author: Miquel Rojas
//
// Description:
// Dirige los zombies y los pone en vectores
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CZOMBIEMANAGER_H
#define CZOMBIEMANAGER_H

//forward declarations

class CZombieRapido;
class CZombieNormal;
class CZombieVomitador;
class CZombieOlfateador;
class CZombieBoss;
class CXMLTreeNode;
class CCoreFIH;
class CZombieAnimationReport;
class CZombieVomAnimationReport;


// Includes
#include "../Zombie/Zombie.h"
#include <string.h>
#include <vector>
#include "Base.h"
#include <MapManager/MapManager.h>

struct SCoreZombies
{
//name="ZOMBIE_NORMAL" core_animated_model="Zombie" 
//move_speed="2.0" rotate_speed="0.009" radius_smelling="14.0" radius_viewing="3.0" scaled="1.0" health="100"/>
  std::string                       m_type;
  std::string                       m_coremesh;
	float                             m_fMoveSpeed;
	float                             m_fRotSpeed;
  float                             m_fRadiusSmelling;
  float                             m_fRadiusViewing;
  float                             m_fScaled;
	float                             m_fHealth;
};


//Class
class CZombieManager:public CMapManager<CZombie>
{
public:
  // Init and End protocols
  CZombieManager() : m_bIsOk(false), m_zombieAttackCallback(0), m_zombieDieCallback (0), m_zombieReceiveHitCallback(0)
  ,m_zombieVomAppearCallback(0), m_zombieVomShootCallback (0),m_zombieVomReceiveHitCallback(0), m_zombieVomGoOutCallback (0)
  ,m_zombieVomDieCallback (0),m_sFilename(""), m_craterCallback(0), m_zombieBoss(0) { RegistroFuncionesLUA(); }
  virtual ~CZombieManager() { Done(); }

  virtual bool				                  Init								           ();
  virtual void				                  Done               	           ();
  //comment if is child
  bool								                  IsOk               	           () const { return m_bIsOk; }

  void                                  Update                         (float elapsedTime);
  void                                  Render                         (CRenderManager *rm);
  void                                  CreateZombie                   (CXMLTreeNode&);
  void                                  CreateZombie                   (std::string _type, std::string _name, Vect3f _initPos, Vect3f _initRot);
  void                                  DestroyZombie                  (const std::string &_name);
  void                                  CreateCoreZombie               (CXMLTreeNode&);
  void                                  AsignaCoreCorrespondiente      (CZombie&,SCoreZombies );
  void                                  RegistroAnimacionesCallback    (void);
  void                                  DeregistroAnimacionesCallback  (void);
  void                                  ActivaDesactivaZombies         (bool);
  void                                  SetActiveZombies               (bool status);
  void                                  SetStatus                      (unsigned int status);
  bool                                  Load                           (const char* file);
  void                                  Reload                         ();

  std::vector<CZombieNormal*>           GetZombiesNormales             ()  {return m_vZombiesNormales;}
  std::vector<CZombieRapido*>           GetZombiesRapidos              ()  {return m_vZombiesRapidos;}
  std::vector<CZombieVomitador*>        GetZombiesVomitadores          ()  {return m_vZombiesVomitadores;}
  std::vector<CZombieOlfateador*>       GetZombiesOlfateadores         ()  {return m_vZombiesOlfateadores;}
  CZombieBoss*                          GetZombieBoss                  ()  {return m_zombieBoss;}
  void                                  RegistroFuncionesLUA           ();

  std::map<std::string,SCoreZombies*>   GetZombiCores                  ()  {return m_vCoreZombies;}

private:
  void        				Release   					();

  // member variables
  std::vector<CZombieNormal*>     m_vZombiesNormales;
  std::vector<CZombieRapido*>     m_vZombiesRapidos;
  std::vector<CZombieVomitador*>  m_vZombiesVomitadores;
  std::vector<CZombieOlfateador*> m_vZombiesOlfateadores;
  std::map<std::string,SCoreZombies*>          m_vCoreZombies;

  bool                m_bIsOk;          // Initialization boolean control
  CZombieAnimationReport *m_zombieAttackCallback ;
  CZombieAnimationReport *m_zombieDieCallback ;
  CZombieAnimationReport *m_zombieReceiveHitCallback ;

  CZombieAnimationReport *m_zombieFastAttackCallback ;
  CZombieAnimationReport *m_zombieFastDieCallback ;
  CZombieAnimationReport *m_zombieFastReceiveHitCallback ;

  CZombieVomAnimationReport *m_zombieVomAppearCallback ;
  CZombieVomAnimationReport *m_zombieVomShootCallback ;
  CZombieVomAnimationReport *m_zombieVomReceiveHitCallback ;
  CZombieVomAnimationReport *m_zombieVomDieCallback ;
  CZombieVomAnimationReport *m_zombieVomGoOutCallback ;

  CZombieVomAnimationReport *m_craterCallback ;
  
  CZombieBoss* m_zombieBoss;
  
  std::string m_sFilename;
};

#endif