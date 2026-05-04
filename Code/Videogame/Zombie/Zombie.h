//----------------------------------------------------------------------------------
// Class CZombie
// Author: Miquel Rojas
//
// Description:
// Class Base con propiedeades de zombis como position, rotation, health,etc...
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CZOMBIE_H_
#define CZOMBIE_H_

// Includes
#include <vector>
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableObject.h"
#include "ZombieResources/ZombieResources.h"

#include "Math\Vector3.h"
#include "../Personaje/Personaje.h"
//Class
class CXMLTreeNode;
class CRenderableAnimatedInstanceModel;
class CEjeMovimiento;
class CTrigger;
class CPlayer;

class CZombie:public CPersonaje
{
public:
///<summary>
/// CZombie:: enum Estados : estados de zombis.
//</summary>
  enum 
  {
    MUERTO = -1,
    OLIENDO = 0,
    MIRANDO,
    RECIBIENDOGOLPE,
    MURIENDO,  
    ATACANDO,
    IDLE,
    //DESENTERRANDO,
    DISPARANDO,
    VOLVIENDOINICIO,
    ANDANDO,
//    ENCARANDO,
    EVADIENDO,
    INACTIVO
  };
  enum
  {
    INIT = 0,
    UPDATE,
    DONE
  };
public:
  // Init and End protocols
	CZombie();
  virtual ~CZombie() { Done(); }

///<summary>
/// CZombie:: Init : Lectura de los XML. Creación y asignación de zombis.
///</summary>
///<param name="m">XMLTreeNode</param>
///<returns name="">bool</returns>
	virtual	bool				Init								(const CXMLTreeNode &m);

///<summary>
/// CZombie:: Init : Recogida de valores de la clase base. Creación y asignación de zombis.
///</summary>
///<param name="z">CZombie</param>
///<returns name="">bool</returns>
  virtual bool        Init                (const CZombie&);

  virtual void        InicializaController();

  virtual void				Done               	();
  bool								IsOk               	() const { return m_bIsOk; }

///<summary>
/// CZombie:: Operator = : asignacion de valores.
///</summary>
///<param name="z">CZombie</param>
///<returns name="">CZombie</returns>
  CZombie& operator = (CZombie&);

  // -----------------------
  // Logic functions
  // -----------------------

///<summary>
/// CZombie:: GetState : Obtiene el estado del zombi.
///</summary>
///<param name="">void</param>
///<returns name="">char</returns>
  char                              GetState            (void){return this->m_cState;}

///<summary>
/// CZombie:: GetRadiusSmelling : Obtiene el radio de olor del zombi.
///</summary>
///<param name="">void</param>
///<returns name="">float</returns>
  float                             GetRadiusSmelling   (void)              {return m_fRadiusSmelling;}

///<summary>
/// CZombie:: GetRadiusViewing : Obtiene el radio de vista del zombi.
///</summary>
///<param name="">void</param>
///<returns name="">float</returns>
  float                             GetRadiusViewing    (void)              {return m_fRadiusViewing;}


  CEjeMovimiento*                   GetEjeMovimientoIA  (void)              {return m_movIA;}

  void                              SetEjeMovimientoIA  (CEjeMovimiento*em) {m_movIA = em;}


  virtual const Vect3f &GetPositionHero() {return CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetPosition();}


///<summary>
/// CZombie:: SetState : Da estado al zombi.
///</summary>
///<param name="_state">char</param>
///<returns name="">void</returns>
  void                              SetState            (char _state)       {m_cState = _state;}

///<summary>
/// CZombie:: SetRadiusSmelling : Da el radio de olor al zombi.
///</summary>
///<param name="_s">float</param>
///<returns name="">void</returns>
  void                              SetRadiusSmelling   (float _s)          {m_fRadiusSmelling = _s;}

///<summary>
/// CZombie:: SetRadiusViewing : Da el radio de vision al zombi.
///</summary>
///<param name="_v">float</param>
///<returns name="">void</returns>
  void                              SetRadiuwViewing    (float _v)          {m_fRadiusViewing = _v;}

  // -----------------------
  // Action functions
  // -----------------------
///<summary>
/// CZombie::   RecalculateIA : maquina de estados de zombis.
///</summary>
///<param name="_v">void</param>
///<returns name="">char</returns>
  virtual char                      RecalculateIA       (Vect3f _heroPosition);//{return (char)255;}// = 0;
  virtual void                      Update              (float elapsedTime);// = 0;
  virtual void                      Render              (void);// = 0;

  virtual void GoIdle(Vect3f hisPosition,Vect3f myPosition);
  virtual void GoOliendo(Vect3f hisPosition,Vect3f myPosition);
  virtual void GoAtacando(Vect3f hisPosition,Vect3f myPosition);
  virtual void GoAndando(Vect3f hisPosition,Vect3f myPosition);
  virtual void GoVolviendoInicio(Vect3f hisPosition,Vect3f myPosition);
  virtual void GoMirando(Vect3f hisPosition,Vect3f myPosition);
  virtual void GoRecibiendoGolpe(Vect3f hisPosition,Vect3f myPosition);
  virtual void GoMuriendo();
	virtual bool GoMuerto(float elapsedTime);

  virtual void PosicionaTrigger(const Vect3f&, CPhysicActor*);
  virtual void PosicionaManoDerecha();
  virtual void PosicionaManoIzquierda();
	virtual void PosicionaPieDerecho();

  void         Perseguir(Vect3f, float, float height = 2.0f);
  void         Encarando(Vect3f, float, float height = 2.0f);
  void         VolviendoInicio(float, float height = 2.0f);
	void         Evadir(Vect3f, float,float speed, float height = 2.0f);

  void         DestruyeZombie(char fase);
  CZombieUserData*    GetUserData                 ()                                    {return m_pUserData;}
  void                SetUserData                 (CZombieUserData* userdata)           {m_pUserData = userdata;}
  bool                InicializaUserData          (std::string name);
	void                HumoMuerte                  (Vect3f pos);
	Vect3f              GetPosicionPieDerecho       ();
  void                EscondePunyos               ();
	virtual void        RandomItem	                (Vect3f pos);

  void SetFaseEstado(unsigned int f){m_faseEstado = f;}
  unsigned int GetFaseEstado(void){return m_faseEstado;}
  //member variables
  virtual bool         GeneraManoIzquierda();
  virtual bool         GeneraManoDerecha();
//  Vect3f GetPosicionTorso() const {return V3PosicionTorso;}  

  CPhysicActor*        GetPxActorTriggerManoDerecha     () {return m_actManoDerecha;}
  CPhysicActor*        GetPxActorTriggerManoIzquierda   () {return m_actManoIzquierda;}
  CTrigger*            GetTriggerManoDerecha            () {return m_triggerManoIzquierda;}
  CTrigger*            GetTriggerManoIzquierda          () {return m_triggerManoDerecha;}
protected:
  CEjeMovimiento*      m_movIA;
  unsigned int         m_faseEstado;
		CRenderableAnimatedInstanceModel* m_pModel;
  
  CPhysicActor*        m_actManoDerecha;
  CPhysicActor*        m_actManoIzquierda;
  CPhysicActor*m_actPieDerecho;
  CPhysicActor*m_actTorso;
  CTrigger*            m_triggerManoIzquierda;
  CTrigger*            m_triggerManoDerecha;
  Vect3f               V3PosicionManoIzquierda;
  Vect3f               V3PosicionManoDerecha;
  Vect3f       		   V3PosicionPieDerecho;
  Vect3f       		   V3PosicionTorso;

protected:
  char                m_iStepMuerto;
private:
  void        				Release   					();
  
  // member variables
  bool                m_bIsOk;          // Initialization boolean control
  float               m_fRadiusSmelling;
  float               m_fRadiusViewing;
  char                m_cState;
  CZombieUserData*    m_pUserData;
	CPlayer *           m_pPlayer;
	float								m_timeDead;
  bool                m_bVisible;
  typedef CPersonaje  Inherited;
};

#endif
