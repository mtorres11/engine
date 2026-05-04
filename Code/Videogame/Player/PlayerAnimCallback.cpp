#include "PlayerAnimCallback.h"

#include "Base.h"
#include "PlayerAnimData.h"
#include "SoundManager.h"
#include "Cal3d/Cal3d.h"

//TODO: solo para debug, eliminar
#include "Math/MathUtils.h"
#include "Utils/BaseUtils.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

void CPlayerAnimCallback::AnimationUpdate(float anim_time, CalModel *model, void * userData)
{
  CPlayerAnimData* action = (CPlayerAnimData*)userData;
  float l_fDuration;
  int l_fCurrentFrame;

  switch(action->GetCurrentAction())
  {
     case S_L_ATTACK:
      l_fDuration = model->getCoreModel()->getCoreAnimation(S_L_ATTACK)->getDuration();
      //l_fFrameDuration = l_fDuration / (float)NUMFRAMES_L_ATTACK;
      l_fCurrentFrame = (int) (anim_time * NUMFRAMES_L_ATTACK / l_fDuration);
      //baseUtils::FormatSrting(action->m_sDebug, "PlayerAnimData> amim: %f | currentFrame: %d", anim_time, l_fCurrentFrame);
      if (l_fCurrentFrame <= END_ATTACKFRAME_L_ATTACK)
      {
        action->SetCheckAttackCollision(true);
      }
      else
      {
        action->SetCheckAttackCollision(false);
      }
      break;

    case S_S_ATTACK:
      l_fDuration = model->getCoreModel()->getCoreAnimation(S_S_ATTACK)->getDuration();
      l_fCurrentFrame = (int) (anim_time * NUMFRAMES_S_ATTACK / l_fDuration);
      //baseUtils::FormatSrting(action->m_sDebug, "PlayerAnimData> amim: %f | currentFrame: %d", anim_time, l_fCurrentFrame);

      if (l_fCurrentFrame == START_SOUND_FRAME_S_ATTACK)
      {
        if (!action->GetIsPlayingSound())
        {
          CORE->GetSoundManager()->PlaySound("swordSwing2");
          action->SetIsPlayingSound(true);
        }
      }

      if (l_fCurrentFrame <= END_ATTACKFRAME_S_ATTACK)
      {
        action->SetCheckAttackCollision(true);
      }
      else
      {
        action->SetCheckAttackCollision(false);
      }
      break;

    case S_JUMP:
      l_fDuration = model->getCoreModel()->getCoreAnimation(S_JUMP)->getDuration();
      l_fCurrentFrame = (int) (anim_time * NUMFRAMES_JUMP / l_fDuration);
      
      if (l_fCurrentFrame >= START_JUMP_FRAME && l_fCurrentFrame < END_JUMP_FRAME)
      {
        action->SetIsJumping(true);
      }
      else
      {
        action->SetIsJumping(false);
      }

      break;

  }
}

void CPlayerAnimCallback::AnimationComplete(CalModel *model, void * userData)
{
  CPlayerAnimData* action = (CPlayerAnimData*)userData;
  action->SetCurrentAction(S_NO_ACTION);
}
