//----------------------------------------------------------------------------------
// CNamed Class
// Author: Katherine Capdevilla
//
// Description:
// Para ver si una clase esta activa
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_ACTIVE_H_
#define INC_ACTIVE_H_

class CActive
{
	public:
	  
		CActive () : m_bIsOk(false){}
		virtual ~CActive() { Done(); }

		virtual	bool				Init								();
		virtual void				Done               	();
		bool								IsOk               	() const { return m_bIsOk; }

		bool								GetActive             ();
		void                SetActive             (bool state);
    
		void        				Release   					();
		bool                m_bIsOk;          
		
};

#endif