#ifndef INC_ONDULATORIO_H_
#define INC_ONDULATORIO_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"



//Efecto Ondulatorio
//--------------------------------------------------
float4 OndulatorioPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR 
{
	IN.UV.y = IN.UV.y + (sin(IN.UV.x*30.0)*0.015*sin(g_Time));
	float4 color = tex2D(DifuminadoTextureSampler, IN.UV);//todos

	return color;
}
technique RenderOndulatorioTechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 OndulatorioPS();
	}
}
//----------------------------------------


#endif