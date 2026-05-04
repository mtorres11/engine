#include "Object3D.h"
//#include "BoundingObject.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"
#include "BoundingCapsule.h"
#include "BoundingMesh.h"
#include "Base.h"
#include "Math/Matrix44.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//CObject3D::CObject3D(const Vect3f& pos, float yaw, float pitch, float roll)
//: m_Position(pos)
//, m_Scalar( Vect3f(0.f,0.f,0.f))
//, m_fYaw(yaw)
//, m_fPitch(pitch)
//, m_fRoll(roll)
//{
//
//}

CObject3D::CObject3D(const Vect3f& pos, float yaw, float pitch, float roll, float scalarX, float scalarY, float scalarZ)
: m_Position(pos)
, m_ScalarX(scalarX)
, m_ScalarY(scalarY)
, m_ScalarZ(scalarZ)
, m_fYaw(yaw)
, m_fPitch(pitch)
, m_fRoll(roll)
, m_BoundingObject(0)
{
  RecalculateMat44f();
}

CObject3D::CObject3D()
: m_Position( Vect3f(0.f,0.f,0.f))
, m_ScalarX(1.f)
, m_ScalarY(1.f)
, m_ScalarZ(1.f)
, m_fYaw(0.f)
, m_fPitch(0.f)
, m_fRoll(0.f)
, m_BoundingObject(0)
{
  m_Mat44 = m44fIDENTITY;
}

CObject3D::~CObject3D()
{
  if(m_BoundingObject != 0)
  {
    CHECKED_DELETE(m_BoundingObject);
  }
}

bool CObject3D::InitializeBoundingObject(int BoundingObject)
{
  bool IsOk = true;
  //Si no tiene fisica se crea una caja para el renderizado
  //por medio del frustum para mejorar eficiencia
  if(BoundingObject==NO_BOUNDING_OBJECT)
  {
    CBoundingBox * box = new CBoundingBox();
    m_BoundingObject = (CBoundingObject*)box;
  }
  else if(BoundingObject==BOUNDING_BOX)
  {
    CBoundingBox * box = new CBoundingBox();
    m_BoundingObject = (CBoundingObject*)box;
  }
  else if(BoundingObject==BOUNDING_SPHERE)
  {
    CBoundingSphere * sphere = new CBoundingSphere();
    m_BoundingObject = (CBoundingObject*)sphere;
  }
  else if(BoundingObject==BOUNDING_CAPSULE)
	{
		CBoundingCapsule * capsule = new CBoundingCapsule();
    m_BoundingObject = (CBoundingObject*)capsule;
	}
  else if(BoundingObject==BOUNDING_MESH)
	{
    CBoundingMesh * mesh = new CBoundingMesh();
    m_BoundingObject = (CBoundingObject*)mesh;
	}
	else
  {
    IsOk = false;
  }
  return IsOk;
}

Mat44f CObject3D::GetTransform()
{
  return GetMat44();
  /*Mat44f l_Rotation, l_Translation;
  
  Mat44f l_Scale;
  l_Translation.SetIdentity();
  l_Scale.SetFromScale(m_ScalarX, m_ScalarY, m_ScalarZ);
  
  l_Rotation.SetIdentity();
  l_Rotation.SetFromPitchRollYaw(Vect3f(m_fPitch, m_fRoll, m_fYaw));
  
  l_Translation.SetIdentity();
  l_Translation.Translate(m_Position);
  
  return l_Translation * l_Rotation * l_Scale;*/
}

void CObject3D::SetPosition(const Vect3f& Pos)
{
  m_Position = Pos;
  //RecalculateMat44f();
  m_Mat44.Translate(Pos);
}

void CObject3D::SetYaw( float Yaw )
{
  m_fYaw = Yaw;
  RecalculateMat44f();
}

void CObject3D::SetPitch( float Pitch )
{
  m_fPitch = Pitch;
  RecalculateMat44f();
}

void CObject3D::SetRoll( float Roll )
{
  m_fRoll = Roll;
  RecalculateMat44f();
}
void CObject3D::SetScalar(Vect3f scale)
{
  m_ScalarX = scale.x;
  m_ScalarY = scale.y;
  m_ScalarZ = scale.z;
  RecalculateMat44f();
}
void CObject3D::SetScalar(float scalarX, float scalarY, float scalarZ)
{
  m_ScalarX = scalarX;
  m_ScalarY = scalarY;
  m_ScalarZ = scalarZ;
  RecalculateMat44f();
}
void CObject3D::SetScalarX(float scalarX)
{
  m_ScalarX = scalarX;
  RecalculateMat44f();
}
void CObject3D::SetScalarY(float scalarY)
{
  m_ScalarY = scalarY;
  RecalculateMat44f();
}
void CObject3D::SetScalarZ(float scalarZ)
{
  m_ScalarZ = scalarZ;
  RecalculateMat44f();
}
 /*Vect3f CObject3D::GetCenterPosition()
 {
   return m_Position+m_BoundingObject.GetMiddlePoint();
 }*/
 void  CObject3D::RecalculateMat44f   ()
 {
    Mat44f r, r2, r3, t, l_Scale;
    t.SetIdentity();
    r.SetIdentity();
    r2.SetIdentity();
    r3.SetIdentity();
    t.Translate(m_Position);
    r.SetFromAngleY(-m_fYaw);
    r2.SetFromAngleZ(m_fPitch);
    r3.SetFromAngleX(m_fRoll);
    l_Scale.SetFromScale(m_ScalarX, m_ScalarY, m_ScalarZ);
    m_Mat44 = t*r*r2*r3*l_Scale;  
 }

 void CObject3D::SetMat44(Mat44f matrix) 
 {
   m_Mat44 = matrix;
   m_fPitch = matrix.GetPitch();
   m_fYaw = matrix.GetYaw();
   m_fRoll = matrix.GetRoll();
   m_Position = matrix.GetPos();
   m_ScalarX = matrix.GetScale().x;
   m_ScalarY = matrix.GetScale().y;
   m_ScalarZ = matrix.GetScale().z;
 }