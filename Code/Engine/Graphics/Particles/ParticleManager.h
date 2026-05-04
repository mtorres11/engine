//----------------------------------------------------------------------------------
// CParticleManager class
// Author: Andrea Cristobo
// Last Modification: 
// Description:
// Esta clase será donde se leerá el XML para la modificación de las partículas.
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_PARTICLE_MANAGER_H_
#define INC_PARTICLE_MANAGER_H_

#include <d3dx9.h>
#include <string>
#include "Base.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Logger/Logger.h"
#include <map>
#include "MapManager/MapManager.h"
#include "ParticleEmitter.h"
#include "Texture/Texture.h"

//----Forwards Declarations----
class CParticleEmitter;
class CTexture;
//-----------------------------

//structs
	struct SCOLOR
	{
		Vect4f			color1;
		Vect4f			color2;
		float				time;

		SCOLOR()
		: color1(1.f, 1.f, 1.f, 1.f),
			color2(1.f, 1.f, 1.f, 1.f),
			time(0)
		{}
	};

	struct SDIR
	{
		Vect3f			  dir1;
		Vect3f				dir2;
		float					time;

		SDIR()
		: dir1(1.f, 1.f, 1.f),
			dir2(1.f, 1.f, 1.f),
			time(0)
		{}
	};

	struct SINSTANCE
	{
		std::string		id;
		Vect3f				vPos1;
		Vect3f				vPos2;

		SINSTANCE()
		: id(""),
		  vPos1(1.f, 1.f, 1.f),
			vPos2(1.f, 1.f, 1.f)
		{}
	};

	struct STEXTURE
	{
		CTexture*   name;
		float				time;
	};

	struct SSIZE
	{
		float sizeMin;
		float sizeMax;
		float time;
	};

	struct SVELOCITY
	{
		Vect3f velocity;
		float time;
	};

	struct SANGLE
	{
		float angle;
		float time;
	};

	struct SINFO
	{
		std::string								sName;
		std::vector<SINSTANCE>		vInstance;
		std::vector<SCOLOR>				vColor;
		std::vector<SDIR>					vDir;
		std::vector<STEXTURE>			vTexture;
		std::vector<SSIZE>				vSize;
		std::vector<SVELOCITY>		vVelocity;
		std::vector<SANGLE>				vAngle;
		Vect3f										vpos;
		float											fMinSize;
		float											fMaxSize;
		float											fEmitRate1;
		float											fEmitRate2;
		float											fLifeMin;
		float											fLifeMax;
		bool              				bGravity;
		CTexture*									pTexture;

		SINFO()
		: vpos(1.f, 1.f, 1.f), 
			fMinSize(0.5f),
			fMaxSize(2.0f),
			fEmitRate1(1.f),
			fEmitRate2(5.f),
			fLifeMin(0.3f),
			fLifeMax(3.f)
			//pTexture(0)
		{}
	};

	struct SINSTEMITTER
	{
		int numInstTotal;
		int numInstDispo;
		int numInit;
	};

class CParticleManager
{
	public:
		CParticleManager(): m_vParticleEmitter(NULL) {}
		~CParticleManager() { Done(); }

		void 											Done            				();
		bool 											Init										(const std::string & fileName);
		bool 											IsOk                		() const														{ return m_bIsOk; }

		bool											LoadXML             		(const std::string & fileName); 
		void											Release									();
		void											Render									();
		void											Update									(float elapsedTime);
		void											Init										(CRenderManager* rm);
		void											SetProperties						(int cont, int i);
		float											RandomNumber						(float fMin, float fMax);
		CParticleEmitter*					GetEmitter							(int id)														{ return m_vParticleEmitter[id]; }
		void											CalculateColor					(int cont);
		void											CalculateDir						(int cont);
		void											CalculateSize						(int cont);
		void											CalculateVelocity   		(int cont);
		void											CalculateAngle					(int cont);
		D3DXVECTOR3								CalculatePos1						(int cont, int i);
		D3DXVECTOR3								CalculatePos2						(int cont, int i);
		void                      SetPEmitterByInstance		();
		CParticleEmitter *        GetPEmitterByInstance   (std::string name);


		//Gets para GUI
		Vect3f 										GetPos									()																	{ return m_stInfo.vpos; } 
		Vect3f 										GetPos1									()																	{ return m_stInstance.vPos1; } 
		Vect3f 										GetPos2									()																	{ return m_stInstance.vPos2; } 
		Vect4f 										GetColor1								()																	{ return m_stColor.color1; }
		Vect4f 										GetColor2								()																	{ return m_stColor.color2; }
		float  										GetMinSize							()																	{ return m_stInfo.fMinSize; }
		float  										GetMaxSize							()																	{ return m_stInfo.fMaxSize; }
		Vect3f 										GetDir1									()																	{ return m_stDir.dir1; }
		Vect3f 										GetDir2									()																	{ return m_stDir.dir2; }
		float  										GetMinEmitRate					()																	{ return m_stInfo.fEmitRate1; }
		float  										GetMaxEmitRate					()																	{ return m_stInfo.fEmitRate2; }
		float  										GetMinLife							()																	{ return m_stInfo.fLifeMin; }
		float  										GetMaxLife							()																	{ return m_stInfo.fLifeMax; }
		CTexture*									GetTexture							()																	{ return m_stInfo.pTexture; }

		//Gets para GUI
		void 											SetPos									(Vect3f pos)												{ m_stInfo.vpos = pos; } 
		void 											SetPos1									(Vect3f pos)												{ m_stInstance.vPos1 = pos; } 
		void 											SetPos2									(Vect3f pos)												{ m_stInstance.vPos2 = pos; } 
		void 											SetColor1								(Vect4f col1)												{ m_stColor.color1 = col1; }
		void 											SetColor2								(Vect4f col2)												{ m_stColor.color2 = col2; }
		void 											SetMinSize							(float minSize)											{ m_stInfo.fMinSize = minSize; }
		void 											SetMaxSize							(float maxSize)											{ m_stInfo.fMaxSize = maxSize; }
		void 											SetDir1									(Vect3f dir1)												{ m_stDir.dir1 = dir1; }
		void 											SetDir2									(Vect3f dir2)												{ m_stDir.dir2 = dir2; }
		void 											SetMinEmitRate					(float minEmit)											{ m_stInfo.fEmitRate1 = minEmit; }
		void 											SetMaxEmitRate					(float maxEmit)											{ m_stInfo.fEmitRate2 = maxEmit; }
		void 											SetMinLife							(float minLife)											{ m_stInfo.fLifeMin = minLife; }
		void 											SetMaxLife							(float maxLife)											{ m_stInfo.fLifeMax = maxLife; }
		void 											SetTexture							(CTexture* texture)									{ m_stInfo.pTexture = texture; }

  private:
		bool																	 m_bIsOk;
		std::vector<CParticleEmitter*>		  	 m_vParticleEmitter;
		std::vector<SINFO>										 m_vInfo;
		SINFO																	 m_stInfo;
		SCOLOR																 m_stColor;
		SDIR																	 m_stDir;
		SINSTANCE														   m_stInstance;
		STEXTURE															 m_stTexture;
		SSIZE																	 m_stSize;
		SVELOCITY															 m_stVelocity;
		SANGLE																 m_stAngle;
		std::vector<D3DXCOLOR>								 m_vColor;
		std::vector<float>										 m_vTimeColor;
		std::vector<D3DXVECTOR3>       				 m_vDir;
		std::vector<float>										 m_vTimeDir;
		std::vector<float>										 m_vSize;
		std::vector<float>										 m_vTimeSize;
		std::vector<D3DXVECTOR3>							 m_vVelocity;
		std::vector<float>										 m_vTimeVel;
		std::vector<float>										 m_vAngle;
		std::vector<float>										 m_vTimeAngle;
		std::vector<D3DXVECTOR3>							 m_vInstance;
		std::map<std::string, SINSTEMITTER>	   m_vPEmitterByInstance; 
		SINSTEMITTER												   m_stNumInstEmitter;
};

#endif