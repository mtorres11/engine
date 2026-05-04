#include "Camera.h"
#include "Assert.h"
#include "Math\MathUtils.h"
#include "Object3D/Object3D.h"
//#include "Frustum.h"

CCamera::CCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, ETypeCamera typeCamera, std::string name, bool IsStatic)
: m_pObject3D(object3D)
,	m_fFOV(fov)
,	m_fAspectRatio(aspect)
,	m_fZNear(zn)				
,	m_fZFar(zf)		
, m_fView_d(2.f)
, m_eTypeCamera(typeCamera)
, m_Name(name)
, m_IsStatic(IsStatic)
, m_ShakeTime(0)
, m_Shake(false)
{
	assert(m_pObject3D);
  //m_Frustum = new CFrustum();
}

CCamera::CCamera()
: m_pObject3D(0)
,	m_fFOV( mathUtils::Deg2Rad(60.f) )
,	m_fAspectRatio(4.f/3.f)
,	m_fZNear(0.1f)				
,	m_fZFar(1000.f)	
, m_fView_d(2.f)
, m_eTypeCamera(TC_THPS)
, m_Name("")
, m_IsStatic(true)
, m_ShakeTime(0)
, m_Shake(false)
{
  assert(m_pObject3D);
  //m_Frustum = new CFrustum();
}

///<summary>
/// CCamera:: GetMatrixView : Retorna la Matriz de Vision de la camara.
///</summary>
///<param name="void"></param>
///<returns name="D3DXMATRIX"></returns>
D3DXMATRIX CCamera::GetMatrixView()
{
  D3DXMATRIX l_matView;
  Vect3f eye = this->GetEye();
  Vect3f vup = this->GetVecUp();
  Vect3f lookat = this->GetLookAt();

  D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z);  
	D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);
  D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);
  //Setup Matrix view
  D3DXMatrixLookAtLH( &l_matView, &l_Eye, &l_LookAt, &l_VUP);
  
  return l_matView;
}
///<summary>
/// CCamera:: GetMatrixProj : Retorna la Matriz de Proyeccion de la camara.
///</summary>
///<param name="void"></param>
///<returns name="D3DXMATRIX"></returns>
D3DXMATRIX CCamera::GetMatrixProj()
{
  D3DXMATRIX l_matProject;
  D3DXMatrixPerspectiveFovLH(	&l_matProject, this->GetFov(), this->GetAspectRatio(),
		                          this->GetZn(), this->GetZf());
  return l_matProject;
}

 void CCamera::SetParameters(Vect3f _V3fPosition, float _fYaw, float _fPitch, float _fRoll, float _fFOV/*, float _fAspectRatio*/, float _fZNear, float _fZFar)
 {
   m_pObject3D->SetPosition(_V3fPosition);
   m_pObject3D->SetYaw(_fYaw);
   m_pObject3D->SetPitch(_fPitch);
   m_pObject3D->SetRoll(_fRoll);
   m_fFOV = _fFOV;
   //m_fAspectRatio = _fAspectRatio;
   m_fZNear = _fZNear;
   m_fZFar = _fZFar;
 }
/*Vibration Camera Functions*/
void CCamera::InitShake()
{
  m_Shake = true;
}

void CCamera::CameraShake(float elapsedTime)
{
  if(m_Shake)
  {
    float pitch = m_pObject3D->GetPitch();

    if(m_ShakeTime < 1)
    {
      pitch = (float)(sin(4.5*pitch)/(pitch))*-0.1f;
      m_ShakeTime += elapsedTime;
    }
    else
    {
      m_ShakeTime = 0.0f;
      m_Shake = false;
      pitch = -0.39f;
    }
    m_pObject3D->SetPitch(pitch);
  }
}