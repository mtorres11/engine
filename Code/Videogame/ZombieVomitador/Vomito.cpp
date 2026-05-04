#include "Vomito.h"

#include "Math/Matrix44.h"
#include <vector>
#include "Utils/BaseUtils.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "RenderablePhysX/RenderablePhysXManager.h"
#include "RenderablePhysX/RenderablePhysXObject.h"

#include "Particles/ParticleManager.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/Particle.h"

#include "CoreFIH.h"
#include "ZombieManager/ZombieManager.h"
#include "Player/Player.h"
#include "CharacterController/PhysicController.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "ScriptManager.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

#define DANYOVOMITO 15.0f

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CVomito::Init()
{
	m_bIsOk = Inherited::Init();
  SetActive(false);
  SetChocado(false);

	m_pEmisor = CORE->GetParticleManager()->GetPEmitterByInstance("pVomito");
	m_pEmisor2 = CORE->GetParticleManager()->GetPEmitterByInstance("pVomito2");

  assert(m_pEmisor);
	assert(m_pEmisor2);
  m_bIsOk = (m_pEmisor != 0);

  if (!m_bIsOk){
    Release();
  }
  m_bIsOk = (m_pEmisor2 != 0);

  if (!m_bIsOk){
    Release();
  }

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CVomito::Done()
{
	Inherited::Done();			//Parent class Done
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CVomito::Release()
{
//free memory
}

void CVomito::Update(float elapsedTime, float _size, float _speed)
{
  //cae
  SetPosicionActual(Vect3f(m_V3PosicionActual.x, m_V3PosicionActual.y - (0.8f * elapsedTime), m_V3PosicionActual.z));
  SetPosicionActual(m_V3PosicionActual + (m_V3Desplazamiento * _speed * elapsedTime));

  Vect3f v = CORE->GetParticleManager()->GetPos();
  if(m_pEmisor)
  {
    m_pEmisor->SetPos1(D3DXVECTOR3(m_V3PosicionActual.x + m_V3Desplazamiento.x * _size + (_size * 0.5f), m_V3PosicionActual.y, m_V3PosicionActual.z + m_V3Desplazamiento.z * _size - (_size * 1.5f)));
    m_pEmisor->SetPos2(D3DXVECTOR3(m_V3PosicionActual.x + m_V3Desplazamiento.x * _size + (_size * 0.5f), m_V3PosicionActual.y, m_V3PosicionActual.z + m_V3Desplazamiento.z * _size + (_size * 1.5f)));
  }
  
  if((m_pEmisor) && (m_pEmisor2))
  {  
	  m_pEmisor2->SetPos1(m_pEmisor->GetPos1());
	  m_pEmisor2->SetPos2(m_pEmisor->GetPos2());
  }
  
  std::vector<CPhysicUserData*> l_vCollisionObjects;
  CORE->GetPhysicsManager()->OverlapSphereActor(0.2f, GetPosicionActual(), l_vCollisionObjects, PX_MSK_VOMITO_ZOMBIE, true);

  ChocaContraObjeto(l_vCollisionObjects);
}

void CVomito::Render(CRenderManager* rm)
{
  CORE->GetRenderManager()->DrawLine(GetPosicionActual(), GetPosicionActual() + GetDesplazamiento(), colWHITE);

  Mat44f l_t;
  l_t.SetIdentity();
  l_t.Translate(GetPosicionActual());
  CORE->GetRenderManager()->SetTransform(l_t);
  CORE->GetRenderManager()->DrawSphere(0.2f,colYELLOW,20);

}

void CVomito::CreaVomito(Vect3f myPosicion, float myHeight, Vect3f hisPosicion)
{
  std::vector<CPhysicUserData*> l_vObjects;
  Vect3f mypos(myPosicion.x,myPosicion.y + myHeight + 0.5f ,myPosicion.z);
  SetPosicionActual(mypos);

  Vect3f hispos(hisPosicion.x,hisPosicion.y, hisPosicion.z);
  SetPosicionDestino(hisPosicion);

  m_V3Trayectoria = hisPosicion - myPosicion;

  m_V3Desplazamiento = m_V3Trayectoria.GetNormalized();
}


void CVomito::ChocaContraObjeto(const std::vector<CPhysicUserData*>& _vectOverlap)
{
  std::vector<CPhysicUserData*>::const_iterator l_iter = _vectOverlap.begin();
  for( ; l_iter != _vectOverlap.end() ; ++l_iter)
  {
    std::string l_sName = (*l_iter)->GetName();
    if(!(l_sName.compare(CCoreFIH::GetSingletonPtr()->GetPlayer()->GetInstance()->GetName().c_str())))
    {
      //quita vida
      if(CCoreFIH::GetSingletonPtr()->GetPlayer()->CollideEnemies())
      {
        std::string script;
        baseUtils::FormatSrting(script, "player:hit(%f)",DANYOVOMITO);
        CORE->GetScriptManager()->RunCode(script);
        SetChocado(true);
      }
    }
    if(l_sName.find("z_inst_") != std::string::npos)
    {
      SetChocado(true);
    }

    SetChocado(true);

  }

}



//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------