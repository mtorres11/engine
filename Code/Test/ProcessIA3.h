/*----------------------------------------------------------------------------------*\
 * CProcessIA3 class                                                               *
 * Author: Miquel Rojas                                                             *
 * Last Modification: Miquel Rojas                                                  *
 * Description:                                                                     *
 * Clase ProcessIA3.                                                              *
\*----------------------------------------------------------------------------------*/
#pragma once
#ifndef INC_IA3_PROCESS_H_
#define INC_IA3_PROCESS_H_

#include "Process.h"
#include "InputManager.h"
//#include "TexturedVertex.h"
#include "Math/MathUtils.h"
#include "StaticMeshes/StaticMeshManager.h"


//-----------FORWARD DECLARATION-----------
class CObject3D;
class CRenderManager;
class CActionToInput;
class CRenderableAnimatedInstanceModel;
class CRenderableObject;
//-----------------------------------------

class CProcessIA3: public CProcess
{
public:
  //Construct
	CProcessIA3();
  //Destruct
	virtual ~CProcessIA3();

  //functions
  ///<summary>
/// CProcessIA3:: Init : Lectura de los XML. Creación y asignación de las cámaras a los objetos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
	void							Init						      					();


///<summary>
/// CProcessIA3:: Update : Actualización de todos los procesos.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
	void							Update					      					(float elapsedTime);

///<summary>
/// CProcessIA3:: PreRender : Prerenderización de efectos.
///</summary>
///<param name="rm">RenderManager.</param>
///<returns name=""></returns>
  void              PreRender                       (CRenderManager * rm);


///<summary>
/// CProcessIA3:: Render : Renderiza todo.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
  void							Render					      					(CRenderManager * rm);


  ///<summary>
/// CProcessIA3:: RenderScene : Renderiza la escena.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
  void							RenderScene											(CRenderManager * rm);

///<summary>
/// CProcessIA3:: RenderDebugInfo() : muestra la información del pitch, yaw, position y número de caras de los objetos estáticos y dinámicos.
///</summary>
///<param name=""></param>
///<returns name=""></returns>

  void							RenderDebugInfo									();

private:

	//functions

///<summary>
/// CProcessIA3:: CheckActionsFromInputs : Según una entrada de teclado ejecuta su acción asociada.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
  void  						CheckActionsFromInputs					(float elapsedTime);

  ///<summary>
/// CProcessIA3:: LoadXMLs : Carga todos los XML.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void  						LoadXMLs												();

///<summary>
/// CProcessIA3:: InitCameras : Crea las cámaras y las asocia a un objeto.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
void  						InitCameras											();
  
///<summary>
/// CProcessIA3:: GetPositionObj : Nos devuelve un Vect3f con la posición del objeto.
///</summary>
///<param name="p_vObj">Objeto del que vamos a obtener la posición.</param>
///<returns name="posicion">Vector de 3 floats con la posición del objeto.</returns>
  Vect3f						GetPositionObj									(CObject3D* p_vObj);

  
///<summary>
/// CProcessIA3:: UpdateLua : Scripts de LUA.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
  void 							UpdateLua												();

  
///<summary>
/// CProcessIA3:: MovPlayer : Calcula los movimientos y la velocidad del player.
///</summary>
///<param name="elapsedTime">Tiempo transcurrido.</param>
///<returns name=""></returns>
  void							MovPlayer                       (float elapsedTime);
  void              MovCamera                       ();
	void							MovPlayerIA                     (float elapsedTime);

///<summary>
/// CProcessIA3:: MovMouse : Movimiento del ratón asociado a la cámara.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
  void							MovMouse												(); 

///<summary>
/// CProcessIA3:: Render3D : Renderiza la parte de 2D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
  void							Render2D												(CRenderManager* rm);

  
///<summary>
/// CProcessIA3:: Render3D : Renderiza la parte de 3D.
///</summary>
///<param name="rm">Puntero a render manager.</param>
///<returns name=""></returns>
  void							Render3D												(CRenderManager* rm);
  void							RenderMovIA 										(CRenderManager* rm);
//  float             AjusteDireccion                 ();

///<summary>
/// CProcessIA3:: MemoryTest : Pruebas para grabar en RAM o VRAM.
///</summary>
///<param name=""></param>
///<returns name=""></returns>
  void							MemoryTest											();

	//variables
	//float																							m_fPosX, m_fPosY, m_TimerCounter, m_iDirection, m_fFPS;
	//float 																						m_fRotacion;
	//float																							m_MaxVision, m_MinVision;
	//bool																							m_bDrawCube;
  float                                             m_DeltaFromXML;
	//CObject3D*																				m_pObject3D;
	//CObject3D*  																			m_pObject3D_Pivotal;
  CActionToInput* m_pAction;
	CRenderableAnimatedInstanceModel * 								m_pPlayer1;
//	CRenderableAnimatedInstanceModel * 								m_pPlayer2;
  float m_fSpeed1;
 // float m_fSpeedZombie;
 // float m_fAngleParcialZombie;
 // float m_fAngleTotalZombie;
 // Vect3f m_V3DirPlayer2Optimo;
 // Vect3f m_V3DirectorPlayer2;
 // Vect3f m_V3RightPlayer2;
 // Vect3f m_V3UpPlayer2;
 // 
 // bool m_bChaser; //false=cazador player 1, true=cazador player 2
	//float																							m_MovementSpeed, m_RunTime, m_RestTime;
//  CSceneEffectManager* m_SceneEffectManager;
};

#endif

