#ifndef INC_GLOW_H_
#define INC_GLOW_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"

//Efecto Glow
//--------------------------------------------------
TNORMAL_2TEXTURED_VERTEX_PS GlowVS(TNORMAL_2TEXTURED_VERTEX_VS IN)
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
float4 GlowPS(TNORMAL_2TEXTURED_VERTEX_PS IN):COLOR
{
	float4 l_Color = tex2D(DiffuseTextureSampler, IN.UV);
	float4 l_Alpha = tex2D(GlowTextureSampler, IN.UV2);
	
//	if((l_Color.r == l_Color.g)&&(l_Color.r == l_Color.b))  // filtro
//	{
		if(l_Color.a <0.2)
		{
			l_Color.a = 1.0;
			return l_Color;
		}
		else
		{
			return float4(0.0,0.0,0.0,0.0);
		}
//	}
//	else
//	{
//		return float4(0.0,0.0,0.0,0.0);
//	}
}
technique RenderGlowTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 GlowVS();
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 GlowPS();
	}
}
technique  Cal3DGlowTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 GlowVS();
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 GlowPS();
	}
}
float4 RenderGlowPostFXPS(in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_Total=0;
	int l_Counter=0;
	for(int i=-(GLOWVTXCOUNTER-1);i<(GLOWVTXCOUNTER);++i)
	{
		float l_XTextureInc=i*g_IncrementTexture;
		for(int j=-(GLOWVTXCOUNTER-1);j<(GLOWVTXCOUNTER);++j)
		{
			float l_YTextureInc=j*g_IncrementTexture;
			float l_KT=(g_KT[abs(i)]+g_KT[abs(j)])*2.5;
			float4 l_Color=tex2D( GlowTextureSampler, UV + float2(l_XTextureInc, l_YTextureInc));
			if(l_Color.a>0.0)
			{
				l_Total+=l_Color*l_KT;//(l_Color/**l_Color.a*/);
				l_Color.a=1.0;
			}
			else
			{
				l_Color.a=0.0;
			}
			l_Counter += l_Color.a;
			//l_Counter=l_Counter+1*l_Color.a;
			
		}
	}
	return l_Total/max(1,l_Counter);
}
technique RenderGlowPostFXTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		CullMode = CCW;
		PixelShader = compile ps_3_0 RenderGlowPostFXPS();
	}
}
//--------------------------------------------------

#endif