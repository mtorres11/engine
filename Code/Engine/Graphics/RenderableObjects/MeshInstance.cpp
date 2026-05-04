#include "MeshInstance.h"
#include "RenderManager.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "Math/Matrix44.h"
#include "Shaders/EffectManager.h"
#include "Core.h"
#include "Cameras/CameraManager.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"
#include "Object3D/BoundingObject.h"
#include "Object3D/BoundingBox.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


CInstanceMesh::CInstanceMesh(const std::string &Name, const std::string &CoreName)
{
  m_StaticMesh = NULL;
  m_bIsOk = true;

  CStaticMeshManager *sm = CORE->GetStaticMeshManager();
  m_StaticMesh = sm->GetResource(CoreName);
  if(m_StaticMesh == NULL)
  {
    m_bIsOk = false;
  }
  else
  {
    SetName(Name);
    SetPitch(0.0);
    SetPosition(0.0);
    SetRoll(0.0);
    SetVisible(false);
    SetYaw(0.0);
  }
}

CInstanceMesh::~CInstanceMesh()
{
  
}

void CInstanceMesh::Render(CRenderManager *RM)
{
  if(GetVisible())
  {
    CFrustum * l_Frustum = CORE->GetCameraManager()->GetFrustum();
    D3DXVECTOR3 l_DXMiddlePoint;
    Vect3f l_MiddlePoint = m_BoundingObject->GetMiddlePoint();
    float l_Radius = m_BoundingObject->GetMaxRadius();
    l_MiddlePoint += m_Position;
    l_DXMiddlePoint.x = l_MiddlePoint.x;
    l_DXMiddlePoint.y = l_MiddlePoint.y;
    l_DXMiddlePoint.z = l_MiddlePoint.z;
    bool l_Visible = l_Frustum->SphereVisible(l_DXMiddlePoint,l_Radius);
    if(l_Visible)
    {
      Mat44f l_Transform = GetTransform();
      RM->SetTransform(l_Transform);
      CORE->GetEffectManager()->SetWorldMatrix(l_Transform);
      m_StaticMesh->Render(RM);
    }
  }
}

/*void CInstanceMesh::Update(float ElapsedTime)
{
  ElapsedTime = 0.f;
}
*/
//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CInstanceMesh::Init()
{
  m_bIsOk = true;



  if (!m_bIsOk){
    Release();
  }

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CInstanceMesh::Done(){
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CInstanceMesh::Release(){
//free memory
}



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------