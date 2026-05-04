#ifndef INC_NOISEANDVIGNETTING_H_
#define INC_NOISEANDVIGNETTING_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"

//Efecto RenderNoiseAndVignetting
//--------------------------------------------------
float4 RenderNoiseAndVignettingPS(in float2 UV : TEXCOORD0) : COLOR
{
	float2 l_Offset=float2(g_Time*0.39*0.001,g_Time*0.79*0.001);
	float2 l_UV=UV+l_Offset;
	float4 l_VignettingColor=tex2D(NormalTextureSampler, l_UV);
	float4 l_NoiseColor=tex2D(DiffuseTextureSampler, l_UV);
	//return float4(l_VignettingColor.xyz,l_VignettingColor.a*10);
	//float l_C=l_NoiseColor.a;
	//return float4(l_C.x,l_C.x,l_C.x,0.8);
	return float4(l_NoiseColor.xyz*l_VignettingColor.xyz, l_NoiseColor.a*2+l_VignettingColor.a*13);
}
technique RenderNoiseAndVignettingTechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 RenderNoiseAndVignettingPS();
	}
}
//--------------------------------------------------

#endif