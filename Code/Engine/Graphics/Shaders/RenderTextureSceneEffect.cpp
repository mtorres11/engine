
#include "RenderTextureSceneEffect.h"
#include "Math/Matrix44.h"
#include <string>
#include "xml/XMLTreeNode.h"
#include "Exceptions/Exception.h"
#include "Logger/Logger.h"
#include "RenderManager.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "Timer/Timer.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


///<summary>
/// CRenderTextureSceneEffect:: Constructor : crear la textura de tipo RENDERTARGET y adquirir su Surface para después poder capturar el frame buffer.
///</summary>
///<param name="atts">Gestiona la lectura y escritura en ficheros XML</param>
CRenderTextureSceneEffect::CRenderTextureSceneEffect(CXMLTreeNode &atts)
: CSceneEffect(atts)
, m_Texture(NULL)
, m_Surface(NULL)
{
	bool l_bTextureWidthFrameBuff = atts.GetBoolProperty("texture_width_as_frame_buffer");
	std::string l_sTexture = atts.GetPszProperty("texture");
  std::string l_sFormat = atts.GetPszProperty("format_type");
  unsigned short int l_usiWidth = (unsigned short int)atts.GetIntProperty("width");
  unsigned short int l_usiHeight = (unsigned short int)atts.GetIntProperty("height");
  std::string l_sName = atts.GetPszProperty("name");

  if (l_bTextureWidthFrameBuff)
  {
      //crea prerender en textura
      m_Texture = new CTexture();  
	  
	    uint32 l_Width, l_Height;
	    CORE->GetRenderManager()->GetWandH(l_Width, l_Height);

	  m_Texture->Create(l_sName, l_Width, l_Height, 1,CTexture::RENDERTARGET,CTexture::DEFAULT,CTexture::GetFormatTypeFromString(l_sFormat));
  }
  else
  {
      //crea textura para efectos
      m_Texture = new CTexture();  
      this->m_Texture->Create(l_sName, l_usiHeight, l_usiHeight, 1,this->m_Texture->RENDERTARGET,this->m_Texture->DEFAULT,m_Texture->GetFormatTypeFromString(l_sFormat));
  }
	
  m_Texture->GetD3DTexture()->GetSurfaceLevel(0, &m_Surface);
  CTextureManager *tm= CORE->GetTextureManager();
  tm->AddResource(l_sTexture, m_Texture);
}

///<summary>
/// CRenderTextureSceneEffect:: Destructor.
///</summary>
CRenderTextureSceneEffect::~CRenderTextureSceneEffect()
{
	//CHECKED_DELETE(m_Texture);
	CHECKED_RELEASE(m_Surface);
}

///<summary>
/// CRenderTextureSceneEffect:: CaptureFrameBuffers : captura el frame buffer del device en DirectX.
///</summary>
///<param name="RM">RenderManager</param>
void CRenderTextureSceneEffect::CaptureFrameBuffers(CRenderManager *RM)
{
	LPDIRECT3DSURFACE9 l_RenderTarget;
  
	RM->GetDevice()->GetRenderTarget(0,&l_RenderTarget);
	RM->GetDevice()->StretchRect( l_RenderTarget, NULL,m_Surface, NULL, D3DTEXF_NONE);

  if(CORE->GetInputManager()->GetActionToInput()->DoAction("Screenshot") != 0.0f )
  {
    char pathFoto[256];
    sprintf_s(pathFoto, "./Data/Screenshots/screen%d.bmp", (int) CORE->GetTimer()->GetTotalTime());
    this->m_Texture->Save(pathFoto);
  }    

  l_RenderTarget->Release();
}
