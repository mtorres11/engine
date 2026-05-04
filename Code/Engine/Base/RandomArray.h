//----------------------------------------------------------------------------------
// Class RandomArray
// Author: Miquel Rojas
//
// Description: Genera un array de n elementos con valores aleatorios entre 0 y 1
//----------------------------------------------------------------------------------
#pragma once

#ifndef RANDOMARRAY_H_
#define RANDOMARRAY_H_

// Includes
#include "Base.h"
#include <vector>

//Class
class CRandomArray
{
public:
  // Init and End protocols
  CRandomArray() : m_bIsOk(false),m_iNumElems(0){}
  virtual ~CRandomArray() { Done(); }

///<summary>
/// CRandomArray:: Init : inicializacion.
///</summary>
///<param name="m">void</param>
///<returns name="">bool</returns>
	virtual	bool				Init								();

///<summary>
/// CRandomArray:: Done : finalizacion.
///</summary>
///<param name="m">void</param>
///<returns name="">void</returns>
  virtual void				Done               	();

///<summary>
/// CRandomArray:: IsOk : Devuelve si es correcto.
///</summary>
///<param name="">void</param>
///<returns name="">bool</returns>
  bool								IsOk               	() const { return m_bIsOk; }
  

  // -----------------------
  // Read functions
  // -----------------------

///<summary>
/// CRandomArray:: GetNumElems: Devuelve numero de elementos.
///</summary>
///<param name="">void</param>
///<returns name="">unsigned int</returns>
  unsigned int        GetNumElems         ()                      {return m_iNumElems;}

///<summary>
/// CRandomArray:: GetIndex: Devuelve la posicion del indice del array actual.
///</summary>
///<param name="">void</param>
///<returns name="">unsigned int</returns>
  unsigned int        GetIndex            ()                      {return m_iCurrentIndex;}

///<summary>
  /// CRandomArray:: GetValue: Avanza una posicion del array y lo devuelve (valor entre 0 y 1).
///</summary>
///<param name="">void</param>
///<returns name="">float</returns>
  float               GetValue            ()                      {AdvanceIndex();return m_vRandomArray[m_iCurrentIndex];}

  // -----------------------
  // Write functions
  // -----------------------

///<summary>
/// CRandomArray:: GenerateArray: Crea el array de aleatorios con numero de elementos correspondiente.
///</summary>
///<param name="nElems">int</param>
///<returns name="">void</returns>
  virtual void        GenerateArray       (unsigned int nElems);

///<summary>
/// CRandomArray:: SetIndex: Pone indice en la posicion deseada dentro del array.
///</summary>
///<param name="">unsigned int</param>
///<returns name="">void</returns>
  void                SetIndex            (unsigned int pos)               {m_iCurrentIndex = (pos < m_iNumElems)? pos : 0;}

private:

///<summary>
/// CRandomArray:: Release: Libera memoria.
///</summary>
///<param name="">void</param>
///<returns name="">void</returns>
  void        				Release   					();

///<summary>
/// CRandomArray:: AdvanceIndex: Avanza una posicion en el array, generalmente cuando se obtiene un valor de el.
///</summary>
///<param name="">void</param>
///<returns name="">void</returns>
  void                AdvanceIndex        ();
  
  // member variables
  bool                m_bIsOk;          // Initialization boolean control
  unsigned int        m_iNumElems;
  static unsigned int m_iCurrentIndex;  
  /*static*/ std::vector<float>  m_vRandomArray;
};

#endif