#pragma once

#include "Cal3d/global.h"
#include "Cal3d/model.h"
#include "Cal3d/animcallback.h"

class CZombieVomAnimationReport :  public CalAnimationCallback
{
public:
  CZombieVomAnimationReport(void){}
  virtual ~CZombieVomAnimationReport() {}
  virtual void AnimationUpdate(float anim_time,CalModel *model, void * userData);
  virtual void AnimationComplete(CalModel *model, void * userData);
};
