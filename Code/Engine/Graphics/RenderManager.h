//----------------------------------------------------------------------------------
// CRenderManager class
// Author: Enric Vergara
//
// Description:
// blablabla...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_RENDER_MANAGER_H_
#define INC_RENDER_MANAGER_H_

//---Forward declarations---
class CCamera;
//--------------------------

#include <d3dx9.h> 
#include <string>
#include "../Base/Base.h"
#include "Math\Color.h"

class CTexture;

struct SRectangle2D
{
   Vect2f    m_vBottomLeft;
   Vect2f    m_vTopRight;
};



class CRenderManager
{
public:

	typedef enum ETypeFlip{ 
	NONE_FLIP, 
	FLIP_X, 
	FLIP_Y
};

 typedef enum ETypeAlignment {
	 CENTER, 
	 UPPER_LEFT, 
	 UPPER_RIGHT, 
	 LOWER_LEFT, 
	 LOWER_RIGHT
 };

 typedef enum ETypeModePaint {
	 PAINT_WIREFRAME, 
	 PAINT_BOTH, 
	 PAINT_SOLID
 };

 typedef enum EtypeSphere {
	 COMPLETE, 
	 HALF_TOP, 
	 HALF_BOTTOM
 };

	CRenderManager(void):	m_bIsOk(false), m_pD3D(0), m_pD3DDevice(0),
												m_uWidth(0), m_uHeight(0) {};

  virtual ~CRenderManager(void) {Done();};

  bool                  Init(SInitParams _params, HWND hWnd);

  void                  Done            				() {if(IsOk()) Relase(); m_bIsOk=false;};
  bool                  IsOk            				() const  {return m_bIsOk;};

  void                  BeginRendering  				();
  void                  EndRendering    				();
  void                  Present									();

  void                  SetupMatrices   				(CCamera * camera);

	LPDIRECT3DDEVICE9			GetDevice								() {return m_pD3DDevice;}
	
	//----DebugRender Functions-------------------------------------------
	void									DrawLine								(const Vect3f &PosA, const Vect3f &PosB, CColor Color);
	void									DrawAxis								(float Size);
	void									DrawCube								(float Size, CColor color);
  void									DrawCube                (Vect3f Size, CColor color);
	void									DrawCamera							(CCamera* camera);
	void									DrawGrid								(float Size, CColor Color=colWHITE, int GridX=10, int32 GridZ=10 );
	void									DrawPlane								(float size, const Vect3f& normal, float distance, CColor Color, int GridX, int GridZ );
	//void									DrawPlane								(float Size, const Vect3f& normal, float distance,  CColor Color=colWHITE, int GridX=10, int32 GridZ=10, ETypeModePaint mode = PAINT_WIREFRAME);
	void									DrawRectangle2D					(const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor,uint32 edge_w, uint32 edge_h, CColor& edgeColor);
	//DrawSphere
  void	                DrawCircleV              (float x, float y, float _radius, CColor _color);
  void	                DrawCircleH              (float x, float z, float _radius, CColor _color);
	
  void									DrawSphere							(float Radius, const CColor&  Color, int Aristas);
	//void									DrawSphere							(float Radius, CColor Color=colWHITE, uint32 Aristas=10,  ETypeModePaint mode = PAINT_WIREFRAME,  EtypeSphere typeSphere = COMPLETE);
	//DrawQuad2D
	void									DrawQuad2D							(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color = colBLUE);
	void									DrawQuad2D							(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, SRectangle2D& coordText);
  void									DrawQuad2D							(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, ETypeFlip flip = NONE_FLIP);
	void									DrawTexturedQuad2D			(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture *Texture, CColor color = colBLUE);
	//DrawQuad3D
	void									DrawQuad3D							(const Vect3f& ul, const Vect3f& ur, const Vect3f& dl, const Vect3f& dr, CColor color);
	void									DrawQuad3D							(const Vect3f& pos, const Vect3f& up, const Vect3f& right, float w, float h, CColor color);
	//--------------------------------------------------------------------
	void									EnableAlphaBlend				(); //Habilitar Transparencias
	void									DisableAlphaBlend				();

	void									SetTransform						(D3DXMATRIX& matrix);
	void									SetTransform    				(Mat44f& m);

	void									GetWandH								(uint32& w, uint32& h)  { w = m_uWidth; h = m_uHeight;}

private:
	//functions
	void                  Relase									();
	void									GetWindowRect						(HWND hWnd);
  void									CalculateAlignment			(uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos);

	//variables
  bool                  m_bIsOk;
	LPDIRECT3D9						m_pD3D;										// direct3d interface
	LPDIRECT3DDEVICE9	  	m_pD3DDevice;	   					// direct3d device
	uint32								m_uWidth;
	uint32								m_uHeight;
};

#endif //INC_RENDER_MANAGER_H_