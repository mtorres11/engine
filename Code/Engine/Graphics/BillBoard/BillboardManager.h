//----------------------------------------------------------------------------------
// CBillboardManager class
// Author: Andrea Cristobo
// Last Modification: 
// Description:
// Esta clase será donde se leerá el XML para las partículas animadas (BillboardAnimation).
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_BILLBOARD_MANAGER_H_
#define INC_BILLBOARD_MANAGER_H_

#include <d3dx9.h>
#include <string>
#include "Base.h"
#include "Texture/Texture.h"
#include "BillBoard/BillboardAnimation.h"

//----Forwards Declarations----
class CTexture;
class CBillboardAnimation;
//-----------------------------

struct SINST
{
	std::string		id;
	Vect3f				vPos;
};

struct SNUMINSTANCIAS
{
	int numInstTotal;
	int numInstDispo;
	int numInit;
};

struct SINF
	{
		std::string								sName;
		std::vector<SINST>				vInstance;
		Vect4f										vColor;
		int												iNumText;
		float											fSize;
		std::vector<CTexture*>		vTextures;
		bool											bLoop;
		float											fTimePerImage;
	};

class CBillboardManager
{
	public:
		CBillboardManager() { ; }
		~CBillboardManager() { m_bActive = true; Done(); }

		void 											Done            				();
		bool 											Init										(const std::string & fileName);
		bool 											IsOk                		() const														{return m_bIsOk;}
		void                      Release									();

		bool											LoadXML             		(const std::string & fileName); 
		void											SetProperties						(int cont, int i);
		void                      SetVectTextures     		(std::vector<CTexture*> vText);
		void 											Render									();
		void 											RenderById							(int id);
		void 											Update									(float elapsedTime);
		void 											Init										(CRenderManager* rm);
		void								      SetBAnimationByInstance ();
		CBillboardAnimation	*		  GetBillboardAnimation		(int id)														{return m_vBillboardAnimation[id];}
		CBillboardAnimation *     GetBAnimationByInstance (std::string name);
		bool											GetActive								()                                  {return m_bActive;}
		void											SetActive								(bool active)												{m_bActive = active;}

  private:
		bool																							 m_bIsOk;
		bool																							 m_bActive;
		std::vector<SINF>										      				 m_vInfo;
		SINF																	    				 m_stInfo;
		std::vector<CBillboardAnimation*>		  	  				 m_vBillboardAnimation;
		SINST																							 m_stInstance;
		std::vector<LPDIRECT3DTEXTURE9>										 m_vD3DTextures;
		std::vector<CTexture*>														 m_vText;
		std::map<std::string, SNUMINSTANCIAS>		  	  		 m_vBAnimationByInstance; 
		SNUMINSTANCIAS																		 m_stNumInst;
};

#endif