/*----------------------------------------------------------------------------------*\
 * CVideoGame class                                                               *
 * Author: Todos                                                            *
 * Last Modification:                                                  *
 * Description:                                                                     *
 * Proceso Principal                                                              *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_VIDEOGAME_H_
#define INC_VIDEOGAME_H_

#include "Process.h"

// estados del juego
#define GS_NO_STATE             0
#define GS_CAMERA_CINEMATIC     1
#define GS_LEVEL_UPDATE         2
#define GS_PREPARE_CHANGE_LEVEL 3
#define GS_CHANGE_LEVEL         4
#define GS_GAME_OVER_TRANSITION 5
#define GS_GAME_OVER            6
#define GS_LEVEL_SELECTED       7

#define G_GAME_OVER             0

//-----------FORWARD DECLARATION-----------
class CRenderManager;
class CRenderableObject;
class CPlayer;
class CCore;
class CCoreFIH;
class CPhysicUserData;
class CCameraKeyController;
//-----------------------------------------

class CVideoGame: public CProcess
{
public:
  //Construct
	CVideoGame();
  //Destruct
	virtual ~CVideoGame();

  //functions
	void							Init						      					();
	void							Update					      					(float elapsedTime);
	void							Render					      					(CRenderManager * rm);
	void							RenderScene											(CRenderManager * rm);
  void							RenderDebugInfo									();
  void              PreRender                       (CRenderManager* rm);
  void              ChangeLevel                     (uint8 _level);
  void              GameOver                        ();
  void              ReStart                         ();

private:

	//functions
	void  						CheckActionsFromInputs					(float elapsedTime);
	void  						LoadCommonXMLs								  ();
	void  						LoadLevelXMLs										(uint8 _level);
  bool              LoadGameConfig                  ();
	void							Render2D												(CRenderManager* rm);
	void							Render3D												(CRenderManager* rm);
  void              Register2LUA                    ();
  void              LoadLuaScripts                  ();
  void              LoadLevel                       (uint8 _level);
  void              ReleaseLevel                    ();

	//extras de IA
	CCoreFIH*         GetCoreGame                     ()    {return m_pCoregame;}

	void              UpdateCamera                   ();

	//variables
	float                                             m_DeltaFromXML;
  float                                             m_fYawFactor;
  float                                             m_fPitchFactor;
  float                                             m_fPitchUpperLimit;
  float                                             m_fPitchLowerLimit;
  uint8                                             m_ucCurrentLevel;
  uint8                                             m_ucChangeLevel;
  bool                                              m_bSonando;
  uint8                                             m_ucGameState;
  float                                             m_fTimer;
	
	CCore *                                           m_pCore;
  CCoreFIH*																					m_pCoregame;
  CPlayer *                                         m_pPlayer;
  CCameraKeyController*                             m_pCameraKeyController;

  CPhysicUserData*                                  m_ClosestActorCamera;
  bool                                              m_bDebugRender;
	bool                                              m_bEndAnimation;
};

#endif

