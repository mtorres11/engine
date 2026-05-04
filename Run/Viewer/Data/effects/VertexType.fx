#ifndef INC_VERTEXTYPE_H_
#define INC_VERTEXTYPE_H_

struct TNORMAL_TEXTURED_VERTEX_VS 
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
};

struct TNORMAL_TEXTURED_VERTEX_PS
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float4 PosLight: TEXCOORD2;
	float3 WorldPosition : TEXCOORD3;
	float3 SpotLightVec : TEXCOORD4;
	float fogFactor : FOG;
	//float fogFactor : TEXCOORD5;
};

struct TNORMALMAP_TEXTURED_VERTEX_VS
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float3 Tangent: TANGENT;
};
//Para declarar una estructura de vértice que utilizaremos como salida para el Vertex Shader y entrada para el Píxel Shader
struct TNORMALMAP_TEXTURED_VERTEX_PS 
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
	float3 WorldTangent: TEXCOORD3;
	float3 WorldBinormal: TEXCOORD4;
	float4 PosLight: TEXCOORD5;
};

struct TNORMAL_2TEXTURED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float4 UV2 : TEXCOORD1;
};

struct TNORMAL_2TEXTURED_VERTEX_PS{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float2 UV2 : TEXCOORD1;
	float3 WorldNormal : TEXCOORD2;
	float3 WorldPosition : TEXCOORD3;
	float4 PosLight: TEXCOORD4;
};

struct PNormalVertex
{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

//-- Andres ---------------------------
struct CAL3D_HW_VERTEX_VS
{
	float3 Position : POSITION;
	float4 Weight : BLENDWEIGHT;
	float4 Indices : BLENDINDICES;
	float4 Normal : NORMAL;
	float4 Tangent : TANGENT0;
	float4 BiNormal : BINORMAL0;
	float2 TexCoord : TEXCOORD0;
};

struct CAL3D_HW_VERTEX_PS
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
	float3 WorldTangent : TEXCOORD3;
	float3 WorldBinormal : TEXCOORD4;
};


struct ZBLUR_TEXTURED_VERTEX_PS
{
	float4 HPosition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
	float4 PosLight : TEXCOORD3;
	float2 Depth : TEXCOORD4;
};

#endif

