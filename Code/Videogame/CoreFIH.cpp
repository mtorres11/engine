#include "CoreFIH.h"

#include "Player/Player.h"
#include "ZombieManager/ZombieManager.h"
#include "GUI/HealthIndicator.h"

#include "Spirit/Spirit.h"

#include "ScriptManager.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CCoreFIH::Init()
{
  m_pZombieManager = new CZombieManager();
  m_bIsOk = (m_pZombieManager != NULL);
  
  if (!m_bIsOk){
    Release();
  }
  else
  {
    m_pZombieManager->Init();
  }
  
  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Init Spirit
//----------------------------------------------------------------------------
bool CCoreFIH::InitSpirit()
{
  m_pSpirit = new CSpirit();
  if(m_pSpirit)
  {
    m_pSpirit->Init();
  }
  else 
  {
    Release();
    return false;
  }
  return true;
}

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CCoreFIH::InitPlayer(std::string _file)
{
  m_pPlayer = new CPlayer();
  m_pPlayer->Init(_file);

  m_bIsOk = (m_pPlayer != NULL);
  if (!m_bIsOk)
  {
    Release();
  }
  
  m_pHealthIndicator = new CHealthIndicator();
  m_bIsOk = m_pHealthIndicator->Init(m_pPlayer->GetHealth());
  
  Register2LUA();

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CCoreFIH::Done()
{
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CCoreFIH::Release()
{
//free memory
  CHECKED_DELETE(m_pPlayer);
  CHECKED_DELETE(m_pZombieManager);
  CHECKED_DELETE(m_pHealthIndicator);
  ReleaseSpirit();
}

//----------------------------------------------------------------------------
// Free memory spirit
//----------------------------------------------------------------------------
void CCoreFIH::ReleaseSpirit()
{
  CHECKED_DELETE(m_pSpirit);
}

//----------------------------------------------------------------------------
// registro las clases del juego y sus instancias
//----------------------------------------------------------------------------
void CCoreFIH::Register2LUA()
{
  lua_State * l_LS = CORE->GetScriptManager()->GetLuaState();
	using namespace luabind;
  
  // Player ---------------------------------------------
  module(l_LS)
    [
      // registramos la clase CGUIManager
      class_<CPlayer, CPersonaje>("CPlayer")
      // registramos su constructor
      .def(constructor<>())
      .def("hit", &CPlayer::Hit)
      .def("c_get_health", &CPlayer::GetHealth)
      .def("c_set_health", &CPlayer::SetHealth)
      .def("health_up", &CPlayer::HealthUp)
      .def("revive", &CPlayer::Revive)
      .def("collide_enemies", &CPlayer::CollideEnemies)
      .def("set_state", &CPlayer::SetSate)
      .def("re_start", &CPlayer::ReStart)
      .def("jump", &CPlayer::Jump)
      .def("cancel_jump", &CPlayer::CancelJump)
      .def("fall", &CPlayer::Fall)
      .def("res_pos", &CPlayer::RestartPosition)
      .def("get_pos", &CPlayer::GetPosition)
      .def("set_pos", &CPlayer::SetPosition)
      .def("go_back", &CPlayer::GoBack)
    ];

  globals(l_LS)["player"] = m_pPlayer;


  // Game GUI ---------------------------------------------
  module(l_LS)
    [
      // registramos la clase CGUIManager
      class_<CHealthIndicator>("CHealthIndicator")
      // registramos su constructor
      .def(constructor<>())
      .def("hit", &CHealthIndicator::Hit)
      .def("health_up", &CHealthIndicator::HealthUp)
      .def("max_health", &CHealthIndicator::MaxHealth)
    ];

  globals(l_LS)["gui_HI"] = m_pHealthIndicator;
}



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------