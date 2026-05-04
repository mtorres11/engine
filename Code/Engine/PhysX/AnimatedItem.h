#pragma once
#ifndef INC_ANIMATEDITEM_H_
#define INC_ANIMATEDITEM_H_

class CRenderManager;
class CPhysicActor;
class CRenderableObject;

class CAnimatedItem
{
public:
	CAnimatedItem(void);
	~CAnimatedItem(void);
	void Update();
	void Render(CRenderManager*);
private:
	CPhysicActor* m_pPhysXActor;
	CRenderableObject* m_pRenderableObject;
};
#endif