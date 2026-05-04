/*----------------------------------------------------------------------------------*\
 * CCore class                                                                      *
 * Author: Marco Torres                                                             *
 * Last Modification: 10/01/2010 - CLightManager - Andrea Cristobo                  *
 * Last Modification: 01/05/2010 - CTriggerManager - Miquel Rojas                   *
 * Description:                                                                     *
 * Clase Engine.                                                                    *
 * Descripcion aqui                                                                 *
\*----------------------------------------------------------------------------------*/

#pragma once
#ifndef INC_CORE_H_
#define INC_CORE_H_

#include "Base.h"
#include "Singleton.h"
#include <vector>
#include <Windows.h>


//--Forward declarations--
class CEngine;
class CRenderManager;
class CFontManager;
class CLanguageManager;
class CInputManager;
class CStaticMeshManager;
class CRenderableObjectsManager;
class CLightManager;
class CAnimatedModelManager;
class CTextureManager;
class CEffectManager;
class CSceneEffectManager;
class CTimer;
class CScriptManager;
class CCameraManager;
class CGUIManager;
class CPhysicsManager;
class CParticleManager;
class CSoundManager;
class CRenderablePhysXManager;
class CTriggerManager;
class CItemManager;
class CBillboardManager;

class CCore: public CSingleton<CCore>{
public:
  //Consruct
  CCore(): m_pEngine(0), m_pRenderManager(0), m_pFontManager(0), m_pLanguageManager(0), m_pInputManager(0), m_pStaticMeshManager(0), m_pRenderableObjectsManager(0), m_pLightManager(0), 
					 m_pAnimatedModelManager(0), m_TextureManager(0), m_pEffectManager(0), m_pSceneEffectManager(0), m_pScriptManager(0), m_pCameraManager(0), m_pGUIManager(0),
           m_pPhysicsManager(0), m_pParticleManager(0) , m_pSoundManager(0), m_RenderablePhysXManager(0), m_pTriggerManager(0), m_pItemManager(0), m_pBillboardManager(0) {}

  //Destruct
	virtual ~CCore();

  //functions
  bool														Init																	(SInitParams _params, HWND hWnd, CEngine* _engine);
	void														Update																(float elapsedTime);
	void														Render																();
  void  													CheckActionsFromInputs								();

  //get set
	void														SetRenderManager											(CRenderManager * rm)		{m_pRenderManager = rm;}
	CRenderManager *								GetRenderManager											()											{return m_pRenderManager;}
	CFontManager *									GetFontManager												()											{return m_pFontManager;}
	CLanguageManager *							GetLanguageManager										()											{return m_pLanguageManager;}
	CInputManager *									GetInputManager												()											{return m_pInputManager;}
  CStaticMeshManager*							GetStaticMeshManager									()											{return m_pStaticMeshManager;}
  CRenderableObjectsManager*			GetRenderableObjectsManager						()											{return m_pRenderableObjectsManager;}
	CLightManager*									GetLigthManager												()											{return m_pLightManager;}
	CAnimatedModelManager*					GetAnimatedModelManager								()											{return m_pAnimatedModelManager;}
  CTextureManager *								GetTextureManager											()											{return m_TextureManager;}
  CEffectManager *								GetEffectManager											()											{return m_pEffectManager;}
  CSceneEffectManager *						GetSceneEffectManager									()											{return m_pSceneEffectManager;}
  CGUIManager *								    GetGUIManager											    ()											{return m_pGUIManager;}
  CTimer*                         GetTimer															()											{return m_pTimer;}
  CScriptManager *                GetScriptManager											()											{return m_pScriptManager;}
	CCameraManager *								GetCameraManager											()											{return m_pCameraManager;}
	CPhysicsManager *								GetPhysicsManager											()											{return m_pPhysicsManager;}
	CParticleManager *							GetParticleManager										()											{return m_pParticleManager;}
	CSoundManager *									GetSoundManager												()											{return m_pSoundManager;}
  CRenderablePhysXManager *				GetRenderablePhysXManager							()											{return m_RenderablePhysXManager;}
  CTriggerManager*                GetTriggerManager                     ()                      {return m_pTriggerManager;}
	CItemManager*										GetItemManager											  ()                      {return m_pItemManager;}
	CBillboardManager*							GetBillboardManager                   ()											{return m_pBillboardManager;}
	
	void														SetProcess(const std::string& _sProcess);
	void														Exit();

private:
	CEngine *												m_pEngine;
  CBillboardManager *							m_pBillboardManager;
  CRenderManager *								m_pRenderManager;
	CFontManager*										m_pFontManager;
	CLanguageManager*								m_pLanguageManager;
	CInputManager*									m_pInputManager;
	CLightManager*									m_pLightManager;
  CStaticMeshManager*							m_pStaticMeshManager;
  CRenderableObjectsManager*			m_pRenderableObjectsManager;
	CAnimatedModelManager*      		m_pAnimatedModelManager;
  CTextureManager *           		m_TextureManager;
  CEffectManager *            		m_pEffectManager;
  CSceneEffectManager *           m_pSceneEffectManager;
	bool														m_bIsOk;
  CTimer*                     		m_pTimer;
  CScriptManager *            		m_pScriptManager;
	CCameraManager *            		m_pCameraManager;
  CGUIManager *               		m_pGUIManager;
	CPhysicsManager*								m_pPhysicsManager;
	CParticleManager *							m_pParticleManager;
	CSoundManager *                 m_pSoundManager;
  CRenderablePhysXManager *       m_RenderablePhysXManager;
  CTriggerManager*                m_pTriggerManager;
	CItemManager*										m_pItemManager;
};

#endif //INC_CORE_H_