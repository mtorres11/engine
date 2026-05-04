//----------------------------------------------------------------------------------
// Class CTrigger
// Author: Miquel Rojas
//
// Description: Activador de eventos
//----------------------------------------------------------------------------------
#pragma once

#ifndef CTRIGGER_H_
#define CTRIGGER_H_

// Includes
#include "Base.h"
#include "Math/Vector3.h"
#include <string>
#include "Active/Active.h"
#include "Math\MathTypes.h"
#include "Math\Matrix44.h"
#include "Core.h"
#include "../RenderablePhysX/RenderablePhysXManager.h"

//Class
class CTrigger: public CActive
{
public:
  // Init and End protocols
  CTrigger() :CActive(),m_bIsOk(false), m_RenderObj(0), m_PhysXObj(0){}
  virtual ~CTrigger() { Done(); }

	virtual	bool						Init									();
  virtual void						Done               		();
  bool										IsOk               		() const													{return m_bIsOk;}
	void										Render								(Vect3f size, Vect3f position);
	void										CalculateMatriz44f    ();
	void										MoveRenderableObject	(std::string idObject);
	// -----------------------
  // Read functions
  // -----------------------
  virtual	Vect3f					GetPosition						()																{return m_v3Position;}
  virtual	Vect3f					GetDimensions					()																{return m_v3Dimensions;}
	virtual	Vect3f					GetMax								()																{return m_v3Max;}
	virtual	Vect3f					GetMin								()																{return m_v3Min;}
  std::string							GetEntity             ()																{return m_sEntity;}
  virtual std::string     GetOnEnterCode				()																{return m_onEnter;}
  virtual std::string     GetOnLeaveCode				()																{return m_onLeave;}
  virtual Mat44f          GetTransformation			()																{return m_Transformation;}
	virtual float						GetRotationX					()																{return m_fRotationX;}
	virtual float						GetRotationY					()																{return m_fRotationY;}
	virtual float						GetRotationZ					()																{return m_fRotationZ;}
	virtual Vect3f					GetSize								()																{return m_fSize;}
	virtual std::string     GetName								()																{return m_stName;}
	virtual bool						GetTriggerActive			()																{return m_bTriggerActive;}
	virtual Mat44f					GetMatrizTrigger			()																{return m_matrizTrigger;}

  // -----------------------
  // Write functions
  // -----------------------
  virtual void						SetPosition						(Vect3f pos)											{m_v3Position.x = pos.x; m_v3Position.y = pos.y; m_v3Position.z = pos.z;}
  virtual void						SetMax								(Vect3f pos)											{m_v3Max.x = pos.x; m_v3Max.y = pos.y; m_v3Max.z = pos.z;}
	virtual void						SetMin								(Vect3f pos)											{m_v3Min.x = pos.x; m_v3Min.y = pos.y; m_v3Min.z = pos.z;}
	virtual void						SetDimensions					(Vect3f dim)											{m_v3Dimensions.x = dim.x; m_v3Dimensions.y = dim.y; m_v3Dimensions.z = dim.z;}
  void										SetEntity							(std::string c)										{m_sEntity = c;}
  virtual void						SetOnEnterCode				(std::string c)										{m_onEnter = c;}
  virtual void						SetOnLeaveCode				(std::string c)										{m_onLeave = c;}
	virtual void						SetSize								(Vect3f size)											{m_fSize = size;}
	virtual void						SetName               (std::string name)								{m_stName = name;}
	virtual void						SetRotationX					(float AngleX)										{m_fRotationX = AngleX;}
	virtual void						SetRotationY					(float AngleY)										{m_fRotationY = AngleY;}
	virtual void						SetRotationZ					(float AngleZ)										{m_fRotationZ = AngleZ;}
	virtual void						SetTriggerActive			(bool status)											{m_bTriggerActive = status;}
	virtual void						SetMatrizTrigger			(Mat44f matriz)					  				{m_matrizTrigger = matriz;}

private:
  void        						Release   						();

  // member variables
  bool										m_bIsOk;          // Initialization boolean control
  Vect3f									m_v3Position;     // x,y,z
	Vect3f									m_v3Max;					// Max size x,y,z
	Vect3f									m_v3Min;					// Min size x,y,z
  Vect3f									m_v3Dimensions;   // ancho,alto,fondo
  Mat44f									m_Transformation;
  std::string							m_sEntity;
  std::string							m_onEnter;
  std::string							m_onLeave;
	float										m_fRotationX;
	float										m_fRotationY;
	float										m_fRotationZ;
	Vect3f									m_fSize;
	std::string							m_stName;
	bool										m_bTriggerActive;
	Mat44f									m_matrizTrigger;
	CRenderableObject*			m_RenderObj;
	CRenderablePhysXObject* m_PhysXObj;
};

#endif