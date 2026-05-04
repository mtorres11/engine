//----------------------------------------------------------------------------------
// Class
// Author: 
//
// Description:
// 
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef _H_
#define _H_

// Includes

//Class
class CXMLTreeNode
{
public:
  // Init and End protocols
  CXMLTreeNode () : m_bIsOk(false){}
  virtual ~CXMLTreeNode () { Done(); }

	virtual	bool				Init								();
  virtual void				Done               	();
  bool								IsOk               	() const { return m_bIsOk; }

  // -----------------------
  // Read functions
  // -----------------------

  // -----------------------
  // Write functions
  // -----------------------

private:
  void        				Release   					();

  // member variables
  bool                m_bIsOk;          // Initialization boolean control

	// Types 
	//uncomment to derived classes
  //typedef CProcess Inherited;
};


#endif