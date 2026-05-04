#include "ZombieOlfateador.h"
#include "XML\XMLTreeNode.h"
#include "RenderManager.h"
#include "IA.h"
#include "Heuristica.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "InputManager.h"
#include "ActionToInput.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

#define DIMCOLISION 0.5
//#define MAXINDEX 99

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CZombieOlfateador::Init(const CXMLTreeNode &m){
  SetDummyStart(m.GetPszProperty("dummy_start"));
  m_sNodoActual = GetDummyStart();
  m_sNodoSiguiente = "";

  m_AgentIA = new C_IA();
  m_AgentIA->Init();
  std::map<std::string,SNodo*> miApuntadorGrafo = m_AgentIA->GetGrafo();
  std::map<std::string,SNodo*>::iterator l_iterGrafo =  miApuntadorGrafo.find(GetDummyStart());
  GetInstance()->SetPosition(l_iterGrafo->second->posicion);

  this->SetAgent(m_AgentIA);
  m_V3RotIni			= m.GetVect3fProperty("rot",NULL);
  GetInstance()->SetYaw(m_V3RotIni.x);
  GetInstance()->SetPitch(m_V3RotIni.y);
  GetInstance()->SetRoll(m_V3RotIni.z);

  GetInstance()->SetName( m.GetPszProperty("name"));
  SetSiguiendoRuta(false);

  return true;
}

bool CZombieOlfateador::Init(CZombie &z)
{
	bool bIsOk = Inherited::Init(z);
  if (!bIsOk)
  {
		printf("a CZombieOlfateador instace couldnt allocate memory");
    Done();								//We call Done()  to release before the parent class
	}
  else
  {
    SetHealth(z.GetHealth());
    SetInstance(z.GetInstance());
    SetModelFilename(z.GetModelFilename());
    SetMoveSpeed(z.GetMoveSpeed());
    SetRotSpeed(z.GetRotSpeed());
    GetInstance()->SetScalar(z.GetInstance()->GetScalarX(),z.GetInstance()->GetScalarY(),z.GetInstance()->GetScalarZ());
    SetSiguiendoRuta(false);
  }

  return bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CZombieOlfateador::Done(){
	Inherited::Done();			//Parent class Done
  bool l_bIsOk = true;
  if (l_bIsOk)
  {
    Release();
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CZombieOlfateador::Release(){
//free memory
  delete m_AgentIA;
}	


//----------------------------------------------------------------------------
// Recalculate IA
//----------------------------------------------------------------------------
char CZombieOlfateador::RecalculateIA(Vect3f hisPosition)
{
  return (char) IDLE;
}


//----------------------------------------------------------------------------
// Update
//----------------------------------------------------------------------------
void CZombieOlfateador::Update(float elapsedTime)
{
  Vect3f hisPosition = GetPositionHero();
  //dibujar cuando la distancia es la mitad a smelling
  if((hisPosition.SqDistance(GetInstance()->GetPosition()) < GetRadiusSmelling() * GetRadiusSmelling() * 0.25))
  {
    GetInstance()->SetVisible(true);
  }
  else
  {
    GetInstance()->SetVisible(false);
  }

  if(m_bSiguiendoRuta)
  {
    m_sNodoActual = m_sNodoActual;
    m_sNodoSiguiente=m_sNodoSiguiente;
    if(strcmp(m_sNodoActual.c_str(),m_sNodoSiguiente.c_str())) 
    {
      Vect3f miPosicion = GetInstance()->GetPosition();
      if((m_v3PosicionNodoAVisitar.x > miPosicion.x - DIMCOLISION)
        &&(m_v3PosicionNodoAVisitar.z > miPosicion.z - DIMCOLISION)
        &&(m_v3PosicionNodoAVisitar.x < miPosicion.x + DIMCOLISION)
        &&(m_v3PosicionNodoAVisitar.z < miPosicion.z + DIMCOLISION))
      {
        CalculaSiguienteNodo();//obtiene el ultimo nodo del vector mejorcamino y su posicion es m_v3posicionnodoavisitar
      }
      else
      {
        CalculaSiguientePosicionZombie(elapsedTime);//posiciona al zombi en la posicion correcta siguiendo su proximo nodo a visitar
      }
    }
    else
    {
      m_bSiguiendoRuta = false;
    }
  }
  else
  {
    if(CORE->GetInputManager()->IsAnyKeyPressed())
    {
      UpdateGoto();
    }
  }
}

//----------------------------------------------------------------------------
// Calcula siguiente nodo que debe visitar segun A*
//----------------------------------------------------------------------------
void CZombieOlfateador::CalculaSiguienteNodo(void)
{
    if(m_indiceCamino > 0)
    {
//      std::map<std::string,SNodo*>::iterator l_iterGrafo =  miApuntadorGrafo.find(GetAgentIA()->GetMejorCamino().back());
//      GetAgentIA()->GetMejorCamino().pop_back();
      std::map<std::string,SNodo*> miApuntadorGrafo = GetAgentIA()->GetGrafo();
      std::map<std::string,SNodo*>::iterator l_iterGrafo =  miApuntadorGrafo.find(GetAgentIA()->GetMejorCamino()[m_indiceCamino - 1]);
      m_sNodoActual = m_sNodoSiguiente;
      m_sNodoSiguiente = l_iterGrafo->first;
      m_v3PosicionNodoAVisitar = l_iterGrafo->second->posicion;
      m_indiceCamino --;
    }
    else
    {
      m_sNodoActual = m_sNodoSiguiente;
    }
}

void CZombieOlfateador::CalculaSiguientePosicionZombie(float _elapsedTime)
{
  Vect3f l_v3MiPosicion = GetInstance()->GetPosition();
  //m_v3PosicionNodoAVisitar
  Vect3f l_v3VectorDirector = m_v3PosicionNodoAVisitar - l_v3MiPosicion;
  l_v3VectorDirector = l_v3VectorDirector.GetNormalized();
  l_v3MiPosicion.x += l_v3VectorDirector.x * _elapsedTime * GetMoveSpeed();//TODO..m_fMoveSpeed;
  l_v3MiPosicion.y = 0.0f;
  l_v3MiPosicion.z += l_v3VectorDirector.z * _elapsedTime * GetMoveSpeed();//m_fMoveSpeed;
  GetInstance()->SetPosition(l_v3MiPosicion);
  GetInstance()->SetYaw(-l_v3VectorDirector.GetAngleY());
}


//----------------------------------------------------------------------------
// Render
//----------------------------------------------------------------------------
void CZombieOlfateador::Render(CRenderManager *rm)
{
}


void CZombieOlfateador::UpdateGoto(void)
{
  //IA
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy1")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"1\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }

  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy2")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"2\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy3")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"3\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy4")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"4\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy5")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"5\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy6")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"6\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy7")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"7\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy8")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"8\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy9")) != 0.0f )
  { 
    CHeuristicaDistanciaReal2Puntos miHeuristica;

    GetAgentIA()->AStar(m_sNodoActual, "\"9\"", (CHeuristica &)miHeuristica);  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }

  //IA
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy1D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"1\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }

  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy2D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"2\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy3D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"3\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy4D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"4\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy5D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"5\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy6D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"6\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy7D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"7\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy8D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"8\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }
  if((CORE->GetInputManager()->GetActionToInput()->DoAction("GotoDummy9D")) != 0.0f )
  { 
    GetAgentIA()->Dijsktra(m_sNodoActual, "\"9\"");  
    m_indiceCamino = GetAgentIA()->GetMejorCamino().size();
    CalculaSiguienteNodo();
    m_bSiguiendoRuta =true;
  }

}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
