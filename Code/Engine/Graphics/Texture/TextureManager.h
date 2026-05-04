//----------------------------------------------------------------------------------
// CTexture class
// Author: Marco Torres
// 
// Description:
// Texture Manager (Clase base)
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_TEXTURE_MANAGER_H_
#define INC_TEXTURE_MANAGER_H_

#include "MapManager/MapManager.h"
#include <string>

class CTexture;

class CTextureManager : public CMapManager<CTexture>
{
  public:
	  CTextureManager(): m_pDefaultTexture(0) {};
	  ~CTextureManager();

    
	  void				  Init							();
    CTexture *    GetTexture        (const std::string &FileName);
    CTexture *    GetDefualtTexture () {return m_pDefaultTexture;}
    void          Reload            ();
  
  private:
    CTexture* m_pDefaultTexture;
};

#endif