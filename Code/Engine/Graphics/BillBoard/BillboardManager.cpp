#include "BillboardManager.h"
#include "Vertex/VertexType.h"
#include "Logger/Logger.h"
#include "Texture/TextureManager.h"
#include "Texture/Texture.h"
#include "XML/XMLTreeNode.h"
#include <string>
#include "BillboardAnimation.h"
#include "RenderManager.h"
#include "Utils/BaseUtils.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CBillboardManager:: Init : Inicialización.
///</summary>
///<param name="void"></param>
///<returns name="result">Booleano que indica si la inicialización se ha realizado con éxito.</returns>
bool CBillboardManager::Init (const std::string & fileName)
{
	m_bIsOk = true;
	return m_bIsOk;
}

///<summary>
/// CBillboardManager:: Done : Finalize data.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CBillboardManager::Done ()
{
  if (IsOk())
  {
		Release();
    m_bIsOk = false;
  }
}

///<summary>
/// CBillboardManager:: Release : Liberación de memoria.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CBillboardManager::Release()
{
	//vector de BillboardAnimation
	std::vector<CBillboardAnimation*>::iterator l_Iter = m_vBillboardAnimation.begin();
  for(;l_Iter != m_vBillboardAnimation.end();++l_Iter) 
  {
    CHECKED_DELETE(*l_Iter);
  }

  m_vBillboardAnimation.clear();
	m_vBAnimationByInstance.clear();
	m_stInfo.vInstance.clear();
	m_stInfo.vTextures.clear();
	
  m_vInfo.clear();
	m_vD3DTextures.clear();
	m_vText.clear();
}

///<summary>
/// CBillboardManager:: LoadXML : Lectura de Particles.xml.
///</summary>
///<param name="fileName">Ruta del XML.</param>
///<returns name="result">Booleano que indica si la lectura se ha realizado con éxito.</returns>
bool CBillboardManager::LoadXML (const std::string & fileName)
{
	bool l_bIsOk = true;
	CXMLTreeNode filexml;
	if (!filexml.LoadFile(fileName.c_str()))
	{
		LOGGER->AddNewLog(ELL_ERROR, "ParticleAnimation::LoadXML ---Error al cargar el XML---");
		l_bIsOk = false;
	}
	else
	{
		CXMLTreeNode l_pBillboardAnimation = filexml["BillboardAnimation"];
		if (l_pBillboardAnimation.Exists())
		{
			CXMLTreeNode l_pBillboard = l_pBillboardAnimation["Billboard"];
			int numNodes = l_pBillboard.GetNumChildren(); 
			for (int i = 0; i<numNodes; ++i) 
			{
				//<Animation name="pSangre1">
				CXMLTreeNode l_pAnimation = l_pBillboard(i)["Animation"];
				if (l_pAnimation.Exists() && !l_pAnimation.IsComment())
				{
					m_stInfo.sName = l_pAnimation.GetPszProperty("name");

					//  <color col="1.0 1.0 1.0 1.0"/>
					//  <size siz="0.5"/>
					CXMLTreeNode l_pColor = l_pAnimation["color"];
					if (l_pColor.Exists() && !l_pColor.IsComment())
					{
						m_stInfo.vColor = l_pColor.GetVect4fProperty("col", Vect4f(0.f, 0.f, 0.f, 0.f));
					}
					CXMLTreeNode l_pSize = l_pAnimation["size"];
					if (l_pSize.Exists() && !l_pSize.IsComment())
					{
						m_stInfo.fSize = l_pSize.GetFloatProperty("siz");
					}
					//<numText num="7"/>
					CXMLTreeNode l_pNumText = l_pAnimation["numText"];
					if (l_pNumText.Exists() && !l_pNumText.IsComment())
					{
						m_stInfo.iNumText = l_pNumText.GetIntProperty("num");
					}
					//<timePerImage time ="0.9"/>
					CXMLTreeNode l_pTimePerImage = l_pAnimation["timePerImage"];
					if (l_pTimePerImage.Exists() && !l_pTimePerImage.IsComment())
					{
						m_stInfo.fTimePerImage = l_pTimePerImage.GetFloatProperty("time");
					}
					//<loop bucle="false"/>
					CXMLTreeNode l_pLoop = l_pAnimation["loop"];
					if (l_pLoop.Exists() && !l_pLoop.IsComment())
					{
						m_stInfo.bLoop = l_pLoop.GetBoolProperty("bucle");
					}
					//<texture tex1="Data/Textures/Punto_Alfa.dds" tex2="" tex3=""/>
					CXMLTreeNode l_pText = l_pAnimation["texture"];
					if (l_pText.Exists() && !l_pText.IsComment())
					{
						std::string l_sNameText = "";
						std::string l_sName = "tex";
						for (int i=1; i<(m_stInfo.iNumText+1); i++)
						{
							baseUtils::FormatSrting(l_sNameText, "%s%d", l_sName.c_str (), i);
							m_stInfo.vTextures.push_back(CORE->GetTextureManager()->GetTexture(l_pText.GetPszProperty(l_sNameText.c_str(), "")));
						}
					}//end for texture
					m_vInfo.push_back(m_stInfo);
					m_stInfo.vTextures.clear();
				}//end ParticleEmitter	
			}
			//<Instance id="pSangre1" pos="0.0 0.0 0.0"/>
			CXMLTreeNode l_pInstance = l_pBillboardAnimation["Instances"];
			int numNodesInst = l_pInstance.GetNumChildren(); 
			for (int i = 0; i<numNodesInst; i++) 
			{
				CXMLTreeNode l_pInst = l_pInstance(i)["Instance"];
				if (l_pInst.Exists() && !l_pInst.IsComment())
				{	
					m_stInstance.id = l_pInst.GetPszProperty("id",0);
					m_stInstance.vPos = l_pInst.GetVect3fProperty("pos", Vect3f(0.f,0.f,0.f));
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
	LOGGER->AddNewLog(ELL_INFORMATION, "ParticleAnimation::LoadXML ---XML cargado correctamente---");
	SetBAnimationByInstance();
	return l_bIsOk;
}

void CBillboardManager::SetProperties(int cont, int i)
{
	CBillboardAnimation * l_pBillboardAnimation = new CBillboardAnimation();
	
	//color
	Vect4f l_color = m_vInfo[cont].vColor;
	D3DXCOLOR l_vD3DColor = (D3DXCOLOR(l_color.x, l_color.y, l_color.z, l_color.w));
	l_pBillboardAnimation->SetColor(l_vD3DColor);

	//name, bucle, numTextures, size, pos, timePerImage
	l_pBillboardAnimation->SetName(m_vInfo[cont].sName);
	l_pBillboardAnimation->SetLoop(m_vInfo[cont].bLoop);
	l_pBillboardAnimation->SetNumTextures(m_vInfo[cont].iNumText);
	l_pBillboardAnimation->SetSize(m_vInfo[cont].fSize, m_vInfo[cont].fSize);
	l_pBillboardAnimation->SetPos(m_vInfo[cont].vInstance[i].vPos);
	l_pBillboardAnimation->SetTimePerImage(m_vInfo[cont].fTimePerImage);

	//textures
	m_vText = m_vInfo[cont].vTextures;
	SetVectTextures(m_vText);
	l_pBillboardAnimation->SetVTextures(m_vD3DTextures);

	//guardamos en el vector de billboards
	m_vBillboardAnimation.push_back(l_pBillboardAnimation);

	//eliminamos los datos del vector de texturas
	m_vD3DTextures.clear();
}

///<summary>
/// CBillboardManager:: Render : Dibujado de los billboard animados del vector.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CBillboardManager::Render()
{
	std::vector<CBillboardAnimation*>::const_iterator l_Iter = m_vBillboardAnimation.begin();
  for(int i=0;l_Iter != m_vBillboardAnimation.end();++l_Iter, i++) 
  {
		if(!m_vBillboardAnimation[i]->IsEnd())
		{
			(*l_Iter)->Render(m_vBillboardAnimation[i]->GetTexturesVect());
		}
  }
}

void CBillboardManager::RenderById(int id)
{
	if(!m_vBillboardAnimation[id]->IsEnd())
	{
		GetBillboardAnimation(id)->Render(m_vBillboardAnimation[id]->GetTexturesVect());
	}
}

///<summary>
/// CBillboardManager:: Update : Actualiza cada billboard vivo y crea billboards animados nuevos cuando toque.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name="void"></returns>
void CBillboardManager::Update(float elapsedTime)
{
	std::vector<CBillboardAnimation*>::const_iterator l_Iter = m_vBillboardAnimation.begin();
  for(;l_Iter != m_vBillboardAnimation.end();++l_Iter) 
  {
    (*l_Iter)->Update(elapsedTime);
  }
}

///<summary>
/// CBillboardManager:: Init : Lectura de ParticlesAnimation.xml.
///</summary>
///<param name="rm" name2=fileName>El rm para crear el VB</param>
///<returns name="void"></returns>
void CBillboardManager::Init(CRenderManager* rm)
{
	std::vector<CBillboardAnimation*>::const_iterator l_Iter = m_vBillboardAnimation.begin();
  for(;l_Iter != m_vBillboardAnimation.end();++l_Iter) 
  {
    (*l_Iter)->Init(rm);
  }
  m_bIsOk = true;
}

void CBillboardManager::SetVectTextures (std::vector<CTexture*> vText)
{
	//pasamos las texturas del XML a un vector de LPDIRECT3DTEXTURE9
	for (uint32 i=0; i<vText.size(); i++)
	{
		CTexture * myText = vText[i];
		LPDIRECT3DTEXTURE9 m_tex = myText->GetD3DTexture();
		m_vD3DTextures.push_back(m_tex);
	}
}

void CBillboardManager::SetBAnimationByInstance()
{
	//guardamos el nombre del emitter y el número de instancias q tiene
	int l_iCont = 0;
	int l_iTotalInst = 0;
	m_stNumInst.numInstDispo = 0;
	m_stNumInst.numInstTotal = 0;
	std::vector<CBillboardAnimation*>::const_iterator l_Iter = m_vBillboardAnimation.begin();
  for(;l_Iter != m_vBillboardAnimation.end();++l_Iter) 
  {
		std::string l_sName = (*l_Iter)->GetName();
		std::map<std::string, SNUMINSTANCIAS>::iterator l_IterInstance = m_vBAnimationByInstance.find(l_sName);
		if (l_IterInstance == m_vBAnimationByInstance.end())
		{
			l_iCont = 0;
			m_vBAnimationByInstance[l_sName].numInit = l_iTotalInst;
		}
		m_vBAnimationByInstance[l_sName].numInstTotal = ++l_iCont;
		m_vBAnimationByInstance[l_sName].numInstDispo = l_iCont;
		l_iTotalInst++;
	}
}

CBillboardAnimation * CBillboardManager::GetBAnimationByInstance(std::string name)
{
	//buscamos el billboard por nombre
	std::map<std::string, SNUMINSTANCIAS>::iterator l_IterName = m_vBAnimationByInstance.find(name);
	if (l_IterName != m_vBAnimationByInstance.end())
	{
		if ((*l_IterName).second.numInstDispo == 0)
		{
			(*l_IterName).second.numInstDispo = (*l_IterName).second.numInstTotal;
		}
		int l_iInst = (*l_IterName).second.numInit + ((*l_IterName).second.numInstTotal - (*l_IterName).second.numInstDispo);
		(*l_IterName).second.numInstDispo--;
		return m_vBillboardAnimation[l_iInst];
	}
	return NULL;
}