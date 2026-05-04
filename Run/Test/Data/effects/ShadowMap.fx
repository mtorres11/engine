#ifndef INC_SHADOWMAP_H_
#define INC_SHADOWMAP_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"
#include "lights.fx"
//#include "NormalTexturedVertex.fx"

//-------------Lista de Efectos--------------------
//--- ShadowMap -----------------------------------
//--- Cal3D ---------------------------------------


float3 CalcAnimtedPos(float4 Position, float4 Indices, float4 Weight)
{
	float3 l_Position=0;
	l_Position = mul(g_Bones[Indices.x], Position) * Weight.x;
	l_Position += mul(g_Bones[Indices.y], Position) * Weight.y;
	l_Position += mul(g_Bones[Indices.z], Position) * Weight.z;
	l_Position += mul(g_Bones[Indices.w], Position) * Weight.w;
	return l_Position;
}

//Efecto ShadowMap
//--------------------------------------------------
void AnimatedShadow(CAL3D_HW_VERTEX_VS IN, out float4 oPos : POSITION, out float2 Depth : TEXCOORD0)
{
   float3 l_Position = CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
   oPos = mul( float4(l_Position,1.0), g_WorldViewProjectionMatrix );
   Depth.xy = oPos.zw;
}

void VertShadow(float4 Pos : POSITION,float3 Normal : NORMAL,out float4 oPos : POSITION,out float2 Depth : TEXCOORD0 )
{
	// Compute the projected coordinates
	oPos = mul( Pos, g_WorldViewProjectionMatrix );
	
	// Store z and w in our spare texcoord
	Depth.xy = oPos.zw;
}
void PixShadow( float2 Depth : TEXCOORD0,out float4 Color : COLOR)
{
	// Depth is z / w
//	Color = (Depth.x / Depth.y)/100.0;
	Color = Depth.x / Depth.y;
	//Color=float4(l_Color,l_Color,l_Color,1.0);
}

//Efecto ShadowMap para static meshes
//--------------------------------------------------
technique StaticMeshesTechniqueShadowMap
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}
//----------------------------------------


//Efecto ShadowMap para modelos animados
//--------------------------------------------------
technique Cal3DTechniqueShadow
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 AnimatedShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}

void CalcAnimatedNormalTangent(float3 Normal, float3 Tangent, float4 Indices, float4 Weight, out float3 OutNormal, out float3 OutTangent)
{
	OutNormal = 0;
	OutTangent =0;
	float3x3 m;
	m[0].xyz = g_Bones[Indices.x][0].xyz;
	m[1].xyz = g_Bones[Indices.x][1].xyz;
	m[2].xyz = g_Bones[Indices.x][2].xyz;
	OutNormal += mul(m, Normal.xyz)* Weight.x;
	OutTangent += mul(m, Tangent.xyz)* Weight.x;
	m[0].xyz = g_Bones[Indices.y][0].xyz;
	m[1].xyz = g_Bones[Indices.y][1].xyz;
	m[2].xyz = g_Bones[Indices.y][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.y);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.y);
	m[0].xyz = g_Bones[Indices.z][0].xyz;
	m[1].xyz = g_Bones[Indices.z][1].xyz;
	m[2].xyz = g_Bones[Indices.z][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.z);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.z);
	m[0].xyz = g_Bones[Indices.w][0].xyz;
	m[1].xyz = g_Bones[Indices.w][1].xyz;
	m[2].xyz = g_Bones[Indices.w][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.w);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.w);
	OutNormal = normalize(OutNormal);
	OutTangent = normalize(OutTangent);
}
CAL3D_HW_VERTEX_PS RenderCal3DHWVS(CAL3D_HW_VERTEX_VS IN)
{
	CAL3D_HW_VERTEX_PS OUT=(CAL3D_HW_VERTEX_PS)0;
	float3 l_Normal= 0;
	float3 l_Tangent=0;
	CalcAnimatedNormalTangent(IN.Normal.xyz, IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal, l_Tangent);
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	l_Position.x = -l_Position.x;
	float4 l_WorldPosition=float4(l_Position, 1.0);
	OUT.WorldPosition=mul(l_WorldPosition,g_WorldMatrix);
	OUT.WorldNormal=normalize(mul(l_Normal,g_WorldMatrix));
	OUT.WorldTangent=normalize(mul(l_Tangent,g_WorldMatrix));
	OUT.WorldBinormal=mul(cross(l_Tangent,l_Normal),(float3x3)g_WorldMatrix);
	OUT.UV = IN.TexCoord.xy;
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProjectionMatrix );
	return OUT;
}
float4 RenderCal3DHWPS(CAL3D_HW_VERTEX_PS IN) : COLOR
{
	float3 Nn=normalize(IN.WorldNormal);
	//CalcBumpMap(IN.WorldPosition, IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);
	float4 l_SpecularColor = 1.0;
	float4 l_DiffuseColor=tex2D(DiffuseTextureSampler, IN.UV);
	//CalcLighting (IN.WorldPosition, Nn, l_DiffuseColor, l_SpecularColor);
	float3 l_TotalDiffuseValue=0;
	float3 l_TotalSpecularValue=0;

	CalcLighting(Nn, IN.WorldPosition, l_TotalDiffuseValue, l_TotalSpecularValue);
	// float3 l_Ambient=g_LightAmbient*g_LightAmbientColor*l_DiffuseColor.xyz;
	// return float4(l_Ambient+l_TotalDiffuseValue+l_TotalSpecularValue,l_DiffuseColor.a);
	//return l_DiffuseColor;
	return tex2D(DiffuseTextureSampler,IN.UV)*float4(g_LightAmbient.xyz * g_LightAmbient.w + (l_TotalDiffuseValue+l_TotalSpecularValue), 1.0);
}

float4 NormalTexturedVertexPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
	//En el pixel shader deberemos introducir el siguiente código
	float2 ShadowTexC = 0.5 * IN.PosLight.xy / IN.PosLight.w + float2( 0.5, 0.5 );
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	// PETA EN ESTA LINEA
	float LightAmount = (tex2D( ShadowTextureSampler, ShadowTexC ) + SHADOW_EPSILON) < (IN.PosLight.z / IN.PosLight.w) ? 0.0f: 1.0f;
	//float LightAmount = (tex2D( ShadowTextureSampler, ShadowTexC ) + SHADOW_EPSILON < IN.PosLight.z / IN.PosLight.w)? 0.0f: 1.0f;
	// 1 – Píxel iluminado
	// 0 – Píxel en sombra
	//return float4(LightAmount,LightAmount,LightAmount,1.0);
	//return tex2D(ShadowTextureSampler,IN.UV);
	
	return LightAmount*tex2D(DiffuseTextureSampler,IN.UV);

}

technique Cal3DTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CW;
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 RenderCal3DHWPS();
		//PixelShader = compile ps_3_0 NormalTexturedVertexPS();
	}
}
//---------------------------------------------------------

#endif




