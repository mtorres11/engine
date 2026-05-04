//----------------------------------------------------------------------------------
// CAnimatedInstanceModel class
// Author: Andrés
// Última modificación: Katherine y Andrea
// Description:
// Esta clase encapsulará a la clase CalModel del Cal3D.
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_ANIMATED_INSTANCE_MODEL_H_
#define INC_ANIMATED_INSTANCE_MODEL_H_

//------Forward declarations---------
class CAnimatedCoreModel;
class CRenderManager;
//----------------------------------

#include "cal3d/cal3d.h"
#include "Named/Named.h"
#include <d3dx9.h>

class CTexture;
class CEffectTechnique;

#define MAXBONES 40


class CAnimatedInstanceModel : public CNamed
{
	private:
		CalModel *								m_CalModel;
		CAnimatedCoreModel *			m_AnimatedCoreModel;
		std::vector<CTexture *>		m_TextureList;
		int												m_CurrentCycle;
		int												m_CurrentAnimationId;
		float											m_CurrentAnimationTime;
		LPDIRECT3DVERTEXBUFFER9		m_pVB;
		LPDIRECT3DINDEXBUFFER9		m_pIB;
		CEffectTechnique					*m_EffectTechnique;
		void											LoadTextures											();
    int                       m_iNumAnimations;
		

	public:
    CAnimatedInstanceModel();
		~CAnimatedInstanceModel();

		void											Render														(CRenderManager *RM);
		void											RenderModelByHardware							(CRenderManager *RM);
		void											RenderModelBySoftware							(CRenderManager *RM);
		void											Update														(float ElapsedTime);
		void											Initialize												(CAnimatedCoreModel *AnimatedCoreModel);
		void											InitD3D														(CRenderManager *RM);
		void											Destroy														();
    bool                      RemoveAction                      (int Id);
		void											ExecuteAction											(int Id, float Time);
		void											ExecuteAction											(int Id, float inTime, float outTime);
    void                      ExecuteActionBlocked              (int Id, float Time);
    void											BlendCycle												(int Id, float Time, float W= 1.0f);
		void											ClearCycle												(float Time);
		int												GetCurrentCycle										() const;
		int												GetCurrentAction									() const;
    int                       GetNumAnimations                  () const {return m_iNumAnimations;}
    CalModel *								GetModelCal3DHW                   ()       {return  m_CalModel;}
};

#endif

