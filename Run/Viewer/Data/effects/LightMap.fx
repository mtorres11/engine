#ifndef INC_LIGHTMAP_H_
#define INC_LIGHTMAP_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"
#include "lights.fx"


// TNORMAL_TEXTURED_VERTEX_PS NormalTexturedVertexVS(TNORMAL_TEXTURED_VERTEX_VS IN)
// {
	// TNORMAL_TEXTURED_VERTEX_PS OUT = (TNORMAL_TEXTURED_VERTEX_PS)0;
	// OUT.WorldNormal = mul(IN.Normal,(float3x3)g_WorldMatrix);
	// OUT.UV = IN.UV.xy;
	
	// float3 vPos = mul( float4(IN.Position.xyz,1.0), g_WorldViewMatrix ).xyz;
	// OUT.PosLight = mul( float4(vPos, 1.0), g_WorldToLightProjectionMatrix );
	
	// //OUT.HPosition=OUT.PosLight;
	// OUT.HPosition = mul(float4(IN.Position,1.0),g_WorldViewProjectionMatrix);
	// return OUT;
// }


//Efecto LightMap
//--------------------------------------------------
TNORMAL_2TEXTURED_VERTEX_PS LightmapVS(TNORMAL_2TEXTURED_VERTEX_VS IN)
{
	TNORMAL_2TEXTURED_VERTEX_PS OUT = (TNORMAL_2TEXTURED_VERTEX_PS)0;
	OUT.WorldNormal = mul(IN.Normal,(float3x3)g_WorldMatrix);
	OUT.UV = IN.UV.xy;
	OUT.UV2 = IN.UV2.xy;
	OUT.HPosition = mul(float4(IN.Position,1.0),g_WorldViewProjectionMatrix);
	
	float3 vPos = mul( float4(IN.Position.xyz,1.0), g_WorldViewMatrix ).xyz;
	OUT.PosLight = mul( float4(vPos, 1.0), g_WorldToLightProjectionMatrix );
	OUT.WorldPosition = mul(float4(IN.Position,1.0),g_WorldMatrix).xyz;

	return OUT;
}
float4 LightmapPS(TNORMAL_2TEXTURED_VERTEX_PS IN): COLOR
{
//	return float4(1,1,1,1);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV);
	float4 l_LightmapColor = tex2D(LightmapTextureSampler, IN.UV2)*1;
	
	float4 l_Color= l_DiffuseColor*l_LightmapColor*1;
	
	float3 Nn=normalize(IN.WorldNormal);
	float3 l_TotalDiffuseValue=0;
	float3 l_TotalSpecularValue=0;

	CalcLighting(g_LightAmbient, l_DiffuseColor, Nn, IN.WorldPosition,l_TotalDiffuseValue, l_TotalSpecularValue);
	return l_LightmapColor*tex2D(DiffuseTextureSampler,IN.UV)*float4(g_LightAmbientColor*g_LightAmbient+(l_TotalDiffuseValue+l_TotalSpecularValue), 1.0);
}
technique LightmapTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 LightmapVS();
		PixelShader = compile ps_3_0 LightmapPS();
	}
}
//--------------------------------------------------

#endif