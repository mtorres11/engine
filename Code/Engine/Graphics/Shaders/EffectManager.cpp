#include "EffectManager.h"
#include "Math/Matrix44.h"
#include <string>
#include "xml/XMLTreeNode.h"
#include "Effect.h"
#include "Exceptions/Exception.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CEffectManager:: Constructor.
///</summary>
CEffectManager::CEffectManager()
:	m_WorldMatrix (m44fZERO)
, m_ProjectionMatrix (m44fZERO)
, m_ViewMatrix (m44fZERO)
, m_ViewProjectionMatrix (m44fZERO)
, m_LightViewMatrix (m44fZERO)
, m_ShadowProjectionMatrix (m44fZERO)
, m_FileName ("")
, m_CameraEye(0.0f, 0.0f, 0.0f)
, m_StaticMeshTechnique (0)
, m_AnimatedModelTechnique(0)
//, m_Effects(NULL)
//, m_DefaultTechniqueEffectMap (NULL)
{
}

///<summary>
/// CEffectManager:: Destructor.
///</summary>
CEffectManager::~CEffectManager()
{
	CHECKED_DELETE(m_StaticMeshTechnique);
	CHECKED_DELETE(m_AnimatedModelTechnique);
  m_Effects.Destroy();
  CleanUp();
}

///<summary>
/// CEffectManager:: GetWorldMatrix : Devuelve la matriz m_WorldMatrix.
///</summary>
///<returns name="m_WorldMatrix">Variable que devuelve la matriz del mundo de tipo Mat44f</returns>
const Mat44f& CEffectManager::GetWorldMatrix() const
{
	return m_WorldMatrix;
}

///<summary>
/// CEffectManager:: GetProjectionMatrix : Devuelve la matriz m_ProjectionMatrix.
///</summary>
///<returns name="m_ProjectionMatrix">Variable que devuelve la proyección de la matriz de tipo Mat44f</returns>
const Mat44f& CEffectManager::GetProjectionMatrix() const
{
	return m_ProjectionMatrix;
}

///<summary>
/// CEffectManager:: GetViewMatrix : Devuelve la matriz m_ViewMatrix.
///</summary>
///<returns name="m_ViewMatrix">Variable que devuelve la View de la matriz de tipo Mat44f</returns>
const Mat44f& CEffectManager::GetViewMatrix() const
{
	return m_ViewMatrix;
}

///<summary>
/// CEffectManager:: GetViewProjectionMatrix : Devuelve la matriz m_ViewProjectionMatrix.
///</summary>
///<returns name="m_ViewProjectionMatrix">Variable que devuelve la proyección de view de la matriz de tipo Mat44f</returns>
const Mat44f& CEffectManager::GetViewProjectionMatrix()
{
	return m_ViewProjectionMatrix;
}

///<summary>
/// CEffectManager:: GetCameraEye : Devuelve el vector m_CameraEye.
///</summary>
///<returns name="m_CameraEye">Variable que devuelve la cámara de tipo Vect3f</returns>
const Vect3f& CEffectManager::GetCameraEye()
{
	return m_CameraEye;
}

///<summary>
/// CEffectManager:: GetLightViewMatrix : Devuelve la matriz m_LightViewMatrix.
///</summary>
///<returns name="m_LightViewMatrix">Variable que devuelve la luz de tipo Mat44f</returns>
const Mat44f& CEffectManager::GetLightViewMatrix() const
{
	return m_LightViewMatrix;
}

///<summary>
/// CEffectManager:: GetShadowProjectionMatrix : Devuelve la matriz m_ShadowProjectionMatrix.
///</summary>
///<returns name="m_ShadowProjectionMatrix">Variable que devuelve ShadowProjection de la matriz de tipo Mat44f</returns>
const Mat44f& CEffectManager::GetShadowProjectionMatrix()
{
	return m_ShadowProjectionMatrix;
}

///<summary>
/// CEffectManager:: SetWorldMatrix : Guarda el valor de la matriz que le pasamos en m_WorldMatrix.
///</summary>
///<param name="&Matrix">Variable que almacena la matriz de tipo Mat44f</param>
void CEffectManager::SetWorldMatrix(const Mat44f &Matrix)
{
	m_WorldMatrix = Matrix;
}

///<summary>
/// CEffectManager:: SetProjectionMatrix : Guarda el valor de la matriz que le pasamos en m_ProjectionMatrix.
///</summary>
///<param name="&Matrix">Variable que almacena ProjectionMatrix de tipo Mat44f</param>
void CEffectManager::SetProjectionMatrix(const Mat44f &Matrix)
{
	m_ProjectionMatrix = Matrix;
}

///<summary>
/// CEffectManager:: SetViewMatrix : Guarda el valor de la matriz que le pasamos en m_ViewMatrix.
///</summary>
///<param name="&Matrix">Variable que almacena ViewMatrix de tipo Mat44f</param>
void CEffectManager::SetViewMatrix(const Mat44f &Matrix)
{
	m_ViewMatrix = Matrix;
}

///<summary>
/// CEffectManager:: SetViewProjectionMatrix : Guarda el valor de la matriz que le pasamos en m_ViewProjectionMatrix.
///</summary>
///<param name="&Matrix">Variable que almacena ViewProjectionMatrix de tipo Mat44f</param>
void CEffectManager::SetViewProjectionMatrix(const Mat44f &ViewProjectionMatrix)
{
	m_ViewProjectionMatrix = ViewProjectionMatrix;
}

///<summary>
/// CEffectManager:: SetLightViewMatrix : Guarda el valor de la matriz que le pasamos en m_LightViewMatrix.
///</summary>
///<param name="&Matrix">Variable que almacena LigthViewMatrix de tipo Mat44f</param>
void CEffectManager::SetLightViewMatrix(const Mat44f &Matrix)
{
	m_LightViewMatrix = Matrix;
}

///<summary>
/// CEffectManager:: SetShadowProjectionMatrix : Guarda el valor de la matriz que le pasamos en m_ShadowProjectionMatrix.
///</summary>
///<param name="&Matrix">Variable que almacena ShadowProjection de tipo Mat44f</param>
void CEffectManager::SetShadowProjectionMatrix(const Mat44f &Matrix)
{
	m_ShadowProjectionMatrix = Matrix;
}

///<summary>
/// CEffectManager:: SetCameraEye : Guarda el valor del vector que le pasamos en m_CameraEye.
///</summary>
///<param name="&CameraEye">Variable que almacena CameraEye de tipo Vect3f</param>
void CEffectManager::SetCameraEye(const Vect3f &CameraEye)
{
	m_CameraEye = CameraEye;
}

///<summary>
/// CEffectManager:: SetStaticMeshTechnique : Guarda el valor de m_StaticMeshTechnique.
///</summary>
///<param name="StaticMeshTechnique">Variable que almacena StaticMeshTechnique de tipo CEffectTechnique*</param>
void CEffectManager::SetStaticMeshTechnique(CEffectTechnique *StaticMeshTechnique)
{
	m_StaticMeshTechnique = StaticMeshTechnique;
}

///<summary>
/// CEffectManager:: SetAnimatedModelTechnique : Guarda el valor de m_AnimatedModelTechnique.
///</summary>
///<param name="AnimatedModelTechnique">Variable que almacena AnimatedModelTechnique de tipo CEffectTechnique*</param>
void CEffectManager::SetAnimatedModelTechnique(CEffectTechnique *AnimatedModelTechnique)
{
	m_AnimatedModelTechnique = AnimatedModelTechnique;
}

///<summary>
/// CEffectManager:: CleanUp : Limpia el mapa de CEffectTechnique.
///</summary>
void CEffectManager::CleanUp()
{
	//limpiar el mapa
	Destroy();
}

///<summary>
/// CEffectManager:: GetStaticMeshTechnique : Devuelve el puntero m_StaticMeshTechnique.
///</summary>
///<returns name="m_StaticMeshTechnique">Variable que devuelve StaticMeshTechnique de tipo CEffectTechnique</returns>
CEffectTechnique * CEffectManager::GetStaticMeshTechnique() const
{
	return m_StaticMeshTechnique;
}

///<summary>
/// CEffectManager:: GetAnimatedModelTechnique : Devuelve el puntero m_AnimatedModelTechnique.
///</summary>
///<returns name="m_AnimatedModelTechnique">Variable que devuelve AnimatedModelTechnique de tipo CEffectTechnique</returns>
CEffectTechnique * CEffectManager::GetAnimatedModelTechnique() const
{
	return m_AnimatedModelTechnique;
}

///<summary>
/// CEffectManager:: Reload : Limpia el mapa y carga de nuevo el fichero XML.
///</summary>
void CEffectManager::Reload()
{
	//limpiar el mapa
	//Destroy();
	//cargar el fichero .xml
	m_Effects.Destroy();
  CleanUp();
  Load(m_FileName);
}

///<summary>
/// CEffectManager:: GetEffect : Devuelve un puntero según el nombre del efecto que le pasamos.
///</summary>
///<param name"Name">Nombre del efecto</param>
///<returns name="m_Effects">Variable que devuelve el efecto</returns>
CEffect * CEffectManager::GetEffect(const std::string &Name)
{
	return m_Effects.GetResource(Name);;
}

///<summary>
/// CEffectManager:: GetEffectTechnique : Devuelve la technique según el nombre del efecto.
///</summary>
///<param name"Name">Nombre de la técnica</param>
///<returns name="technique">Devuelve la técnica</returns>
CEffectTechnique * CEffectManager::GetEffectTechnique(const std::string &Name)
{
 return GetResource(Name);
}

///<summary>
/// CEffectManager:: GetTechniqueEffectNameByVertexDefault : Devuelve la technique por defecto.
///</summary>
///<param name"VertexType">Tipo de vértice</param>
///<returns name="techniqueDefault">Técnica por defecto</returns>
std::string CEffectManager::GetTechniqueEffectNameByVertexDefault(unsigned short VertexType)
{
	TDefaultTechniqueEffectMap::iterator iter = m_DefaultTechniqueEffectMap.find(VertexType);
	if(iter != m_DefaultTechniqueEffectMap.end())
	{
		return iter->second;
	} 
	return "";
}

///<summary>
/// CEffectManager:: GetMaxLights : Devuelve el número máximo de luces (4).
///</summary>
///<returns name="MAX_LIGHTS_BY_SHADER">Número máximo de luces por shader</returns>
size_t CEffectManager::GetMaxLights() const
{
	return MAX_LIGHTS_BY_SHADER;
}

//---------------------------------------------------------------------------------
//<?xml version="1.0" encoding="ISO-8859-1"?>
//<effects>
//	<default_technique vertex_type="19" technique="NormalTextureVertexTechnique"/>
//
//	<effect name="DefaultEffect" file="data/effects/ DefaultEffect.fx"/>
//	<effect name="Cal3DEffect" file="data/effects/cal3d.fx"/>
//
//	<technique name=" NormalTextureVertexTechnique " effect=" DefaultEffect " 
//						use_world_matrix="true" 
//						use_world_view_projection_matrix="true" 
//						use_lights="true" num_of_lights="4" 
//						use_camera_position="true" 
//						use_projection_matrix="true"
//						use_world_view_matrix="true" 
//						use_view_to_light_projection_matrix="true"/>
//</effects>
//----------------------------------------------------------------------------------
///<summary>
/// CEffectManager:: Load : Carga el fichero Effects.xml.
///</summary>
///<param name="FileName">Ubicación del fichero</param>
///<returns name="l_bIsOk">Variable que indica si se ha leído bien el fichero XML</returns>
bool CEffectManager::Load(const std::string &FileName)
{
	bool l_bIsOk = true;
	m_FileName = FileName;
	CXMLTreeNode parser;

	if (!parser.LoadFile(m_FileName.c_str()))
	{
		//Guardar el mensaje de error en el LOGGER
		std::string msg_error = "EffectManager::Load->Error al intentar leer el archivo de efectos: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
		l_bIsOk = false;
	}

	LOGGER->AddNewLog(ELL_INFORMATION, "EffectManager::Load-> Parseando fichero de efectos: %s", m_FileName.c_str());
	
	CXMLTreeNode  renders = parser["effects"];
	if (renders.Exists())
	{
    //inicializacion de parametros
    m_DefaultTechniqueEffectMap.clear();


    if (!renders.IsComment())
		{
			int numNodes = renders.GetNumChildren();

      for (int i = 0; i < numNodes; ++i)
			{
				if (!renders(i).IsComment())
				{
					//Verificamos si existe TECNICA POR DEFECTO
					//<default_technique vertex_type="19" technique="NormalTextureVertexTechnique"/>
					CXMLTreeNode techDefecto = renders(i)["default_technique"];
					if (techDefecto.Exists())
					{
						if (!techDefecto.IsComment())
						{
							int l_iType = techDefecto.GetIntProperty("vertex_type");
							std::string l_sTecnique = techDefecto.GetPszProperty("technique");
              //Comprobar que no exista en el mapa
							TDefaultTechniqueEffectMap::iterator iterador = m_DefaultTechniqueEffectMap.find(l_iType);
							if (iterador == m_DefaultTechniqueEffectMap.end()) 
							{
								m_DefaultTechniqueEffectMap[l_iType]=l_sTecnique;
							}
							else
							{
								std::string msg_warning = "EffectManager::Load->El elemento ya existe dentro del mapa: " + l_iType;
								LOGGER->AddNewLog(ELL_WARNING, msg_warning.c_str());
								//throw CException(__FILE__, __LINE__, msg_error);
								l_bIsOk = false;
							}
						}
						continue;
					}//end if

					//Verificamos si existen EFECTOS
					//<effect name="DefaultEffect" file="data/effects/DefaultEffect.fx"/>
					CXMLTreeNode efecto = renders(i)["effect"];
					if (efecto.Exists())
					{
						if (!efecto.IsComment())
						{
							std::string l_sName = efecto.GetPszProperty("name");
							std::string l_sFile = efecto.GetPszProperty("file");
			       
							CEffect* l_effect=m_Effects.GetResource(l_sName);
							if(l_effect == NULL)
							{
								l_effect=new CEffect;
								if(l_effect->Load(l_sFile))
								{
									m_Effects.AddResource(l_sName,l_effect);
								}
							}
							else
								l_effect->Reload();
						}
						continue;
					}

					//Verificamos si existen TECNICAS
					//<technique name=" NormalTextureVertexTechnique " effect=" DefaultEffect " use_world_matrix="true" 
					//use_world_view_projection_matrix="true" use_lights="true" num_of_lights="4" use_camera_position="true" 
					//use_projection_matrix="true" use_world_view_matrix="true" use_view_to_light_projection_matrix="true"/>
					CXMLTreeNode technique = renders(i)["technique"];
					if (technique.Exists())
					{
						if (!technique.IsComment())
						{
							std::string l_sName = technique.GetPszProperty("name");
              CEffectTechnique *l_EffectTechnique = this->GetResource(l_sName);

              if(l_EffectTechnique)
              {
                l_EffectTechnique->Init(technique);
							  l_EffectTechnique->Refresh();
              }
              else
              {
                l_EffectTechnique = new CEffectTechnique();
                l_EffectTechnique->Init(technique);
							  l_EffectTechnique->Refresh();
                AddResource(l_sName, l_EffectTechnique);
              }
							continue;
						}
					}//end if technique
				}//end if IsComment()
			}//end for
		}
	}
  else
	{
		//Guardar el mensaje de error en el LOGGER
		std::string msg_error = "EffectManager::Load->Error al intentar leer el <effects> en archivo de efectos: " + m_FileName;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
		l_bIsOk = false;
	}
	return l_bIsOk;
}

///<summary>
/// CEffectManager:: ActivateCamera : Inicializa las variables m_ViewMatrix, m_ProjectionMatrix y m_CameraEye.
///</summary>
///<param name="ViewMatrix">Variable que almacena ViewMatrix de tipo Mat44f</param>
///<param name="ProjectionMatrix">Variable que almacena ProjectionMatrix de tipo Mat44f</param>
///<param name="CameraEye">Variable que almacena CameraEye de tipo Vect3f</param>
void CEffectManager::ActivateCamera(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix,	const Vect3f &CameraEye)
{
	m_ViewMatrix=ViewMatrix;
	m_ProjectionMatrix=ProjectionMatrix;
	m_CameraEye=CameraEye;
}

