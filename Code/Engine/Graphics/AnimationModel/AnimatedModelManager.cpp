#include "AnimatedModelManager.h"
#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"

#include "XML/XMLTreeNode.h"
#include "Exceptions/Exception.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CAnimatedModelManager::CAnimatedModelManager(){
  
}

CAnimatedModelManager::~CAnimatedModelManager(){
  Destroy();
}

//Cargar un CAnimatedCoreModel en caso de no existir y devolver su dirección de memoria. 
//Utilizaremos el mapa de la clase CmapManager para guardar la estructura.
CAnimatedCoreModel * CAnimatedModelManager::GetCore(const std::string &Name, const std::string &Path)
{
	
	CAnimatedCoreModel* l_AnimatedCoreModel= NULL;
	l_AnimatedCoreModel = GetResource(Name);
	if(l_AnimatedCoreModel==NULL)
	{
		if(Path != "")
		{
			l_AnimatedCoreModel = new CAnimatedCoreModel(Name);

			//crear el core
			l_AnimatedCoreModel->Load(Path);

			//meter core
			if(l_AnimatedCoreModel != NULL)
			{
				AddResource(Name,l_AnimatedCoreModel);
			}
		}
	}

  return l_AnimatedCoreModel; 
	
}

//Devolverá una instancia de tipo CAnimatedInstanceModel según el nombre de la core cargada previamente.
CAnimatedInstanceModel * CAnimatedModelManager::GetInstance(const std::string &Name)
{
	CAnimatedCoreModel *l_AnimatedCoreModel=GetCore(Name,"");
	if(l_AnimatedCoreModel!=NULL)
	{
		CAnimatedInstanceModel *m_pAnimatedInstanceModel = new CAnimatedInstanceModel();
		m_pAnimatedInstanceModel->Initialize(l_AnimatedCoreModel);
		m_pAnimatedInstanceModel->InitD3D(CORE->GetRenderManager());
		return m_pAnimatedInstanceModel;
	}
	return NULL;
}

bool CAnimatedModelManager::Load (const std::string &FileName)
{
  bool l_bIsOk = true;
  m_TotalNumFaces = 0;

  CXMLTreeNode filexml;
  if (!filexml.LoadFile(FileName.c_str()))
  {
		//Guardar el mensaje de error en el LOGGER
    std::string msg_error = "CAnimatedModelManager::Load->Error al intentar leer el XML: " + FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
    l_bIsOk = false;
  }
  else
  {
		//<?xml version="1.0" encoding="ISO-8859-1"?>
    //<animated_models>
    //    <animated_model name="bot" path="data/AnimatedModels/bot/"/>
    //</animated_models>

		CXMLTreeNode animatedModels = filexml["animated_models"];
    if (animatedModels.Exists())
    {
			int32 numNodes = animatedModels.GetNumChildren();	
		
      CXMLTreeNode linea;
      std::string l_name;
      std::string l_path;
			bool l_instancing;

			for(int i = 0; i < numNodes; i++)
      {
				if (!animatedModels(i).IsComment())
				{
					linea = animatedModels(i);
					l_name = linea.GetPszProperty("name");
					l_path = linea.GetPszProperty("path");
					l_instancing = linea.GetBoolProperty("instancing");
					
          // cargo el modelo animado solo si no ha sido cargado previamente (Andres)
          if (GetResource(l_name) == NULL)
          {
            CAnimatedCoreModel * l_AnimatedCoreModel = new CAnimatedCoreModel(l_name);
            if (l_AnimatedCoreModel->Load(l_path))
            {
              AddResource(l_name, l_AnimatedCoreModel);
              //TODO..Comprobar que al recargar se cargan los modelos correctos asi como su numero de poligonos
              m_TotalNumFaces += l_AnimatedCoreModel->GetNumFaces();

							if(l_instancing)
							{
								std::string l_instancespath=l_path+"Instances.xml";
								l_AnimatedCoreModel->LoadInstances(l_instancespath);

							}

            }
            else
            {
              CHECKED_DELETE(l_AnimatedCoreModel)
            }
          }
          else
          {
            std::string msg_error = "CAnimatedModelManager::Load->Se ha intentado cargar 2 veces el AnimatedModel: " + l_name;
		        LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
          }
				}
			}
		}
	}

  return l_bIsOk;
}