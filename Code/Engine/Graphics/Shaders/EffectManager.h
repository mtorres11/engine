//----------------------------------------------------------------------------------
// CEffectManager class
// Author: Andrea Cristobo
//
// Description:
// Mánager de efectos dónde tendremos almacenados los diferentes efectos que 
// cargaremos mediante un fichero .xml
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_EFFECT_MANAGER_H_
#define INC_EFFECT_MANAGER_H_

#include "Base.h"
#include "MapManager/MapManager.h"
#include <string>
#include "Object3D/Object3D.h"
#include "Math/Matrix44.h"
#include <d3dx9.h>
#include "EffectTechnique.h"

class CEffectManager : public CMapManager<CEffectTechnique>
{
	private:
		typedef std::map<int,std::string>							TDefaultTechniqueEffectMap;
		std::string                                   m_FileName;
		TDefaultTechniqueEffectMap										m_DefaultTechniqueEffectMap;
		Mat44f																				m_WorldMatrix;
		Mat44f																				m_ProjectionMatrix;
		Mat44f																				m_ViewMatrix;
		Mat44f																				m_ViewProjectionMatrix;
		Mat44f																				m_LightViewMatrix;
		Mat44f																				m_ShadowProjectionMatrix;
		Vect3f																				m_CameraEye;
		CMapManager<CEffect>													m_Effects;
		CEffectTechnique*															m_StaticMeshTechnique;
		CEffectTechnique*															m_AnimatedModelTechnique;

	public:
		CEffectManager();
		~CEffectManager();

		const Mat44f&					GetWorldMatrix																() const;
		const Mat44f&					GetProjectionMatrix														() const;
		const Mat44f&					GetViewMatrix																	() const;
		const Mat44f&					GetViewProjectionMatrix												();
		const Vect3f&					GetCameraEye																	();
		const Mat44f&					GetLightViewMatrix														() const;
		const Mat44f&					GetShadowProjectionMatrix											();
		void									ActivateCamera																(const Mat44f &ViewMatrix, const Mat44f &ProjectionMatrix,	const Vect3f &CameraEye);
		void									SetWorldMatrix																(const Mat44f &Matrix);
		void									SetProjectionMatrix														(const Mat44f &Matrix);
		void									SetViewMatrix																	(const Mat44f &Matrix);
		void									SetViewProjectionMatrix												(const Mat44f &ViewProjectionMatrix);
		void									SetLightViewMatrix														(const Mat44f &Matrix);
		void									SetShadowProjectionMatrix											(const Mat44f &Matrix);
		void									SetCameraEye																	(const Vect3f &CameraEye);
		bool									Load																					(const std::string &FileName);
		void									Reload																				();
		std::string						GetTechniqueEffectNameByVertexDefault					(unsigned short VertexType);
		size_t								GetMaxLights																	() const;
		CEffect *							GetEffect																			(const std::string &Name);
		CEffectTechnique*			GetEffectTechnique														(const std::string &Name);
		CEffectTechnique*			GetStaticMeshTechnique												() const;
		void									SetStaticMeshTechnique												(CEffectTechnique *StaticMeshTechnique);
		CEffectTechnique*			GetAnimatedModelTechnique											() const;
		void									SetAnimatedModelTechnique											(CEffectTechnique *AnimatedModelTechnique);
		void									CleanUp																				();
};

#endif