#pragma once

#ifndef __BASE_CONTROL_H__
#define __BASE_CONTROL_H__

class CBaseControl
{
public:
									CBaseControl():m_bIsOk    (false)         {};
  virtual       	~CBaseControl             ()              {};
  void          	Done                      ()              {if(IsOk()) {this->Release(); m_bIsOk=false;}};
  bool          	IsOk                      () const        {return m_bIsOk;};
 
private:
  bool          	m_bIsOk;

protected:
  virtual void  	Release                   ()=0;
  void          	SetOk                     (bool _bIsOk)   {m_bIsOk = _bIsOk;};
};

#endif