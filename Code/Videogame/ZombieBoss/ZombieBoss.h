//----------------------------------------------------------------------------------
// Class ZombieBoss
// Author: Miquel Rojas
//
// Description:
// Jefe final
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef ZOMBIE_BOSS_H_
#define ZOMBIE_BOSS_H_

#define ANIMATION_BOSS 50

class CRenderManager;
class CRandomArray;
class CGestionLanzas;

// Includes
#include "../Zombie/Zombie.h"

//Class
class CZombieBoss: public CZombie
{
public:
enum
{
  MUERTO = 49,
  IDLE = ANIMATION_BOSS,
  ATAQUECONARMA,
  PRERUN,
  EMBESTIDA,
  CRASHED,
  PISOTON,
  ATAQUEREVERSO,
  RECIBIENDOGOLPE,
  MURIENDO,
  PERSEGUIR,
  RETIRADA,
  ENCARAR,
  INACTIVO,
  REMUERTO
  
};
public:
  // Init and End protocols
  CZombieBoss() : CZombie(), m_bIsOk(false),m_fTiempoRecuperacionTrasDarGolpe(0.0f),m_fTiempoMinEntreGolpeYGolpe(0.0f)
    ,m_fTiempoMaxEntreGolpeYGolpe(0.0f), m_fTiempoEntreGolpeYGolpe(0.0f), m_fSpeedRetroceso(0.0f), m_fSpeedEmbestida(0.0f)
    ,m_cGolpeAtaque(0),m_vAleatorios(0),m_fElapsedTimeEntreGolpeYGolpe(0.0f), m_fElapsedTimeTrasDarGolpe(0.0f),m_fElapsedTimeInactivoRecibeGolpe(0.0f)
    ,m_fTiempoInactivoRecibeGolpe(0.0f), m_fElapsedTimeEncarando(0.0f), m_fTiempoEncarando(0.0f), m_fAuxRadiusViewing(0.0f)
    ,m_fTiempoAnimAtaqueArma(0.0f), m_fTiempoAnimPrerun(0.0f), m_fTiempoAnimEmbestida(0.0f)
    ,m_fTiempoAnimCrash(0.0f), m_fTiempoAnimPisoton(0.0f), m_fTiempoAnimAtaqueReverso(0.0f), m_fTiempoAnimRecibiendoGolpe(0.0f)
    ,m_fTiempoAnimMuriendo(0.0f), m_fTiempoAnimAndando(0.0f), m_fElapsedTimeAnimAtaqueArma(0.0f), m_fElapsedTimeAnimPrerun(0.0f)
    ,m_fElapsedTimeAnimEmbestida(0.0f), m_fElapsedTimeAnimCrash(0.0f), m_fElapsedTimeAnimPisoton(0.0f), m_fElapsedTimeAnimAtaqueReverso(0.0f)
    ,m_fElapsedTimeAnimRecibiendoGolpe(0.0f), m_fElapsedTimeAnimMuriendo(0.0f), m_fElapsedTimeAnimAndando(0.0f)
    ,m_sNameBlockColliding(""), m_cStartLanzas(0), m_uiNumeroTotalIteracionesLanzas(0), m_uiNumeroLanzas(0), m_pGLanzas(0)
    , m_fTiempoInvencibilidad(0.0f), m_fElapsedTimeInvencibilidad(0.0f), m_bBossGolpeadoToInvencibilidad(false)
    {}
    
        
  virtual ~CZombieBoss() { Done(); }

	virtual	bool				Init								(const CXMLTreeNode &m);
	virtual	bool				Init								(const CZombie &z);
  virtual void				Done               	();
  bool								IsOk               	() const { return m_bIsOk; }
  void                Update              (float);
  void                UpdateLanzasPisoton (float);
  void                Render              (CRenderManager*);
  virtual char        RecalculateIA       (Vect3f _heroPosition);//{return (char)255;}// = 0;
  virtual void        InicializaController();
//  virtual const Vect3f &      GetPositionHero     () {return CORE->GetRenderableObjectsManager()->GetInstance("mi_amigo")->GetPosition();}

  // -----------------------
  // Read functions
    // -----------------------
  float GetVelocidadRetirada                            () const      {return m_fSpeedRetroceso;}
  float GetVelocidadEmbestida                           () const      {return m_fSpeedEmbestida;}

  float GetTiempoRecuperacionTrasDarGolpe               () const      {return m_fTiempoRecuperacionTrasDarGolpe;}
  float GetTiempoEntreGolpeYGolpe                       () const      {return m_fTiempoEntreGolpeYGolpe;}
  float GetTiempoInactivo                               () const      {return m_fTiempoInactivoRecibeGolpe;}
  float GetTiempoEncarando                              () const      {return m_fTiempoEncarando;}
  float GetOriginalRadiusViewing                        () const      {return m_fAuxRadiusViewing;}
  
  float GetTiempoAnimacionAtaqueConArma                 () const      {return m_fTiempoAnimAtaqueArma; }
  float GetTiempoAnimacionPreRun                        () const      {return m_fTiempoAnimPrerun; }         
  float GetTiempoAnimacionEmbestida                     () const      {return m_fTiempoAnimEmbestida; }
  float GetTiempoAnimacionCrash                         () const      {return m_fTiempoAnimCrash; }
  float GetTiempoAnimacionPisoton                       () const      {return m_fTiempoAnimPisoton; }
  float GetTiempoAnimacionAtaqueReverso                 () const      {return m_fTiempoAnimAtaqueReverso; }
  float GetTiempoAnimacionRecibiendoGolpe               () const      {return m_fTiempoAnimRecibiendoGolpe; }
  float GetTiempoAnimacionMuriendo                      () const      {return m_fTiempoAnimMuriendo; }
  float GetTiempoAnimacionAndando                       () const      {return m_fTiempoAnimAndando; }
  bool  GetInvencible                                   () const      {return m_bBossGolpeadoToInvencibilidad; }
  std::string GetNameBlockColliding                     () const      {return m_sNameBlockColliding;}
  Vect3f GetPosicionTorso                               () ;
  CGestionLanzas* GetGestionLanzas                      () const      {return m_pGLanzas; }                         
  
  // -----------------------
  // Write functions
  // -----------------------
  void AcumulaTiempoTrasDarGolpe                        (float dt)    {m_fElapsedTimeTrasDarGolpe += dt;}
  bool FinTiempoTrasDarGolpe                            ()            {return m_fElapsedTimeTrasDarGolpe >= m_fTiempoRecuperacionTrasDarGolpe;}
  void ResetTiempoTrasDarGolpe                          ()            {m_fElapsedTimeTrasDarGolpe = 0.0f;}
  
  void AcumulaTiempoEntreGolpeYGolpe                    (float dt)    {m_fElapsedTimeEntreGolpeYGolpe += dt;}
  bool FinTiempoEntreGolpeYGolpe                        ()            {return m_fElapsedTimeEntreGolpeYGolpe >= m_fTiempoEntreGolpeYGolpe;}
  void ResetTiempoEntreGolpeYGolpe                      ()            {m_fElapsedTimeEntreGolpeYGolpe = 0.0f;}
  
  void AcumulaTiempoInactivo                            (float dt)    {m_fElapsedTimeInactivoRecibeGolpe += dt;}
  bool FinTiempoInactivo                                ()            {return m_fElapsedTimeInactivoRecibeGolpe >= m_fTiempoInactivoRecibeGolpe;}
  void ResetTiempoInactivo                              ()            {m_fElapsedTimeInactivoRecibeGolpe = 0.0f;}  

  void AcumulaTiempoInvencibilidad                      (float dt)    {m_fElapsedTimeInvencibilidad += dt;}
  bool FinTiempoInvencibilidad                          ()            {return m_fElapsedTimeInvencibilidad >= m_fTiempoInvencibilidad;}
  void ResetTiempoInvencibilidad                        ()            {m_fElapsedTimeInvencibilidad = 0.0f;}  

  void AcumulaTiempoEncarando                           (float dt)    {m_fElapsedTimeEncarando += dt;}
  bool FinTiempoEncarando                               ()            {return m_fElapsedTimeEncarando >= m_fTiempoEncarando;}
  void ResetTiempoEncarando                             ()            {m_fElapsedTimeEncarando = 0.0f;}  
  
  void SetInvencible                                    (bool i)      {m_bBossGolpeadoToInvencibilidad = i;}

  //control animaciones
  void AcumulaTiempoAnimAtaqueArma                      (float dt)    {m_fElapsedTimeAnimAtaqueArma += dt;}
  bool FinTiempoAnimAtaqueArma                          ()            {return m_fElapsedTimeAnimAtaqueArma>= m_fTiempoAnimAtaqueArma;}
  void ResetTiempoAnimAtaqueArma                        ()            {m_fElapsedTimeAnimAtaqueArma = 0.0f;}
  bool MomentoDelGolpe                                  ()            {return ((m_fElapsedTimeAnimAtaqueArma > (m_fTiempoAnimAtaqueArma/2.0f)-0.3f)
                                                                        &&(m_fElapsedTimeAnimAtaqueArma < (m_fTiempoAnimAtaqueArma/2.0f)+0.3f));}

  void AcumulaTiempoAnimPrerun                          (float dt)    {m_fElapsedTimeAnimPrerun += dt;}
  bool FinTiempoAnimPrerun                              ()            {return m_fElapsedTimeAnimPrerun >= m_fTiempoAnimPrerun;}
  void ResetTiempoAnimPrerun                            ()            {m_fElapsedTimeAnimPrerun = 0.0f;}

  void AcumulaTiempoAnimEmbestida                       (float dt)    {m_fElapsedTimeAnimEmbestida += dt;}
  bool FinTiempoAnimEmbestida                           ()            {return m_fElapsedTimeAnimEmbestida >= m_fTiempoAnimEmbestida;}
  void ResetTiempoAnimEmbestida                         ()            {m_fElapsedTimeAnimEmbestida = 0.0f;}

  void AcumulaTiempoAnimCrash                           (float dt)    {m_fElapsedTimeAnimCrash += dt;}
  bool FinTiempoAnimCrash                               ()            {return m_fElapsedTimeAnimCrash >= m_fTiempoAnimCrash;}
  void ResetTiempoAnimCrash                             ()            {m_fElapsedTimeAnimCrash = 0.0f;}

  void AcumulaTiempoAnimPisoton                         (float dt)    {m_fElapsedTimeAnimPisoton += dt;}
  bool FinTiempoAnimPisoton                             ()            {return m_fElapsedTimeAnimPisoton >= m_fTiempoAnimPisoton;}
  void ResetTiempoAnimPisoton                           ()            {m_fElapsedTimeAnimPisoton = 0.0f;}

  void AcumulaTiempoAnimAtaqueReverso                   (float dt)    {m_fElapsedTimeAnimAtaqueReverso += dt;}
  bool FinTiempoAnimAtaqueReverso                       ()            {return m_fElapsedTimeAnimAtaqueReverso >= m_fTiempoAnimAtaqueReverso;}
  void ResetTiempoAnimAtaqueReverso                     ()            {m_fElapsedTimeAnimAtaqueReverso = 0.0f;}

  void AcumulaTiempoAnimRecibiendoGolpe                 (float dt)    {m_fElapsedTimeAnimRecibiendoGolpe += dt;}
  bool FinTiempoAnimRecibiendoGolpe                     ()            {return m_fElapsedTimeAnimRecibiendoGolpe >= m_fTiempoAnimRecibiendoGolpe;}
  void ResetTiempoAnimRecibiendoGolpe                   ()            {m_fElapsedTimeAnimRecibiendoGolpe = 0.0f;}

  void AcumulaTiempoAnimMuriendo                        (float dt)    {m_fElapsedTimeAnimMuriendo += dt;}
  bool FinTiempoAnimMuriendo                            ()            {return m_fElapsedTimeAnimMuriendo >= m_fTiempoAnimMuriendo;}
  void ResetTiempoAnimMuriendo                          ()            {m_fElapsedTimeAnimMuriendo = 0.0f;}

  void AcumulaTiempoAnimAndando                         (float dt)    {m_fElapsedTimeAnimAndando += dt;}
  bool FinTiempoAnimAndando                             ()            {return m_fElapsedTimeAnimAndando >= m_fTiempoAnimAndando;}
  void ResetTiempoAnimAndando                           ()            {m_fElapsedTimeAnimAndando = 0.0f;}

  void SetOriginalRadiusViewing                         (float rv)    {m_fAuxRadiusViewing = rv;}  
  void IniciaLanzas                                     (char v)      {m_cStartLanzas = v;}
  // -----------------------
  // Logic functions
  // -----------------------
  //test
  void Test();
  void                EscondeTriggerManoDerecha();
  virtual bool        GeneraManoDerecha();
  virtual void        PosicionaManoDerecha();
  void                GetDuracionAnimacionesBoss        ();
  void                PosicionaTorso();
  void                CalculaTiempoEntreGolpeYGolpe     ();

private:
  void                RegistraBossLUA                   ();
  void        				Release   					              ();
  char                GeneraAleatorioAtaqueZombi        ();
    
  // member variables
  bool                m_bIsOk;          // Initialization boolean control
  float               m_fTiempoRecuperacionTrasDarGolpe;
  float               m_fElapsedTimeTrasDarGolpe; //tiempo transcurrido
  float               m_fTiempoMinEntreGolpeYGolpe;
  float               m_fTiempoMaxEntreGolpeYGolpe;
  float               m_fTiempoEntreGolpeYGolpe; // se genera despues de cada golpe dado y se calcula despues de tiempo de recuperacion tras dar golpe
  float               m_fElapsedTimeEntreGolpeYGolpe; //tiempo transcurrido
  float               m_fSpeedRetroceso;
  float               m_fSpeedEmbestida;
  char                m_cGolpeAtaque; // [1-3]
  float               m_fElapsedTimeInactivoRecibeGolpe;
  float               m_fTiempoInactivoRecibeGolpe;
  float               m_fElapsedTimeInvencibilidad;
  float               m_fTiempoInvencibilidad;
  float               m_fElapsedTimeEncarando;
  float               m_fTiempoEncarando;
  float               m_fAuxRadiusViewing;
  unsigned int        m_uiNumeroLanzas;
  unsigned int        m_uiNumeroTotalIteracionesLanzas;
  
  //elapseds de las animaciones
  float               m_fTiempoAnimAtaqueArma;
  float               m_fTiempoAnimPrerun;
  float               m_fTiempoAnimEmbestida;
  float               m_fTiempoAnimCrash;  
  float               m_fTiempoAnimPisoton;      
  float               m_fTiempoAnimAtaqueReverso;      
  float               m_fTiempoAnimRecibiendoGolpe;      
  float               m_fTiempoAnimMuriendo;      
  float               m_fTiempoAnimAndando;      
  
  float               m_fElapsedTimeAnimAtaqueArma;
  float               m_fElapsedTimeAnimPrerun;
  float               m_fElapsedTimeAnimEmbestida;
  float               m_fElapsedTimeAnimCrash;  
  float               m_fElapsedTimeAnimPisoton;      
  float               m_fElapsedTimeAnimAtaqueReverso;      
  float               m_fElapsedTimeAnimRecibiendoGolpe;      
  float               m_fElapsedTimeAnimMuriendo;      
  float               m_fElapsedTimeAnimAndando;      
  std::string         m_sNameBlockColliding;
  bool                m_bBossGolpeadoToInvencibilidad;    
      
  CRandomArray*       m_vAleatorios;
  CGestionLanzas*     m_pGLanzas;
  char                m_cStartLanzas;
  
  typedef CZombie Inherited;
};


#endif