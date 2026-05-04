#include "Core.h"
#include "Camera.h"
#include "Cameras/ThPSCamera.h"
#include "Cameras/FPSCamera.h"
#include "CameraManager.h"
#include "Object3D/Object3D.h"
#include "RenderManager.h"
#include "XML/XMLTreeNode.h"
#include "Exceptions/Exception.h"
#include "Cameras/Frustum.h"
#include "Object3d/BoundingObject.h"
#include "Object3d/BoundingSphere.h"

CCameraManager::CCameraManager()
:m_DefaultCamera(0)
{
  m_Frustum = new CFrustum();
}

CCameraManager::~CCameraManager()
{
	CHECKED_DELETE(m_DefaultCamera);
  CHECKED_DELETE(m_Frustum);
	CleanUp();
}

///<summary>
/// CCameraManager:: Init : Inicializa el CameraManager.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
bool CCameraManager::Init()
{
  bool l_IsOk = Load("Data/Cameras.xml");
  if(!l_IsOk)
  {
	  //AspectRatio
	  uint32 w,h;
	  CORE->GetRenderManager()->GetWandH(w,h);
	  float aspectRatio = (float)w/h;

	  //Cámara principal
	  CObject3D * l_pObject3D = new CObject3D();//TODO tratar de asignar directamente la cámara al bot
	  m_DefaultCamera = new CThPSCamera(0.1f, 100.f, 45.0f * D3DX_PI / 180.0f, aspectRatio, l_pObject3D, 10.f, "DefaultCamera");
	  CORE->GetCameraManager()->AddCamera("DefaultCamera", (CCamera*)m_DefaultCamera);

    //Guardar el mensaje de error en el LOGGER
		std::string msg_error = " CCameraManager::Init->Se ha agregado la camara por defecto.";
		msg_error.append(m_FileName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);

	  //TODO: Eliminar esta linea cuando ya la camara esté en el personaje. 
	  //CORE->GetCameraManager()->GetCamera()->GetObject3D()->SetPitch(-FLOAT_PI_VALUE/8);
  }
	return l_IsOk;
}

///<summary>
/// CCameraManager:: CleanUp : Hace el release de cada uno de los elementos del mapa. 
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CCameraManager::CleanUp()
{
	Destroy();
}

///<summary>
/// CCameraManager:: AddCamera : Agrega una cámara al mapa.
///</summary>
///<param name="&Name"></param>
///<param name="camera"></param>
///<returns name=""></returns>
void CCameraManager::AddCamera(const std::string &Name, CCamera * camera)
{
	if (m_Resources.find(Name)==m_Resources.end()) 
	{
		CMapManager::AddResource(Name, camera);
	}
	else
	{
		//Guardar el mensaje de error en el LOGGER
		std::string msg_error = "CCameraManager::AddCamera->Error al intentar añadir el recurso: " + Name;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
}

///<summary>
/// CCameraManager:: GetCamera : Devuelve una cámara del mapa con el nombre de CameraName.
///</summary>
///<param name="CameraName">Nombre de la cámara a buscar dentro del mapa.</param>
///<returns name="CCamera *">Devuelve la cámara con nombre CameraName.</returns>
CCamera *	CCameraManager::GetCameraByName(const std::string CameraName)
{
	//return GetResource(CameraName);
  TDefaultCameraMap::iterator iter = m_CamerasMap.find(CameraName);
	if(iter == m_CamerasMap.end())
	{
		return NULL;
	}
	else{
		return iter->second;
	}
}

///<summary>
/// CCameraManager:: Reload : Recargar el XML de camaras
///</summary>
///<param name="FileName" type="std::string">Nombre del archivo XML con las camaras, falso en caso contrario.</param>
///<returns name="Result">Devuelve verdadero si la ejecución es exitosa.</returns>
bool CCameraManager::Load(const std::string &FileName)
{
	bool l_bIsOk = true;
	m_FileName = FileName;
	CXMLTreeNode filexml;
	if (!filexml.LoadFile(m_FileName.c_str()))
	{
		//Guardar el mensaje de error en el LOGGER
		std::string msg_error = " CCameraManager::Load->Error al intentar abrir el archivo: ";
		msg_error.append(m_FileName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
		l_bIsOk = false;
	}
	else
	{
		//<cameras>
    //  <camera name="Camera0" default="true" position="0 5 0" yaw="1.0f" pitch="-0.39f" roll="0" znear="0.1f" zfar="100.f" fov="0.78f" zoom="10.f"/>
		//</cameras>
		CXMLTreeNode cameras = filexml["cameras"];
		if (cameras.Exists())
		{
			if (!cameras.IsComment())
			{
				int32 numNodes = cameras.GetNumChildren();		
				for(int i=0; i < numNodes; i++)
				{
					if (!cameras(i).IsComment())
					{
						CXMLTreeNode camera = cameras(i)["camera"];
						if (camera.Exists())
						{
							if (!camera.IsComment())
							{			               
								std::string l_sName = camera.GetPszProperty("name");
                bool l_CameraDefault = camera.GetBoolProperty("default");
								Vect3f l_fPosition = camera.GetVect3fProperty("position", Vect3f(0.f,0.f,0.f));
								float l_fYaw = camera.GetFloatProperty("yaw");
								float l_fPitch = camera.GetFloatProperty("pitch");
								float l_fRoll = camera.GetFloatProperty("roll");
								float l_fZNear = camera.GetFloatProperty("znear");
								float l_fZFar = camera.GetFloatProperty("zfar");
								float l_fFov = camera.GetFloatProperty("fov");
								//float l_fAspectRatio = camera.GetFloatProperty("aspectRatio");
                float l_fAspectRatio = 4.f/3.f;
								float l_fZoom = camera.GetFloatProperty("zoom");
                bool l_IsStatic = camera.GetBoolProperty("static");

                CObject3D * l_pObject3D = new CObject3D(l_fPosition,l_fYaw,l_fPitch,l_fRoll);
								CCamera * l_pCamera;
                //If ZOOM == 0 => Camara = FPSCamera
								if (l_fZoom != 0)
								{
									l_pCamera = new CThPSCamera(l_fZNear, l_fZFar, l_fFov, l_fAspectRatio, l_pObject3D, l_fZoom, l_sName, l_IsStatic);
								}
								else
								{
									l_pCamera = new CFPSCamera(l_fZNear, l_fZFar, l_fFov, l_fAspectRatio, l_pObject3D, l_sName, l_IsStatic);
								}
								//Comprobar que no exista en el mapa
								TDefaultCameraMap::iterator iterador = m_CamerasMap.find(l_sName);
								if (iterador == m_CamerasMap.end()) 
								{
									m_CamerasMap[l_sName]=l_pCamera;
								}
								else
								{
									std::string msg_warning = "CCameraManager::Load->El elemento ya existe dentro del mapa: " + l_sName;
									LOGGER->AddNewLog(ELL_WARNING, msg_warning.c_str());
									throw CException(__FILE__, __LINE__, msg_warning);
									l_bIsOk = false;
								}
                if(l_CameraDefault)
                {
                  m_DefaultCamera = l_pCamera;
                }
							}
						}//endif CameraTHPS
					}
				}
			}
		}
	}
	return l_bIsOk;
}


///<summary>
/// CCameraManager:: Reload : Recargar el XML de camaras
///</summary>
///<param name="void"></param>
///<returns name="Result">Devuelve verdadero se la ejecución es existosa, falso en caso contrario.</returns>
bool CCameraManager::Reload()
{
	CleanUp();
	bool temp = Load(m_FileName);
	return temp;
}

///<summary>
/// CCameraManager:: SetCamera : Guardamos la cámara.
///</summary>
///<param name="camera">Cámara que vamos a guardar.</param>
///<returns name=""></returns>
void CCameraManager::SetCamera (CCamera* camera)
{
	m_DefaultCamera = camera;
}

///<summary>
/// CCameraManager:: ResetToDefaultCamera : Volvemos a fijar la cámara por defecto.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void CCameraManager::ResetToDefaultCamera ()
{
	SetCamera(GetCameraByName("DefaultCamera"));
}

///<summary>
/// CCameraManager:: DrawCameras : Dibuja todas las camaras que existan.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CCameraManager::DrawCameras ()
{
	TDefaultCameraMap::iterator iter = m_CamerasMap.begin();
  TDefaultCameraMap::iterator itEnd = m_CamerasMap.end();
  for(; iter != itEnd; ++iter)
  { 
    CORE->GetRenderManager()->DrawCamera(iter->second);
  }
  m_Resources.clear();
}
///<summary>
/// CCameraManager:: UpdateFrustumCamera : Actualiza el Frsutum de la camara activa.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CCameraManager::UpdateFrustumCamera()
{
  D3DXMATRIX matView;
  D3DXMATRIX matProj;

  matView = m_DefaultCamera->GetMatrixView();
  matProj  = m_DefaultCamera->GetMatrixProj();
  m_Frustum->Update( matView * matProj ); 
}