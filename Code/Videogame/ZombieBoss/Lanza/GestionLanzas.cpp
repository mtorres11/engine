#include "GestionLanzas.h"
#include "Lanza.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "CoreFIH.h"
#include "ZombieManager/ZombieManager.h"
#include "ZombieBoss/ZombieBoss.h"

#include "Math/MathTypes.h"
#include "Math/Random.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CGestionLanzas::Init(unsigned int _numLanzas, unsigned int _numIters)
{
  m_bIsOk                       = true;  
  m_uiNumTotalIteracionLanzas   = _numIters;
  m_uiNumLanzas                 = _numLanzas;
   
  for(unsigned int i=0; i < _numLanzas ;i++)
  {
    if(m_bIsOk)
    {
      CLanza* lanza = new CLanza();
      if (lanza)
      {
        lanza->Init( i );
        m_vLanzas.push_back(lanza);
      }
      else
        m_bIsOk = false; 
    }
  }
  
  //genera seed de aleatorios
  //SYSTEMTIME ahora;
  //GetLocalTime(&ahora);
  //srand((unsigned int)ahora.wMilliseconds);

  if (!m_bIsOk)
  {
    Release();
  }

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CGestionLanzas::Done()
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
void CGestionLanzas::Release()
{
//free memory
  std::vector<CLanza*>::iterator l_iter = m_vLanzas.begin();
  
  for ( ; l_iter != m_vLanzas.end() ; ++l_iter )
  {  
    CHECKED_DELETE( (*l_iter) );
//    m_vLanzas.pop_back();
  }    
  m_vLanzas.clear();  
}

void CGestionLanzas::PosicionaLanzas(Vect3f _centro, float _dist)
{
  float l_fAnguloSeparacion = e2PIf / m_uiNumLanzas;
  std::vector<CLanza*>::iterator l_iter = m_vLanzas.begin();
  Vect3f l_v3Aux;
  
  for(unsigned int i = 0;i < m_uiNumLanzas; i++, ++l_iter)
  {
    if(_dist < 0.0f)//valor aleatorio segun un radio
    {
    	float l_fVal=((rand()/static_cast<float>(RAND_MAX)) * (-_dist));
    
      l_v3Aux(_centro.x + cos( i * l_fAnguloSeparacion) * l_fVal  //negativo para poner positivo el valor
                        ,2.0f
                        ,_centro.z + sin( i * l_fAnguloSeparacion) * l_fVal  //negativo para poner positivo el valor
                     );
                             
    }
    else            //valor del radio segun el centro para cada 
    {
      l_v3Aux(  _centro.x + cos( i * l_fAnguloSeparacion ) * _dist
                , 2.0f
                , _centro.z + sin( i * l_fAnguloSeparacion ) * _dist 
             );
    }
    (*l_iter)->GetSalidaLanza(l_v3Aux, CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss()->GetNameBlockColliding());
  
    (*l_iter)->GetInstance()->SetPosition( Vect3f ( l_v3Aux.x, l_v3Aux.y, l_v3Aux.z ) );
  }
}

void CGestionLanzas::DefinirOleadaLanza()
{
//  [PosicionaLanzas(centro,dist)]
  switch(m_uiNumIteracionLanzas)
  {
    case 0:
      PosicionaLanzas(CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss()->GetInstance()->GetPosition() , 5.0f);      
    break;
    case 1:
      PosicionaLanzas(CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss()->GetInstance()->GetPosition() , 2.0f);
    break;
    case 2:
      PosicionaLanzas(CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss()->GetPositionHero() , -8.0f);
    break;
    case 3:
      PosicionaLanzas(CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss()->GetInstance()->GetPosition() , -10.0f);
    break;
    case 4:
      PosicionaLanzas(CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss()->GetPositionHero() , 3.0f);
    break;
    default:
      PosicionaLanzas(CCoreFIH::GetSingletonPtr()->GetZombieManager()->GetZombieBoss()->GetInstance()->GetPosition() , -5.0f);
    break;
  }
}


void CGestionLanzas::Update(float _elapsedTime)
{
  std::vector<CLanza*>::const_iterator l_iter = m_vLanzas.begin();
  std::vector<CLanza*>::const_iterator l_iter_end = m_vLanzas.end();


  //init
  if(m_uiNumIteracionLanzas == 0)
  {
    DefinirOleadaLanza();
    m_uiNumIteracionLanzas++;
  }

  //todas lanzas
  for(;l_iter != l_iter_end; ++l_iter)
  {
    (*l_iter)->Update(_elapsedTime); 
  }
  //end
  
  if(m_vLanzas[m_uiNumLanzas - 1]->FinAnimacionLanza())
  {
      DefinirOleadaLanza();
      m_uiNumIteracionLanzas++;
  }
}

void CGestionLanzas::Render(CRenderManager* rm)
{
  std::vector<CLanza*>::const_iterator l_iter = m_vLanzas.begin();
  std::vector<CLanza*>::const_iterator l_iter_end = m_vLanzas.end();

  for(;l_iter != l_iter_end; ++l_iter)
  {
    (*l_iter)->Render(rm); 
  }

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------