#ifndef INC_NORMALTEXTUREDVERTEX_H_
#define INC_NORMALTEXTUREDVERTEX_H_

#include "lights.fx"


//NormalTexturedVertex Shader
TNORMAL_TEXTURED_VERTEX_PS NormalTexturedVertexVS(TNORMAL_TEXTURED_VERTEX_VS IN)
{
	TNORMAL_TEXTURED_VERTEX_PS OUT = (TNORMAL_TEXTURED_VERTEX_PS)0;
	
	OUT.HPosition = mul(IN.Position, g_WorldViewProjectionMatrix);
	OUT.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.UV = IN.UV;
	
	OUT.HPosition = mul(IN.Position, g_WorldViewProjectionMatrix);
	float4 vPos = mul(IN.Position, g_WorldViewMatrix );
	OUT.PosLight = mul(vPos, g_WorldToLightProjectionMatrix);
	OUT.WorldPosition = mul(IN.Position, g_WorldMatrix);
	
	
	// --- FOG ? -------------
	// Calculate the camera position.
	float4 cameraPosition;
    cameraPosition = mul(IN.Position, g_WorldMatrix);
    cameraPosition = mul(cameraPosition, g_WorldViewMatrix);
	
	 // Calculate linear fog.    
    OUT.fogFactor = saturate((g_FogEnd - cameraPosition.z) / (g_FogEnd - g_FogStart));
	//OUT.fogFactor = 1.0f - ( length( IN.Position - cameraPosition.z) / 50.0f);
	
	//Calculate exponential fog
	//OUT.fogFactor = 1.0 / 2.71828 power (cameraPosition.z * FogDensity);
	
	return OUT;
}

//NormalTexturedPixel Shader
float4 NormalTexturedVertexPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
	float2 ShadowTexC = 0.5 * IN.PosLight.xy / IN.PosLight.w + float2( 0.5, 0.5 );
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	float LightAmount = (tex2D( ShadowTextureSampler, ShadowTexC ) + SHADOW_EPSILON) < (IN.PosLight.z / IN.PosLight.w) ? 0.0f: 1.0f;

	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV);
	float4 l_Ambient = float4(g_LightAmbient.xyz * g_LightAmbient.w, 1.0);
	
	float3 Nn = normalize(IN.WorldNormal);
	float3 l_TotalDiffuseValue =0;
	float3 l_TotalSpecularValue=0;

	CalcLighting(Nn, IN.WorldPosition,l_TotalDiffuseValue, l_TotalSpecularValue);

	float4 result = LightAmount * l_DiffuseColor * (l_Ambient + float4(l_TotalDiffuseValue + l_TotalSpecularValue, 1));
	//return float4(result.x, result.y, result.z, 0.3);
	//return float4(result.x, result.y, result.z, l_DiffuseColor.w);
	return float4(result.xyz, l_DiffuseColor.w);
//	return LightAmount * l_DiffuseColor * (l_Ambient + float4(l_TotalDiffuseValue + l_TotalSpecularValue, 1));
	
	//return tex2D(DiffuseTextureSampler,IN.UV);
	// Calculate the final color using the fog effect equation.
	//float4 finalColor;  
   // finalColor = IN.fogFactor * tex2D(DiffuseTextureSampler,IN.UV) * float4(g_LightAmbientg_LightAmbientColor*g_LightAmbient+(l_TotalDiffuseValue+l_TotalSpecularValue), 1.0) * g_fogColor + (1.0 - IN.fogFactor) ;

    //return float4(IN.UV.x,IN.UV.y,0.0,1.0);
	//return finalColor;

}

//NormalTexturedPixel with Self-Illumination Shader
float4 NormalTexturedVertexPS_SI(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{

	float2 ShadowTexC = 0.5 * IN.PosLight.xy / IN.PosLight.w + float2( 0.5, 0.5 );
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	float LightAmount = (tex2D( ShadowTextureSampler, ShadowTexC ) + SHADOW_EPSILON) < (IN.PosLight.z / IN.PosLight.w) ? 0.0f: 1.0f;

	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV);
	
	float3 Nn = normalize(IN.WorldNormal);
	float3 l_TotalDiffuseValue =0;
	float3 l_TotalSpecularValue=0;

	CalcLighting(Nn, IN.WorldPosition,l_TotalDiffuseValue, l_TotalSpecularValue);
	
	float4 result = LightAmount * l_DiffuseColor * float4(l_TotalDiffuseValue + l_TotalSpecularValue + selfIlluminationColor, 1);
	
	return float4(result.xyz, l_DiffuseColor.w);
}

//NormalTexturedPixel Shader DEBUG RED
float4 NormalTexturedVertexPS_DEBUG_RED(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
	return RED;
}

//NormalTexturedPixel Shader DEBUG ALPHA
float4 NormalTexturedVertexPS_DEBUG_ALPHA(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV);
	//return l_DiffuseColor;
	//return float4(l_DiffuseColor.w, 0.0, 0.0, 1.0);
	return float4(l_DiffuseColor.xyz, l_DiffuseColor.w);
	//return float4(1.0, 0.5, 0.5, 0.5);
	//return BLUE;
}

float4 ShowNormalPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
	float3 Nn=normalize(IN.WorldNormal);
	return float4(Nn, 1.0);
}

technique NormalTextureVertexTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
	    AlphaTestEnable = false;
		FogEnable=false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS();
	}
}
technique NormalTextureVertexTechnique_SI
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
	    AlphaTestEnable = false;
		FogEnable=false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS_SI();
	}
}
technique NormalTextureVertexTechnique_AT
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
		AlphaRef = 0x0060;
		//AlphaRef = 0.9; cambios para el glow		
		FogEnable=false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS();
	}
}
technique NormalTextureVertexTechnique_SI_AT
{
	pass p0
	{
		AlphaBlendEnable = false;
	    AlphaTestEnable = true;
		//AlphaTestEnable = true; cambios para el glow
		AlphaFunc = GREATER;
		AlphaRef = 0x0060;
		//AlphaRef = 0.9; cambios para el glow	
		CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS_SI();
	}
}

technique NormalTextureVertexTechnique_AB
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
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS();
		// PixelShader = compile ps_3_0 NormalTexturedVertexPS_DEBUG_ALPHA();
	}
}
technique NormalTextureVertexTechnique_SI_AB
{
	pass p0
	{
		AlphaBlendEnable = true;
	    DestBlend = INVSRCALPHA;
        SrcBlend = SRCALPHA;
        CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS_SI();
	}
}

technique NormalTextureVertexTechnique_DEBUG_RED
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
	    AlphaTestEnable = false;
		AlphaFunc = Greater;
		AlphaRef = 0.2;
		FogEnable=false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS_DEBUG_RED();
	}
}

technique NormalTextureVertexTechnique_DEBUG_ALPHA
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = true;
	    //AlphaTestEnable = false;
		//AlphaTestEnable = true; cambios para el glow
		//AlphaFunc = Greater;
		//AlphaRef = 0.2;
		//AlphaRef = 0.9; cambios para el glow		
		//FogEnable=false;
		//CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS_DEBUG_ALPHA();
	}
}
//--------------------------------------------------

#endif
