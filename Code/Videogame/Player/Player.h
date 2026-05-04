//----------------------------------------------------------------------------------
// Class CPlayer
// Author: Andres
//
// Description:
// La muerte: personaje controlable
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef CPLAYER_H_
#define CPLAYER_H_

#include "Base.h"
#include "Math/Matrix44.h"
#include "Personaje/Personaje.h"

#include <map>

class CAnimatedInstanceModel;
class CActionToInput;
class CCamera;
class CPlayerAnimData;
class CPlayerAnimCallback;
class CPhysicUserData;
class CLight;

#define MOVE_EPSILON 0.001f

class CPlayer : public CPersonaje
{
public:
  CPlayer();
  ~CPlayer();
  
  bool Init                                 (std::string _file);
  void Done            											();
  void Update                               (float elapsedTime);
  void Render                               ();

  void HealthUp                             (float _fHealtPoints);
  void Hit                                  (float damage);
  bool CollideEnemies                       () { return m_bCollideEnemies; }
  void Revive                               ();
  void SetSate                              (int _iState) { m_iPlayerState = _iState; }

  void TurnToEnemy                          (Vect3f _hisPos);
  void ReStart                              ();

  void Jump                                 (std::string _sPlatformId = "", Vect3f _V3fDirection = v3fZERO);
  void CancelJump                           (std::string _sPlatformId);
  void RestartPosition                      (); 
  Vect3f GetPosition                        ();
  void SetPosition                          (Vect3f p);

  void GoBack                               (float);
  void Fall                                 ();
  void InitCamera                           ();
  void AssignLights                         ();
  void AssingAnimation                      ();
  void InicializaController                 ();
  
  #if defined( _DEBUG )
  #endif // defined( _DEBUG )
  
  bool Reload               ();

  

  //TODO: variable sólo para debug, eliminarla
  std::string               m_sDebug;

private:
  void 		ActionStateMachine       ();
  bool 		MovPlayer                (float elapsedTime);
  void 		RotPlayer                (float elapsedTime);  // rota al personaje para que "mire" en la direccion de la camara
  void 		Die                      ();
  void 		RegAnimCallback          ();
  void 		UpdateAttackCollisionBox ();
  void 		CheckAttackCollision     ();
  bool 		IsZombie                 (CPhysicUserData * _UserData); // retorna bool: true si el PhysicUserData dado pertenece a un zombie
  bool    IsFacingPlatform         ();
	void		PosicionaTorso					 (std::string nombre);
	Vect3f  GetPosicionTorso				 (std::string nombre);
	void		SalpicarSangre           (std::string name);

  // acciones
  void LightAttack          ();
  void StrongAttack         ();
  void DoJump               ();

  #if defined( _DEBUG )
  #endif // defined( _DEBUG )
    void MovPlayerDebug       (float elapsedTime);

    std::string   m_sSetupFileName;
    float         m_f_dJumpVerticalSpeed;
    float         m_f_dJumpMoveSpeed;
    float         m_f_dHitDamage;
    float         m_f_dRunSpeedFactor;
  
 
  
  
  bool                      m_bIsOk;

  CAnimatedInstanceModel *  m_pAnimation;
  CPlayerAnimData *         m_pAnimData;
  CActionToInput *          m_pAction2Input;
  CCamera *                 m_pPlayerCamera;
  CPlayerAnimCallback *     m_pAnimCallback;
  std::string               m_sAttackLightName;
  CLight *                  m_pAttackLight;  // luz para el ataque
  bool                      m_bAttackLightIsOn;

  std::string               m_sInstanceName;
  float                     m_fDefaultMoveSpeed;
  float                     m_fJumpVerticalSpeed;
  float                     m_fJumpSpeed;
  float                     m_fRotSpeedFactor;
  float                     m_fMaxHealth;
  float                     m_fLightAttackDamage;
  float                     m_fStrongAttackDamage;
  Vect3f                    m_V3IniPos; // Posicion inicial del personaje
  float                     m_fIniCameraYaw; //angulo yaw de la camara al iniciar el nivel
  float                     m_fIniCameraPitch; // angulo pitch de la camara al inicar el nivel
  
  int                       m_iPlayerState;
  int                       m_iPlayerActionState;

  bool                      m_bCollideEnemies;
  Vect3f                    m_V3fJumpDirection;
  float                     m_fCurrentAttackDamage;
  Mat44f                    m_AttackCollisionBox;

  // variables de creacion del physx controller
  float                     m_fControllerRadius;
  float                     m_fControllerHeight;
  float                     m_fControllerSlope;
  float                     m_fControllerSkinWidth;
  float                     m_fControllerStepOffset;

  // variables para controlar el salto de plataformas
  bool                            m_bJumpTiming;
  bool                            m_bOnJumpTrigger;
  float                           m_fJumpTimeCounter;
  float                           m_fJumpTimeToStart;
  float                           m_bIsJumping;
  float                           m_fJumpDiretionCosAngle;
  std::map<std::string, Vect3f>   m_MapJumpDirections;

	// variables para la sangre
	Vect3f										      m_v3PosicionTorso;

  //encarar el enemigo
  Vect3f                          m_v3Player;
  Vect3f                          m_v3PlayerRight;
  Vect3f                          m_v3Enemy;
  
  float                           m_fAngleRotation;
  float                           m_fCameraRotation;
};

#endif
