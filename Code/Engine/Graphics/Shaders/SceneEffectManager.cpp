#include "SceneEffectManager.h"
#include <string>
#include "XML/XMLTreeNode.h"
#include "Logger/Logger.h"
#include "Exceptions/Exception.h"
#include <vector>
#include "XML/XMLTreeNode.h"
#include <string>
#include "Math/MathTypes.h"
#include "SceneEffect.h"
#include "RenderToTextureSceneEffect.h"
#include "RenderTextureSceneEffect.h"
#include "DrawQuadSceneEffect.h"
#include "ShadowMapRenderToTextureSceneEffect.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CSceneEffectManager:: Constructor.
///</summary>
CSceneEffectManager::CSceneEffectManager()
:m_Filename("")
{
}

///<summary>
/// CSceneEffectManager:: Destructor.
///</summary>
CSceneEffectManager::~CSceneEffectManager()
{
  std::vector<CSceneEffect*>::iterator it;
  std::vector<CSceneEffect*>::iterator itEnd;	
  
  it = m_PreRenderSceneEffects.begin();
  itEnd = m_PreRenderSceneEffects.end();
  while (it != itEnd)
  {
    CHECKED_DELETE(*it);
    ++it;
  }
  m_PreRenderSceneEffects.clear();
 
  it = m_RenderSceneEffects.begin();
  itEnd = m_RenderSceneEffects.end();
  while (it != itEnd)
  {
    CHECKED_DELETE(*it);
    ++it;
  }
  m_RenderSceneEffects.clear();

  it = m_CaptureFrameBufferSceneEffects.begin();
  itEnd = m_CaptureFrameBufferSceneEffects.end();
  while (it != itEnd)
  {
    CHECKED_DELETE(*it);
    ++it;
  }
  m_CaptureFrameBufferSceneEffects.clear();

  it = m_CaptureFrameBufferSceneEffectsAfterPostRender.begin();
  itEnd = m_CaptureFrameBufferSceneEffectsAfterPostRender.end();
  while (it != itEnd)
  {
    CHECKED_DELETE(*it);
    ++it;
  }
  m_CaptureFrameBufferSceneEffectsAfterPostRender.clear();

  it = m_PostRenderSceneEffects.begin();
  itEnd = m_PostRenderSceneEffects.end();
  while (it != itEnd)
  {
    CHECKED_DELETE(*it);
    ++it;
  }
  m_PostRenderSceneEffects.clear();
  //Destroy();
}

///<summary>
/// CSceneEffectManager:: Destroy().
///</summary>
void CSceneEffectManager::Destroy()
{
  m_PreRenderSceneEffects.clear();
  m_RenderSceneEffects.clear();
  m_CaptureFrameBufferSceneEffects.clear();
  m_CaptureFrameBufferSceneEffectsAfterPostRender.clear();
  m_PostRenderSceneEffects.clear();
}

///<summary>
/// CSceneEffectManager:: LoadXML : lectura del fichero XML SceneEffects.XML
///</summary>
///<param name="FileName">Ubicación del fichero XML</param>
void CSceneEffectManager::LoadXML(const std::string &FileName)
{
  bool l_bIsOk = true;
  CXMLTreeNode parser;
  m_Filename = FileName;

  if (!parser.LoadFile(FileName.c_str()))
  {
    //Guardar el mensaje de error en el LOGGER
    std::string msg_error = "SceneEffectManager::Load->Error al intentar leer el archivo SceneEffects.xml: " + FileName;
    LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
    l_bIsOk = false;
  }

  LOGGER->AddNewLog(ELL_INFORMATION, "SceneEffectManager::Load-> Parseando fichero SceneEffect.xml: %s", FileName.c_str());

  std::vector<CSceneEffect *> l_PreRenderSceneEffects;
	std::vector<CSceneEffect *>	l_RenderSceneEffects;
	std::vector<CSceneEffect *>	l_CaptureFrameBufferSceneEffects;
	std::vector<CSceneEffect *>	l_CaptureFrameBufferSceneEffectsAfterPostRender;
	std::vector<CSceneEffect *>	l_PostRenderSceneEffects;

  //<scene_effects>
  CXMLTreeNode renders = parser["scene_effects"];
  if (renders.Exists())
  {
    int numNodes = renders.GetNumChildren();
    for (int i = 0; i < numNodes; ++i)
    {
      CXMLTreeNode node = renders(i);
      if(!node.IsComment())
      {
        bool bEffectFound = false;
        if(!strcmp(node.GetName(),"pre_render"))
        {
          std::string l_sName = node.GetPszProperty("name");
          std::string l_sType = node.GetPszProperty("type");
          //<pre_render type="shadow_map_render_to_texture" name="shadow_map_scene_effect" texture="ShadowMapTexture" 
          //width="1024" height="1024" static_mesh_technique="RenderShadowTechnique" animated_model_technique="Cal3DTechniqueShadow" 
          //light_shadow_cast="TPhotometricLight02" active="true" format_type="R32F"/>
          if(!strcmp(l_sType.c_str(),"shadow_map_render_to_texture"))
          {
            std::vector<CSceneEffect*>::iterator iter = m_PreRenderSceneEffects.begin();
            for(;iter != m_PreRenderSceneEffects.end();++iter)
            {
              if((*iter)->GetName() == l_sName)
              {
                bEffectFound = true;
                l_PreRenderSceneEffects.push_back((*iter));
                break;
              }
            }

            if (!bEffectFound) 
            {
              CShadowMapRenderToTextureSceneEffect* l_eff = new CShadowMapRenderToTextureSceneEffect(node);
              l_PreRenderSceneEffects.push_back(l_eff);
            }
          }
          else if(!strcmp(l_sType.c_str(),"render_to_texture"))
          {
            std::vector<CSceneEffect*>::iterator iter = m_PreRenderSceneEffects.begin();

            for(;iter != m_PreRenderSceneEffects.end();++iter)
            {
              if((*iter)->GetName() == l_sName)
              {
                bEffectFound = true;
                l_PreRenderSceneEffects.push_back((*iter));
                break;
              }
            }

            if (!bEffectFound) 
            {
              CRenderToTextureSceneEffect* l_eff = new CRenderToTextureSceneEffect(node);	
              l_PreRenderSceneEffects.push_back(l_eff);
            }
          }
        }
        //<render name="default_scene_effect" active="true">
        //<texture stage_id="5" file="ShadowMapTexture"/>
        //</render>
        else if(!strcmp(node.GetName(),"render"))
        {
          std::string l_sName = node.GetPszProperty("name");
          std::vector<CSceneEffect*>::iterator iter = m_RenderSceneEffects.begin();
          for(;iter != m_RenderSceneEffects.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              bEffectFound = true;
              l_RenderSceneEffects.push_back((*iter));
              break;
            }
          }

          if (!bEffectFound) 
          {
            CSceneEffect *l_eff = new CSceneEffect(node);
            l_RenderSceneEffects.push_back(l_eff);
          }
        }

        //<capture_frame_buffer name="capture_frame_buffer_scene_effect_without_post_fx" texture="FrameBufferTextureWithoutPostFX" 
        // texture_width_as_frame_buffer="true" active="true" format_type="A8R8G8B8"/>
        else if(!strcmp(node.GetName(),"capture_frame_buffer"))
        {
          std::string l_sName = node.GetPszProperty("name");

          std::vector<CSceneEffect*>::iterator iter = m_CaptureFrameBufferSceneEffects.begin();
          for(;iter != m_CaptureFrameBufferSceneEffects.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              bEffectFound = true;
              l_CaptureFrameBufferSceneEffects.push_back((*iter));
              break;
            }
          }

          if (!bEffectFound) 
          {
            CRenderTextureSceneEffect* l_eff = new CRenderTextureSceneEffect(node);
            l_CaptureFrameBufferSceneEffects.push_back(l_eff);
          }
        }
        //<capture_frame_buffer_post_fx name="capture_frame_buffer_scene_effect_with_post_fx" texture="FrameBufferTextureWithPostFX" 
        // texture_width_as_frame_buffer="true" active="true" format_type="A8R8G8B8"/>
        else if(!strcmp(node.GetName(),"capture_frame_buffer_post_fx"))
        {
          std::string l_sName = node.GetPszProperty("name");

          std::vector<CSceneEffect*>::iterator iter = m_CaptureFrameBufferSceneEffectsAfterPostRender.begin();
          for(;iter != m_CaptureFrameBufferSceneEffectsAfterPostRender.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              bEffectFound = true;
              l_CaptureFrameBufferSceneEffectsAfterPostRender.push_back((*iter));
              break;
            }
          }

          if (!bEffectFound) 
          {
            CRenderTextureSceneEffect* l_eff = new CRenderTextureSceneEffect(node);
            l_CaptureFrameBufferSceneEffectsAfterPostRender.push_back(l_eff);
          }
        }
        //<post_render type="draw_quad" name="post_zblur_scene_effect" technique="UsePS20ThirteenLookupsTechnique" active="true">
        //	<texture stage_id="0" file="ZBlurTexture"/>
        //	<texture stage_id="1" file="FrameBufferTextureWithoutPostFX"/>
        //</post_render>
        else if(!strcmp(node.GetName(),"post_render"))
        {
          std::string l_sName = node.GetPszProperty("name");

          std::vector<CSceneEffect*>::iterator iter = m_PostRenderSceneEffects.begin();
          for(;iter != m_PostRenderSceneEffects.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              bEffectFound = true;
              l_PostRenderSceneEffects.push_back((*iter));
              break;
            }
          }

          if (!bEffectFound) 
          {
            CDrawQuadSceneEffect* l_eff = new CDrawQuadSceneEffect(node);
            l_PostRenderSceneEffects.push_back(l_eff);
          }
        }
      }
    }
  }
  Reload(m_PreRenderSceneEffects,l_PreRenderSceneEffects);
  Reload(m_RenderSceneEffects,l_RenderSceneEffects);
  Reload(m_CaptureFrameBufferSceneEffects,l_CaptureFrameBufferSceneEffects);
  Reload(m_CaptureFrameBufferSceneEffectsAfterPostRender,l_CaptureFrameBufferSceneEffectsAfterPostRender);
  Reload(m_PostRenderSceneEffects,l_PostRenderSceneEffects);
}

///<summary>
/// CSceneEffectManager:: PreRender : En este método ejecutaremos los SceneEffects del vector m_PreRenderSceneEffects.
///</summary>
///<param name="RM"></param>
///<param name="Proc"></param>
//TODO.. CProcess o C3DProc???
void CSceneEffectManager::PreRender(CRenderManager *RM, CProcess *Proc)
{
  std::vector<CSceneEffect *>::iterator iter = m_PreRenderSceneEffects.begin();
  for(;iter != m_PreRenderSceneEffects.end(); ++iter)
  {
    if((*iter)->GetActive())         (*iter)->PreRender(RM, Proc);
  }
}

///<summary>
/// CSceneEffectManager:: ActivateRenderSceneEffects : 
///</summary>
void CSceneEffectManager::ActivateRenderSceneEffects()
{
  std::vector<CSceneEffect *>::iterator iter = m_RenderSceneEffects.begin();
  for(;iter != m_RenderSceneEffects.end(); ++iter)
  {
    if((*iter)->GetActive())          (*iter)->ActivateTextures();
  }
}

///<summary>
/// CSceneEffectManager:: CaptureFrameBuffers : 
///</summary>
///<param name="RM"></param>
void CSceneEffectManager::CaptureFrameBuffers(CRenderManager *RM)
{
  std::vector<CSceneEffect *>::iterator iter = m_CaptureFrameBufferSceneEffects.begin();
  for(;iter != m_CaptureFrameBufferSceneEffects.end(); ++iter)
  {
    if((*iter)->GetActive())         (*iter)->CaptureFrameBuffers(RM);
  }
}

///<summary>
/// CSceneEffectManager:: PostRender : 
///</summary>
///<param name="RM"></param>
void CSceneEffectManager::PostRender(CRenderManager *RM)
{
  std::vector<CSceneEffect *>::iterator iter = m_PostRenderSceneEffects.begin();
  for(;iter != m_PostRenderSceneEffects.end(); ++iter)
  {
    if((*iter)->GetActive())     
      (*iter)->PostRender(RM);
  }
}

///<summary>
/// CSceneEffectManager:: CaptureFrameBuffersAfterPostRender :
///</summary>
///<param name="RM"></param>
void CSceneEffectManager::CaptureFrameBuffersAfterPostRender(CRenderManager *RM)
{
  std::vector<CSceneEffect *>::iterator iter = m_CaptureFrameBufferSceneEffectsAfterPostRender.begin();
  for(;iter != m_CaptureFrameBufferSceneEffectsAfterPostRender.end(); ++iter)
  {
    if((*iter)->GetActive())         (*iter)->CaptureFrameBuffers(RM);
  }
}

void CSceneEffectManager::Reload(std::vector<CSceneEffect*>& membre, std::vector<CSceneEffect*>& aux)
{
  std::vector<CSceneEffect*>::iterator It;
  std::vector<CSceneEffect*>::iterator It2;

  for (It =  membre.begin(); It !=  membre.end(); ++It)
  {
    bool bFound = false;
    for (It2 =  aux.begin(); It2 !=  aux.end(); ++It2)
    {
      if ((*It2)->GetName() == (*It)->GetName())
      {
        bFound = true;
        (*It)->SetActive(true);
        break;
      }
    }

    if (!bFound)
    {
      (*It)->SetActive(false);
    }
  }

  for (It2 =  aux.begin(); It2 !=  aux.end(); ++It2)
  {
    bool bFound = false;
    for (It =  membre.begin(); It !=  membre.end(); ++It)
    {
      if ((*It2)->GetName() == (*It)->GetName())
      {
        bFound = true;
        break;
      }
    }
    
    if (!bFound)
    {
      membre.push_back((*It2));
    }
  }


  //if(membre.empty())
  //{
  //  membre.clear();
  //  membre = aux;
  //}
}

///<summary>
/// CSceneEffectManager:: LoadXML : lectura del fichero XML SceneEffects.XML
///</summary>
///<param name="FileName">Ubicación del fichero XML</param>
void CSceneEffectManager::ReloadXML()
{
  bool l_bIsOk = true;
  CXMLTreeNode parser;

  if (!parser.LoadFile(m_Filename.c_str()))
  {
    //Guardar el mensaje de error en el LOGGER
    std::string msg_error = "SceneEffectManager::ReLoad->Error al intentar leer el archivo SceneEffects.xml: " + m_Filename;
    LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
    l_bIsOk = false;
  }

  LOGGER->AddNewLog(ELL_INFORMATION, "SceneEffectManager::ReLoad-> Parseando fichero SceneEffect.xml: %s", m_Filename.c_str());

  //<scene_effects>
  CXMLTreeNode renders = parser["scene_effects"];
  if (renders.Exists())
  {
    int numNodes = renders.GetNumChildren();
    for (int i = 0; i < numNodes; ++i)
    {
      CXMLTreeNode node = renders(i);
      if(!node.IsComment())
      {
        bool bEffectFound = false;
        if(!strcmp(node.GetName(),"pre_render"))
        {
          std::string l_sName = node.GetPszProperty("name");
          bool l_bActive = node.GetBoolProperty("active");

          std::vector<CSceneEffect*>::iterator iter = m_PreRenderSceneEffects.begin();
          for(;iter != m_PreRenderSceneEffects.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              (*iter)->SetActive(l_bActive);                  
              break;
            }
          }
        }
        //<render name="default_scene_effect" active="true">
        //<texture stage_id="5" file="ShadowMapTexture"/>
        //</render>
        else if(!strcmp(node.GetName(),"render"))
        {
          std::string l_sName = node.GetPszProperty("name");
          bool l_bActive = node.GetBoolProperty("active");

          std::vector<CSceneEffect*>::iterator iter = m_RenderSceneEffects.begin();
          for(;iter != m_RenderSceneEffects.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              (*iter)->SetActive(l_bActive);                  
              break;
            }
          }
        }
        //<capture_frame_buffer name="capture_frame_buffer_scene_effect_without_post_fx" texture="FrameBufferTextureWithoutPostFX" 
        // texture_width_as_frame_buffer="true" active="true" format_type="A8R8G8B8"/>
        else if(!strcmp(node.GetName(),"capture_frame_buffer"))
        {
          std::string l_sName = node.GetPszProperty("name");
          bool l_bActive = node.GetBoolProperty("active");

          std::vector<CSceneEffect*>::iterator iter = m_CaptureFrameBufferSceneEffects.begin();
          for(;iter != m_CaptureFrameBufferSceneEffects.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              (*iter)->SetActive(l_bActive);                  
              break;
            }
          }
        }
        //<capture_frame_buffer_post_fx name="capture_frame_buffer_scene_effect_with_post_fx" texture="FrameBufferTextureWithPostFX" 
        // texture_width_as_frame_buffer="true" active="true" format_type="A8R8G8B8"/>
        else if(!strcmp(node.GetName(),"capture_frame_buffer_post_fx"))
        {
          std::string l_sName = node.GetPszProperty("name");
          bool l_bActive = node.GetBoolProperty("active");

          std::vector<CSceneEffect*>::iterator iter = m_CaptureFrameBufferSceneEffectsAfterPostRender.begin();
          for(;iter != m_CaptureFrameBufferSceneEffectsAfterPostRender.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              (*iter)->SetActive(l_bActive);                  
              break;
            }
          }
        }
        //<post_render type="draw_quad" name="post_zblur_scene_effect" technique="UsePS20ThirteenLookupsTechnique" active="true">
        //	<texture stage_id="0" file="ZBlurTexture"/>
        //	<texture stage_id="1" file="FrameBufferTextureWithoutPostFX"/>
        //</post_render>
        else if(!strcmp(node.GetName(),"post_render"))
        {
          std::string l_sName = node.GetPszProperty("name");
          bool l_bActive = node.GetBoolProperty("active");

          std::vector<CSceneEffect*>::iterator iter = m_PostRenderSceneEffects.begin();
          for(;iter != m_PostRenderSceneEffects.end();++iter)
          {
            if((*iter)->GetName() == l_sName)
            {
              (*iter)->SetActive(l_bActive);                  
              break;
            }
          }
        }
      }
    }
  }
}

