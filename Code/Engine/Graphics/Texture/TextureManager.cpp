#include "TextureManager.h"
#include "Base.h"
#include "Logger/Logger.h"
#include "Texture.h"
#include "Exceptions/Exception.h"
#include "Math/Color.h"
#include <d3dx9.h>

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CTextureManager::~CTextureManager()
{
  for(TMapResource::iterator it = m_Resources.begin(); it != m_Resources.end(); ++it)
  {
		if (!it->second->IsDefaultTexture())
		{
			CHECKED_DELETE(it->second)
		}
  }

  CHECKED_DELETE(m_pDefaultTexture)	
}

void CTextureManager::Init ()
{
  m_pDefaultTexture = new CTexture;
	m_pDefaultTexture->SetAsDefaultTexture();
  bool isOk = false;
  if (m_pDefaultTexture->CreateTexture(32, 32))
  {
    #ifdef _DEBUG
      CColor l_textureColor = colGREEN;
    #else
      CColor l_textureColor = colBLACK;
    #endif

    if (m_pDefaultTexture->FillTextureWithColor(32, 32, l_textureColor))
    {
      isOk = true;
    }
  }

  if(!isOk)
  {
		std::string msg_error = "CTextureManager::Init-> Error al intentar crear la defualtTexture en la inicialización de CTextureManager";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
  }
}

void CTextureManager::Reload()
{
	TMapResource::iterator l_It=m_Resources.begin();
	while(l_It!=m_Resources.end())
	{
		l_It->second->Reload();
		++l_It;
	}
}

CTexture * CTextureManager::GetTexture(const std::string &FileName)
{
  CTexture *l_Texture = GetResource(FileName);
  if (l_Texture == NULL)
  {
    l_Texture = new CTexture();
    if (!l_Texture->Load(FileName)) {
      CHECKED_DELETE(l_Texture);
      l_Texture = m_pDefaultTexture;
      std::string msg_error = "CTextureManager::GetTexture-> Error al intentar cargar la textura: %s";
		  LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str(), FileName.c_str());
    }
    AddResource(FileName, l_Texture);
  }

  return l_Texture;
}
