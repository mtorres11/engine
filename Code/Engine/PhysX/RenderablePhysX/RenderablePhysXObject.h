/*----------------------------------------------------------------------------------*\
 * CGameProcess class                                                               *
 * Author: Marco Torres                                                             *
 * Last Modification: Marco Torres                                                  *
 * Description:                                                                     *
 * Clase CameraManager.                                                             *
 * Gestiona todas las cámaras de la escena																				  *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef RENDERABLE_PHYSX_OBJECT_H_
#define RENDERABLE_PHYSX_OBJECT_H_

#include "Base.h"
#include "Math/MathUtils.h"
#include <string>

//--- Forward Declaration
class CPhysicActor;
class CPhysicUserData;
class CRenderableObject;
class CRenderManager;

class CRenderablePhysXObject
{
public:
  CRenderablePhysXObject(std::string RenderableId);
  //CRenderablePhysXObject(std::string RenderableId, float density, float angularDamping, std::string type);
  ~CRenderablePhysXObject(void);

  //--- Get Functions
  CPhysicActor *        GetPhysXActor()       {return m_PhysXActor;}
  CRenderableObject *   GetRenderableObject() {return m_RenderableObject;}
  std::string           GetName()             {return m_Name;}
	CPhysicUserData*			GetUserData	(){return m_UserData;}
  Mat44f                GetTransform();

  //--- Set Functions
  void SetPhysXActor        (CPhysicActor * PhysXActor)             {m_PhysXActor = PhysXActor;}
  void SetRenderableObject  (CRenderableObject * RenderableObject)  {m_RenderableObject = RenderableObject;}
  void SetName              (std::string name)                      {m_Name = name;}
	void SetUserData					(CPhysicUserData* userdata)							{m_UserData=userdata;}
  void SetTransform         (Mat44f Transform);

  //--- Update Matrix
  void Update               (float ElapsedTime);

private:
  CPhysicActor *        m_PhysXActor;
	CRenderableObject *   m_RenderableObject;
  std::string           m_Name;
  CPhysicUserData *m_UserData;

};
#endif  