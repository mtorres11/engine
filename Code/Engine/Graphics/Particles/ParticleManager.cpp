#include "ParticleManager.h"
#include "ParticleEmitter.h"
#include "Vertex/VertexType.h"
#include "Logger/Logger.h"
#include "Texture/TextureManager.h"
#include "Texture/Texture.h"
#include "XML/XMLTreeNode.h"
#include <string>

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CParticleManager:: Init : Inicialización.
///</summary>
///<param name="void"></param>
///<returns name="result">Booleano que indica si la inicialización se ha realizado con éxito.</returns>
bool CParticleManager::Init (const std::string & fileName)
{
	m_bIsOk = true;
	return m_bIsOk;
}

///<summary>
/// CParticleManager:: Done : Finalize data.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CParticleManager::Done ()
{
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

///<summary>
/// CParticleManager:: Release : Liberación de memoria.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CParticleManager::Release()
{
	//vector de ParticleEmitter
	std::vector<CParticleEmitter*>::iterator l_Iter = m_vParticleEmitter.begin();
  for(;l_Iter != m_vParticleEmitter.end();++l_Iter) 
  {
    CHECKED_DELETE(*l_Iter);
  }
	m_vParticleEmitter.clear();
	m_stInfo.vColor.clear();
	m_stInfo.vDir.clear();
	m_stInfo.vInstance.clear();
	m_vInfo.clear();
	m_vColor.clear();
	m_vTimeColor.clear();
	m_vDir.clear();
	m_vTimeDir.clear();
	m_vSize.clear();
	m_vTimeSize.clear();
	m_vVelocity.clear();
	m_vTimeVel.clear();
	m_vAngle.clear();
	m_vTimeAngle.clear();

  m_vInstance.clear();
  m_vPEmitterByInstance.clear();
}

///<summary>
/// CParticleManager:: LoadXML : Lectura de Particles.xml.
///</summary>
///<param name="fileName">Ruta del XML.</param>
///<returns name="result">Booleano que indica si la lectura se ha realizado con éxito.</returns>
bool CParticleManager::LoadXML (const std::string & fileName)
{
	bool l_bIsOk = true;
	CXMLTreeNode filexml;
	if (!filexml.LoadFile(fileName.c_str()))
	{
		LOGGER->AddNewLog(ELL_ERROR, "ParticleManager::LoadXML ---Error al cargar el XML---");
		l_bIsOk = false;
	}
	else
	{
		CXMLTreeNode l_pManager = filexml["ParticleManager"];
		if (l_pManager.Exists())
		{
			//<ParticleEmitter name="pEmitter1">
			//  <color color1="0.0 0.0 1.0 1.0" color2="0.0 1.0 0.0 1.0" time="0.0">
			//    <time t="2.0" col1="0.0 1.0 0.0 1.0" col2="0.0 1.0 0.0 1.0"/>
			//  </color>
			CXMLTreeNode l_pEmitters = l_pManager["ParticleEmitters"];
			int numNodes = l_pEmitters.GetNumChildren(); 
			for (int i = 0; i<numNodes; ++i) 
			{
				CXMLTreeNode l_pEmitter = l_pEmitters(i)["ParticleEmitter"];
				if (l_pEmitter.Exists() && !l_pEmitter.IsComment())
				{
					m_stInfo.sName = l_pEmitter.GetPszProperty("name");
					CXMLTreeNode l_pColor = l_pEmitter["color"];

					if (l_pColor.Exists() && !l_pColor.IsComment())
					{
						m_stColor.color1 = l_pColor.GetVect4fProperty("color1", Vect4f(0.f, 0.f, 0.f, 0.f));
						m_stColor.color2 = l_pColor.GetVect4fProperty("color2", Vect4f(0.f, 0.f, 0.f, 0.f));
						m_stColor.time	 = l_pColor.GetFloatProperty("time");

						m_stInfo.vColor.push_back(m_stColor);
					}
					int num = l_pColor.GetNumChildren();
					for (int j=0; j<num; ++j)
					{
						CXMLTreeNode l_pTime = l_pColor(j)["time"];
						if (l_pTime.Exists() && !l_pTime.IsComment())
						{
							m_stColor.time   = l_pTime.GetFloatProperty("t");
							m_stColor.color1 = l_pTime.GetVect4fProperty("col1", Vect4f(0.f, 0.f, 0.f, 0.f));
							m_stColor.color2 = l_pTime.GetVect4fProperty("col2", Vect4f(0.f, 0.f, 0.f, 0.f));
						}
						m_stInfo.vColor.push_back(m_stColor);
					}//end for color
					//<direction d1="0.0 5.0 0.0" d2="0.0 6.0 0.0" time="0.0">
					//	<time t="2.0" dir1="1.0 5.0 0.0" dir2="2.0 9.0 0.0"/>
					//</direction>
					CXMLTreeNode l_pDir = l_pEmitter["direction"];
					if (l_pDir.Exists() && !l_pDir.IsComment())
					{
						m_stDir.dir1 = l_pDir.GetVect3fProperty("d1", Vect3f(0.f, 0.f, 0.f)); 
						m_stDir.dir2 = l_pDir.GetVect3fProperty("d2", Vect3f(0.f, 0.f, 0.f)); 
						m_stDir.time = l_pDir.GetFloatProperty("time");

						m_stInfo.vDir.push_back(m_stDir);
					}
					int cont = l_pDir.GetNumChildren();
					for (int k=0; k<cont; ++k)
					{
						CXMLTreeNode l_pTime = l_pDir(k)["time"];
						if (l_pTime.Exists() && !l_pTime.IsComment())
						{
							m_stDir.time = l_pTime.GetFloatProperty("t");
							m_stDir.dir1 = l_pTime.GetVect3fProperty("dir1", Vect3f(0.f, 0.f, 0.f));
							m_stDir.dir2 = l_pTime.GetVect3fProperty("dir2", Vect3f(0.f, 0.f, 0.f));
						}
						m_stInfo.vDir.push_back(m_stDir);
					}//end for direction
					//<size mins="0.1" maxs="0.1" time="0.0">
					//	<time t="0.5" siz1="0.3" siz2="0.3"/>
					//</size>
					CXMLTreeNode l_pSize = l_pEmitter["size"];
					if (l_pSize.Exists() && !l_pSize.IsComment())
					{
						m_stSize.sizeMin = l_pSize.GetFloatProperty("mins"); 
						m_stSize.sizeMax = l_pSize.GetFloatProperty("maxs"); 
						m_stSize.time = l_pSize.GetFloatProperty("time");

						m_stInfo.vSize.push_back(m_stSize);
					}
					int size = l_pSize.GetNumChildren();
					for (int m=0; m<size; ++m)
					{
						CXMLTreeNode l_pTime = l_pSize(m)["time"];
						if (l_pTime.Exists() && !l_pTime.IsComment())
						{
							m_stSize.time = l_pTime.GetFloatProperty("t");
							m_stSize.sizeMin = l_pTime.GetFloatProperty("siz1");
							m_stSize.sizeMax = l_pTime.GetFloatProperty("siz2");
						}
						m_stInfo.vSize.push_back(m_stSize);
					}//end for size
					//<life_time life1="1.0" life2="5.0"/>
					CXMLTreeNode l_pLife = l_pEmitter["life_time"];
					if (l_pLife.Exists() && !l_pLife.IsComment())
					{
						m_stInfo.fLifeMin = l_pLife.GetFloatProperty("life1");
						m_stInfo.fLifeMax = l_pLife.GetFloatProperty("life2");
					}
					//<emit_rate er1="20.0" er2="30.0"/>
					CXMLTreeNode l_pEmitRate = l_pEmitter["emit_rate"];
					if (l_pEmitRate.Exists() && !l_pEmitRate.IsComment())
					{
						m_stInfo.fEmitRate1 = l_pEmitRate.GetFloatProperty("er1");
						m_stInfo.fEmitRate2 = l_pEmitRate.GetFloatProperty("er2");
					}
					//<gravity gr="true"/>
					CXMLTreeNode l_pGravity = l_pEmitter["gravity"];
					if(l_pGravity.Exists() && !l_pGravity.IsComment())
					{
						m_stInfo.bGravity = l_pGravity.GetBoolProperty("gr");
					}
					//<velocity vel="1.0 1.0 1.0" time="0.0">
					//	<time t="1.0" v="0.1 0.4 -0.2"/>
					//</velocity>
					CXMLTreeNode l_pVelocity = l_pEmitter["velocity"];
					if(l_pVelocity.Exists() && !l_pVelocity.IsComment())
					{
						m_stVelocity.velocity = l_pVelocity.GetVect3fProperty("vel", (0.0f, 0.0f, 0.0f)); 
						m_stVelocity.time = l_pVelocity.GetFloatProperty("time");

						m_stInfo.vVelocity.push_back(m_stVelocity);
					}
					int tam = l_pVelocity.GetNumChildren();
					for (int j=0; j<tam; ++j)
					{
						CXMLTreeNode l_pTime = l_pVelocity(j)["time"];
						if (l_pTime.Exists() && !l_pTime.IsComment())
						{
							m_stVelocity.time = l_pTime.GetFloatProperty("t");
							m_stVelocity.velocity = l_pTime.GetVect3fProperty("v",  (0.0f, 0.0f, 0.0f));
						}
						m_stInfo.vVelocity.push_back(m_stVelocity);
					}//end for velocity
					//<angle ang="0.5" time="0.0">
					//	<time t="0.6" a="-0.5"/>
					//</angle>
					CXMLTreeNode l_pAngle = l_pEmitter["angle"];
					if(l_pAngle.Exists() && !l_pAngle.IsComment())
					{
						m_stAngle.angle = l_pAngle.GetFloatProperty("ang"); 
						m_stAngle.time = l_pAngle.GetFloatProperty("time");

						m_stInfo.vAngle.push_back(m_stAngle);
					}
					int ang = l_pAngle.GetNumChildren();
					for (int j=0; j<ang; ++j)
					{
						CXMLTreeNode l_pTime = l_pAngle(j)["time"];
						if (l_pTime.Exists() && !l_pTime.IsComment())
						{
							m_stAngle.time = l_pTime.GetFloatProperty("t");
							m_stAngle.angle = l_pTime.GetFloatProperty("a");
						}
						m_stInfo.vAngle.push_back(m_stAngle);
					}//end for angle
					//<texture tex="Data/Textures/Punto_Alfa.dds" />
					CXMLTreeNode l_pText = l_pEmitter["texture"];
					if (l_pText.Exists() && !l_pText.IsComment())
					{
						m_stInfo.pTexture = CORE->GetTextureManager()->GetTexture(l_pText.GetPszProperty("tex", ""));
					}
					m_vInfo.push_back(m_stInfo);
					m_stInfo.vColor.clear();
					m_stInfo.vDir.clear();
					m_stInfo.vSize.clear();
					m_stInfo.vVelocity.clear();
					m_stInfo.vAngle.clear();
				}//end ParticleEmitter	
			}
			//<Instance id="pEmitter1" pos1="0.0 0.0 0.0" pos2="8.0 0.0 0.0"/>
			CXMLTreeNode l_pInstance = l_pManager["Instances"];
			int numNodesInst = l_pInstance.GetNumChildren(); 
			for (int i = 0; i<numNodesInst; i++) 
			{
				CXMLTreeNode l_pInst = l_pInstance(i)["Instance"];
				if (l_pInst.Exists() && !l_pInst.IsComment())
				{	
					m_stInstance.id = l_pInst.GetPszProperty("id",0);
					m_stInstance.vPos1 = l_pInst.GetVect3fProperty("pos1", Vect3f(0.f,0.f,0.f));
					m_stInstance.vPos2 = l_pInst.GetVect3fProperty("pos2", Vect3f(0.f,0.f,0.f));
					m_stInfo.vInstance.push_back(m_stInstance);
				}
			}
			for (unsigned int c=0; c<m_vInfo.size();c++)
			{
				int cont = -1;
				for (unsigned int i = 0; i<m_stInfo.vInstance.size(); i++) 
				{
					if (m_stInfo.vInstance[i].id == m_vInfo[c].sName)
					{
						cont++;
						m_vInfo[c].vInstance.push_back(m_stInfo.vInstance[i]);
						SetProperties(c,cont);
					}
				}
			}
		}
	}
	LOGGER->AddNewLog(ELL_INFORMATION, "ParticleManager::LoadXML ---XML cargado correctamente---");
	SetPEmitterByInstance();
	return l_bIsOk;
}

///<summary>
/// CParticleManager:: Render : Dibujado de las partículas almacenadas en el vector.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CParticleManager::Render()
{
	std::vector<CParticleEmitter*>::const_iterator l_Iter = m_vParticleEmitter.begin();
  for(;l_Iter != m_vParticleEmitter.end();++l_Iter) 
  {
    (*l_Iter)->Render();
  }
}

///<summary>
/// CParticleManager:: Update : Actualiza cada partícula viva y crea partículas nuevas cuando toque.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name="void"></returns>
void CParticleManager::Update(float elapsedTime)
{
	std::vector<CParticleEmitter*>::const_iterator l_Iter = m_vParticleEmitter.begin();
  for(;l_Iter != m_vParticleEmitter.end();++l_Iter) 
  {
    (*l_Iter)->Update(elapsedTime);
  }
}

///<summary>
/// CParticleManager:: Init : Lectura de Particles.xml.
///</summary>
///<param name="rm" name2=fileName>El rm para crear el VB y la ruta de la textura que queremos usar.</param>
///<returns name="void"></returns>
void CParticleManager::Init(CRenderManager* rm)
{
	std::vector<CParticleEmitter*>::const_iterator l_Iter = m_vParticleEmitter.begin();
  for(;l_Iter != m_vParticleEmitter.end();++l_Iter) 
  {
    (*l_Iter)->Init(rm);
  }
  m_bIsOk = true;
}

void CParticleManager::SetProperties(int cont, int i)
{
	CParticleEmitter* l_pE = new CParticleEmitter();
	l_pE->SetMinLife(m_vInfo[cont].fLifeMin);
	l_pE->SetMaxLife(m_vInfo[cont].fLifeMax);
	l_pE->SetTexture(m_vInfo[cont].pTexture);
	l_pE->SetMaxEmitRate(m_vInfo[cont].fEmitRate2);
	l_pE->SetMinEmitRate(m_vInfo[cont].fEmitRate1);
	l_pE->SetGravity(m_vInfo[cont].bGravity);
	l_pE->SetName(m_vInfo[cont].sName);

	//calcular posiciones del emisor
	l_pE->SetPos1(CalculatePos1(cont, i));
	l_pE->SetPos2(CalculatePos2(cont, i));

	//calcular color, dirección, tamaño, velocidad y ángulo de rotación
	CalculateColor(cont);
	CalculateDir(cont);
	CalculateSize(cont);
	CalculateVelocity(cont);
	CalculateAngle(cont);

	//fijar color, dirección, tamaño, velocidad y ángulo de rotación
	l_pE->SetColorTime(m_vTimeColor);
	l_pE->SetColorV(m_vColor);
	l_pE->SetDirTime(m_vTimeDir);
	l_pE->SetDirV(m_vDir);
	l_pE->SetSizeTime(m_vTimeSize);
	l_pE->SetSizeV(m_vSize);
	l_pE->SetVelocityTime(m_vTimeVel);
	l_pE->SetVelocityV(m_vVelocity);
	l_pE->SetAngleTime(m_vTimeAngle);
	l_pE->SetAngleV(m_vAngle);

	//borrar memoria
	m_vInfo[cont].vColor.clear();
	m_vInfo[cont].vDir.clear();
	m_vInfo[cont].vSize.clear();
	m_vInfo[cont].vVelocity.clear();
	m_vInfo[cont].vAngle.clear();

	m_vParticleEmitter.push_back(l_pE);
}

D3DXVECTOR3 CParticleManager::CalculatePos1(int cont, int i)
{
	//calcular posición 1
	float l_posX = m_vInfo[cont].vInstance[i].vPos1.x;
	float l_posY = m_vInfo[cont].vInstance[i].vPos1.y;
	float l_posZ = m_vInfo[cont].vInstance[i].vPos1.z;
	D3DXVECTOR3 l_pos1 = D3DXVECTOR3(l_posX, l_posY, l_posZ);
	return l_pos1;
}

D3DXVECTOR3 CParticleManager::CalculatePos2(int cont, int i)
{
	//calcular posición 2
	float l_pos2X = m_vInfo[cont].vInstance[i].vPos2.x;
	float l_pos2Y = m_vInfo[cont].vInstance[i].vPos2.y;
	float l_pos2Z = m_vInfo[cont].vInstance[i].vPos2.z;
	D3DXVECTOR3 l_pos2 = D3DXVECTOR3(l_pos2X, l_pos2Y, l_pos2Z);
	return l_pos2;
}

void CParticleManager::CalculateColor(int cont)
{
	unsigned int l_iSizeColor = m_vInfo[cont].vColor.size();
	for (unsigned int c=0; c<l_iSizeColor; c++)
	{
		//Time
 		SCOLOR l_col = m_vInfo[cont].vColor[c];
		float l_timeColor  = l_col.time;
		m_vTimeColor.push_back(l_timeColor);
		//Color
		Vect4f l_color1		 = l_col.color1;
		Vect4f l_color2		 = l_col.color2;
		float fRandR			 = RandomNumber(l_color1.x, l_color2.x);
		float fRandG			 = RandomNumber(l_color1.y, l_color2.y);
		float fRandB			 = RandomNumber(l_color1.z, l_color2.z);
		float fRandA			 = RandomNumber(l_color1.w, l_color2.w);
		m_vColor.push_back(D3DXCOLOR(fRandR, fRandG, fRandB, fRandA));
	}
}

void CParticleManager::CalculateDir(int cont)
{
	unsigned int l_iSizeDir = m_vInfo[cont].vDir.size();
	for (unsigned int j=0; j<l_iSizeDir; j++)
	{
		//Time
		SDIR l_dir = m_vInfo[cont].vDir[j];
		float l_timeDir    = l_dir.time;
		m_vTimeDir.push_back(l_timeDir);
		//Dir
		Vect3f l_dir1      = l_dir.dir1;
		Vect3f l_dir2      = l_dir.dir2;
		float fRandX			 = RandomNumber(l_dir1.x, l_dir2.x);
		float fRandY			 = RandomNumber(l_dir1.y, l_dir2.y);
		float fRandZ		 	 = RandomNumber(l_dir1.z, l_dir2.z);
		m_vDir.push_back(D3DXVECTOR3(fRandX, fRandY, fRandZ));
	}
}

void CParticleManager::CalculateSize(int cont)
{
	unsigned int l_iSizeSize = m_vInfo[cont].vSize.size();
	for (unsigned int j=0; j<l_iSizeSize; j++)
	{
		//Time
		SSIZE l_size = m_vInfo[cont].vSize[j];
		float l_timeSize    = l_size.time;
		m_vTimeSize.push_back(l_timeSize);
		//Size
		float l_sizMin      = l_size.sizeMin;
		float l_sizMax      = l_size.sizeMax;
		float fRand 			  = RandomNumber(l_sizMin, l_sizMax);
		m_vSize.push_back(fRand);
	}
}

void CParticleManager::CalculateVelocity(int cont)
{
	unsigned l_iSizeVelocity = m_vInfo[cont].vVelocity.size();
	for (unsigned int j=0; j<l_iSizeVelocity; j++)
	{
		SVELOCITY l_vel = m_vInfo[cont].vVelocity[j];
		float l_timeVel = l_vel.time;
		Vect3f l_velocity = l_vel.velocity;
		m_vTimeVel.push_back(l_timeVel);
		m_vVelocity.push_back(D3DXVECTOR3(l_velocity.x, l_velocity.y, l_velocity.z));
	}
}

void CParticleManager::CalculateAngle(int cont)
{
	unsigned l_iSizeAngle = m_vInfo[cont].vAngle.size();
	for (unsigned int j=0; j<l_iSizeAngle; j++)
	{
		SANGLE l_ang = m_vInfo[cont].vAngle[j];
		float timeAngle = l_ang.time;
		float l_angle = l_ang.angle;
		m_vTimeAngle.push_back(timeAngle);
		m_vAngle.push_back(l_angle);
	}
}

float CParticleManager::RandomNumber(float fMin, float fMax)
{
  float l_fNumber;
  if(fMin> fMax)
  {
    float l_fAux = fMax;
    fMax = fMin;
    fMin=l_fAux;
  }
  l_fNumber=((rand()/static_cast<float>(RAND_MAX)) * (fMax - fMin) + fMin);

  return l_fNumber;
}

void CParticleManager::SetPEmitterByInstance()
{
	//guardamos el nombre del emitter y el número de instancias q tiene
	int l_iCont = 0;
	int l_iTotalInst = 0;
	m_stNumInstEmitter.numInstDispo = 0;
	m_stNumInstEmitter.numInstTotal = 0;
	std::vector<CParticleEmitter*>::const_iterator l_Iter = m_vParticleEmitter.begin();
  for(;l_Iter != m_vParticleEmitter.end();++l_Iter) 
  {
		std::string l_sName = (*l_Iter)->GetName();
		std::map<std::string, SINSTEMITTER>::iterator l_IterInstance = m_vPEmitterByInstance.find(l_sName);
		if (l_IterInstance == m_vPEmitterByInstance.end())
		{
			l_iCont = 0;
			m_vPEmitterByInstance[l_sName].numInit = l_iTotalInst;
		}
		m_vPEmitterByInstance[l_sName].numInstTotal = ++l_iCont;
		m_vPEmitterByInstance[l_sName].numInstDispo = l_iCont;
		l_iTotalInst++;
	}
}

CParticleEmitter * CParticleManager::GetPEmitterByInstance(std::string name)
{
	//buscamos el emitter por nombre
	std::map<std::string, SINSTEMITTER>::iterator l_IterName = m_vPEmitterByInstance.find(name);
	if (l_IterName != m_vPEmitterByInstance.end())
	{
		if ((*l_IterName).second.numInstDispo == 0)
		{
			(*l_IterName).second.numInstDispo = (*l_IterName).second.numInstTotal;
		}
		int l_iInst = (*l_IterName).second.numInit + ((*l_IterName).second.numInstTotal - (*l_IterName).second.numInstDispo);
		(*l_IterName).second.numInstDispo--;
		return m_vParticleEmitter[l_iInst];
	}
	return NULL;
}