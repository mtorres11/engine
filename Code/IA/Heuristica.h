//----------------------------------------------------------------------------------
// Class Heuristica
// Author: Miquel Rojas
//
// Description:
// Heuristicas para aplicar IA
// 
//----------------------------------------------------------------------------------
#pragma once

#ifndef HEURISTICA_H_
#define HEURISTICA_H_

class CHeuristica
{
public:
  virtual float AplicaHeuristica(const Vect3f&, const Vect3f&) = 0;
};

class CHeuristicaDistanciaReal2Puntos: public CHeuristica
{
public:
  float AplicaHeuristica(const Vect3f& p1,const Vect3f& p2){return p1.SqDistance(p2);}
};

class CHeuristicaMurosEntre2Puntos: public CHeuristica
{
public:
  float AplicaHeuristica(const Vect3f& p1,const Vect3f& p2){/*lanzar un ray de physx*/;}
};

#endif
