#include "PhysicControllerReport.h"
#include "PhysicsManager.h"
#include "NxActor.h"
#include "../../Videogame/ZombieResources/ZombieControllerReport.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

NxControllerAction  CPhysicControllerReport::onShapeHit(const NxControllerShapeHit& hit)
{
  if(hit.shape)
  {
    NxCollisionGroup l_group = hit.shape->getGroup();
    //if(l_group==PX_CG_SCENARIO)
    //{
    //  CZombieControllerReport call;
    //  call.onShapeHit(hit);
    //}
    if(l_group==PX_CG_STATIC_ELEMENT_DENSITY)
    //if(l_group==0)
    {
      NxActor& l_actor = hit.shape->getActor();

      if(hit.dir.y==0.0f)
      {
        NxF32 l_coeff = l_actor.getMass() * hit.length * 10.0f;
        l_actor.addForceAtLocalPos(hit.dir*l_coeff, NxVec3(0,0,0), NX_IMPULSE);
      }
    }
  }
  return NX_ACTION_NONE;
}

NxControllerAction  CPhysicControllerReport::onControllerHit(const NxControllersHit& hit)
{
  CPhysicUserData* thisControllerUserdata = (CPhysicUserData*) hit.controller->getActor()->userData;
  CPhysicUserData* otherControllerUserdata= (CPhysicUserData*) hit.other->getActor()->userData;

  CZombieControllerReport call;
  call.onControllerHit(thisControllerUserdata->GetName(), otherControllerUserdata->GetName());

  //no empuja al objeto
//  return NX_ACTION_NONE;
  return NX_ACTION_PUSH;
}

