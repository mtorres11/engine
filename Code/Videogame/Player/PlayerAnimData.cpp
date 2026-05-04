#include "PlayerAnimData.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

void CPlayerAnimData::Init()
{
  m_iCurrentAction = S_NO_ACTION;
  m_bIsPlayingAnimation = false;
  m_bCheckAttackCollision = false;
  m_bIsJumping = false;
}

bool CPlayerAnimData::IsPlaying(int _iAnimationId)
{
  if (m_iCurrentAction == _iAnimationId)
    return true;
  else
    return false;
}

void CPlayerAnimData::SetCurrentAction(int _iAnimationId)
{
  m_iCurrentAction = _iAnimationId;
}

void CPlayerAnimData::SetCheckAttackCollision(bool _bNewState)
{
  if (m_bCheckAttackCollision != _bNewState)
    m_bCheckAttackCollision = _bNewState;
}

void CPlayerAnimData::SetIsJumping(bool _value)
{
  if (m_bIsJumping != _value)
    m_bIsJumping = _value;
}
