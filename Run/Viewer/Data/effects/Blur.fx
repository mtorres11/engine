#ifndef INC_BLUR_H_
#define INC_BLUR_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"


// Efecto Blur
//--------------------------------------------------
float4 BlurPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR 
{
	float4 color = (1.0,1.0,1.0,0.95);
	return tex2D(DifuminadoTextureSampler, IN.UV)*color;
}

technique RenderGUITechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 BlurPS();
	}
}
//----------------------------------------

#endif