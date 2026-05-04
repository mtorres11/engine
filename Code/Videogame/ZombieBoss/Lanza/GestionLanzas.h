//----------------------------------------------------------------------------------
// Class
// Author: Miquel Rojas
//
// Description:
// control de lanzas
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef GESTIONLANZAS_H_
#define GESTIONLANZAS_H_

// Includes
class CRenderManager;
class CLanza;
#include <vector>
#include "Math/Vector3.h"

//Class
class CGestionLanzas
{
public:
  // Init and End protocols
  CGestionLanzas() : m_bIsOk(false),m_uiNumIteracionLanzas(0), m_uiNumTotalIteracionLanzas(0), m_uiNumLanzas(0), m_vLanzas(){}
  virtual ~CGestionLanzas() { Done(); }

	virtual	bool				Init								(unsigned int _numLanzas, unsigned int _numIters);
  virtual void				Done               	();
  bool								IsOk               	() const { return m_bIsOk; }

  virtual void        Update              (float _elapsedTime);
  virtual void        Render              (CRenderManager* rm);

  // -----------------------
  // Read functions
  // -----------------------
  unsigned int        GetNumIteracionLanzas()                                 {return m_uiNumIteracionLanzas;}
  unsigned int        GetNumTotalIteracionLanzas  ()                          {return m_uiNumTotalIteracionLanzas;}

  // -----------------------
  // Write functions
  // -----------------------
  void                SetNumIteracionLanzas(unsigned int n)                    {m_uiNumIteracionLanzas = n;}
  void                SetNumTotalIteracionLanzas(unsigned int n)                    {m_uiNumTotalIteracionLanzas = n;}

  // -----------------------
  // Logic functions
  // -----------------------
  void                DefinirOleadaLanza  ();
  void                PosicionaLanzas     (Vect3f _centro, float _dist);
private:
  void        				Release   					();

  // member variables
  bool                m_bIsOk;          // Initialization boolean control
  unsigned int        m_uiNumIteracionLanzas;
  unsigned int        m_uiNumTotalIteracionLanzas;
  unsigned int        m_uiNumLanzas;
  std::vector<CLanza*> m_vLanzas;
};


#endif