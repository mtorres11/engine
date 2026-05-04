//----------------------------------------------------------------------------------
// CBillboard class
// Author: Andrea Cristobo
// Last Modification: 
// Description:
// Esta clase será donde se creará el billboard para las partículas.
//----------------------------------------------------------------------------------

#pragma once

#ifndef INC_BILLBOARD_H_
#define INC_BILLBOARD_H_

#include <d3dx9.h>
#include "Base.h"
#include "Cameras/Camera.h"

class CBillboard
{
	public:
										CBillboard				();
										~CBillboard				();
		void						Update						(float elapsedTime, float angle);
		void						Render						(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 & texture);
		void						SetPos						(const Vect3f position)																			{m_Position = position;} //para cambiar la pos del Quad
		void						SetSize						(const float & sizeX, const float & sizeY)									{m_fSizeX = sizeX; m_fSizeY = sizeY;} //cambiar el ancho y alto del Quad
		
		//Gets
		Vect3f          GetPosition       ()																													{return m_Position;}
		Vect3f					GetPointA         ()																													{return m_PointA;}
		Vect3f					GetPointB					()																													{return m_PointB;}
		Vect3f					GetPointC         ()																													{return m_PointC;}
		Vect3f					GetPointD					()																													{return m_PointD;}
		float						GetSizeX					()																													{return m_fSizeX;}
		float						GetSizeY					()																													{return m_fSizeY;}
		D3DCOLOR        GetColor					()																													{return m_Color;}

		//Sets
		void						SetPointA         (Vect3f pointA)                                             {m_PointA = pointA;}
		void						SetPointB         (Vect3f pointB)                                             {m_PointB = pointB;}
		void						SetPointC         (Vect3f pointC)                                             {m_PointC = pointC;}
		void						SetPointD         (Vect3f pointD)                                             {m_PointD = pointD;}
		void						SetSizeX					(float sizeX)																								{m_fSizeX = sizeX;}
		void						SetSizeY					(float sizeY)																								{m_fSizeY = sizeY;}
		void						SetColor					(D3DCOLOR color)																						{m_Color = color;}

	private:
		
		Vect3f					m_Position; //posicion 3D donde se situará el Billboard a pintar
		Vect3f					m_PointA;   //los 4 puntos que forman el Quad del Billboard
		Vect3f          m_PointB;
		Vect3f          m_PointC;
		Vect3f          m_PointD;
		float					  m_fSizeX;   //ancho y alto del Quad
		float					  m_fSizeY; 
		D3DCOLOR				m_Color;
};

#endif
