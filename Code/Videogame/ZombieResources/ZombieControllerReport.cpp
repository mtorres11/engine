#include "ZombieControllerReport.h"
#include "ZombieResources.h"
#include "NxActor.h"
#include "../ZombieManager/ZombieManager.h"
#include "../CoreFIH.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

void CZombieControllerReport::onShapeHit(const NxControllerShapeHit& hit)
{

}

void CZombieControllerReport::onControllerHit(const std::string &controller, const std::string &other)
{
  CCoreFIH* core = CCoreFIH::GetSingletonPtr();
  CZombieManager*zm = core->GetZombieManager();
  CZombieUserData* userdata = 0;

  //TODO..comprobar con quien choca para golpear.+ QUITAR MI_AMIGO
  if(!((controller.compare("mi_amigo")) && (other.compare("mi_amigo"))))
  {
    if(controller.compare("mi_amigo"))
    {
      CZombie* zombie1 = zm->GetResource(controller);
      userdata = zombie1->GetUserData();
      userdata->SetIsCollided(true);
      zombie1->SetUserData(userdata);
    }
    if(other.compare("mi_amigo"))
    {
      CZombie* zombie2  = zm->GetResource(other);
      userdata = zombie2->GetUserData();
      userdata->SetIsCollided(true);
      zombie2->SetUserData(userdata);
    }
  }
    //CZombieUserData* controllerUserData = (CZombieUserData*)controller;
  //controllerUserData->SetIsCollided(true);
}

