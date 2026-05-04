#include "SceneEffect.h"
#include "Math/Matrix44.h"
#include "RenderManager.h"
#include "Texture/TextureManager.h"
#include "Texture/Texture.h"
#include "XML/XMLTreeNode.h"
#include "Process.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
CSceneEffect::CSceneEffect(CXMLTreeNode &atts):CActive(),CNamed()
{
  CNamed::Init();//inicia CNamed
  SetName(atts.GetPszProperty("name"));
  SetActive(atts.GetBoolProperty("active"));

  CXMLTreeNode texture = atts["texture"];
	if (texture.Exists())
	{
		if (!texture.IsComment())
		{
			int32 numNodes = atts.GetNumChildren();	

			for(int i=0; i < numNodes; i++)
			{
				int l_iStageId = atts(i).GetIntProperty("stage_id");
				std::string l_sFile = atts(i).GetPszProperty("file");
				bool l_bLoadFile = atts(i).GetBoolProperty("load_file");

				CTexture * l_Texture = NULL;
				if(l_bLoadFile)
				{
					l_Texture=CORE->GetTextureManager()->GetTexture(l_sFile);
					if (l_Texture==NULL)
					{
      				//Guardar el mensaje de warning en el LOGGER
							std::string msg_warning = "SceneEffect::Constructor->Error al intentar leer el archivo de efectos: " + l_sFile;
							LOGGER->AddNewLog(ELL_WARNING, msg_warning.c_str());
					}
				}
				else
				{
					//la creas dinámicamente la textura
					l_Texture=CORE->GetTextureManager()->GetResource(l_sFile);
				}
				if(l_Texture!=NULL)
					AddStageTexture(l_iStageId, l_Texture);
			}//end for
		}
  }    
}

//----------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------
CSceneEffect::CStageTexture::CStageTexture(int StageId, CTexture *Texture)
{
  m_StageId = StageId;
  m_Texture = Texture;
}

//----------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------
CSceneEffect::~CSceneEffect()
{
  //destruye las texturas del vector m_StageTextures.
  /*std::vector<CStageTexture>::iterator iter = m_StageTextures.begin();
  while(iter != m_StageTextures.end())
  {
    //delete(iter->m_Texture);
    //CHECKED_DELETE(iter->m_Texture);
	  ++iter;
  }*/
  m_StageTextures.clear();
}

//----------------------------------------------------------------------------
// Activate de CStageTexture
//----------------------------------------------------------------------------
void CSceneEffect::CStageTexture::Activate()
{
  m_Texture->Activate(m_StageId);
}


//----------------------------------------------------------------------------
// ActivateTextures: Activa las texturas del vector m_StageTextures
//----------------------------------------------------------------------------
void CSceneEffect::ActivateTextures()
{
  std::vector<CStageTexture>::iterator iter = m_StageTextures.begin();

  while(iter != m_StageTextures.end())
  {
    iter->Activate();
	  ++iter;
  }

}

//----------------------------------------------------------------------------
// AddStageTexture: establece una textura en una etapa determinada de nuestro shader
//----------------------------------------------------------------------------
void CSceneEffect::AddStageTexture(int StageId, CTexture *Texture)
{
  m_StageTextures.push_back(CStageTexture(StageId, Texture)); 
}


//----------------------------------------------------------------------------
// PreRender:
//----------------------------------------------------------------------------
void CSceneEffect::PreRender(CRenderManager *RM, CProcess *Proc)
{

}


//----------------------------------------------------------------------------
// PostRender:
//----------------------------------------------------------------------------
void CSceneEffect::PostRender(CRenderManager *RM)
{

}


//----------------------------------------------------------------------------
// CaptureFrameBuffers:
//----------------------------------------------------------------------------
void CSceneEffect::CaptureFrameBuffers(CRenderManager *RM)
{

}
