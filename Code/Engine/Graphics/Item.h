//----------------------------------------------------------------------------------
// CItem class
// Author: Katherine Capdevila
// Last Modification: 
// Description:
// Esta clase será la que manejara todos los items de vida.
//----------------------------------------------------------------------------------

#pragma once

#ifndef INC_ITEM_H_
#define INC_ITEM_H_

//#include <d3dx9.h>
#include "Base.h"
#include "MapManager/MapManager.h"
#include <string>
#include "Object3D/Object3D.h"
#include "Math/Matrix44.h"
#include "Math/Color.h"
#include "Math/Vector3.h"

//--- Forward Declaration
class CPhysicActor;
class CPhysicUserData;
class CRenderableObject;
class CRenderManager;


class CItem: public CObject3D
{
	public:
													CItem				();
	 virtual								~CItem			(){Done();}
											

		bool									Init        ();
	  virtual void					Done        ();
		bool									IsOk        () const { return m_bIsOk; }

		void									CreateItem	(std::string _sName, Vect3f _Position);
		std::string           GetName			(){return m_sName;}
		void									SetName     (std::string _sName){m_sName = _sName;}
		void									Update			(float elapsedTime);
		int									  GetTimer		(){return m_iCountDownTimer;}
		void									SetTimer		(int _iTime){m_iCountDownTimer = _iTime;}
		bool									GetStatus		(){return m_bStatus;}
		void									SetStatus		(bool _bStatus){m_bStatus = _bStatus;}
		CPhysicUserData*			GetUserData	(){return m_pUserData;}
		void									SetUserData	(CPhysicUserData* _pUserData){m_pUserData = _pUserData;}


	private:
		void        					Release   	();
		bool									m_bIsOk;          
		CRenderableObject*		m_pItem;		
		CPhysicActor *        m_pPhysXActor;
		std::string           m_sName;
		CPhysicUserData*			m_pUserData;
		int										m_iCountDownTimer;
		bool									m_bStatus;
		float									m_fTime;
};

#endif