//----------------------------------------------------------------------------------
// CTexture class
// Author: Marco Torres
//
// Description:
// Texture (Derivada)
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_TEXTURE_H_
#define INC_TEXTURE_H_

#include "Math/Color.h"
#include <string>
#include <d3dx9.h>


class CTexture
{
  protected:
	  LPDIRECT3DTEXTURE9 m_Texture;
	  std::string m_FileName;
	  LPDIRECT3DSURFACE9							m_DepthStencilRenderTargetTexture;
	  LPDIRECT3DSURFACE9							m_OldDepthStencilRenderTarget;
	  LPDIRECT3DSURFACE9							m_OldRenderTarget;
	  LPDIRECT3DSURFACE9							m_RenderTargetTexture;
	  unsigned int m_Width;
	  unsigned int m_Height;

  	
	  //-------------Metodos-------------
	  virtual	bool									LoadFile					();
	  void													Unload						();

  public:
	  CTexture():
      m_Texture(NULL),
		  m_FileName(""),
      m_bDefaultTexture(false)
	  {}
	  ~CTexture();
	  enum TPoolType 
	  {
		  DEFAULT=0,
		  SYSTEMMEM
	  };

	  enum TUsageType 
	  {
		  DYNAMIC=0,
		  RENDERTARGET
	  };

	  enum TTextureType 
	  {
		  TGA=0,
		  JPG,
		  BMP
	  };

	  enum TFormatType 
	  {
		  A8R8G8B8=0,
		  R8G8B8,
		  X8R8G8B8,
		  R32F
	  };

	  const	std::string &						GetFileName								() const;
	  bool													Load											(const std::string &FileName);
	  bool													Save											(const std::string &FileName);
	  bool													Reload										();
	  void													Activate									(size_t StageId);
	  bool													Create										(const std::string &Name, unsigned int Width, unsigned int Height, unsigned int MipMaps, TUsageType UsageType, TPoolType PoolType, TFormatType FormatType);
    bool		    									CreateTexture	  					(unsigned int width, unsigned int height, bool renderTarget = false);
	  void                          Deactivate								(size_t Stage);
	  bool													SetAsRenderTarget					();
	  void										    	SetAsDefaultTexture			  () {m_bDefaultTexture = true;}
	  bool					    						IsDefaultTexture			  	() const {return m_bDefaultTexture;}
    bool    											FillTextureWithColor		  (unsigned int width, unsigned int height, const CColor& color);
	  void													UnsetAsRenderTarget				();
	  static TFormatType						GetFormatTypeFromString		(const std::string &FormatType);
	  LPDIRECT3DTEXTURE9						GetD3DTexture()						{return m_Texture;}

  private:
	  bool m_bDefaultTexture;
};

#endif