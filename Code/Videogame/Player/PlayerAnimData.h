#pragma once
#ifndef CPLAYER_ANIM_DATA_H
#define CPLAYER_ANIM_DATA_H

#include <string>

#define S_NO_ACTION      -1

#define S_DEATH           0
#define S_IDLE            1
#define S_JUMP            2
#define S_WALK            3
#define S_HIT             4
#define S_S_ATTACK        5
#define S_L_ATTACK        6
#define S_L_ATTACK_MOVING 7


#define NUMFRAMES_L_ATTACK  28
#define NUMFRAMES_S_ATTACK  71
#define NUMFRAMES_JUMP      30

#define END_ATTACKFRAME_L_ATTACK  12
#define END_ATTACKFRAME_S_ATTACK  12

#define START_JUMP_FRAME  7
#define END_JUMP_FRAME   18 

#define START_SOUND_FRAME_S_ATTACK  6


class CPlayerAnimData
{
  public:
    CPlayerAnimData(){}
    ~CPlayerAnimData() {}
    
    void  Init();

    bool  IsPlaying                (int _iAnimationId);
    

    int   GetCurrentAction         () { return m_iCurrentAction; }
    bool  GetCheckAttackCollision  () { return m_bCheckAttackCollision; }
    bool  GetIsPlayingSound        () { return m_bIsPlayingSound; }
    bool  GetIsJumping             () { return m_bIsJumping; }

    void  SetCurrentAction         (int _iAnimationId);
    void  SetCheckAttackCollision  (bool _bNewState);
    void  SetIsPlayingSound        (bool _bNewState) { m_bIsPlayingSound = _bNewState; }
    void  SetIsJumping             (bool _value);
    
    std::string m_sDebug;

  private:
    int   m_iCurrentAction;
    bool  m_bIsPlayingAnimation;
    bool  m_bIsPlayingSound;
    bool  m_bIsJumping;
    bool  m_bCheckAttackCollision; // indica si se debe gestionar la colisión de ataque
};

#endif

