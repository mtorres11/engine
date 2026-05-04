//----------------------------------------------------------------------------------
// CItem class
// Author: Katherine Capdevila
// Last Modification: 
// Description:
// Esta clase será la que manejara todos los items de vida.
//----------------------------------------------------------------------------------

#pragma once

#ifndef INC_ITEM_MANAGER_H_
#define INC_ITEM_MANAGER_H_

#include <d3dx9.h>
#include "Base.h"
#include "MapManager/MapManager.h"
#include "Math/Vector3.h"
#include "Item.h"
#include "RenderableObjects/RenderableObject.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "Utils/CountDownTimer.h"

class CPhysicActor;


class CItemManager: public CMapManager<CItem>
{
	public:
										CItemManager() :	m_bIsOk(false), m_iNumItems(0){}
		virtual					~CItemManager()		{ Done(); }
								
		virtual bool		Init							();
		virtual void		Done              ();
		void        		Release   				();
		
		bool						IsOk              () const { return m_bIsOk; }

		void						AddItem						(Vect3f _vPosition);
		void						Update						(float elapsedTime);
		void						CheckColisions		();
		void						CheckTimer				();
		
	private:
		bool                m_bIsOk;
		int									m_iNumItems;
		CPhysicUserData*		m_pPhysicUserdata;

};

#endif