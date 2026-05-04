#include "ScriptManager.h"
#include "ZombieBossAnimationReport.h"
#include "Cal3d/Cal3d.h"
#include "PhysicsManager.h"
#include "CoreFIH.h"
#include "ZombieManager/ZombieManager.h"
#include "ZombieResources.h"
#include "Utils/BaseUtils.h"
#include "../Zombie/Zombie.h"
#include "../ZombieBoss/ZombieBoss.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

void CZombieBossAnimationReport::AnimationUpdate(float anim_time,CalModel *model, void * userData)
{
  CZombieUserData* userdata = (CZombieUserData*)userData;

  //if(userdata->GetCurrentAction() == CZombieVomitador::VOM_SALIDA)
  //{
  //  float l_fDuracionTotal = model->getCoreModel()->getCoreAnimation( CZombieVomitador::VOM_SALIDA - OFFSET_ANIMATIONS )->getDuration();

  //  if ((l_fDuracionTotal - 0.2f) <= anim_time)
  //  {
  //    userdata->SetIsActionFinalized( CZombieVomitador::VOM_SALIDA);
  //  }
  //}
}

void CZombieBossAnimationReport::AnimationComplete(CalModel *model, void * userData)
{
  CZombieUserData* accion = (CZombieUserData*)userData;
  //if(accion->GetCurrentAction() == CZombieVomitador::VOM_APARECIENDO)
  //{
  //  accion->SetIsActionFinalized( CZombieVomitador::VOM_APARECIENDO);
  //}
}
