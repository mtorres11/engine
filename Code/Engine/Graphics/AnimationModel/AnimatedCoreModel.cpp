#include "AnimatedCoreModel.h"
#include "AnimatedInstanceModel.h"
#include "XML/XMLTreeNode.h"
#include "Exceptions/Exception.h"
#include "Vertex/VertexType.h"
#include "Vertex/IndexedVertex.h"
  //#include "cal3d/coremodel.h"
#include "Vertex/RenderableVertex.h"
#include "Math\MathTypes.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif

///<summary>
/// CAnimatedCoreModel:: Destructor
///</summary>
CAnimatedCoreModel::~CAnimatedCoreModel()
{
	CHECKED_DELETE(m_CalCoreModel);
	CHECKED_DELETE(m_CalHardwareModel);
	CHECKED_DELETE(m_RenderableVertexs);
	m_vInstancePosition.clear();
	m_vInstanceYaw.clear();
}

///<summary>
/// CAnimatedCoreModel:: Constructor
///</summary>
CAnimatedCoreModel::CAnimatedCoreModel(std::string _sName)
: m_Name("")
,	m_MeshFilename("")
,	m_SkeletonFilename("")
,	m_Path("")
, m_TextureFilenameList(0)
,	m_NumVtxs(0)
,	m_NumFaces(0)
, m_NumAnimations(0)
, m_RenderableVertexs(0)
, m_CalHardwareModel(0)
{
  m_CalCoreModel = new CalCoreModel(_sName);
}

///<summary>
/// CAnimatedCoreModel:: LoadMesh: Cargar la malla del modelo animado con los métodos correspondientes del Cal3D.
///</summary
///<returns name=""></returns>
bool CAnimatedCoreModel::LoadMesh()
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadMesh Cargando la Malla \"%s\"", m_MeshFilename.c_str());

  //Carga la malla en Cal3D
  if(m_CalCoreModel->loadCoreMesh(m_Path+m_MeshFilename)==-1)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadMesh Error Cal3D");
		return false;
	}
	else
	{
		return true;
	}
}

///<summary>
/// CAnimatedCoreModel:: LoadSkeleton: Cargar el esqueleto del modelo animado.
///</summary>
bool CAnimatedCoreModel::LoadSkeleton()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadSkeleton Cargando el Esqueleto \"%s\"", m_SkeletonFilename.c_str());
	
  //Carga el esqueleto en Cal3D
  if(!m_CalCoreModel->loadCoreSkeleton(m_Path+m_SkeletonFilename))
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadSkeleton Error Cal3D ");
		return false;
	}
	else
	{
		return true;
	}
}

///<summary>
/// CAnimatedCoreModel:: LoadAnimation: Cargar la animación del modelo según el FileName.
///</summary>
///<param name="Name">Nombre de la animacion</param>
///<param name="Filename">Nombre Archivo .CAF</param>
bool CAnimatedCoreModel::LoadAnimation(const std::string &Name, const std::string &Filename)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadAnimation Cargando la Animacion \"%s\"", Filename.c_str());
	
  //Carga la animación en Cal3D
  if(m_CalCoreModel->loadCoreAnimation(m_Path+Filename,Name)==-1)
	{
    LOGGER->AddNewLog(ELL_INFORMATION, "CAnimatedCoreModel::LoadAnimation Error Cal3D ");
		return false;
	}
	else
	{
		return true;
	}
}

///<summary>
/// CAnimatedCoreModel:: GetCoreModel: Devuelve el modelo core del Cal3D.
///</summary>
///<returns name="m_CalCoreModel">Devuelve el Core del modelo</returns>
CalCoreModel * CAnimatedCoreModel::GetCoreModel( )
{
	return m_CalCoreModel;
}

///<summary>
/// CAnimatedCoreModel:: GetTextureName: cargar el esqueleto del modelo animado.
///</summary>
///<param name="id">Indice de la textura de la core</param>
///<returns name="m_TextureFilenameList"></returns>
const std::string & CAnimatedCoreModel::GetTextureName(size_t id)
{
	return m_TextureFilenameList[id];
}

///<summary>
/// CAnimatedCoreModel:: Load: Cargar un fichero xml y todos los elementos de este modelo a partir del path pasado como parámetro.
///</summary>
///<param name="Path">Ubicación del fichero xml</param>
bool CAnimatedCoreModel::Load(const std::string &Path)
{
	bool l_bIsOk = true;
  m_NumAnimations = 0;
  //Asigna el path
	m_Path = Path;
	std::string l_FileName=m_Path+"actor.xml";

  //Intento de carga del archivo xml
  CXMLTreeNode filexml;
  if (!filexml.LoadFile(l_FileName.c_str()))
  {
		//Guardar el mensaje de error en el LOGGER
    std::string msg_error = "CAnimatedCoreModel::Load->Error al intentar leer el XML: " + m_Path;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
    l_bIsOk = false;
  }
  else
  {
		//<actor name="bot" radius_bs="2.0" pos_bs="0.0 1.0 0.0" effect="cal3d" technique="cal3dTechnique">
		//	<skeleton filename="bot.csf"/>
		//	<mesh filename="bot.CMF"/>
		//	<texture filename="rocketl.jpg"/>
		//	<texture filename="ranger.jpg"/>
		//	<texture filename="ranger_h.jpg"/>
		//	<animation name="idle" filename="idle.CAF"/>
		//	<animation name="run" filename="run.CAF"/>
		//	<animation name="hit" filename="hit.CAF"/>
		//	<animation name="die" filename="die.CAF"/>
		//	<animation name="shoot" filename="shoot.CAF"/>
		//</actor>

    //Intento de carga de nodos actor
		CXMLTreeNode animatedModels = filexml["actor"];
    if (animatedModels.Exists())
    {
			if (!animatedModels.IsComment())
			{
				//recogida de datos del nodo
				m_Name = animatedModels.GetPszProperty("name"); 
				float l_sModelRadio = animatedModels.GetFloatProperty("radius_bs");        
				Vect3f l_sModelPos = animatedModels.GetVect3fProperty("pos_bs", Vect3f(0.0, 0.0, 0.0));        
				std::string l_sModelEffect = animatedModels.GetPszProperty("effect");
				m_Technique= animatedModels.GetPszProperty("technique");
				
				//Numero de cores de actores 
				int32 numNodes = animatedModels.GetNumChildren();	
				for(int i=0; i < numNodes; i++)
				{
	        
					//Intento de carga del esqueleto correspondiente
					CXMLTreeNode linea = animatedModels(i);
					if(strcmp(linea.GetName(),"skeleton") == 0)
					{
						m_SkeletonFilename = linea.GetPszProperty("filename");
						this->LoadSkeleton();
					}
	        
					//Intento de carga de la mallas correspondientes (pueden haber varias)
					else if(strcmp(linea.GetName(),"mesh") == 0)
					{
						m_MeshFilename = linea.GetPszProperty("filename");
						this->LoadMesh();
					}
	        
					//Intento de carga de texturas correspondientes (pueden haber varias)
					else if(strcmp(linea.GetName(),"texture") == 0)
					{
						//recogida del nombre de los ficheros de texturas 
						std::string l_sFile = linea.GetPszProperty("filename");
						std::string pathfilename = Path + l_sFile; 

						//Añadir el nombre de textura en el vector  m_TextureFilenameList
						m_TextureFilenameList.push_back(pathfilename);
					}
	        
					//Intento de carga de animaciones correspondientes (pueden haber varias)
					else if(strcmp(linea.GetName(),"animation") == 0)
					{
						std::string l_sModelName = linea.GetPszProperty("name"); 
						std::string l_sFileName = linea.GetPszProperty("filename");
						if(LoadAnimation(l_sModelName, l_sFileName))
              m_NumAnimations++;
					}
				}//end for
			}
		}//end if
	}//end else
	LoadVertexBuffer();
	return l_bIsOk;
}

///<summary>
/// CAnimatedCoreModel:: Load: Cargar un fichero xml y todas las posiciones que se quieran cargar a partir del path pasado como parámetro.
///</summary>
///<param name="Path">Ubicación del fichero xml</param>
bool CAnimatedCoreModel::LoadInstances(const std::string &Path)
{
	bool l_bIsOk = true;

  //Intento de carga del archivo xml
  CXMLTreeNode filexml;
  if (!filexml.LoadFile(Path.c_str()))
  {
		//Guardar el mensaje de error en el LOGGER
    std::string msg_error = "CAnimatedCoreModel::LoadInstances->Error al intentar leer el XML: " + Path;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
    l_bIsOk = false;
  }
  else
  {
		/*<animated_model name="zombiecoliseo1" >
				<instance pos="23.3592 8.4533 -0.0840303" yaw="88.5052" pitch="0.0" roll="0.0" />
				<instance pos="9.01954 1.70311 10.2429" yaw="151.1616" pitch="0.0" roll="0.0" />
		  </animated_model>*/

    //Intento de carga de nodos instances

		CXMLTreeNode instanceModels = filexml["animated_model"];
    if (instanceModels.Exists())
    {
			if (!instanceModels.IsComment())
			{
				//recogida de datos del nodo
				std::string l_zombiename= instanceModels.GetPszProperty("name");    
				
				int32 numNodes = instanceModels.GetNumChildren();	
				for(int i=0; i < numNodes; i++)
				{
					CXMLTreeNode linea = instanceModels(i);
					if(strcmp(linea.GetName(),"instance") == 0)
					{
						Vect3f l_instancePos = linea.GetVect3fProperty("pos", Vect3f(0.0, 0.0, 0.0));
						m_vInstancePosition.push_back(l_instancePos);
						float l_fObjectYaw = linea.GetFloatProperty("yaw", 0.f, false);
						m_vInstanceYaw.push_back(l_fObjectYaw);
						 
					}

				}//end for
			}
		}//end if
	}//end else
	return l_bIsOk;
}

///<summary>
/// CAnimatedCoreModel:: GetCalHardwareModel: Devuelve el modelo del hardware
///</summary>
///<returns name="m_CalHardwareModel"></returns>
CalHardwareModel * CAnimatedCoreModel::GetCalHardwareModel() const
{
	return m_CalHardwareModel;
}

///<summary>
/// CAnimatedCoreModel:: LoadVertexBuffer: creamos el modelo de hardware, un VertexBuffer y un IndexBuffer en memoria RAM,
/// donde escribiremos los vértices y los índices de nuestro modelo.
///</summary>
///<param name="Model">Modelo que le pasamos</param>
bool CAnimatedCoreModel::LoadVertexBuffer	()
{	
	m_CalHardwareModel = new CalHardwareModel(m_CalCoreModel);

	m_NumVtxs=0;
	m_NumFaces=0;
	for(int i=0;i<m_CalCoreModel->getCoreMeshCount();++i)
	{
		CalCoreMesh *l_CoreMesh=m_CalCoreModel->getCoreMesh(i);
		for(int j=0;j<l_CoreMesh->getCoreSubmeshCount();++j)
		{
			m_NumVtxs+=l_CoreMesh->getCoreSubmesh(j)->getVertexCount();
			m_NumFaces+=l_CoreMesh->getCoreSubmesh(j)->getFaceCount();
		}
	}
	
	//Cogemos el doble de vértices necesarios porque al crear el model de hardware puede necesitar más vértices que el modelo por software
	CAL3D_HW_VERTEX *l_Vtxs=new CAL3D_HW_VERTEX[m_NumVtxs*2]; 
	unsigned short *l_Idxs=new unsigned short[m_NumFaces*3];

	m_CalHardwareModel->setVertexBuffer((char*) l_Vtxs, sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setWeightBuffer(((char*)l_Vtxs) + 12, sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setMatrixIndexBuffer(((char*)l_Vtxs) + 28, sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setNormalBuffer(((char*)l_Vtxs)+44, sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setTextureCoordNum(1);
	m_CalHardwareModel->setTextureCoordBuffer(0,((char*)l_Vtxs)+56, sizeof(CAL3D_HW_VERTEX));
	m_CalHardwareModel->setIndexBuffer(l_Idxs);
	m_CalHardwareModel->load( 0, 0, MAXBONES);
	m_NumVtxs=m_CalHardwareModel->getTotalVertexCount();
	//CalcTangentsAndBinormals(l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces*3, sizeof(CAL3D_HW_VERTEX),0, 44, 60, 76, 92);
	CRenderManager * RM = CORE->GetRenderManager();

	m_RenderableVertexs=new CIndexedVertex<CAL3D_HW_VERTEX>(RM, l_Vtxs, l_Idxs, m_NumVtxs, m_NumFaces*3);
	

	delete []l_Vtxs;
	delete []l_Idxs;

	return 0;
}

///<summary>
/// CAnimatedCoreModel:: GetRenderableVertexs: 
///</summary>
///<returns name="m_RenderableVertexs"></returns>
CRenderableVertexs * CAnimatedCoreModel::GetRenderableVertexs	() const
{
	return m_RenderableVertexs;
}
