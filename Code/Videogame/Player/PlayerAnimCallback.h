#pragma once
#ifndef CPLAYER_ANIM_CALLBACK_H
#define CPLAYER_ANIM_CALLBACK_H

#include "Cal3d/model.h"
#include "Cal3d/animcallback.h"

class CPlayerAnimCallback :  public CalAnimationCallback
{
  public:
    CPlayerAnimCallback(void){}
    ~CPlayerAnimCallback() {}
    void AnimationUpdate(float anim_time,CalModel *model, void * userData);
    void AnimationComplete(CalModel *model, void * userData);
};

#endif

