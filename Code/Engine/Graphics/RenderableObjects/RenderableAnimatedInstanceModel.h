//----------------------------------------------------------------------------------
// RenderableAnimatedInstanceModel class
// Author: Katherine Capdevilla
// 
// Description:
// 
//----------------------------------------------------------------------------------

#pragma once

#ifndef RENDERABLE_ANIMATED_INSTANCE_MODEL_H_
#define RENDERABLE_ANIMATED_INSTANCE_MODEL_H_

#include "RenderableObjects/RenderableObject.h"

/*--- Forward declarations ---*/
class CAnimatedInstanceModel;
class CRenderManager;
//-----------------------------

class CRenderableAnimatedInstanceModel : public CRenderableObject
{
	protected:
		CAnimatedInstanceModel * m_AnimatedInstanceModel;

	public:
		CRenderableAnimatedInstanceModel(const std::string &Name, const std::string &CoreName);
		~CRenderableAnimatedInstanceModel();

    bool Init();
    void Done();
    void Render(CRenderManager *RM);
    void Update(float ElapsedTime);
    CAnimatedInstanceModel* GetAnimatedInstanceModel(){return m_AnimatedInstanceModel;}

    //void ExecuteAction(int Id, float Time);
};

#endif