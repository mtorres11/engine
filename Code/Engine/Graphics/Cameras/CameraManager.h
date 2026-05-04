/*----------------------------------------------------------------------------------*\
 * CGameProcess class                                                               *
 * Author: Marco Torres                                                             *
 * Last Modification: Andrea Cristobo                                               *
 * Description:                                                                     *
 * Clase CameraManager.                                                             *
 * Gestiona todas las cámaras de la escena																				  *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef CAMERA_MANAGER_H_
#define CAMERA_MANAGER_H_

//-----------FORWARD DECLARATION-----------
class CCamera;
class CThPSCamera;
class CFPSCamera;
class CObject3D;
class CFrustum;
class CCameraKeyController;
//-----------------------------------------

#include "Base.h"
#include "MapManager/MapManager.h"
#include <vector>
#include "Math/Vector3.h"
#include <string>

class CCameraManager : CMapManager<CCamera>
{
public:
  //Construct
	CCameraManager();
  //Destruct
	virtual ~CCameraManager();

  //functions
	bool      Init						      ();
	bool      Load									(const std::string &FileName);
	bool      Reload								();
	void      CleanUp								();
	void      AddCamera					    (const std::string &Name, CCamera * camera); //agregar un Object3D en lugar de camara
  void      AddCameraKeyController (const std::string &FileName, CCamera* _pCamera, bool _bCycle, bool _bReverse);
	void      SetCamera             (CCamera* camera);
	void      ResetToDefaultCamera  ();
  void      DrawCameras           ();
  CCamera *	GetCamera					    (){return m_DefaultCamera;}
	CCamera *	GetCameraByName       (const std::string CameraName);

  /*FRUSTUM*/
  void      UpdateFrustumCamera   ();
  CFrustum* GetFrustum            () const { return m_Frustum; }
	
	//TODO función para cambiar de cámaras (asigne una cámara del mapa a la ppal)
  
private:
	typedef std::map<std::string, CCamera*>				TDefaultCameraMap;
	TDefaultCameraMap															m_CamerasMap;
  std::string                                   m_FileName;
	CCamera*																			m_DefaultCamera;
  CFrustum*                                     m_Frustum;

};

#endif