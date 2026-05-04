#include "RenderablePhysXManager.h"
#include "RenderablePhysXObject.h"
#include "Exceptions/Exception.h"
#include "Object3D/BoundingObject.h"
#include "Object3D/BoundingBox.h"
#include "Object3D/BoundingSphere.h"
#include "Object3D/BoundingCapsule.h"
#include "Object3D/BoundingMesh.h"
#include "RenderableObjects/RenderableObject.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "Actor/PhysicActor.h"
#include "PhysicsManager.h"
#include "CookingMesh/PhysicCookingMesh.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


CRenderablePhysXManager::CRenderablePhysXManager(void)
{
}

CRenderablePhysXManager::~CRenderablePhysXManager(void)
{
  Destroy();
  //CleanUp();
}

/*void CRenderablePhysXManager::AddResource(CRenderablePhysXObject * ObjectPhysX)
{
  std::string l_Name = ObjectPhysX->GetName();
  if (m_RenderablePhysXObjects.find(l_Name)==m_RenderablePhysXObjects.end()) 
  {
    CMapManager::AddResource(l_Name, ObjectPhysX);
  }
  else
  {
    //Guardar el mensaje de error en el LOGGER
    std::string msg_error = "CRenderablePhysXManager::AddRenderablePhysXObject->Error al intentar añadir el recurso: " + l_Name;
    LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
	}
}*/
void CRenderablePhysXManager::AddRenderablePhysXObject(std::string RenderableObjectName, Vect3f Min, Vect3f Max, float Density, float AngularDamping, int TYPE, float Visible)
{
  if(TYPE != NO_BOUNDING_OBJECT)
  {
    CRenderableObject * l_RenderableObject = CORE->GetRenderableObjectsManager()->GetInstance(RenderableObjectName);
    
    l_RenderableObject->SetVisible(Visible);

    //Posicion absoluta
    Vect3f l_positionObject3D = l_RenderableObject->GetPosition();

    Vect3f l_MiddlePoint = l_RenderableObject->GetBoundingObject()->GetMiddlePoint();
    l_MiddlePoint.x = 0.0f;
    l_MiddlePoint.z = 0.0f;

    //Types: box, sphere, capsule, mesh
    CRenderablePhysXObject * l_PhysXTest = new CRenderablePhysXObject(RenderableObjectName);
    
    if(TYPE==BOUNDING_BOX)
    {
      //TODO: Verificar el tamaño de la caja.
      //Ayuda de PhysX ir a Box Shape a la dimension
      Vect3f l_Size = l_RenderableObject->GetBoundingObject()->GetDimension();
      //Vect3f l_Size(1.f,1.f,1.f);
      l_PhysXTest->GetPhysXActor()->AddBoxSphape(l_Size*0.5f, (Max+Min)*0.5f,0,Density > 0.001f ? PX_CG_STATIC_ELEMENT_DENSITY : PX_CG_STATIC_ELEMENT);
      //l_PhysXTest->GetPhysXActor()->AddBoxSphape(l_Size*0.5f, (Max+Min)*0.5f,0,PX_CG_ITEM);
      //NxActor* a = l_PhysXTest->GetPhysXActor()->GetPhXActor()->getGroup();
    }
    if(TYPE==BOUNDING_SPHERE)
    {
		  l_MiddlePoint.y = 0.0f;
		  CBoundingSphere* tmp = (CBoundingSphere*)l_RenderableObject->GetBoundingObject();
		  float l_Radius = tmp->GetRadius();
      l_PhysXTest->GetPhysXActor()->AddSphereShape(l_Radius*0.5f,l_MiddlePoint,0,Density > 0.001f ? PX_CG_STATIC_ELEMENT_DENSITY : PX_CG_STATIC_ELEMENT);
    }
    else if(TYPE==BOUNDING_CAPSULE)
    {
		  CBoundingCapsule* tmp = (CBoundingCapsule*)l_RenderableObject->GetBoundingObject();
		  float l_Radius = tmp->GetRadius();
		  float l_Height = tmp->GetHeight();
      l_PhysXTest->GetPhysXActor()->AddCapsuleShape(l_Radius,l_Height,l_MiddlePoint,0,Density > 0.001f ? PX_CG_STATIC_ELEMENT_DENSITY : PX_CG_STATIC_ELEMENT);
    }
    else if(TYPE==BOUNDING_MESH)
    {
      CBoundingMesh * tmp = (CBoundingMesh*)l_RenderableObject->GetBoundingObject();
      CPhysicCookingMesh * l_CookingMesh = CORE->GetPhysicsManager()->GetCookingMesh();

      std::vector<Vect3f> l_VertexBufferPhysX = tmp->GetVertexBuffer();
      std::vector<unsigned int> l_IndexBufferPhysX = tmp->GetIndexBuffer();
      
      l_CookingMesh->CreatePhysicMesh(l_VertexBufferPhysX,l_IndexBufferPhysX,tmp->GetName());
      l_PhysXTest->GetPhysXActor()->AddMeshShape(l_CookingMesh->GetPhysicMesh(tmp->GetName()),Vect3f(0.0f,0.0f,0.0f),PX_CG_SCENARIO);
    }
    l_PhysXTest->GetPhysXActor()->SetGlobalPosition(l_positionObject3D);

    if(Density > 0.001f)//Agrega fisica dinamica al objeto 
    {
      l_PhysXTest->GetPhysXActor()->CreateBody(Density);
    }
    CORE->GetPhysicsManager()->AddPhysicActor(l_PhysXTest->GetPhysXActor());

    Mat44f l_MatrixToPhysX = l_RenderableObject->GetMat44();
    l_PhysXTest->GetPhysXActor()->SetMat44(l_MatrixToPhysX);

    AddResource(l_PhysXTest->GetName(), l_PhysXTest);
  }
}
CRenderablePhysXObject * CRenderablePhysXManager::GetRenderablePhysXObject(std::string ObjectId)
{
  return GetResource(ObjectId);
}
void CRenderablePhysXManager::Update(float ElapsedTime)
{
  std::map<std::string, CRenderablePhysXObject*>::iterator it_1			= CMapManager::m_Resources.begin();// m_RenderablePhysXObjects.begin();
  std::map<std::string, CRenderablePhysXObject*>::iterator itEnd_1	= CMapManager::m_Resources.end();//m_RenderablePhysXObjects.end();
  while (it_1 != itEnd_1)
  {
    it_1->second->Update(ElapsedTime);
    ++it_1;
  }
}
// Andres: Esta funcion ya no se usa pero la hizo marco la dejo comentada por si acaso 
/*void CRenderablePhysXManager::CleanUp()
{
  std::map<std::string, CRenderablePhysXObject*>::iterator it_1			= m_RenderablePhysXObjects.begin();
  std::map<std::string, CRenderablePhysXObject*>::iterator itEnd_1	= m_RenderablePhysXObjects.end();
  while (it_1 != itEnd_1)
  {
    CHECKED_DELETE(it_1->second);
    ++it_1;
  }
}*/