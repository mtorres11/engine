#ifndef INC_LIGHTMAP_H_
#define INC_LIGHTMAP_H_

#include "lights.fx"

//Efecto LightMap
//--------------------------------------------------
TNORMAL_2TEXTURED_VERTEX_PS LightmapVS(TNORMAL_2TEXTURED_VERTEX_VS IN)
{
	TNORMAL_2TEXTURED_VERTEX_PS OUT = (TNORMAL_2TEXTURED_VERTEX_PS)0;
	OUT.HPosition = mul(IN.Position, g_WorldViewProjectionMatrix);
	OUT.WorldNormal = mul(IN.Normal, g_WorldMatrix);
	OUT.UV = IN.UV;
	OUT.UV2 = IN.UV2;
	
	float4 vPos = mul(IN.Position, g_WorldViewMatrix );
	OUT.PosLight = mul(vPos, g_WorldToLightProjectionMatrix );
	OUT.WorldPosition = mul(IN.Position, g_WorldMatrix);

	return OUT;
}
float4 LightmapPS(TNORMAL_2TEXTURED_VERTEX_PS IN): COLOR
{
	float4 l_Ambient = float4(g_LightAmbient.xyz * g_LightAmbient.w, 1.0);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV);
	float4 l_LightmapColor = g_LightMapFactor * tex2D(LightmapTextureSampler, IN.UV2);
	//return l_LightmapColor * (l_Ambient * l_DiffuseColor);
	float4 colour = l_DiffuseColor * (l_Ambient + l_LightmapColor);
	//return colour;
	//float4 l_Color = l_DiffuseColor * l_LightmapColor;
	//return l_Color;
	
	float3 Nn = normalize(IN.WorldNormal);
	float3 l_TotalDiffuseValue=0;
	float3 l_TotalSpecularValue=0;

	CalcLighting(Nn, IN.WorldPosition,l_TotalDiffuseValue, l_TotalSpecularValue);
	
	return colour * (l_Ambient + float4(l_TotalDiffuseValue + l_TotalSpecularValue, 1));
	return l_LightmapColor*l_DiffuseColor * float4(l_TotalDiffuseValue+l_TotalSpecularValue, 1.0);
}

// Lightmap con self-illumination
float4 LightmapPS_SI(TNORMAL_2TEXTURED_VERTEX_PS IN): COLOR
{
	float4 l_Ambient = float4(g_LightAmbient.xyz * g_LightAmbient.w, 1.0);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV);
	float4 l_LightmapColor = g_LightMapFactor * tex2D(LightmapTextureSampler, IN.UV2);
	float4 colour = l_DiffuseColor * (l_Ambient + l_LightmapColor);
	
	float3 Nn = normalize(IN.WorldNormal);
	float3 l_TotalDiffuseValue=0;
	float3 l_TotalSpecularValue=0;

	CalcLighting(Nn, IN.WorldPosition,l_TotalDiffuseValue, l_TotalSpecularValue);
	
	return colour * (l_Ambient + float4(l_TotalDiffuseValue + l_TotalSpecularValue + selfIlluminationColor, 1));
}

// Pixel Shader DEBUG RED
float4 LightmapPS_DEBUG_RED(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
	return RED;
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
technique LightmapTechnique_SI
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 LightmapVS();
		PixelShader = compile ps_3_0 LightmapPS_SI();
	}
}

technique LightmapTechnique_AT
{
	pass p0
	{
		//ZEnable = true;
		//ZWriteEnable = true;
		//ZFunc = LessEqual;
		//ZFunc = LessEqual;
		AlphaBlendEnable = false;
	    AlphaTestEnable = true;
		//AlphaTestEnable = true; cambios para el glow
		AlphaFunc = GREATER;
		//AlphaRef = 0.2;
//		AlphaRef = 0x0048;
		AlphaRef = 0x00DD;
		//AlphaRef = 0.9; cambios para el glow		
		FogEnable=false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 LightmapVS();
		PixelShader = compile ps_3_0 LightmapPS();
	}
}
technique LightmapTechnique_SI_AT
{
	pass p0
	{
		AlphaBlendEnable = false;
	    AlphaTestEnable = true;
		AlphaFunc = GREATER;
		AlphaRef = 0x00DD;
		FogEnable=false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 LightmapVS();
		PixelShader = compile ps_3_0 LightmapPS_SI();
	}
}

technique LightmapTechnique_AB
{
	pass p0
	{
		//ZEnable = true;
		//ZWriteEnable = true;
		//ZFunc = LessEqual;
		//ZFunc = LessEqual;
		AlphaBlendEnable = true;
	    // DestBlend = INVSRCALPHA;
        // SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
        SrcBlend = SRCALPHA;
        //AlphaTestEnable = true;
		//AlphaTestEnable = true; cambios para el glow
		//AlphaFunc = GREATER;
		//AlphaRef = 0.2;
		//AlphaRef = 0x0048;
		//AlphaRef = 0.9; cambios para el glow		
		//FogEnable=false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 LightmapVS();
		PixelShader = compile ps_3_0 LightmapPS();
		// PixelShader = compile ps_3_0 NormalTexturedVertexPS_DEBUG_ALPHA();
	}
}
technique LightmapTechnique_SI_AB
{
	pass p0
	{
		AlphaBlendEnable = true;
	    DestBlend = INVSRCALPHA;
        SrcBlend = SRCALPHA;
        CullMode = CCW;
		VertexShader = compile vs_3_0 LightmapVS();
		PixelShader = compile ps_3_0 LightmapPS_SI();
	}
}
//--------------------------------------------------

#endif