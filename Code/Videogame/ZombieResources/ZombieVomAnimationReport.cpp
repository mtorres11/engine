#include "ScriptManager.h"
#include "ZombieVomAnimationReport.h"
#include "Cal3d/Cal3d.h"
#include "PhysicsManager.h"
#include "CoreFIH.h"
#include "ZombieManager/ZombieManager.h"
#include "ZombieResources.h"
#include "Utils/BaseUtils.h"
#include "../Zombie/Zombie.h"
#include "../ZombieVomitador/ZombieVomitador.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

void CZombieVomAnimationReport::AnimationUpdate(float anim_time,CalModel *model, void * userData)
{
  CZombieUserData* userdata = (CZombieUserData*)userData;


  //if(userdata->GetCurrentAction() == CZombieVomitador::VOM_APARECIENDO)
  //{
  //  if((anim_time > 2.0f)&&(anim_time < 2.5f))
  //    userdata->SetIsCollided(true);
  //}

  if(userdata->GetCurrentAction() == CZombieVomitador::VOM_SALIDA)
  {
    float l_fDuracionTotal = model->getCoreModel()->getCoreAnimation( CZombieVomitador::VOM_SALIDA - OFFSET_ANIMATIONS )->getDuration();

    if ((l_fDuracionTotal - 0.2f) <= anim_time)
    {
      userdata->SetIsActionFinalized( CZombieVomitador::VOM_SALIDA);
    }
  }

  if(userdata->GetCurrentAction() == CZombieVomitador::VOM_MURIENDO)
    {
      float l_fDuracionTotal = model->getCoreModel()->getCoreAnimation( CZombieVomitador::VOM_MURIENDO - OFFSET_ANIMATIONS )->getDuration();
      if (l_fDuracionTotal == anim_time)
      {
        userdata->SetIsActionFinalized( CZombieVomitador::VOM_MURIENDO );
      }
    }
}

void CZombieVomAnimationReport::AnimationComplete(CalModel *model, void * userData)
{
  CZombieUserData* accion = (CZombieUserData*)userData;
  if(accion->GetCurrentAction() == CZombieVomitador::VOM_APARECIENDO)
  {
    accion->SetIsActionFinalized( CZombieVomitador::VOM_APARECIENDO);
  }

  if(accion->GetCurrentAction() == CZombieVomitador::VOM_DISPARANDO)
  {
    accion->SetIsActionFinalized( CZombieVomitador::VOM_DISPARANDO );
  }

  if(accion->GetCurrentAction() == CZombieVomitador::VOM_RECIBEGOLPE)
  {
    accion->SetIsActionFinalized( CZombieVomitador::VOM_RECIBEGOLPE);
  }
}
