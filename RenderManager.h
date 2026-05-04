//----------------------------------------------------------------------------------
// CRenderManager class
// Author: Enric Vergara
//
// Description:
// This secures availability of the necessary render functions.
// It internally uses the direct3d library.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_RENDER_MANAGER_H_
#define INC_RENDER_MANAGER_H_

#include <d3dx9.h>
#include <vector>
#include <map>
#include "Math/Color.h"
#include "Script/ScriptRegister.h"

//---Forward Declarations--
class CCamera;
class CTexture;
class CScriptManager;
//-------------------------

//---------Definición de nuevos tipos--------------
struct SRectangle2D
{
	Vect2f	m_vBottomLeft;
	Vect2f	m_vTopRight;
};

typedef enum ETypeAlignment { CENTER, UPPER_LEFT, UPPER_RIGHT, LOWER_LEFT, LOWER_RIGHT };
typedef enum ETypeFlip			{ NONE_FLIP, FLIP_X, FLIP_Y};
typedef enum ETypeModePaint { PAINT_SOLID, PAINT_WIREFRAME, PAINT_BOTH};
typedef enum EtypeSphere		{ COMPLETE, HALF_TOP, HALF_BOTTOM };	
//-------------------------------------------------

class CRenderManager: public CScriptRegister 
{
public:
	//--- Init and End protocols------------------------------------------
	CRenderManager(): m_bIsOk(false), m_pD3D(NULL), m_pD3DDevice(NULL), m_hWnd(NULL), m_bPaintSolid(true),
										m_BackbufferColor_debug(0.5f,0.5f,0.5f), m_BackbufferColor_release(colBLACK),
										m_pBackBuffer(NULL) {}

	virtual ~CRenderManager() { Done(); }  

	bool		            Init			        (HWND hWnd, bool fullscreenMode, uint32 widthScreen, uint32 heightScreen);
	void		            Done			        ();
	bool		            IsOk			        () const { return m_bIsOk; }

	//----CScriptRegister interface---------------------------------------
	virtual void	RegisterFunctions				(CScriptManager* scriptManager);
	

	//---Rendering Stuff--------------------------------------------------
	void		            Screenshot_BeginRendering	();
	void		            Screenshot_EndRendering		();
	void		            BeginRendering	  				();
	void		            EndRendering	    				();
	void								BeginRendering2D					();
	void								EndRendering2D						();
	void							  SetupMatrices	    				(CCamera* camera);
	
	
	//----States Renderings-----------------------------------------------
	void								EnableAlphaBlend					();
	void								DisbaleAlphaBlend					();
	void								EnableZBuffering					();
	void								DisbaleZBuffering					();

	//----DebugRender Functions-------------------------------------------
	void								DrawAxis									(float Size);
	void								DrawGrid									(float Size, CColor Color=colWHITE, int GridX=10, int32 GridZ=10, ETypeModePaint mode = PAINT_WIREFRAME);
	void								DrawPlane									(	float Size, const Vect3f& normal, float distance,
																									CColor Color=colWHITE, int GridX=10, int32 GridZ=10, ETypeModePaint mode = PAINT_WIREFRAME );
	void								DrawCube									(float Size, CColor color=colWHITE, ETypeModePaint mode = PAINT_WIREFRAME);
	void								DrawBox										(float SizeX, float SizeY, float SizeZ, CColor Color=colWHITE, ETypeModePaint mode = PAINT_WIREFRAME);
	void								DrawSphere								(	float Radius, CColor Color=colWHITE, uint32 Aristas=10,  ETypeModePaint mode = PAINT_WIREFRAME,
																									EtypeSphere typeSphere = COMPLETE);
	void								DrawLine									(const Vect3f &PosA, const Vect3f &PosB, CColor Color=colWHITE);
	void								DrawCylinder							(	float Top_Radius, float Bottom_Radius, float h, uint32 Aristas=10, CColor Color=colWHITE, 
																									ETypeModePaint mode = PAINT_WIREFRAME, bool drawCover = false);
	void								DrawCapsule								(	float radius, float h, uint32 Aristas=10, CColor Color=colWHITE, ETypeModePaint mode = PAINT_WIREFRAME);
	void								DrawCamera								(CCamera* camera);
	void								DrawRectangle2D						(	const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor,
																									uint32 edge_w, uint32 edge_h, CColor& edgeColor );
	//----QuadRender Functions----------------------------------------------
	void								DrawQuad2D								(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color = colBLUE);
	void								DrawQuad2D								(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, SRectangle2D& coordText);
	void								DrawQuad2D								(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, ETypeFlip flip = NONE_FLIP);
	
	void								DrawQuad3D								(const Vect3f& pos, const Vect3f& up, const Vect3f& right, float w, float h, CTexture* texture, SRectangle2D& coordText);
	void								DrawQuad3D								(const Vect3f& pos, const Vect3f& up, const Vect3f& right, float w, float h, CTexture* texture);
	void								DrawQuad3D								(const Vect3f& pos, const Vect3f& up, const Vect3f& right, float w, float h, CColor color = colBLUE);
	void								DrawQuad3D								(const Vect3f& ul, const Vect3f& ur, const Vect3f& dl, const Vect3f& dr, CTexture* texture, SRectangle2D& coordText);
	void								DrawQuad3D								(const Vect3f& ul, const Vect3f& ur, const Vect3f& dl, const Vect3f& dr, CTexture* texture);
	void								DrawQuad3D								(const Vect3f& ul, const Vect3f& ur, const Vect3f& dl, const Vect3f& dr, CColor color = colBLUE);
	
	void								DrawTriangle3D						(const Vect3f& down_l, const Vect3f& down_r, const Vect3f& upper, CColor color);


	//----Transform Functions-----------------------------------------------
	void								SetTransform							(D3DXMATRIX& matrix);
	void								SetTransform							(Mat44f& matrix);


	//----Other Functions---------------------------------------------------
	void								GetWidthAndHeight					(uint32& w, uint32& h) const {w = m_uWidth; h = m_uHeight;}
	HWND								GetHWND										() {return m_hWnd;}
	void								SetPaintSolid							(bool flag) {m_bPaintSolid = flag;}
	bool								GetPaintSolid							() const  {return m_bPaintSolid;}
	void								SetBGColor								(const CColor& color);
	LPDIRECT3DDEVICE9		GetDevice() const 				{return m_pD3DDevice;}
	void								GetRay										(const Vect2i& mousePos, Vect3f& posRay, Vect3f& dirRay);
	LPD3DXSPRITE				GetSpriteHandler					()	{return m_pSprite_handler;}


private:
	void		            Release			      				();		// Release API specific stuff
	void								GetWindowRect							(HWND hWnd);
	void								CalculateAlignment				(uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & posFinal);

private:	
	bool				        					m_bIsOk;		     					 // Initialization boolean control
	bool				        					m_bPaintSolid;							// Paint solid or Wireframe
	HWND				        					m_hWnd;			     					 // 3D render window handle
	LPDIRECT3D9			    					m_pD3D;			     					 // direct3d interface
	LPDIRECT3DDEVICE9	  					m_pD3DDevice;	   					 // direct3d device
	uint32												m_uWidth;										// width of the client windows
	uint32												m_uHeight;									// height of the client windows
	CColor												m_BackbufferColor_debug;		// Clear the backbuffer with this color in debug mode 
	CColor												m_BackbufferColor_release;	// Clear the backbuffer with this color in release mode
	LPD3DXSPRITE									m_pSprite_handler;					// Sprite handler (2d quad)
	CTexture	*										m_pScreenshotTexture;
	LPDIRECT3DSURFACE9						m_pBackBuffer;
};

#endif //INC_RENDER_MANAGER_H_

