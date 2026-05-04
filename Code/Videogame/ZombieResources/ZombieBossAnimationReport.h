#pragma once

#include "Cal3d/global.h"
#include "Cal3d/model.h"
#include "Cal3d/animcallback.h"

class CZombieBossAnimationReport :  public CalAnimationCallback
{
public:
  CZombieBossAnimationReport(void){}
  virtual ~CZombieBossAnimationReport() {}
  virtual void AnimationUpdate(float anim_time,CalModel *model, void * userData);
  virtual void AnimationComplete(CalModel *model, void * userData);
};
