#include "ScriptManager.h"
#include "ZombieAnimationReport.h"
#include "Cal3d/Cal3d.h"
#include "PhysicsManager.h"
#include "CoreFIH.h"
#include "ZombieManager/ZombieManager.h"
#include "ZombieResources.h"
#include "Utils/BaseUtils.h"
#include "../Zombie/Zombie.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

void CZombieAnimationReport::AnimationUpdate(float anim_time,CalModel *model, void * userData)
{
  CZombieUserData* accion = (CZombieUserData*)userData;
  CZombie* z = CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetResource(accion->GetName().c_str());
  if(!z->IsDead())
  {
    CZombieUserData* userdata = (CZombieUserData*)userData;
    if(userdata->GetCurrentAction() == CZombie::ATACANDO)
    {
      std::string l_scriptAtaque;
      float l_fDuracionTotal = model->getCoreModel()->getCoreAnimation( CZombie::ATACANDO )->getDuration();
      baseUtils::FormatSrting(l_scriptAtaque, "ataque_zombie(\"%s\", %5f, %5f)",userdata->GetName().c_str(),anim_time, l_fDuracionTotal);
      CORE->GetScriptManager()->RunCode(l_scriptAtaque);
    }

    if(userdata->GetCurrentAction() == CZombie::MURIENDO)
    {
      float l_fDuracionTotal = model->getCoreModel()->getCoreAnimation( CZombie::MURIENDO )->getDuration();
      if (l_fDuracionTotal == anim_time)
      {
        userdata->SetIsActionFinalized( CZombie::MURIENDO );
      }
    }
  }
  else
  {
    if ((z->GetState() != CZombie::MURIENDO) && (z->GetState() != CZombie::MUERTO))
    {
      z->SetState(CZombie::MURIENDO);
      z->SetFaseEstado(0);
    }
  }
}

void CZombieAnimationReport::AnimationComplete(CalModel *model, void * userData)
{
  CZombieUserData* accion = (CZombieUserData*)userData;
  if(accion->GetCurrentAction() == CZombie::ATACANDO)
  {
    accion->SetIsActionFinalized( CZombie::ATACANDO );
  }
  //sin relevancia
  if(accion->GetCurrentAction() == CZombie::MURIENDO)
  {
    accion->SetIsActionFinalized( CZombie::MURIENDO );
  }

  if(accion->GetCurrentAction() == CZombie::RECIBIENDOGOLPE)
  {
    CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetResource(accion->GetName().c_str())->SetState(CZombie::RECIBIENDOGOLPE);
    accion->SetIsActionFinalized( CZombie::RECIBIENDOGOLPE );
  }

  //CPhysicUserData* user = (CPhysicUserData*)userData;
  //std::string l_sName = user->GetName();
  //user->SetIsCollided(true);
}
