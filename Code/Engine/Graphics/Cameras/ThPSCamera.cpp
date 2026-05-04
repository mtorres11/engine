#include "ThPSCamera.h"
#include "Object3D/Object3D.h"
#include "Object3D/BoundingObject.h"
#include "Object3D/BoundingSphere.h"
#include "Actor/PhysicActor.h"
#include "PhysicsManager.h"
#include "assert.h"

CThPSCamera::CThPSCamera(float zn, float zf, float fov, float aspect, CObject3D* object3D, float zoom, std::string name, bool IsStatic)
: CCamera( zn, zf, fov, aspect, object3D, TC_THPS, name, IsStatic)
, m_fZoom( zoom )
{
}

CThPSCamera::CThPSCamera()
: CCamera()
, m_fZoom(50.f)
{
}


Vect3f CThPSCamera::GetDirection () const
{
	assert(m_pObject3D);

	return (m_pObject3D->GetPosition() - GetEye());
}

Vect3f CThPSCamera::GetLookAt () const
{
	assert(m_pObject3D);

	return m_pObject3D->GetPosition();
}

Vect3f CThPSCamera::GetEye () const
{
	assert(m_pObject3D);

	float yaw		= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();
	Vect3f pos	= m_pObject3D->GetPosition();

	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	Vect3f vEyePt(	m_fZoom * cos(yaw) * cos(pitch), 
		m_fZoom * sin(pitch),
		m_fZoom * sin(yaw) * cos(pitch) );

	return (pos - vEyePt); 
}

Vect3f CThPSCamera::GetVecUp () const
{
	assert(m_pObject3D);

	float yaw		= m_pObject3D->GetYaw();
	float pitch	= m_pObject3D->GetPitch();

	Vect3f vUpVec(	-cos(yaw) * sin(pitch), 
		cos(pitch), 
		-sin(yaw) * sin(pitch) );

	return vUpVec;
}

void CThPSCamera::SetZoom (float zoom)
{
	m_fZoom = zoom;

	if( m_fZoom > m_fZFar*0.8f ) {
		m_fZoom = m_fZFar*0.8f;
	}
	else if( m_fZoom < m_fZNear*2.f) {
		m_fZoom = m_fZNear*2.f;
	}
}

void CThPSCamera::AddZoom (float zoom)
{
	m_fZoom += zoom;

	if( m_fZoom > m_fZFar*0.8f ) {
		m_fZoom = m_fZFar*0.8f;
	}
	else if( m_fZoom < m_fZNear*2.f) {
		m_fZoom = m_fZNear*2.f;
	}
}
///<summary>
/// CCamera:: InitCameraPhysX : Inicia el objeto de fisica de la camara.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CThPSCamera::InitCameraPhysX()
{
  /*TODO: Agregar la fisica de las camaras*/
  //Faltan los includes de los objetos de fisica
  //Verificar costos de tener todas las camaras con fisica o si no es necesario.
  //Tambien se puede generar la fisica de la caja cuando esta se active y
  //desactivar en el momento que se deje de utilizar.
  //m_pObject3D->InitializeBoundingObject(BOUNDING_SPHERE);
  //Vect3f l_min, l_max, l_position;
  /*Camera Radius*/
  //l_min.x = 0.0f;
  //l_min.y = 0.0f;
  //l_min.z = 0.0f;
  //l_max.x = 1.0f;
  //l_max.y = 1.0f;
  //l_max.z = 1.0f;
  //l_position = this->GetEye(); //Eye Position
  //((CBoundingSphere*)m_pObject3D->GetBoundingObject())->Init(l_min,l_max,BOUNDING_SPHERE);
  //CBoundingSphere* tmp = (CBoundingSphere*)m_pObject3D->GetBoundingObject();
	//float l_Radius = tmp->GetRadius();
  //CPhysicUserData * l_CameraSphere = new CPhysicUserData(m_Name);
  //m_PhysXActor = new CPhysicActor(l_CameraSphere);
  //m_PhysXActor->AddSphereShape(l_Radius,tmp->GetMiddlePoint());
  //m_PhysXActor->SetGlobalPosition(l_position);
  //m_PhysXActor->SetMat44(m_pObject3D->GetMat44());
  //CORE->GetPhysicsManager()->AddPhysicActor(m_PhysXActor);

  /*//Test
  CPhysicUserData * esfera = new CPhysicUserData("esfera");
	CPhysicActor * l_esfera_test = new CPhysicActor(esfera);
	l_esfera_test->AddSphereShape(0.5,Vect3f(0.5f,0.5f,0.5f));
  l_esfera_test->AddBoxSphape(Vect3f(1.0f,1.0f,1.0f),Vect3f(5.0f,5.0f,5.0f));*/
}