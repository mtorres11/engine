//----------------------------------------------------------------------------------
// CParticleEmitter class
// Author: Andrea Cristobo
// Last Modification: 
// Description:
// Esta clase será donde se crearán las partículas.
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_PARTICLE_EMITTER_H_
#define INC_PARTICLE_EMITTER_H_

#include <string>
#include "RecyclingArray/RecyclingArray.h"
#include "Particle.h"
#include <d3dx9.h>
#include "Vertex/VertexType.h"
#include "XML/XMLTreeNode.h"
#include <map>
#include <vector>
#include "BillBoard/Billboard.h"

class CTexture;

class CParticleEmitter
{
	public:
		CParticleEmitter();
		~CParticleEmitter() { Done(); }

		void 																			Done          				  ();
		bool 																			Init										();
		bool 																			IsOk          				  () const											 {return m_bIsOk;}
		
		void																			Update									(float fElapsedTime);
		void																			Init										(CRenderManager* rm/*, const std::string& texureFileName*/);
		void																			DeInit									();
		bool																			Render									();
		bool																			LoadXML									(const std::string &fileName);
		float																			RandomNumber						(float fMin, float fMax);

		//Gets
		D3DXVECTOR3																GetPos									()																{return m_vPos;}
		float																			GetMinEmitRate					()																{return m_fMinEmitRate;}
		float																			GetMaxEmitRate					()																{return m_fMaxEmitRate;}
		D3DXCOLOR																	GetColor1								()																{return m_Color1;}
		D3DXCOLOR																	GetColor2								()																{return m_Color2;}
		float																			GetMinSize							()																{return m_fMinSize;}
		float																			GetMaxSize							()																{return m_fMaxSize;}
		D3DXVECTOR3 															GetSpawnDir1						()																{return m_vSpawnDir1;}
		D3DXVECTOR3 															GetSpawnDir2						()																{return m_vSpawnDir2;}
		float																			GetNumNewPartsExcess		()																{return m_fNumNewPartsExcess;}
		CTexture* 																GetTexture							()																{return m_pTexture;}
		LPDIRECT3DDEVICE9													GetDevice								()																{return m_pd3dDevice;}
		LPDIRECT3DVERTEXBUFFER9										GetVBParticles					()																{return m_vbParticles;}
		uint32																		GetTotalElements				()																{return m_Particles.GetTotalElements();}
		uint32																		GetFreeElements					()																{return m_Particles.GetNumFreeElements();}
		uint32																		GetUsedElements					()																{return m_Particles.GetNumUsedElements();}
		std::string																GetNameEmitter					()																{return m_sNameEmitter;}
		float																			GetMinLife							()																{return m_fLifeMin;}
		float																			GetMaxLife							()																{return m_fLifeMax;}
		D3DXVECTOR3 															GetPos1									()																{return m_vPos1;}
		D3DXVECTOR3 															GetPos2									()																{return m_vPos2;}
		std::string																GetName									()																{return m_sName;}

		//Sets
		void 																			SetPos									(D3DXVECTOR3 pos)									{m_vPos = pos;}
		void 																			SetMinEmitRate					(float minEmitRate)								{m_fMinEmitRate = minEmitRate;}
		void 																			SetMaxEmitRate					(float maxEmitRate)								{m_fMaxEmitRate = maxEmitRate;}
		void 																			SetColor1								(D3DXCOLOR color)									{m_Color1 = color;}
		void 																			SetColor2								(D3DXCOLOR color)									{m_Color2 = color;}
		void 																			SetMinSize							(float minSize)										{m_fMinSize = minSize;}
		void 																			SetMaxSize							(float maxSize)										{m_fMaxSize = maxSize;}
		void 																			SetSpawnDir1						(D3DXVECTOR3 spawnDir)						{m_vSpawnDir1 = spawnDir;}
		void 																			SetSpawnDir2						(D3DXVECTOR3 spawnDir)						{m_vSpawnDir2 = spawnDir;}
		void																			SetNumNewPartsExcess		(float partsExcess)								{m_fNumNewPartsExcess = partsExcess;}
		void																			SetTexture							(CTexture* texture);
		void																			SetDevice								(LPDIRECT3DDEVICE9 device)				{m_pd3dDevice = device;}
		void																			SetVBSize								(uint32 numPart)									{m_iVBSize = numPart;}
		void																			SetNameEmitter					(std::string name)								{m_sNameEmitter = name;}
		void 																			SetMinLife							(float minLife)										{m_fLifeMin = minLife;}
		void 																			SetMaxLife							(float maxLife)										{m_fLifeMax = maxLife;}
		void 																			SetPos1									(D3DXVECTOR3 pos1)								{m_vPos1 = pos1;}
		void 																			SetPos2									(D3DXVECTOR3 pos2)								{m_vPos2 = pos2;}
		void																			SetColorV								(std::vector<D3DXCOLOR> color)		{m_vectColor = color;}	
		void																			SetColorTime						(std::vector<float> time)					{m_vColorTime = time;}
		void																			SetDirV									(std::vector<D3DXVECTOR3> dir)	  {m_vectDir = dir;}	
		void																			SetDirTime							(std::vector<float> time)					{m_vDirTime = time;}
		void																			SetSizeV								(std::vector<float> size)					{m_vectSize = size;}	
		void																			SetSizeTime							(std::vector<float> time)					{m_vSizeTime = time;}
		void																			SetVelocityV						(std::vector<D3DXVECTOR3> v)			{m_vectVelocity = v;}	
		void																			SetVelocityTime					(std::vector<float> time)					{m_vVelocityTime = time;}
		void																			SetAngleV								(std::vector<float> angle)				{m_vectAngle = angle;}	
		void																			SetAngleTime						(std::vector<float> time)					{m_vAngleTime = time;}
		void																			SetGravity							(bool gravity)										{m_bGravity = gravity;}
		void																			SetVelocity							(D3DXVECTOR3 velocity)						{m_vVelocity = velocity;}
		void																			SetName									(std::string name)								{m_sName = name;}

	private:
		bool																	    m_bIsOk;
		D3DXVECTOR3																m_vPos;
		D3DXVECTOR3																m_vPos1;
		D3DXVECTOR3																m_vPos2;
		float																			m_fMinEmitRate;
		float																			m_fMaxEmitRate;
		D3DXCOLOR																	m_Color1;
		D3DXCOLOR    															m_Color2;
		float        															m_fMinSize;
		float        															m_fMaxSize;
		D3DXVECTOR3  															m_vSpawnDir1;
		D3DXVECTOR3  															m_vSpawnDir2;
		float																			m_fLifeMin;
		float																			m_fLifeMax;
		bool																			m_bGravity;
		D3DXVECTOR3																m_vVelocity;
		float																			m_fAngle;
		float																			m_fNumNewPartsExcess;
		std::string																m_sTexture;
		LPDIRECT3DDEVICE9													m_pd3dDevice;
		LPDIRECT3DVERTEXBUFFER9										m_vbParticles;
		CRecyclingArray<CParticle>								m_Particles;
		uint32																		m_iVBSize;
		LPDIRECT3DTEXTURE9												m_texParticle;
		std::string																m_sNameEmitter;
		CTexture*																	m_pTexture;
		std::vector<D3DXCOLOR>										m_vectColor;
		std::vector<float>												m_vColorTime;
		std::vector<D3DXVECTOR3>       						m_vectDir;
		std::vector<float>												m_vDirTime;
		std::vector<float>												m_vectSize;
		std::vector<float>												m_vSizeTime;
		std::vector<D3DXVECTOR3>									m_vectVelocity;
		std::vector<float>												m_vVelocityTime;
		std::vector<float>												m_vectAngle;
		std::vector<float>												m_vAngleTime;
		CBillboard																m_Billboard;
		std::string																m_sName;
};

#endif;