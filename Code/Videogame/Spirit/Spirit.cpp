#include "Spirit.h"
#include "XML/XMLTreeNode.h"

#include "Particles/ParticleManager.h"
#include "Particles/ParticleEmitter.h"

#include "InputManager.h"
#include "ActionToInput.h"

#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableObject.h"

#include "Lights/LightManager.h"
#include "Lights/Light.h"

#include "EjeMovimientoIA.h"
#include "Timer/Timer.h"

#include "Trigger/TriggerManager.h"
#include "Trigger/Trigger.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CSpirit::Init()
{
  m_eje = new CEjeMovimiento();
  m_bIsOk = false;
  if(m_eje)
  {
    m_bIsOk = GetEjeMovimiento()->Init();
  }
  if (!m_bIsOk)
  {
    Release();
  }

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CSpirit::Done()
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
void CSpirit::Release()
{
//free memory
  CHECKED_DELETE(m_eje);
//  CHECKED_DELETE(m_emitter);

}

void CSpirit::Update(float elapsedTime)
{
  if(CORE->GetInputManager()->GetActionToInput()->DoAction("ReloadSpirit") != 0.0f)
  {
    Reload();
  }

	if(m_emitter)
  {
    m_emitter->SetPos1(D3DXVECTOR3(m_V3Pos.x - m_fAperturaEmisor,m_V3Pos.y,m_V3Pos.z - m_fAperturaEmisor));
    m_emitter->SetPos2(D3DXVECTOR3(m_V3Pos.x + m_fAperturaEmisor,m_V3Pos.y,m_V3Pos.z + m_fAperturaEmisor));
		m_emitter2->SetPos1(D3DXVECTOR3(m_V3Pos.x - m_fAperturaEmisor,m_V3Pos.y,m_V3Pos.z - m_fAperturaEmisor));
		m_emitter2->SetPos2(D3DXVECTOR3(m_V3Pos.x + m_fAperturaEmisor,m_V3Pos.y,m_V3Pos.z + m_fAperturaEmisor));
  }

  //UpdateLevitation(elapsedTime);
  RecalculateIA();
  UpdateIAMovement(elapsedTime);

  // posición de la luz
  if (m_pLight != NULL)
    m_pLight->SetPosition(m_V3Pos);
}

void CSpirit::RecalculateIA()
{
  switch(m_cState)
  {
    case IDLE:
      m_fSpeed = m_fSpeedMin;
    break;
    case PERSIGUIENDO:
      m_fSpeed = m_fSpeedMin;
    break;
    case EVADIENDO:
      m_fSpeed = m_fSpeedMax;
    break;
    case CONTENTO:
      m_fSpeed = m_fSpeedMax;
    break;
    case MOVSEXY:
      m_fSpeed = m_fSpeedMin;
    break;
  }
}


bool CSpirit::LoadXML(std::string fileName)
{
  m_pFileName = fileName;
  CXMLTreeNode header;

  if(!header.LoadFile(fileName.c_str()))
  {
		//Guardar el mensaje de error en el LOGGER
		std::string msg_error = " CSpirit::Load->Error al intentar abrir el archivo: ";
		msg_error.append(fileName.c_str());
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		//throw CException(__FILE__, __LINE__, msg_error);
		return false;
  }
  else
  {
    //<spirit>
  //<parameters posini="3 2 -3" apertura="0.2" speedup="3.3" speeddown="0.5" heightflymax="3.0" heightflymin="1.9" 
  //speedslow="1.0" speedfast="1.5" distnear="4.0" distfar="6.0" 
  //timeidle="5.0" timechase="5.0"
  //numlaps="2" speedcontento="4.0" 
  ///>
    //</spirit>

    //solo un spirit****
    CXMLTreeNode tag = header["spirit"];
    if(tag.Exists())
    {
   		if (!tag.IsComment())
			{
        CXMLTreeNode params = tag(0)["parameters"];
				if (params.Exists())
				{
				  m_V3PosIni = params.GetVect3fProperty("posini", Vect3f(3.0f, 1.5f, -3.8f));
 //         m_V3Pos = params.GetVect3fProperty("posini", Vect3f(3.0f, 1.5f, -3.8f));  //quitado para dejar estatico el espiritu
          m_fSpeed_YUp = params.GetFloatProperty("speedup",3.3f);
          m_fSpeed_YDown = params.GetFloatProperty("speeddown",0.5f);
          m_fPosYMax = params.GetFloatProperty("heightflymax",3.f);
          m_fPosYMin = params.GetFloatProperty("heightflymin",1.9f);
          m_fAperturaEmisor = params.GetFloatProperty("apertura",0.2f); 
          m_fSpeedMax = params.GetFloatProperty("speedfast",1.5f);
          m_fSpeedMin = params.GetFloatProperty("speedslow",1.f);
          m_fDistFar = params.GetFloatProperty("distfar",6.f);
          m_fDistNear = params.GetFloatProperty("distnear",10.f);
          m_fDelayTimeIdle = params.GetFloatProperty("timeidle",5.f);
          m_fDelayTimeChase = params.GetFloatProperty("timechase",5.f);
          m_uiNumeroVueltasContento = params.GetIntProperty("numlaps",1);
          m_fSpeedRotationContento = params.GetFloatProperty("speedcontento",3.f);
          m_pLight = CORE->GetLigthManager()->GetResource(params.GetPszProperty("light", ""));
        }
      }
    }
    else
    {
		  //Guardar el mensaje de error en el LOGGER
		  std::string msg_error = " CSpirit::Load->Parametros mal configurados ";
		  msg_error.append(fileName.c_str());
		  LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		  //throw CException(__FILE__, __LINE__, msg_error);
		  return false;
    }
  }

  return true;
}

void CSpirit::UpdateLevitation(float elapsedTime)
{
  if(m_bLevita)
  {
    if(m_fPosYMax > m_V3Pos.y)
    {
      m_V3Pos.y += m_fSpeed_YUp * elapsedTime;
    }
    else
    {
      m_bLevita = !m_bLevita;
    }
  }
  else
  {
    if(m_fPosYMin < m_V3Pos.y)
    {
      m_V3Pos.y -= m_fSpeed_YDown * elapsedTime;
    }
    else
    {
      m_bLevita = !m_bLevita;
    }
  }
}

void CSpirit::UpdateIAMovement(float elapsedTime)
{
  CRenderableObject* player = CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");

  if(player)
  {
    Vect3f posHero = player->GetPosition();
    switch(m_cState)
    {
      case IDLE:
      m_cState = MOVSEXY; //se fuerza que empieza en ese estado 
      if(m_cState != CONTENTO)
      {
        if(m_bInitState)
        {
          GetEjeMovimiento()->ResetDatosInicializacion();
          m_bInitState = false;
          m_dTimeIdle = CORE->GetTimer()->GetTotalTime();
        }

        if(((m_fDistFar * m_fDistFar) > (posHero.SqDistance(m_V3Pos)) || ((m_dTimeIdle + m_fDelayTimeIdle) < CORE->GetTimer()->GetTotalTime())&&(m_dTimeIdle != 0.0)))
        {
          m_cState = PERSIGUIENDO;
          m_bInitState = true;
          m_dTimeIdle = 0.0;
        }
      }
      break;
      case PERSIGUIENDO:
      if(m_cState != CONTENTO)
      {
        if(m_bInitState)
        {
          GetEjeMovimiento()->ResetDatosInicializacion();
          m_bInitState = false;
          m_dTimeChase = CORE->GetTimer()->GetTotalTime();
        }

        GetEjeMovimiento()->MovAgentIA(posHero, m_V3Pos, m_fSpeed, elapsedTime,true);

        if(((m_dTimeChase + m_fDelayTimeChase) < CORE->GetTimer()->GetTotalTime())&&(m_dTimeChase != 0.0))
        {
          m_cState = IDLE;
          m_bInitState = true;
          m_dTimeChase = 0.0;
        }

        if((((m_fDistNear + 1.0f) * (m_fDistNear + 1.0f)) > (posHero.SqDistance(m_V3Pos))) && ((m_fDistNear * m_fDistNear) < (posHero.SqDistance(m_V3Pos))))
        {
          m_cState = CONTENTO;
          m_bInitState = true;
        }

        if((m_fDistNear * m_fDistNear) > (posHero.SqDistance(m_V3Pos)))
        {
          m_cState = EVADIENDO;
          m_bInitState = true;
        }

      }
      break;
      case EVADIENDO:
      if(m_cState != CONTENTO)
      {
        if(m_bInitState)
        {
          GetEjeMovimiento()->ResetDatosInicializacion();
          m_bInitState = false;
        }
        GetEjeMovimiento()->MovAgentIA(posHero, m_V3Pos, m_fSpeed, elapsedTime,false);

        if((m_fDistFar * m_fDistFar) < (posHero.SqDistance(m_V3Pos)))
        {
          m_cState = IDLE;
          m_bInitState = true;
        }
      }
      break;
      case CONTENTO:
        if(m_bInitState)
        {
          float dir = 0.0f;
          float l_fYawPlayer = 0.0f;
          
          ObtieneVectoresDireccion(player, l_fYawPlayer, dir);
      
          float aux = 0.0f;
          if((m_vecPlayer * m_vecSpirit)>0)
          {
            aux = dir - l_fYawPlayer;
          }
          
          if((fabs(aux) < 0.3f) &&(aux != 0.0f))//radianes encarado del player para activar contento
          {
            GetEjeMovimiento()->ResetDatosInicializacion();
            m_bInitState = false;
          }
          else
          {
            m_cState = IDLE;
            m_bInitState = true;
          }
        }

        if(!m_bInitState)
        {
          if(GoContento(elapsedTime))
          {
            m_cState = IDLE;
            m_bInitState = true;
          }
        }  
      break;
      case MOVSEXY:
        GoMovimientoSexy(elapsedTime);  //movimiento perpetuo          
      break;
      
    }
  }
}

void CSpirit::Reload()
{
  LoadXML(m_pFileName);
  GetEjeMovimiento()->ResetDatosInicializacion();
}

bool CSpirit::GoContento(float elapsedTime)
{
  CRenderableObject* player = CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");
  Mat44f l_pos, l_pos_p, t;
  l_pos.SetIdentity();
  l_pos_p.SetIdentity();
  t.SetIdentity();
  if(m_uiNumeroVueltasContento > m_uiNumeroVueltasContentoActual)
  {
    t.Translate(Vect3f(cos(m_fAngleTurn) * m_fDistNear, 1.0f, sin(m_fAngleTurn)* m_fDistNear));

    l_pos_p = player->GetMat44();

    if(m_fAngleTurn < FLOAT_2PI_VALUE )
    {
      m_fAngleTurn += (RADIANS * m_fSpeedRotationContento * elapsedTime);
    }
    else
    {
      m_uiNumeroVueltasContentoActual ++;
      m_fAngleTurn = 0.0f;
    }

    l_pos =  l_pos_p * t; 
    m_V3Pos = l_pos.GetPos();
    return false;
  }
  else
  {
    Reload();
    m_uiNumeroVueltasContentoActual = 0;
    m_fAngleTurn = 0.0f;
    return true;
  }
}

bool CSpirit::GoMovimientoSexy(float elapsedTime)
{
//  CRenderableObject* palanca= CORE->GetRenderableObjectsManager()->GetInstance("palanca");
  
  
  Mat44f l_pos, l_pos_p, t;
  l_pos.SetIdentity();
  l_pos_p.SetIdentity();
  t.SetIdentity();
  if(m_uiNumeroVueltasContento > m_uiNumeroVueltasContentoActual)
  {
    t.Translate(Vect3f(1.0f, (sin(m_fAngleTurn)* m_fDistNear) - 1.0f, (cos(m_fAngleTurn) * m_fDistNear) - 1.0f) );

    l_pos_p = CORE->GetTriggerManager()->GetTrigger("Trigger_Palanca")->GetMatrizTrigger();
 //   l_pos_p = palanca->GetMat44();

    if(m_fAngleTurn < FLOAT_2PI_VALUE )
    {
      m_fAngleTurn += (RADIANS * m_fSpeedRotationContento * elapsedTime);
    }
    else
    {
      m_uiNumeroVueltasContentoActual ++;
      m_fAngleTurn = 0.0f;
    }

    l_pos =  l_pos_p * t; 
    m_V3Pos = l_pos.GetPos();
    return false;
  }
  else
  {
    Reload();
    m_uiNumeroVueltasContentoActual = 0;
    m_fAngleTurn = 0.0f;
    return true;
  }
}


void CSpirit::SetParticleEmitter(std::string name, std::string name2)
{
	CParticleEmitter* emitter = CORE->GetParticleManager()->GetPEmitterByInstance(name);
  if(emitter)
  {
    m_emitter = CORE->GetParticleManager()->GetPEmitterByInstance(name);
		m_emitter2 = CORE->GetParticleManager()->GetPEmitterByInstance(name2);
  }
}

void CSpirit::Render(CRenderManager* rm)
{
  CRenderableObject* player = CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo");

  Vect3f a(player->GetPosition().x,player->GetPosition().y,player->GetPosition().z); 

  Mat44f vecOptimo;
  vecOptimo.SetIdentity();
  rm->SetTransform(vecOptimo);

  rm->DrawLine(a,a+ m_vecPlayer,colWHITE);
  rm->DrawLine(a, a +m_vecSpirit,colRED);

}

void CSpirit::ObtieneVectoresDireccion(CRenderableObject* player,float& l_fYawPlayer, float& dir)
{
  l_fYawPlayer = - (player->GetYaw() + FLOAT_HALF_PI_VALUE);

  Vect3f l_V3DirOptimo =  m_V3Pos - player->GetPosition();

  dir = l_V3DirOptimo.GetAngleY() - FLOAT_HALF_PI_VALUE;
  fmod(dir, FLOAT_PI_VALUE);

  m_vecPlayer = Vect3f(10.f,1.0f,0.0f);
  m_vecPlayer.RotateY(l_fYawPlayer);

  m_vecSpirit = Vect3f(10.f,1.0f,0.0f);
  m_vecSpirit.RotateY(dir);

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------