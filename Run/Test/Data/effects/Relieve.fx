#ifndef INC_RELIEVE_H_
#define INC_RELIEVE_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"

// Efecto Relieve
//--------------------------------------------------
 float4 RelievePS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
  // Sharpening the image
  float sharpAmount = 15.0f;
  float4 color = tex2D( DifuminadoTextureSampler, IN.UV);
  color += tex2D( DifuminadoTextureSampler, IN.UV - 0.0001) * sharpAmount;
  color -= tex2D( DifuminadoTextureSampler, IN.UV + 0.0001) * sharpAmount;
  return color;
}
technique RenderRelieveTechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 RelievePS();
	}
}
//----------------------------------------

#endif
