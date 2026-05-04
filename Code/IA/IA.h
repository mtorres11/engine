//----------------------------------------------------------------------------------
// Class IA
// Author: Miquel Rojas
//
// Description:
// IA de personajes
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef IA_H_
#define IA_H_

// Includes
#include "Base.h"
#include <vector>
#include <map>
#include <string>
#include "math/Vector3.h"
#include "Heuristica.h"

//forward declarations
class CObject3D;
class CRenderManager;

//structs
struct SArista
{
  std::string idVecino;
  float distancia;
};

struct SNodo
{
  SNodo(){posicion(0.0,0.0,0.0);}
  virtual ~SNodo(){}

  Vect3f posicion;
  std::vector<SArista>vecinos;
};

class SInfoNodo
{
public:
  SInfoNodo():s_sPadre(""){}
  virtual ~SInfoNodo(){/*todo..*/s_sPadre.clear();}

  std::string s_sPadre;
  float       s_fCosteActual;
  bool        s_bVisitado;
};


//Class
class C_IA
{
public:
  // Init and End protocols
  C_IA () : m_bIsOk(false),m_vMejorCamino(0){}
  virtual ~C_IA () { Done(); }

///<summary>
/// C_IA:: Init: Inicializa la IA
///</summary>
///<param name="">void</param>
///<returns name="">bool</returns>
	virtual	bool				Init								();

///<summary>
/// C_IA:: Done: Valida la IA de un personaje
///</summary>
///<param name="">void</param>
///<returns name="">void</returns>
  virtual void				Done               	();

///<summary>
/// C_IA:: IsOk: IA creada correctamente
///</summary>
///<param name="">void</param>
///<returns name="">bool</returns>
  bool								IsOk               	() const { return m_bIsOk; }

  // -----------------------
  // Read functions
  // -----------------------
  
///<summary>
/// C_IA:: CargaGrafo: Crea un grafo en memoria con los datos de paths.dat
///</summary>
///<param name="">std::string</param>
///<returns name="">bool</returns>
  bool CargaGrafo(const std::string&);

///<summary>
/// C_IA:: BuscaNodoMasCercano: Devuelve el nodo mas cercano a un punto 3D
///</summary>
///<param name="">std::string</param>
///<returns name="">Vect3f</returns>
  std::string BuscaNodoMasCercano(Vect3f);

///<summary>
/// C_IA:: Dijsktra: Devuelve el camino mas corto de un grafo sin tener en cuenta ninguna heuristica
///</summary>
///<param name="p1">std::string</param>
///<param name="p2">std::string</param>
///<returns name="">void</returns>
  void Dijsktra(const std::string&, const std::string&);

///<summary>
/// C_IA:: AStar: Devuelve el camino mas corto de un grafo teniendo en cuenta una heuristica
///</summary>
///<param name="p1">std::string</param>
///<param name="p2">std::string</param>
///<returns name="">void</returns>
  void AStar(const std::string&, const std::string&, CHeuristica&);

///<summary>
/// C_IA:: AStar: Devuelve el camino mas corto de un grafo teniendo en cuenta una heuristica(obsoleta, utilizar CHeuristica)
///</summary>
///<param name="p1">std::string</param>
///<param name="p2">std::string</param>
///<returns name="">void</returns>
//  void AStar(const std::string&, const std::string&);

///<summary>
/// C_IA:: GetGrafo: Devuelve el grafo generado en paths.dat
///</summary>
///<param name="">void</param>
///<returns name="">const std::map<std::string, SNodo*></returns>
  const std::map<std::string, SNodo*> GetGrafo(void){return m_Grafo;}

///<summary>
/// C_IA:: GetMejorCamino: Devuelve el mejor camino generado en el algoritmo de recorrido del grafo
///</summary>
///<param name="">void</param>
///<returns name="">std::vector<std::string></returns>
  std::vector<std::string> GetMejorCamino(void){return m_vMejorCamino;}

///<summary>
/// C_IA:: DibujaDummies: Dibuja el grafo generado en paths.dat
///</summary>
///<param name="">void</param>
///<returns name="">const std::map<std::string, SNodo*></returns>
  void DibujaDummies(CRenderManager*rm);

///<summary>
  /// C_IA:: heurDistanciaReal2Puntos: heuristica de distancia entre dos puntos (obsoleta, utilizar CHeuristica)
///</summary>
///<param name="_origen">const Vect3f&</param>
///<param name="_destino">const Vect3f&</param>
///<returns name="">float</returns>
  float heurDistanciaReal2Puntos(const Vect3f &origen, const Vect3f &destino);
  // -----------------------
  // Write functions
  // -----------------------

private:
  void        				Release   					();

  // member variables
  bool                                m_bIsOk;          // Initialization boolean control
  static std::map<std::string,SNodo*> m_Grafo;          // contiene el grafo general
  std::vector<std::string>            m_vMejorCamino;   // mejor camino para cada instancia
  std::map<std::string,SInfoNodo>     m_Info;           // map con informacion
};


#endif