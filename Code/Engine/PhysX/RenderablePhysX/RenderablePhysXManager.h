/*----------------------------------------------------------------------------------*\
 * CGameProcess class                                                               *
 * Author: Marco Torres                                                             *
 * Last Modification: Marco Torres                                                  *
 * Description:                                                                     *
 * Clase CameraManager.                                                             *
 * Gestiona todas las cámaras de la escena																				  *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef RENDERABLE_PHYSX_MANAGER_H_
#define RENDERABLE_PHYSX_MANAGER_H_

#include "MapManager/MapManager.h"
#include "RenderablePhysXObject.h"
#include "Object3D/BoundingObject.h"

//--- Forward Declaration
class CRenderablePhysXObject;

class CRenderablePhysXManager : public CMapManager<CRenderablePhysXObject>
{
public:
  CRenderablePhysXManager(void);
  ~CRenderablePhysXManager(void);

  bool                        Reload								        ();
	//void                        CleanUp								        ();
  void                        AddRenderablePhysXObject      (std::string RenderableObjectName, Vect3f Min, Vect3f Max, float Density, float AngularDamping, int TYPE = BOUNDING_BOX, float Visible = true);
  CRenderablePhysXObject *	  GetRenderablePhysXObject			(std::string ObjectId);
  void                        Update                        (float ElapsedTime);

private:
  // Andres: esta varibale ya no se usa pero la dejo comentada por si acaso
  /*typedef std::map<std::string, CRenderablePhysXObject*>				TDefaultRenderablePhysXObject;
	TDefaultRenderablePhysXObject									                m_RenderablePhysXObjects;*/
};

#endif