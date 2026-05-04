//----------------------------------------------------------------------------------
// CCamera class
// Author: Enric Vergara
//
// Description:
// Clase base que contiene la informacion generica de una camara la cual esta atachada a un 
// puntero de la clase base CObject3D
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "Base.h"
#include "Object3D/Object3D.h"
#include "Actor/PhysicActor.h"
#include "PhysicsManager.h"

#define CAMERA_ZOOM 4.0f

//-------FORWARD DECLARATION----------
//class CObject3D;
//class ETypeCamera;
//------------------------------------

class CCamera : public CNamed
{
public:
	typedef enum ETypeCamera {TC_FPS = 0, TC_THPS};

public:

	CCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, ETypeCamera typeCamera, std::string name, bool IsStatic);
	CCamera();
	virtual ~CCamera(){ CHECKED_DELETE(m_pObject3D); /*m_pObject3D = 0;*/ }

	//--- GET FUNCTIONS ---
	virtual Vect3f					GetDirection		() const = 0;
	virtual Vect3f					GetLookAt				() const = 0;
	virtual Vect3f					GetEye					() const = 0;
	virtual Vect3f					GetVecUp				() const = 0;
	CObject3D*							GetObject3D			() {return m_pObject3D;}
	float										GetZf						() const { return m_fZFar;}
	float										GetZn						() const { return m_fZNear;}
	float										GetFov					() const { return m_fFOV;}
	float										GetViewD				() const { return m_fView_d;}
	float										GetAspectRatio	() const { return m_fAspectRatio;}
	ETypeCamera							GetTypeCamera		() const { return m_eTypeCamera;}
  D3DXMATRIX              GetMatrixView   ();
  D3DXMATRIX              GetMatrixProj   ();
  std::string             GetName         () const { return m_Name; }
  CPhysicActor *          GetPhysXActor   ()       { return m_PhysXActor; }
  bool                    GetShake        () const { return m_Shake;}

	//--- SET FUNCTIONS ---
	void										SetZn						(float amount )	{ m_fZNear = amount; }
	void										SetZf						(float amount )	{ m_fZFar = amount; }
	void										SetObject3D			(CObject3D* object3D )	{ m_pObject3D = object3D;}
  void                    SetName         (std::string Name) { m_Name = Name; }
  void                    SetParameters   (Vect3f _V3fPosition, float _fYaw, float _fPitch, float _fRoll, float _fFOV/*, float _fAspectRatio*/, float _fZNear, float _fZFar);

	//--- ADD FUNCTIONS ---
	void										AddZf						(float amount )			{ m_fZFar += amount; }
	void										AddZn						(float amount )			{ m_fZNear += amount; }
	void										AddFov					(float delta_fov )	{ m_fFOV += delta_fov; }
	void										AddViewD				(float amount )			{ if( m_fView_d + amount > 1) m_fView_d += amount; }

  //--- UTILS ---
  // -----------------------
  // Vibration Functions
  //------------------------
  void              CameraShake                       (float elapsedTime);
  void              InitShake                         ();

protected:

	CObject3D*	  m_pObject3D;
	float				  m_fView_d;					// variable de debug utilizada para pintar el objeto de la camara.	
	float				  m_fFOV;
	float				  m_fAspectRatio;
	float				  m_fZNear;						// valor del z near (a partir de que vemos)
	float				  m_fZFar;						// valor del z far (hasta donde podemos ver)
	ETypeCamera 	m_eTypeCamera;
  CPhysicActor* m_PhysXActor;
  std::string   m_Name;
  bool          m_IsStatic;

  // -- Camera Vibration Variables
  float               m_ShakeTime;
  bool                m_Shake;
};

#endif // INC_CAMERA_H_