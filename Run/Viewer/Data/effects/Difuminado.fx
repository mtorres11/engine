#ifndef INC_DIFUMINADO_H_
#define INC_DIFUMINADO_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"



//Efecto Difuminado
//--------------------------------------------------
float4 DifuminadoPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR 
{
	float4 color = tex2D(DifuminadoTextureSampler, IN.UV);//todos
	color += tex2D(DifuminadoTextureSampler, IN.UV-0.01);//difuminado
	color += tex2D(DifuminadoTextureSampler, IN.UV+0.01);//difuminado
	color = color * 0.333;
	return color;
}
technique RenderDifuminadoTechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 DifuminadoPS();
	}
}
//----------------------------------------


#endif