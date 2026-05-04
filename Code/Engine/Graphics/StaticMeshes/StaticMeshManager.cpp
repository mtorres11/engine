#include "StaticMeshManager.h"
#include "XML/XMLTreeNode.h"
#include "Exceptions/Exception.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CStaticMeshManager::~CStaticMeshManager()
{
  Destroy();
}

///
///
///
bool CStaticMeshManager::Load(const std::string &FileName)
{
  m_TotalObjectsFaces = 0;
  bool l_bIsOk = true;
  m_FileName = FileName;
  CXMLTreeNode filexml;
  if (!filexml.LoadFile(m_FileName.c_str()))
  {
    //Guardar el mensaje de error en el LOGGER
		std::string msg_error = "CStaticMeshManager::Load->Error al intentar abrir el archivo: ";
		msg_error.append(m_FileName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
  }
  else
  {
    CXMLTreeNode cores = filexml["static_meshes"];
    if (cores.Exists())
    {
			if (!cores.IsComment())
			{
				int32 numNodes = cores.GetNumChildren();	

        CStaticMesh* l_static_mesh;
        std::string l_sCoreName;
        std::string l_sCorePath;	
				Vect3f* l_pV3fSelfIlluminationColor;
        Vect3f l_V3fSelfIlluminationColor;

        for(int i=0; i < numNodes; i++)
				{
					if (!cores(i).IsComment())
					{
						l_static_mesh = new CStaticMesh();
						//recibe parametros
		               
						l_sCoreName = cores(i).GetPszProperty("name");
						l_sCorePath = cores(i).GetPszProperty("filename");
            l_V3fSelfIlluminationColor = cores(i).GetVect3fProperty("selfIlluminationColor", v3fZERO);
            
            if (l_V3fSelfIlluminationColor != v3fZERO)
            {
              l_pV3fSelfIlluminationColor = new Vect3f(l_V3fSelfIlluminationColor);
              l_static_mesh->SetSelfIlluminationColor(l_pV3fSelfIlluminationColor);
            }

						if(l_static_mesh->Load(l_sCorePath))
						{
							AddResource(l_sCoreName, l_static_mesh);
              //TODO.. Comprobar que se recargan bien los objetos
              m_TotalObjectsFaces += l_static_mesh->GetNumFaces();
						}
            else
            {
              CHECKED_DELETE(l_static_mesh);
            }
					}
				}
			}
    }
    else
    {
      l_bIsOk = false;
    }
  }//else if (!filexml.LoadFile(m_FileName.c_str()))
  
  return l_bIsOk;
}

bool CStaticMeshManager::Reload()
{
  Destroy();
  return Load(m_FileName);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------