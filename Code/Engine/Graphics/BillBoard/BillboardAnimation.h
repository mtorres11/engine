//----------------------------------------------------------------------------------
// CBillboard class
// Author: Andrea Cristobo
// Last Modification: 
// Description:
// Esta clase será donde se creará el billboard animado para las partículas.
//----------------------------------------------------------------------------------

#pragma once

#ifndef INC_BILLBOARD_ANIMATION_H_
#define INC_BILLBOARD_ANIMATION_H_

#include <d3dx9.h>
#include "Base.h"
#include "Cameras/Camera.h"
#include "BillBoard/Billboard.h"

class CBillboardAnimation
{
	public:
																				CBillboardAnimation		();
																				~CBillboardAnimation	();

		void																Update								(float elapsedTime);
		void																Render								(const std::vector<LPDIRECT3DTEXTURE9> vTextures);
		bool																IsEnd									()																												{return m_bEnd;}
		void																SetPos								(const Vect3f position)																		{m_Position = position;} //para cambiar la pos del Quad
		void																SetSize								(const float & sizeX, const float & sizeY)								{m_fSizeX = sizeX; m_fSizeY = sizeY;} //cambiar el ancho y alto del Quad
		void          											SetVTextures          (std::vector<LPDIRECT3DTEXTURE9>	vD3DTextures);
		void																Init									(CRenderManager* rm);

		//Sets
		void																SetLoop								(bool bLoop)																							{m_bLoop = bLoop;}
		void																SetEnd								(bool bFlag)																							{m_bEnd = bFlag;}
		void																SetNumTextures				(int num_iTextures)																				{m_iNumTextures = num_iTextures;} 
		void																SetTimePerImage				(float fTime)																							{m_fTimePerImage = fTime;}
		void																SetTextureAct					(int iFrame)																							{m_iTextureAct = iFrame;}
		void																SetPointA         		(Vect3f pointA)                                         	{m_PointA = pointA;}
		void																SetPointB         		(Vect3f pointB)                                         	{m_PointB = pointB;}
		void																SetPointC         		(Vect3f pointC)                                         	{m_PointC = pointC;}
		void																SetPointD         		(Vect3f pointD)                                         	{m_PointD = pointD;}
		void																SetSizeX							(float sizeX)																							{m_fSizeX = sizeX;}
		void																SetSizeY							(float sizeY)																							{m_fSizeY = sizeY;}
		void																SetColor							(D3DCOLOR color)																					{m_Color = color;}
		void																SetName								(std::string name)																				{m_sName = name;}

		//Gets
		Vect3f        											GetPosition       		()																												{return m_Position;}
		Vect3f															GetPointA         		()																												{return m_PointA;}
		Vect3f															GetPointB							()																												{return m_PointB;}
		Vect3f															GetPointC         		()																												{return m_PointC;}
		Vect3f															GetPointD							()																												{return m_PointD;}
		float																GetSizeX							()																												{return m_fSizeX;}
		float																GetSizeY							()																												{return m_fSizeY;}
		D3DCOLOR      											GetColor							()																												{return m_Color;}
		int           											GetTextureAct         ()																												{return m_iTextureAct;}
		std::vector<LPDIRECT3DTEXTURE9>     GetTexturesVect				()																												{return m_vTexturesTest;}
		std::string													GetName								()																												{return m_sName;}


	private:
		Vect3f															m_Position, m_PointA, m_PointB, m_PointC, m_PointD;
		float																m_fSizeX, m_fSizeY; 
		D3DCOLOR														m_Color;
		CBillboard													m_pBillboard;
		int																	m_iTextureAct, m_iNumTextures;
		float																m_fTime, m_fTimePerImage;
		bool																m_bLoop, m_bEnd;
		std::vector<LPDIRECT3DTEXTURE9>	    m_vTexturesTest;
		LPDIRECT3DVERTEXBUFFER9							m_vbD3DBillboardAnimation; 	//para el vertex buffer
		LPDIRECT3DDEVICE9										m_pd3dDevice;
		std::string													m_sName;
};
#endif