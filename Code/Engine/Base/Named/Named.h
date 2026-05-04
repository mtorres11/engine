//----------------------------------------------------------------------------------
// CNamed Class
// Author: Miquel Rojas 
//
// Description:
// Trabaja con nombres de objetos
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef NAMED_H_
#define NAMED_H_

// Includes
#include <string>
//Class
class CNamed
{
public:
  // Init and End protocols
  CNamed () : m_bIsOk(false), m_sName(""){}
  CNamed (std::string name) : m_bIsOk(false), m_sName(name){}
  virtual ~CNamed() { Done(); }

	virtual	bool				Init								();
  virtual void				Done               	();
  bool								IsOk               	() const { return m_bIsOk; }

  // -----------------------
  // Read functions
  // -----------------------
  std::string         GetName             (void)const {return m_sName;}
  std::string         GetName             (void) {return m_sName;}
  // -----------------------
  // Write functions
  // -----------------------
  void                SetName             (std::string _n){m_sName = _n;}
private:
  void        				Release   					();

  // member variables
  bool                m_bIsOk;          // Initialization boolean control
  
	// Types 
  std::string m_sName;
  //uncomment to derived classes
  //typedef CProcess Inherited;
};


#endif