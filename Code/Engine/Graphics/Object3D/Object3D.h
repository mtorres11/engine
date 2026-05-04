//----------------------------------------------------------------------------------
// CObject3D class
// Author: Enric Vergara
// Modificada: (andres) getTransform
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_OBJECT_3D_H_
#define INC_OBJECT_3D_H_

#include "Math\MathTypes.h"
#include "Math\Matrix44.h"
#include "Math\Vector3.h"

//Forward declaration
class CBoundingObject;
class CBoundingBox;

//NOTA IMPORTANTE (TODO)
//Si se quisiera optimizar la manipulacion del yaw, pitch y roll se puede agregar
//una variable booleana que indique si esta o no actualizados los valores de yaw,
//pitch y roll y en caso de esta actualizados devuelve el valor que tenia y en caso
//de no estarlo UNICAMENTE en este punto recalcula los valores y los devuelve.
//Se puede hacer en direccion inversa con otra variable para la matriz en caso de
//manipular el yaw, pitch y roll por medio de los GET/SET y asi saber si la matriz
//de transformacion esta o no actualizada y recalcularla solo en caso de que sea necesario.
class CObject3D
{
public:
	CObject3D(const Vect3f& pos, float yaw=0.0f, float pitch=0.0f, float roll=0.0f, float scalarX=1.f, float scalarY=1.f, float scalarZ=1.f);
	//CObject3D(const Vect3f& pos, float yaw, float pitch, float roll = 0);
	CObject3D();
  ~CObject3D(void);
	//virtual ~CObject3D(void) {/*Nothing*/;}

  //---BoundingBox Functions
  bool                InitializeBoundingObject   (int BoundingObject);
	//---Get Functions
	float					    	GetYaw			        () const {return m_fYaw;}
	float					    	GetRoll			        () const {return m_fRoll;}
	float					    	GetPitch		        () const {return m_fPitch;}
	const Vect3f&	    	GetPosition	        () const {return m_Position;}
  float       	    	GetScalarX	        () const {return m_ScalarX;}
  float       	    	GetScalarY	        () const {return m_ScalarY;}
  float       	    	GetScalarZ	        () const {return m_ScalarZ;}
  bool                GetVisible          () const {return m_bVisible;}
  Mat44f              GetMat44            () const {return m_Mat44;}
  CBoundingObject *   GetBoundingObject   () const {return m_BoundingObject;}
  Mat44f              GetTransform        ();

	//---Set Functions
  void    SetMat44          ( Mat44f matrix );
  void		SetYaw			      ( float yaw );
	void		SetPitch		      ( float pitch );
	void		SetRoll			      ( float roll );
  void		SetPosition      	( const Vect3f& pos );
  //Actualizar la matriz cuando se escala un objeto
  void		SetScalar 	      ( Vect3f scale );
  void		SetScalar 	      ( float scalarX, float scalarY, float scalarZ);
	void		SetScalarX	      ( float scalarX );
  void		SetScalarY	      ( float scalarY );
  void		SetScalarZ	      ( float scalarZ );
  void    SetBoundingObject ( CBoundingObject * bb )                      {m_BoundingObject = bb;}
  void    SetVisible        ( bool visible )                              {m_bVisible = visible;}

protected:
	Vect3f	m_Position;
  float   m_ScalarX, m_ScalarY, m_ScalarZ;
	float		m_fYaw;
	float		m_fPitch;
	float		m_fRoll;
  bool    m_bVisible;
  Mat44f  m_Mat44;
  CBoundingObject * m_BoundingObject;

  void RecalculateMat44f();
};

#endif //INC_OBJECT_3D_H_