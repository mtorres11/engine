#include "Texture.h"
#include "Base.h"
#include "Logger/Logger.h"
#include "RenderManager.h"
#include "Named/Named.h"
#include "Process.h"
#include "TextureManager.h"
#include "Exceptions/Exception.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CTexture::~CTexture()
{
	Unload();
}

const std::string & CTexture::GetFileName() const
{
	return m_FileName;
}

bool CTexture::Load(const std::string &FileName)
{
	m_FileName = FileName;
	return LoadFile();
}

bool CTexture::Reload()
{
	Unload();
	return LoadFile();
}

void CTexture::Activate(size_t StageId)
{
	CORE->GetRenderManager()->GetDevice()->SetTexture(StageId, m_Texture);

}
bool CTexture::LoadFile()
{
	LPDIRECT3DDEVICE9 l_device = CORE->GetRenderManager()->GetDevice();	
	HRESULT tex = D3DXCreateTextureFromFile(l_device, m_FileName.c_str(), &m_Texture);

  D3DXIMAGE_INFO pSrcInfo;
	HRESULT info = D3DXGetImageInfoFromFile(m_FileName.c_str(), &pSrcInfo);	
	
	m_Width = pSrcInfo.Width;
	m_Height = pSrcInfo.Height;
	
	int type = pSrcInfo.ImageFileFormat;
	return (tex==D3D_OK);
}

void CTexture::Unload()
{
	CHECKED_RELEASE(m_Texture);
}

bool CTexture::Create(const std::string &Name, unsigned int Width, unsigned int Height, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType, TFormatType FormatType)
{
  CNamed m_sName;
  m_sName.SetName(Name);

  D3DPOOL l_Pool=D3DPOOL_DEFAULT;
	DWORD l_UsageType=D3DUSAGE_DYNAMIC;
	D3DFORMAT l_Format=D3DFMT_A8R8G8B8;
	bool l_CreateDepthStencilSurface=false;

	switch(UsageType)
	{
	  case DYNAMIC:
		  l_UsageType=D3DUSAGE_DYNAMIC;
	  break;

	  case RENDERTARGET:
		  l_CreateDepthStencilSurface=true;
		  l_UsageType=D3DUSAGE_RENDERTARGET;
	  break;
	}

	switch(PoolType)
	{
	  case DEFAULT:
		  l_Pool=D3DPOOL_DEFAULT;
	  break;
	  case SYSTEMMEM:
		  l_Pool=D3DPOOL_SYSTEMMEM;
	  break;
	}

	switch(FormatType)
	{
	  case A8R8G8B8:
		  l_Format=D3DFMT_A8R8G8B8;
	  break;
	  case R8G8B8:
		  l_Format=D3DFMT_R8G8B8;
	  break;
	  case X8R8G8B8:
		  l_Format=D3DFMT_X8R8G8B8;
	  break;
	  case R32F:
		  l_Format=D3DFMT_R32F;
	  break;
	}

	HRESULT hr=CORE->GetRenderManager()->GetDevice()->CreateTexture(Width, Height, MipMaps, l_UsageType, l_Format, l_Pool,&m_Texture,NULL);
	if(l_CreateDepthStencilSurface)
	{
		CORE->GetRenderManager()->GetDevice()->CreateDepthStencilSurface(Width, Height, D3DFMT_D24S8,D3DMULTISAMPLE_NONE, 0, TRUE,&m_DepthStencilRenderTargetTexture, NULL);
		assert(m_DepthStencilRenderTargetTexture!=NULL);
	}
	assert(m_Texture!=NULL);
	assert(hr==D3D_OK);
	m_Width=Width;
	m_Height=Height;

	return hr!=D3D_OK;
}

bool CTexture::CreateTexture (unsigned int width, unsigned int height, bool renderTarget)
{
	Unload();
  CRenderManager *l_RM = CORE->GetRenderManager();
	if (renderTarget)
	{
		l_RM->GetDevice()->CreateTexture(	width, height, 1, D3DUSAGE_RENDERTARGET	, D3DFMT_R5G6B5, D3DPOOL_DEFAULT, &m_Texture, NULL);
	}
	else
	{
		l_RM->GetDevice()->CreateTexture(	width, height, 0, 0	, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_Texture, NULL);
	}

	if (!m_Texture)
	{
		l_RM->GetDevice()->CreateTexture(	width, height, 0, 0	, D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &m_Texture, NULL);
	}

	if (!m_Texture)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::CreateTextureFromBuffer-> Error al crear la textura de buffer");
    return false;
  }
  else
  {
		m_Width	= width;
		m_Width	= height;
    return true;
  }
}

void CTexture::Deactivate(size_t Stage)
{
  LPDIRECT3DDEVICE9 l_Device= CORE->GetRenderManager()->GetDevice();
  l_Device->SetTexture((DWORD)Stage,NULL);
}

bool CTexture::FillTextureWithColor (unsigned int width, unsigned int height, const CColor& color)
{
  if (m_Texture==NULL)
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureWithColor-> m_Texture no ha sido creado aun ");
    return false;
  }

  HRESULT hr;
  D3DLOCKED_RECT lock;
  hr=m_Texture->LockRect(0, &lock, NULL, D3DLOCK_DISCARD);
  if(hr==D3D_OK)
  {
    uint8 *pTxtBuffer; // Bitmap buffer, texture buffer
    pTxtBuffer = (uint8*)lock.pBits;

		
    uint32 j = 0;
    for( uint32 cont = 0; cont< width * height; cont++)
    {
      //BLUE
      pTxtBuffer[cont*4 + 0] = (uint8)(color.GetBlue()*255);
      //GREEN
      pTxtBuffer[cont*4 + 1] = (uint8)(color.GetGreen()*255);
      //RED
      pTxtBuffer[cont*4 + 2] = (uint8)(color.GetRed()*255);
      //ALPHA
      pTxtBuffer[cont*4 + 3] = (uint8)(color.GetAlpha()*255);
    }
    hr=m_Texture->UnlockRect(0);
  }
  else
  {
    LOGGER->AddNewLog(ELL_ERROR,"CTexture::FillTextureWithColor->Error en la llamada lockRect");
    return false;
  }

	m_Width	= width;
	m_Height	= height;

  return true;
}

bool CTexture::SetAsRenderTarget()
{
  LPDIRECT3DDEVICE9 l_Device= CORE->GetRenderManager()->GetDevice();
  l_Device->GetRenderTarget(0,&m_OldRenderTarget);

	if (FAILED(m_Texture->GetSurfaceLevel(0,&m_RenderTargetTexture)))
	{
		return false;
	}

	l_Device->SetRenderTarget(0, m_RenderTargetTexture);
	CHECKED_RELEASE(m_RenderTargetTexture);
	if(FAILED(l_Device->GetDepthStencilSurface(&m_OldDepthStencilRenderTarget)))
	{
		return false;
	}
	l_Device->SetDepthStencilSurface(m_DepthStencilRenderTargetTexture);
	
	return true;
}

void CTexture::UnsetAsRenderTarget()
{
	LPDIRECT3DDEVICE9 l_Device=CORE->GetRenderManager()->GetDevice();
	l_Device->SetDepthStencilSurface(m_OldDepthStencilRenderTarget);
	CHECKED_RELEASE(m_OldDepthStencilRenderTarget);
	l_Device->SetRenderTarget(0, m_OldRenderTarget);
	CHECKED_RELEASE(m_OldRenderTarget);
}

CTexture::TFormatType CTexture::GetFormatTypeFromString(const std::string &FormatType)
	{
		if(FormatType=="R32F")
		{
			return CTexture::R32F;
		}
		else if(FormatType=="A8R8G8B8")
			{
				return CTexture::A8R8G8B8;
			}
		else if(FormatType=="R8G8B8")
			{
				return CTexture::R8G8B8;
			}
		else if(FormatType=="X8R8G8B8")
			{
				return CTexture::X8R8G8B8;
			}
		else
		{
			std::string msg_error = "CTexture::GetFormatTypeFromString->Tipo de formato no valido: " + FormatType;
			LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
			throw CException(__FILE__, __LINE__, msg_error);
		}
		return CTexture::A8R8G8B8;
}

bool CTexture::Save(const std::string &FileName)
{
	return D3DXSaveTextureToFile(FileName.c_str(), D3DXIFF_BMP, m_Texture, NULL)==S_OK;
}