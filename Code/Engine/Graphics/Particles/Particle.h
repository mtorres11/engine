//----------------------------------------------------------------------------------
// CParticle class
// Author: Andrea Cristobo
// Last Modification: 
// Description:
// Esta clase será donde se crearán las partículas.
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_PARTICLE_H_
#define INC_PARTICLE_H_

#include <vector>
#include <d3dx9.h>
#include "BillBoard/Billboard.h"

class CParticle 
{
	public:
		CParticle();
		~CParticle() { Done(); }

		void 						Done            ();
		bool 						Init						();
		bool 						IsOk            () const													{ return m_bIsOk;}
		bool						Update					(float fTimeDelta);
		
		//Gets
		float						GetSize					()																{return m_fSize;}
		float						GetLifetime			()																{return m_fLifetime;}
		float						GetAge					()																{return m_fAge;}
		D3DXCOLOR				GetColor				()																{return m_Color;}
		D3DXVECTOR3			GetPos					()																{return m_vPos;}
		D3DXVECTOR3			GetDir					()																{return m_vDir;}
		Vect3f					GetPointA				()																{return m_PointA;}
		Vect3f					GetPointB				()																{return m_PointB;}
		Vect3f					GetPointC				()																{return m_PointC;}
		Vect3f					GetPointD				()																{return m_PointD;}
		Vect3f          GetPosition     ()															  {return m_Position;}

		//Sets
		void 						SetSize					(float size)			 								{m_fSize = size;}
		void 						SetLifetime			(float lifeTime)	 								{m_fLifetime = lifeTime;}
		void 						SetAge					(float age)				 								{m_fAge = age;}
		void 						SetColor				(D3DXCOLOR color)	 								{m_Color = color;}
		void 						SetPos					(D3DXVECTOR3 pos)	 								{m_vPos = pos;}
		void 						SetDir					(D3DXVECTOR3 dir)									{m_vDir = dir;}
		void						SetColorVect		(std::vector<D3DXCOLOR> color)		{m_ColorVect = color;}
		void						SetDirVect			(std::vector<D3DXVECTOR3> dir)		{m_DirVect = dir;}
		void						SetSizeVect			(std::vector<float> size)					{m_SizeVect = size;}
		void						SetVelVect			(std::vector<D3DXVECTOR3> vel)		{m_VelocityVect = vel;}
		void						SetAngleVect		(std::vector<float> angle)				{m_AngleVect = angle;}
		void						SetTimeAngle		(std::vector<float> timeA)				{m_TimeAngle = timeA;}
		void						SetTimeVel			(std::vector<float> timeV)				{m_TimeVelocity = timeV;}
		void						SetTimeColor		(std::vector<float> timeC)				{m_TimeColor = timeC;}
		void						SetTimeDir			(std::vector<float> timeD)				{m_TimeDir = timeD;}
		void						SetTimeSize			(std::vector<float> timeS)				{m_TimeSize = timeS;}
		void						SetGravity			(bool gravity)										{m_bGravity = gravity;}
		void            SetVelocity			(D3DXVECTOR3 velocity)						{m_vVelocity = velocity;}
		void						SetPointA				(Vect3f pointA)                   {m_PointA = pointA;}
		void						SetPointB				(Vect3f pointB)                   {m_PointB = pointB;}
		void						SetPointC				(Vect3f pointC)                   {m_PointC = pointC;}
		void						SetPointD				(Vect3f pointD)                   {m_PointD = pointD;}
		void						SetPosition			(const Vect3f position)						{m_Position = position;}
		void						SetAngle				(float angle)											{m_fAngle = angle;}
		

	protected:
		bool																			m_bIsOk;
    float																			m_fSize;
    float																			m_fLifetime;
    float																			m_fAge;
    D3DXCOLOR																	m_Color;
    D3DXVECTOR3																m_vPos;
    D3DXVECTOR3																m_vDir; 
		D3DXVECTOR3																m_vVelocity;
		bool																			m_bGravity;
		float																			m_fAngle;
		std::vector<D3DXCOLOR>										m_ColorVect;
		std::vector<D3DXVECTOR3>									m_DirVect;
		std::vector<D3DXVECTOR3>									m_VelocityVect;
		std::vector<float>												m_AngleVect;
		std::vector<float>												m_TimeAngle;
		std::vector<float>												m_TimeVelocity;
		std::vector<float>												m_TimeColor;
		std::vector<float>												m_TimeDir;	
		std::vector<float>												m_SizeVect;
		std::vector<float>												m_TimeSize;
		CBillboard																m_Billboard;
		Vect3f																		m_PointA;   //los 4 puntos que forman el Quad del Billboard
		Vect3f																		m_PointB;
		Vect3f																		m_PointC;
		Vect3f																		m_PointD;
		Vect3f																		m_Position; 
		float																			m_fAngleTotal;
};

#endif;