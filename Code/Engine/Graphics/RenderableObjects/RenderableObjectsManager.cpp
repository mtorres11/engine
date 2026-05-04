#include "RenderableObjectsManager.h"
#include "RenderableObject.h"
#include "MeshInstance.h"
#include "XML/XMLTreeNode.h"
#include "Exceptions/Exception.h"
#include "Vertex/RenderableVertex.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "RenderableAnimatedInstanceModel.h"
#include "Object3D/BoundingObject.h"
#include "Object3D/BoundingBox.h"
#include "Object3D/BoundingMesh.h"
#include "Object3D/BoundingSphere.h"
#include "Object3D/BoundingCapsule.h"
#include "Math/MathUtils.h"
#include "RenderablePhysX/RenderablePhysXManager.h"
#include "StaticMeshes/StaticMesh.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "PhysicsManager.h"
#include "CookingMesh/PhysicCookingMesh.h"
#include "Utils/BaseUtils.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CRenderableObjectsManager::~CRenderableObjectsManager()
{
  CleanUp();
  Destroy();
}

void CRenderableObjectsManager::Release()
{
  CleanUp();
  Destroy();
}

bool CRenderableObjectsManager::Load(const std::string &FileName)
{
	bool l_bIsOk = true;
	m_FileName = FileName;
  CXMLTreeNode filexml;

  if (!filexml.LoadFile(m_FileName.c_str()))
  {
		//Guardar el mensaje de error en el LOGGER
    std::string msg_error = "RenderableObjectsManager::Load->Error al intentar leer el XML: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
    l_bIsOk = false;
  }
  else
  {
		//<renderable_objs>
		//	<object name="Box01" core="Box01" pos="2.27125 0.0 0.0" yaw="0.0" pitch="-90.0" roll="90.0"  />
    //  <animated_model name="mi_amigo" core="bot" pos="2.27125 0.0 0.0" yaw="0.0" pitch="0.0" roll="0.0" scalex="0.0" scaley="0.0" scalez="0.0"/>
    //  <animated_model name="mi_amigo_2" core="bot" pos="8.27125 0.0 0.0" yaw="0.0" pitch="0.0" roll="0.0"/>
		//	<object name="Box02" core="Box01" pos="0.0 0.0 0.0" yaw="0.0" pitch="0.0" roll="0.0" scalex="0.0" scaley="0.0" scalez="0.0"/>
		//</renderable_objs>

    CXMLTreeNode renders = filexml["renderable_objs"];

    if (renders.Exists())
    {
      int32 numNodes = renders.GetNumChildren();		
      for(int i=0; i < numNodes; i++)
      {
				if (!renders(i).IsComment())
				{
					CXMLTreeNode linea = renders(i);
	        
					//recibe parametros
					std::string l_sObjectName = linea.GetPszProperty("name", "", false);
          if (l_sObjectName == "")
          {
            std::string msg_error;
            baseUtils::FormatSrting(msg_error, "RenderableObjectsManager::Load->No se especifico el tag 'name'. Archivo: '%s', nodo: %d", FileName, i);
						LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
          }
					std::string l_sObjectCore = linea.GetPszProperty("core");
					Vect3f l_V3ObjectPos = linea.GetVect3fProperty("pos", Vect3f(0.0, 0.0, 0.0));    
					float l_fObjectYaw = linea.GetFloatProperty("yaw", 0.f, false);        
					float l_fObjectPitch = linea.GetFloatProperty("pitch", 0.f, false); 
					float l_fObjectRoll = linea.GetFloatProperty("roll", 0.f, false); 

          //NOTA IMPORTANTE:
          //Si nos envian un objeto con fisica que hay que escalar
          //el Bounding Object no va a tener el tamaño requerido y
          //la caja de fisica va a ser del tamaño real con el que
          //se haya exportado el objeto.
          //Se debe verificar si se puede escalar el BoundingObject
          float l_fObjectScaleX = linea.GetFloatProperty("scalex", 1.f, false); 
	        float l_fObjectScaleY = linea.GetFloatProperty("scaley", 1.f, false); 
	        float l_fObjectScaleZ = linea.GetFloatProperty("scalez", 1.f, false); 

          Vect3f l_maxSize = linea.GetVect3fProperty("max", Vect3f(1.0f,1.0f,1.0f));
          Vect3f l_minSize = linea.GetVect3fProperty("min", Vect3f(0.0f,0.0f,0.0f));

          float l_Density = linea.GetFloatProperty("density", 0.5f);
          float l_AngularDamping = linea.GetFloatProperty("angularDamping", 0.0f);
          int l_Type = linea.GetIntProperty("type", BOUNDING_BOX);

          bool l_Visible = linea.GetBoolProperty("visible", true);
	          
					//crea instancia de la core
					CRenderableObject *inst;

          if(strcmp(linea.GetName(),"object") == 0)
          {
						inst = AddMeshInstance(l_sObjectCore, l_sObjectName, l_V3ObjectPos, l_fObjectYaw, l_fObjectPitch, l_fObjectRoll, l_fObjectScaleX, l_fObjectScaleY, l_fObjectScaleZ);
            
            if (inst!= NULL)
            {
              inst->InitializeBoundingObject(l_Type);
              if(l_Type == NO_BOUNDING_OBJECT)
              {
                ((CBoundingBox*)inst->GetBoundingObject())->Init(l_minSize,l_maxSize,NO_BOUNDING_OBJECT);
              }
              else if(l_Type == BOUNDING_BOX)
              {
                ((CBoundingBox*)inst->GetBoundingObject())->Init(l_minSize,l_maxSize,BOUNDING_BOX);
              }
              else if(l_Type == BOUNDING_SPHERE)
              {
                ((CBoundingSphere*)inst->GetBoundingObject())->Init(l_minSize,l_maxSize,BOUNDING_SPHERE);
              }
						  else if(l_Type == BOUNDING_CAPSULE)
						  {
							  ((CBoundingCapsule*)inst->GetBoundingObject())->Init(l_minSize,l_maxSize,BOUNDING_CAPSULE);
						  }
              else if(l_Type == BOUNDING_MESH)
              {
                ((CBoundingMesh*)inst->GetBoundingObject())->Init(l_sObjectName,BOUNDING_MESH);
                std::vector<Vect3f> l_VBPX = CORE->GetStaticMeshManager()->GetResource(l_sObjectName)->GetVertexBufferPhysX();
                std::vector<uint32> l_IBPX = CORE->GetStaticMeshManager()->GetResource(l_sObjectName)->GetIndexBufferPhysX();
                ((CBoundingMesh*)inst->GetBoundingObject())->SetVertexBuffer(l_VBPX);
                ((CBoundingMesh*)inst->GetBoundingObject())->SetIndexBuffer(l_IBPX);
              }
              if(l_Visible)
              {
                CORE->GetRenderablePhysXManager()->AddRenderablePhysXObject(l_sObjectName, l_minSize, l_maxSize, l_Density, l_AngularDamping, l_Type, l_Visible);
              }
            }
          }
          else if (strcmp(linea.GetName(),"animated_model") == 0)
          {
            if(l_Visible)
            {
              inst = AddAnimatedModelInstance(l_sObjectCore, l_sObjectName, l_V3ObjectPos,l_fObjectYaw, l_fObjectPitch, l_fObjectRoll, l_fObjectScaleX, l_fObjectScaleY, l_fObjectScaleZ);
            }
          }	        
					
          if(inst == 0)
					{
            std::string msg_error = "RenderableObjectsManager::Load->Error al intentar crear la instancia '" + l_sObjectName + "', no se encuentra el core: '" + l_sObjectCore + "'";
						LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
						//throw CException(__FILE__, __LINE__, msg_error);
						//return false;
					}
				}//end if 
			}
    }
    else
    {
      return false;
    }
  }
	return l_bIsOk;
}

bool CRenderableObjectsManager::Reload()
{
  CleanUp();
  Destroy();
  return this->Load(m_FileName);
}

//Llamar al método update de cada uno de los elementos del vector
void CRenderableObjectsManager::Update(float ElapsedTime)
{
	std::vector<CRenderableObject*>::iterator l_Iter;
  
  for(l_Iter = m_RenderableObjects.begin();l_Iter != m_RenderableObjects.end();++l_Iter) 
  {
    //std::string a = (*l_Iter)->GetName();
    (*l_Iter)->Update(ElapsedTime);
  }
}

//Agregar un elemento al mapa
//Devolver el CORE del elemento a buscar

CRenderableAnimatedInstanceModel * CRenderableObjectsManager::GetInstanceAsAnimatedInstanceModel(const std::string &Name)
{
	return (CRenderableAnimatedInstanceModel *) GetResource(Name);
}

CRenderableObject * CRenderableObjectsManager::GetInstance(const std::string &Name) 
{
  return GetResource(Name);
}

//Llamar al método render de cada uno de los elementos del vector
void CRenderableObjectsManager::Render(CRenderManager *RM)
{
	std::vector<CRenderableObject*>::const_iterator l_Iter = m_RenderableObjects.begin();
  for(;l_Iter != m_RenderableObjects.end();++l_Iter) 
  {
    (*l_Iter)->Render(RM);
  }
}

//Añadir un CRenderableObject de tipo CMeshInstance que tiene una static mesh con su posición, rotación, etc.
CRenderableObject * CRenderableObjectsManager::AddMeshInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position, const float &Yaw, const float &Pitch, const float &Roll, const float &ScaleX, const float &ScaleY, const float &ScaleZ)
{
  //escribe parametros en la instancia de la malla
  CRenderableObject* inst = new CInstanceMesh(InstanceName, CoreMeshName);
  if (inst->IsOk())
  {
    inst->SetPosition(Position);
    inst->SetYaw(mathUtils::Deg2Rad(Yaw));
    inst->SetPitch(mathUtils::Deg2Rad(Pitch));
    inst->SetRoll(mathUtils::Deg2Rad(Roll));
    inst->SetScalar(ScaleX, ScaleY, ScaleZ);
    inst->SetVisible(true);

    //añade el recurso en el manager
    AddResource(InstanceName, inst);
  }
  else
  {
    CHECKED_DELETE(inst)
  }
  return inst;
}
//Añadir un CRenderableAnimatedInstanceModels con su posición, rotación, etc.
CRenderableObject * CRenderableObjectsManager::AddAnimatedModelInstance(const std::string &CoreName, const std::string &InstanceName, const Vect3f &Position, const float &Yaw, const float &Pitch, const float &Roll, const float &ScaleX, const float &ScaleY, const float &ScaleZ)
{
  //escribe parametros en la instancia de la malla
  CRenderableObject* inst = new CRenderableAnimatedInstanceModel(InstanceName, CoreName);
  
  if (inst->IsOk())
  {
    inst->SetPosition(Position);
    inst->SetYaw(mathUtils::Deg2Rad(Yaw));
    inst->SetPitch(mathUtils::Deg2Rad(Pitch));
    inst->SetRoll(mathUtils::Deg2Rad(Roll));
    inst->SetScalar(ScaleX, ScaleY, ScaleZ);
    inst->SetVisible(true);

    //añade el recurso en el manager
    AddResource(InstanceName, inst);
  }
  else
  {
    CHECKED_DELETE(inst)
  }

  return inst;
}

//Agregar un elemento al mapa
void CRenderableObjectsManager::AddResource(const std::string &Name, CRenderableObject *RenderableObject)
{
		if (m_Resources.find(Name)==m_Resources.end()) 
		{
			CMapManager::AddResource(Name, RenderableObject);
			m_RenderableObjects.push_back(RenderableObject);
		}
		else
		{
			//Guardar el mensaje de error en el LOGGER
			std::string msg_error = "RenderableObjectsManager::AddResource->Se intento añadir por segunda vez el recurso: " + Name;
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			//throw CException(__FILE__, __LINE__, msg_error);
		}
}
//En el mapa están todos ordenados por el nombre y en el vector están todos seguidos en un vector.
void CRenderableObjectsManager::CleanUp()
{
	//eliminar el vector
	m_RenderableObjects.clear();
	//eliminar el mapa
	Destroy();
}