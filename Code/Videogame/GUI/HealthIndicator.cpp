#include "HealthIndicator.h"

#include "Base.h"
#include "GUIManager.h"
#include "Image.h"
#include "AnimatedImage.h"
#include "ScriptManager.h"

#include <math.h>

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

// TODO: elementos escritos en codigo, seria mejor cargarlos de un XML o algo asi
bool CHealthIndicator::Init(float _fMaxHealth)
{
  m_bIsOk = AssignGUIElements();

  m_fAnimSpeed = 0.07f;
  m_fMaxHealth = _fMaxHealth;
  
  return m_bIsOk;
}

void CHealthIndicator::Update()
{

}

void CHealthIndicator::Hit(float health, float damage)
{
  m_pHealthBG->SetAlpha(1.0f);
  m_pHealthBG->SetVisible(true);
  m_pHealthBG->FadeOut(2.f, 1.f);

  m_pHealthAnimation->SetAlpha(1.0f);
  m_pHealthAnimation->SetVisible(true);
  m_pHealthAnimation->FadeOut(2.f, 1.f);

  float fFrame = (m_usMaxHealthFrame * (health - damage)) / m_fMaxHealth;
  int16 finalFrame =  (int16) ceil(fFrame);
  if (finalFrame < 0)
    finalFrame = 0;
  m_pHealthAnimation->PlayAnimation(m_fAnimSpeed, false, false, finalFrame);
}

void CHealthIndicator::HealthUp(float health, float healthUp)
{
  m_pHealthBG->SetAlpha(1.0f);
  m_pHealthBG->SetVisible(true);
  m_pHealthBG->FadeOut(2.f, 1.f);

  m_pHealthAnimation->SetAlpha(1.0f);
  m_pHealthAnimation->SetVisible(true);
  m_pHealthAnimation->FadeOut(2.f, 1.f);

  float fFrame = (m_usMaxHealthFrame * (health + healthUp)) / m_fMaxHealth;
  int16 finalFrame = (int16) ceil(fFrame);
  if (finalFrame > m_usMaxHealthFrame)
    finalFrame = m_usMaxHealthFrame;
  m_pHealthAnimation->PlayAnimation(m_fAnimSpeed, false, true, finalFrame);
}

/// <summary>
/// Establece el indicador de energía en el máximo
/// </summary>
void CHealthIndicator::MaxHealth()
{
  m_pHealthBG->SetAlpha(1.0f);
  m_pHealthBG->SetVisible(true);
  m_pHealthBG->FadeOut(2.f, 1.f);

  m_pHealthAnimation->SetAlpha(1.0f);
  m_pHealthAnimation->SetVisible(true);
  m_pHealthAnimation->FadeOut(2.f, 1.f);

  m_pHealthAnimation->GotoAndStop(m_usMaxHealthFrame);
}

/// <summary>
/// Establece el indicador de energía en el máximo
/// </summary>
bool CHealthIndicator::AssignGUIElements()
{
  CAnimatedImage* l_HealthAnim = (CAnimatedImage*)CORE->GetGUIManager()->GetGUIElement("gui_game_health");
  if (l_HealthAnim)
  {
    m_pHealthAnimation = l_HealthAnim;
    m_usMaxHealthFrame = m_pHealthAnimation->NumFrames() - 1;
  }
  else
  {
    return false;
  }

  m_pHealthBG = (CImage*)CORE->GetGUIManager()->GetGUIElement("gui_game_health_bg");
  if (m_pHealthBG == NULL)
  {
      return false;
  }

  return true;
}
