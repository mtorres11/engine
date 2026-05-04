#pragma once
#ifndef INC_VERTEX_TYPE_H_
#define INC_VERTEX_TYPE_H_

#include <d3dx9.h>
#include "base.h"

#define 	VERTEX_TYPE_GEOMETRY			0x0001
#define 	VERTEX_TYPE_NORMAL				0x0002
#define 	VERTEX_TYPE_TANGENT				0x0004
#define 	VERTEX_TYPE_BINORMAL			0x0008
#define 	VERTEX_TYPE_TEXTURE1			0x0010
#define 	VERTEX_TYPE_TEXTURE2			0x0020
#define 	VERTEX_TYPE_DIFFUSE				0x0040
#define 	VERTEX_TYPE_WEIGHTS				0x0080
#define 	VERTEX_TYPE_INDEX_BONES		0x0100

// mascaras para el alpha -- se combinan con el vertex_type
#define   ALPHA_TEST                0x0200
#define   ALPHA_BLEND               0x0400

#define   NUMPARTICLES							50
#define   D3DFVF_PARTICLE           (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_PSIZE)
#define		D3DFVF_VERTEX_TEXTURED		(D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
void CalcTangentsAndBinormals(void *VtxsData, unsigned short *IdxsData, size_t VtxCount, size_t IdxCount, size_t VertexStride, size_t GeometryStride, size_t NormalStride, size_t TangentStride, size_t BiNormalStride, size_t TextureCoordsStride);

struct VERTEX_TEXTURED
{
	float x, y, z;
	D3DCOLOR color;
	float u, v; 

	static inline int GetFVFBillboard()
	{
		return D3DFVF_VERTEX_TEXTURED;
	}
};

struct VERTEX_PARTICLE
{
   D3DXVECTOR3    position;
   float					pointsize;
   D3DCOLOR				color;

	 static inline unsigned short GetFVFVertexParticle()
	 {
		 return D3DFVF_PARTICLE;
	 }
}; 
//Geometría y Color
struct TCOLORED_VERTEX
{
  float x, y, z;
  unsigned long color;
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_DIFFUSE;
  }
  static inline unsigned int GetFVF()
  {
    return D3DFVF_XYZ|D3DFVF_DIFFUSE;
  }
  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

//Geometría, Normal y Color
struct TNORMAL_COLORED_VERTEX
{
  float x, y, z; // geometría
  float nx, ny, nz; // normal
  unsigned long color; // color
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE;
  }
  static inline unsigned int GetFVF()
  {
    return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE;
  }
  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

//Geometría, Normal, Color, Textura1
struct TNORMAL_COLORED_TEXTURE1_VERTEX
{
  float x, y, z; // geometría
  float nx, ny, nz; // normal
  unsigned long color; // color
  float u1, v1; // textura1
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF()
  {
    return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

//Geometría, Normal, Color, Textura2
struct TNORMAL_COLORED_TEXTURE2_VERTEX
{
  float x, y, z; // geometría
  float nx, ny, nz; // normal
  unsigned long color; // color
  float u1, v1; // textura1
  float u2, v2; // textura2
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_DIFFUSE|VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF()
  {
    return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX2;
  }
  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};


//Geometría, Textura1
struct TTEXTURE1_VERTEX
{
  float x, y, z; // geometría
  float u1, v1; // textura1
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF()
  {
    return D3DFVF_XYZ|D3DFVF_TEX1;
  }
  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

//Geometría, Textura1, Textura2
struct TTEXTURE2_VERTEX
{
  float x, y, z; // geometría
  float u1, v1; // textura1
  float u2, v2; // textura2
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF()
  {
    return D3DFVF_XYZ|D3DFVF_TEX2;
  }
  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

//Geometría, Normal, Textura1
struct TNORMAL_TEXTURE1_VERTEX
{
  float x, y, z; // geometría
  float nx, ny, nz; // normal
  float u1, v1; // textura1
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1;
  }
  static inline unsigned int GetFVF()
  {
    return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1;
  }

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }

};

//Geometría, Normal, Textura2
struct TNORMAL_TEXTURE2_VERTEX
{
  float x, y, z; // geometría
  float nx, ny, nz; // normal
  float u1, v1; // textura1
  float u2, v2; // textura2
  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE2;
  }
  static inline unsigned int GetFVF()
  {
    return D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX2;
  }

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

//Geometría, Normal, Tangente, binormal,Textura
struct TNORMAL_TANGENT_BINORMAL_TEXTURED
{
  float x, y, z;
  float nx, ny, nz, nw;
  float tangentx, tangenty, tangentz, tangentw;
  float binormalx, binormaly, binormalz, binormalw;
  float tu,tv;

  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|VERTEX_TYPE_TEXTURE1;
  }

  static inline unsigned int GetFVF()
  {
    return 0;
  }

  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};
//Geometría, Normal, Tangente, binormal,Textura2
struct TNORMAL_TANGENT_BINORMAL_TEXTURED2
{
  float x, y, z;
  float nx, ny, nz, nw;
  float tangentx, tangenty, tangentz, tangentw;
  float binormalx, binormaly, binormalz, binormalw;
  float u1, v1; // textura1
  float u2, v2; // textura2
  
  static inline unsigned short GetVertexType()
  {
    return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TANGENT|VERTEX_TYPE_BINORMAL|VERTEX_TYPE_TEXTURE2;
  }

  static inline unsigned int GetFVF()
  {
    return 0;
  }

  static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;

  static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();

  static void ReleaseVertexDeclaration()
  {
    CHECKED_RELEASE(s_VertexDeclaration);
  }
};

struct CAL3D_HW_VERTEX
{
	float x, y, z;
	float weights[4];
	float indices[4];
	float nx, ny, nz;
	float tu,tv;

	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY|VERTEX_TYPE_NORMAL|VERTEX_TYPE_TEXTURE1|VERTEX_TYPE_WEIGHTS|VERTEX_TYPE_INDEX_BONES;
	}

	static inline unsigned int GetFVF()
	{
		return 0;
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
	
	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
};

struct VERTEX_INSTANCING_DATA
{
  D3DXMATRIX    matrizTrans;
   
	static inline unsigned short GetVertexType()
	{
		return VERTEX_TYPE_GEOMETRY;
	}

	static inline unsigned int GetFVF()
	{
		return 0;
	}
	
	static LPDIRECT3DVERTEXDECLARATION9 s_VertexDeclaration;
	
	static LPDIRECT3DVERTEXDECLARATION9 & GetVertexDeclaration();
	
	static void ReleaseVertexDeclaration()
	{
		CHECKED_RELEASE(s_VertexDeclaration);
	}
}; 


#endif