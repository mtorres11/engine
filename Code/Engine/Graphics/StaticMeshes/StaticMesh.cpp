#include "StaticMesh.h"
#include "RenderManager.h"
#include "Texture/Texture.h"
#include "Vertex/VertexType.h"
#include "Vertex/IndexedVertex.h"
#include "Core.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Texture/TextureManager.h"
#include "Shaders/EffectManager.h"
#include "Shaders/EffectTechnique.h"
#include "Exceptions/Exception.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

using namespace std;

#define _HEADER 0xFF77
#define _FOOTER 0x77FF

CStaticMesh::CStaticMesh()
: m_TechniqueName("")
, m_FileName("")
, m_NumVertexs(0)
, m_NumFaces(0)
, m_Textures(NULL)
, m_Techniques(NULL)
, m_RVs(NULL)
, m_VertexBufferPhysX(0)
, m_IndexBufferPhysX(0)
, m_pV3fSelfIlluminationColor(0)
{
}

CStaticMesh::~CStaticMesh()
{
  CHECKED_DELETE(m_pV3fSelfIlluminationColor)
  m_Techniques.clear();
  m_Textures.clear();
  Release();
}
bool CStaticMesh::Load(const std::string &FileName)
{
  m_FileName = FileName;
  ifstream l_File(m_FileName.c_str(), ios::in|ios::binary);

  if(l_File.is_open())
  {
    unsigned short l_Header = 0;
    unsigned short l_CantMateriales = 0;
    //unsigned short l_CantIndices = 0;
    unsigned short l_VertexType = 0;
    unsigned short l_CountTextures = 0;
    unsigned short l_CantVertices = 0;
    unsigned short l_IndexCount = 0;
    unsigned short l_Footer = 0;
    std::vector<unsigned short> l_vVertexTypes;

    //Lectura del Header
    l_File.read((char *)&l_Header, sizeof(unsigned short));
    if(l_Header != _HEADER)
    {
      LOGGER->AddNewLog(ELL_WARNING, "CStaticMesh::Load Encabezado incorrecto al leer el archivo '%'",m_FileName);
      l_File.close(); 
      return false;
    }
    //Cantidad de materiales
    l_File.read((char *)&l_CantMateriales, sizeof(unsigned short));

    std::string l_TechniqueName;
    
    //ciclo para cada material
    for(int j = 0; j < l_CantMateriales; j++)
    {
      //Lectura del Vertex Type
      l_File.read((char *)&l_VertexType, sizeof(unsigned short));

      //se verifica si tiene alpha_test
      if (l_VertexType & ALPHA_TEST)
      {
        l_VertexType = l_VertexType ^ ALPHA_TEST;
        l_TechniqueName=CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault(l_VertexType);
        
        if (m_pV3fSelfIlluminationColor != NULL)
        {
          m_Techniques.push_back(CORE->GetEffectManager()->GetEffectTechnique(l_TechniqueName + "_SI_AT"));
        }
        else
        {
          m_Techniques.push_back(CORE->GetEffectManager()->GetEffectTechnique(l_TechniqueName + "_AT"));
        }
      }
      else if(l_VertexType & ALPHA_BLEND)
      {
        l_VertexType = l_VertexType ^ ALPHA_BLEND;
        l_TechniqueName=CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault(l_VertexType);
        
        if (m_pV3fSelfIlluminationColor != NULL)
        {
          m_Techniques.push_back(CORE->GetEffectManager()->GetEffectTechnique(l_TechniqueName + "_SI_AB"));
        }
        else
        {
          m_Techniques.push_back(CORE->GetEffectManager()->GetEffectTechnique(l_TechniqueName + "_AB"));
        }
      }
      else // en los otros casos en los que no hay transparencias
      {
        l_TechniqueName = CORE->GetEffectManager()->GetTechniqueEffectNameByVertexDefault(l_VertexType);
        
        if (m_pV3fSelfIlluminationColor != NULL)
        {
          m_Techniques.push_back(CORE->GetEffectManager()->GetEffectTechnique(l_TechniqueName + "_SI"));
        }
        else
        {
          m_Techniques.push_back(CORE->GetEffectManager()->GetEffectTechnique(l_TechniqueName));
        }
      }
      
      l_vVertexTypes.push_back(l_VertexType);

      //Cantidad de texturas
      l_File.read((char *)&l_CountTextures, sizeof(unsigned short));
      //Lectura de materiales. Se leen "l_CountTextures" materiales.
      m_Textures.push_back(std::vector<CTexture *>());
      for(int i = 0; i < l_CountTextures; i++)
      {
        //Path Size
        unsigned short l_pathSize = 0;
        l_File.read((char *)&l_pathSize, sizeof(unsigned short));
        //Reading Path
        char * l_pathMaterial = new char[l_pathSize+1];
        int temp = sizeof(l_pathMaterial);
        l_File.read(l_pathMaterial, sizeof(char) * (l_pathSize+1));

        //Carga la textura y la introduce en su vector

        CTexture *l_Texture=CORE->GetTextureManager()->GetTexture(l_pathMaterial);
        m_Textures[j].push_back(l_Texture);
        CHECKED_DELETE_ARRAY(l_pathMaterial);
      }
    }//fin de ciclo para cada material

    //l_File.read((char *)&l_CantIndices, sizeof(unsigned short));
    for(int j = 0; j < l_CantMateriales; j++)
    {
      l_File.read((char *)&l_CantVertices, sizeof(unsigned short));
      int l_Size=0;

      l_VertexType = l_vVertexTypes.at(j);

      if(l_VertexType == TNORMAL_TEXTURE1_VERTEX::GetVertexType())
      {
        l_Size=sizeof(TNORMAL_TEXTURE1_VERTEX);
      }
      else if(l_VertexType == TNORMAL_TEXTURE2_VERTEX::GetVertexType())
      {
        l_Size=sizeof(TNORMAL_TEXTURE2_VERTEX);
      }
      else if(l_VertexType == TNORMAL_COLORED_VERTEX::GetVertexType())
      {
        l_Size=sizeof(TNORMAL_COLORED_VERTEX);
      }
      else if(l_VertexType == TNORMAL_TANGENT_BINORMAL_TEXTURED::GetVertexType())
      {
        l_Size=sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED);
      }
      else if(l_VertexType == TNORMAL_TANGENT_BINORMAL_TEXTURED2::GetVertexType())
      {
        l_Size=sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED2);
      }

      char *l_Vtxs=new char[l_Size*l_CantVertices];
      l_File.read((char *)&l_Vtxs[0],l_Size*l_CantVertices);

      l_File.read((char *)&l_IndexCount, sizeof(unsigned short));

      unsigned short *l_Indxs = new unsigned short[l_IndexCount];

      l_File.read((char *)&l_Indxs[0],sizeof(unsigned short)*l_IndexCount);

      CRenderableVertexs * l_RV;
      //-----------------------------------
      if(l_VertexType == TNORMAL_TEXTURE1_VERTEX::GetVertexType())
      {
        l_RV = new CIndexedVertex<TNORMAL_TEXTURE1_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Indxs, l_CantVertices, l_IndexCount);
      }
      else if(l_VertexType == TNORMAL_TEXTURE2_VERTEX::GetVertexType())
      {
        l_RV = new CIndexedVertex<TNORMAL_TEXTURE2_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Indxs, l_CantVertices, l_IndexCount);
      }
      else if(l_VertexType == TNORMAL_COLORED_VERTEX::GetVertexType())
      {
        l_RV = new CIndexedVertex<TNORMAL_COLORED_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Indxs, l_CantVertices, l_IndexCount);
      }
      else if(l_VertexType == TNORMAL_TANGENT_BINORMAL_TEXTURED::GetVertexType())
      {
        //calcular en el vertex buffer las tangenciales y binormales
        CalcTangentsAndBinormals(l_Vtxs, l_Indxs, l_CantVertices, l_IndexCount, sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED), 0, 12, 28, 44, 60);	
        TNORMAL_TANGENT_BINORMAL_TEXTURED *l_HackVtxs=(TNORMAL_TANGENT_BINORMAL_TEXTURED *)&l_Vtxs[0];
        Vect3f l_Min, l_Max;
        for(int b=0;b<l_CantVertices;++b)
        {
          if(b==0)
          {
             l_Min=Vect3f(l_HackVtxs[b].x,l_HackVtxs[b].y,l_HackVtxs[b].z);
             l_Max=Vect3f(l_HackVtxs[b].x,l_HackVtxs[b].y,l_HackVtxs[b].z);
          }
          if(l_HackVtxs[b].x<l_Min.x)
            l_Min.x=l_HackVtxs[b].x;
          if(l_HackVtxs[b].y<l_Min.y)
            l_Min.y=l_HackVtxs[b].y;
          if(l_HackVtxs[b].z<l_Min.z)
            l_Min.z=l_HackVtxs[b].z;

          if(l_HackVtxs[b].x>l_Max.x)
            l_Max.x=l_HackVtxs[b].x;
          if(l_HackVtxs[b].y>l_Max.y)
            l_Max.y=l_HackVtxs[b].y;
          if(l_HackVtxs[b].z>l_Max.z)
            l_Max.z=l_HackVtxs[b].z;
        }
        l_RV = new CIndexedVertex<TNORMAL_TANGENT_BINORMAL_TEXTURED>(CORE->GetRenderManager(), l_Vtxs, l_Indxs, l_CantVertices, l_IndexCount);
      }
      else if(l_VertexType == TNORMAL_TANGENT_BINORMAL_TEXTURED2::GetVertexType())
      {
        //calcular en el vertex buffer las tangenciales y binormales
        CalcTangentsAndBinormals(l_Vtxs, l_Indxs, l_CantVertices, l_IndexCount, sizeof(TNORMAL_TANGENT_BINORMAL_TEXTURED2), 0, 12, 28, 44, 60);	
        l_RV = new CIndexedVertex<TNORMAL_TANGENT_BINORMAL_TEXTURED2>(CORE->GetRenderManager(), l_Vtxs, l_Indxs, l_CantVertices, l_IndexCount);
      }
      //CRenderableVertexs *l_RV = new CIndexedVertex<TNORMAL_COLORED_TEXTURE1_VERTEX>(CORE->GetRenderManager(), l_Vtxs, l_Indxs, l_CantVertices, l_IndexCount);
      this->m_NumVertexs = l_IndexCount;
      this->m_NumFaces = l_IndexCount / 3;
      m_RVs.push_back(l_RV);
      //-----------------------------------
      //Prueba PhysX
      for(int i = 0; i < l_CantVertices; i++)
      {
        D3DXVECTOR3 *v1=(D3DXVECTOR3 *) &l_Vtxs[i*l_Size];
        m_VertexBufferPhysX.push_back(Vect3f(v1->x,v1->y,v1->z));
      }
      uint32 l_Index;
      for(int i = 0; i < l_IndexCount; i++)
      {
        l_Index = (uint32) l_Indxs[i];
        m_IndexBufferPhysX.push_back(l_Index);
      }
      //-----------------------------------

      CHECKED_DELETE_ARRAY(l_Vtxs);
      CHECKED_DELETE_ARRAY(l_Indxs);
    }
    l_File.read((char *)&l_Footer, sizeof(unsigned short));
    if(l_Footer != _FOOTER)
    {
      LOGGER->AddNewLog(ELL_ERROR, "CStaticMesh::Load->Pie de archivo incorrecto al leer el archivo '%s'",m_FileName.c_str());
      l_File.close();
      return false;
    }
    l_File.close();
  }
  else
  {
    std::string msg_error = "CStaticMesh::Load->Error al abrir el archivo. El archivo no existe: ";
		msg_error.append(m_FileName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		//throw CException(__FILE__, __LINE__, msg_error);
    return false;
  }
  return true;
}
bool CStaticMesh::ReLoad()
{
  Release();
  return Load(m_FileName);
}
void CStaticMesh::Render(CRenderManager *RM) const 
{
  vector<CRenderableVertexs*>::const_iterator l_Iter = this->m_RVs.begin();
  for(int i=0;l_Iter != m_RVs.end();++i, ++l_Iter) 
  {
    for(size_t j=0;j<m_Textures[i].size();++j)
    {
      m_Textures[i][j]->Activate(j);
    }
    CEffectTechnique *l_Technique=CORE->GetEffectManager()->GetStaticMeshTechnique();
    if(l_Technique==NULL)
      l_Technique=m_Techniques[i];
    if(l_Technique!=NULL)
    {
      l_Technique->BeginRender(m_pV3fSelfIlluminationColor);
      (*l_Iter)->Render(RM, l_Technique);
    }
  }
}

void CStaticMesh::Release()
{
  vector<CRenderableVertexs*>::iterator l_Iter = m_RVs.begin();
  while(l_Iter != m_RVs.end()) 
  {
    CHECKED_DELETE(*l_Iter);
    ++l_Iter;
  }
  m_RVs.clear();
}
