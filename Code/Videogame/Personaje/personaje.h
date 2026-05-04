//----------------------------------------------------------------------------------
// Class CPersonaje
// Author: Miquel Rojas
//
// Description:
// Class Base with properties about characters like his position, rotation, health,etc...
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CPERSONAJE_H_
#define CPERSONAJE_H_

// Includes
#include "Base.h"
#include <vector>
#include "Math\Vector3.h"

class CRenderableAnimatedInstanceModel;
class CXMLTreeNode;
class CPhysicController;

//Class
class CPersonaje
{
public:
  // Init and End protocols
  CPersonaje();
  virtual ~CPersonaje() { Done(); }

	virtual	bool				Init								(const CXMLTreeNode&);
  virtual void        InicializaController(){}
  virtual void				Done               	();
  bool								IsOk               	() const { return m_bIsOk; }

  // -----------------------
  // Logic functions
  // -----------------------
  CRenderableAnimatedInstanceModel* GetInstance         (void)                  {return m_pRenderableAnimatedInstanceModel;}
  Vect3f                            GetPosInicial       (void)                  {return m_V3PosIni;}
  Vect3f                            GetRotInicial       (void)                  {return m_V3RotIni;}
  float                             GetHealth           (void)                  {return m_fHealth;}
  float                             GetMoveSpeed        (void)                  {return m_fMoveSpeed;}
  float                             GetRotSpeed         (void)                  {return m_fRotSpeed;}
  std::string                       GetModelFilename    (void)                  {return m_sMesh;}
  CPhysicController*                GetPhysicController (void)                  {return m_pPhysicController;}

  void                              SetInstance         (CRenderableAnimatedInstanceModel* model) {m_pRenderableAnimatedInstanceModel = model;}
  void                              SetPosInicial       (Vect3f* _pos)                             {m_V3PosIni(_pos->x,_pos->y,_pos->z);}
  void                              SetRotInicial       (Vect3f* _rot)                             {m_V3RotIni(_rot->x,_rot->y,_rot->z);}
  void                              SetHealth           (float _h)                          {m_fHealth = _h;}
  void                              SetMoveSpeed        (float _ms)                                {m_fMoveSpeed = _ms;}
  void                              SetRotSpeed         (float _rs)                                {m_fRotSpeed = _rs;}
  void                              SetModelFilename    (std::string _f)                           {m_sMesh = _f;}
  // -----------------------
  // Real-Time functions
  // -----------------------
  virtual void                      Update              (float elapsedTime) = 0;
  virtual void                      Render              (void) = 0;
  // -----------------------
  // Action functions
  // -----------------------
  float                             IsDead              (void)                  {return (m_fHealth <= 0.0f);}
  float                             ReceiveDamage       (float _damage);

protected:
	//member variables
	Vect3f                            m_V3PosIni;
  Vect3f                            m_V3RotIni;
	float                             m_fMoveSpeed;
	float                             m_fRotSpeed;
	float                             m_fHealth;
	std::string                       m_sMesh;
  CRenderableAnimatedInstanceModel* m_pRenderableAnimatedInstanceModel;
  void        				Release   					();
  CPhysicController*                m_pPhysicController;

  // member variables
  bool                m_bIsOk;          // Initialization boolean control
};

#endif
