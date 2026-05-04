#include "AnimatedItem.h"
#include "RenderManager.h"
#include "Actor/PhysicActor.h"
#include "RenderableObjects/RenderableObject.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CAnimatedItem::CAnimatedItem(void)
:m_pPhysXActor(0),m_pRenderableObject(0)
{}

CAnimatedItem::~CAnimatedItem(void)
{
	CHECKED_DELETE(m_pPhysXActor);
	CHECKED_DELETE(m_pRenderableObject);
}


void CAnimatedItem::Update()
{}

void CAnimatedItem::Render(CRenderManager* rm)
{}
