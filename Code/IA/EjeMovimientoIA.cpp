#include "EjeMovimientoIA.h"
#include "Math/Matrix44.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CEjeMovimiento::Init()
{
  m_V3DirectorPlayer2.SetZero();
  m_V3DirPlayer2Optimo.SetZero();
  m_V3RightPlayer2.SetZero();
  m_V3UpPlayer2.SetZero();
  m_V3DirDistanciaOptimo.SetZero();
  m_bIsOk = true;

  if (!m_bIsOk){
    Release();
  }

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CEjeMovimiento::Done()
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
void CEjeMovimiento::Release()
{
//free memory
}


//----------------------------------------------------------------------------
// Movimiento IA
//----------------------------------------------------------------------------
void CEjeMovimiento::MovAgentIA(Vect3f hisPosition, Vect3f &myPosition, float _mySpeed, float _elapsedtime, bool _chase)
{
  //se obtiene el vector director optimo normalizado y vector director-distancia
  m_V3DirPlayer2Optimo = hisPosition - myPosition;
  m_V3DirDistanciaOptimo = m_V3DirPlayer2Optimo;
  m_V3DirPlayer2Optimo = m_V3DirPlayer2Optimo.GetNormalized();
  
  //en el primer scan la direccion es la optima
  if(!m_bDatosInicializacion)
  {
     m_V3DirectorPlayer2    = m_V3DirPlayer2Optimo;
     m_fAngleParcialPlayer2 = m_V3DirPlayer2Optimo.GetAngleY();
     m_fAngleTotalPlayer2   = m_V3DirPlayer2Optimo.GetAngleY();
     m_bDatosInicializacion = true;
  }

  BuscaEncarar();

  m_V3UpPlayer2(myPosition.x, 1.0f, myPosition.z);
  m_V3RightPlayer2 =  m_V3DirectorPlayer2.GetRotatedY( ePI2f );
  m_V3RightPlayer2.y = 0.0f;

  float dist = myPosition.Distance(hisPosition);
    
  _mySpeed = min(_mySpeed,dist);

  if(_chase)
  {    
    myPosition.x += m_V3DirectorPlayer2.x * _mySpeed;
    myPosition.z += m_V3DirectorPlayer2.z * _mySpeed;
  }
  else
  {
    myPosition.x -= m_V3DirectorPlayer2.x * _mySpeed;
    myPosition.z -= m_V3DirectorPlayer2.z * _mySpeed;
  }
  
  //posicion y rotacion del muñeco

  //m_pPlayer2->SetPosition(Vect3f(myPosition.x,0,myPosition.z));
  //m_pPlayer2->SetYaw(-m_fAngleTotalPlayer2);

  //separacion al colisionar (sin PhysX)
  //if((abs(hisPosition.x - myPosition.x) < 0.1f) && (abs(hisPosition.z - myPosition.z) < 0.1f))
  //{
  //  myPosition.x = hisPosition.x;
  //  myPosition.y = 1.0f;
  //  myPosition.z = hisPosition.z;
  //}
}

//perseguir pseudocodigo Gabriel
//--------------------------------
//Dir = Pe - Pp
//dist = Dir.Distance();
//normDir = Dir / dist;
//fSpeed = SPEED;
//fSpeed = min (fSpeed, dist);
//Pp += normDir * fSpeed * dt;
//if (dist < 0.1f)

/*void CEjeMovimiento::MovAgentIA(Vect3f hisPosition, Vect3f &myPosition, float &_mySpeed_)
{
  //se obtiene el vector director optimo normalizado
  m_V3DirPlayer2Optimo = hisPosition - myPosition;
  m_V3DirPlayer2Optimo = m_V3DirPlayer2Optimo.GetNormalized();

  //en el primer scan la direccion es la optima
  //float m_fAnglePlayer2 = 0.0f;
  if(!m_bDatosInicializacion)
  {
     m_V3DirectorPlayer2 = m_V3DirPlayer2Optimo;
     m_fAngleParcialPlayer2 = m_V3DirPlayer2Optimo.GetAngleY();
     m_fAngleTotalPlayer2 = m_V3DirPlayer2Optimo.GetAngleY();
     m_bDatosInicializacion = true;
  }

  //eje del player2
  //obtiene el vector director real
  m_fAngleParcialPlayer2 = AjusteDireccion();
  m_fAngleTotalPlayer2 += m_fAngleParcialPlayer2;

  m_V3DirectorPlayer2.RotateY(m_fAngleParcialPlayer2);
  m_V3UpPlayer2(myPosition.x, 1.0f, myPosition.z);
  m_V3RightPlayer2 =  m_V3DirectorPlayer2.GetRotatedY( ePI2f );
  m_V3RightPlayer2.y = 0.0;

  float dist = myPosition.Distance(hisPosition);
    
  _mySpeed_ = min(_mySpeed_,dist);
  
}
*/

void CEjeMovimiento::BuscaEncarar()
{
  m_fAngleParcialPlayer2 = AjusteDireccion();
  m_fAngleTotalPlayer2 += m_fAngleParcialPlayer2;

  m_V3DirectorPlayer2.RotateY(m_fAngleParcialPlayer2);
}

//----------------------------------------------------------------------------
// Ajusta Direccion
//----------------------------------------------------------------------------
float CEjeMovimiento::AjusteDireccion()
{
  Vect3f myDir = m_V3DirectorPlayer2;
  Vect3f myRight = m_V3DirectorPlayer2.GetRotatedY( ePI2f );

  if( (m_V3DirPlayer2Optimo * myDir) < 0 )//player esta detras
  {
    if(m_V3DirPlayer2Optimo * myRight < 0)//player esta a la izquierda
      return -RADIANS;
    else    //player esta a la derecha
      return +RADIANS;
  }
  else  //player esta delante
  {
    if(m_V3DirPlayer2Optimo * myRight < 0)//player esta a la izquierda
      return -RADIANS;
    else    //player esta a la derecha
	  return +RADIANS;
  }
}


//----------------------------------------------------------------------------
// Dibuja Agente IA
//----------------------------------------------------------------------------
void CEjeMovimiento::RenderMovIA(CRenderManager *rm, Vect3f _pos_player, Vect3f _mypos, float _visionAngle,float _alturaEje, float _largoEje)
{
  Vect3f l_V3PosPlayer1( _pos_player.x, _alturaEje, _pos_player.z);
  Vect3f l_V3PosEntity ( _mypos.x, _alturaEje, _mypos.z);
  
  //vector optimo
  Mat44f vecOptimo;
  vecOptimo.SetIdentity();
  rm->SetTransform(vecOptimo);
  rm->DrawLine(l_V3PosEntity, l_V3PosPlayer1,colWHITE);
  
  //eje
  Mat44f AxisPlayer2;
  AxisPlayer2.SetIdentity();
  rm->SetTransform(AxisPlayer2);
  
  Vect3f l_V3DirectorPlayer2Aux(m_V3DirectorPlayer2.x, 0.0f,m_V3DirectorPlayer2.z);
  Vect3f l_V3RightPlayer2Aux(m_V3RightPlayer2.x, 0.0f, m_V3RightPlayer2.z);
  Vect3f l_V3UpPlayer2Aux(0.0f,1.0f,0.0f);
  
  rm->DrawLine(l_V3PosEntity, l_V3PosEntity + (l_V3DirectorPlayer2Aux * _largoEje), colRED);
  rm->DrawLine(l_V3PosEntity, l_V3PosEntity + (l_V3RightPlayer2Aux * _largoEje), colYELLOW);
  rm->DrawLine(l_V3PosEntity, l_V3PosEntity + (l_V3UpPlayer2Aux * _largoEje), colGREEN);
  
  //vision
  Mat44f eyes;
  float l_fRadiansVisionAngle =  (_visionAngle * FLOAT_PI_VALUE) / 180.0f;

  eyes.SetIdentity();
  rm->SetTransform(eyes);

  Vect3f l_V3LeftEye = l_V3DirectorPlayer2Aux.GetRotatedY(l_fRadiansVisionAngle * 0.5f);
  rm->DrawLine(l_V3PosEntity, l_V3PosEntity + (l_V3LeftEye * _largoEje * _largoEje), colCYAN);

  eyes.SetIdentity();
  rm->SetTransform(eyes);

  Vect3f l_V3RightEye = l_V3DirectorPlayer2Aux.GetRotatedY(-l_fRadiansVisionAngle * 0.5f);
  rm->DrawLine(l_V3PosEntity, l_V3PosEntity + (l_V3RightEye * _largoEje * _largoEje), colMAGENTA);  
}

//----------------------------------------------------------------------------
// Comprueba player dentro area vision entidad
//----------------------------------------------------------------------------
bool CEjeMovimiento::CompruebaPlayerDentroAreaVision(const float &_visionDistance, const float &_visionAngle)
{
  //TODO.. intentar mejorar por producto escalar
  if(m_bDatosInicializacion)
  {
    float l_fRadiansVisionAngle = (_visionAngle * FLOAT_PI_VALUE) / 180.0f;
    Vect3f l_V3DirectorPlayer2Aux(m_V3DirectorPlayer2.x, 0.0f,m_V3DirectorPlayer2.z);
    Vect3f l_V3LeftEye = l_V3DirectorPlayer2Aux.GetRotatedY(l_fRadiansVisionAngle * 0.5f);
    Vect3f l_V3RightEye = l_V3DirectorPlayer2Aux.GetRotatedY(-l_fRadiansVisionAngle * 0.5f);
    //float testLeftEye = l_V3LeftEye.GetAngleY();
    //float testDirectionEye = l_V3DirectorPlayer2Aux.GetAngleY();
    //float testRightEye = l_V3RightEye.GetAngleY();
    if(((l_V3LeftEye.GetAngleY() < l_V3DirectorPlayer2Aux.GetAngleY())&&(l_V3DirectorPlayer2Aux.GetAngleY() < l_V3RightEye.GetAngleY()))
      ||((l_V3LeftEye.GetAngleY() > l_V3DirectorPlayer2Aux.GetAngleY())&&(l_V3DirectorPlayer2Aux.GetAngleY() > l_V3RightEye.GetAngleY())))
    {      
      //float testOpt = m_V3DirDistanciaOptimo.Length();
      if(m_V3DirDistanciaOptimo.Length() < _visionDistance)
        return true;
    }
  }
  return false;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------