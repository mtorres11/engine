#include "lights.fx"

//Efecto NormalMap
//--------------------------------------------------
TNORMALMAP_2TEXTURED_VERTEX_PS NormalMapLightMapVS(TNORMALMAP_2TEXTURED_VERTEX_VS IN)
{
	TNORMALMAP_2TEXTURED_VERTEX_PS l_OUT=(TNORMALMAP_2TEXTURED_VERTEX_PS)0;
	
	l_OUT.WorldTangent = mul(IN.Tangent, (float3x3)g_WorldMatrix);
	l_OUT.WorldBinormal = mul(cross(IN.Tangent,IN.Normal),(float3x3)g_WorldMatrix);
	l_OUT.WorldNormal = mul(IN.Normal, g_WorldMatrix);
	l_OUT.UV = IN.UV;
	l_OUT.UV2 = IN.UV2;
	l_OUT.HPosition = mul(IN.Position, g_WorldViewProjectionMatrix);
	
	float4 vPos = mul(IN.Position, g_WorldViewMatrix);
	l_OUT.PosLight = mul(vPos, g_WorldToLightProjectionMatrix);
	l_OUT.WorldPosition = mul(IN.Position, g_WorldMatrix);
	
	return l_OUT;
}
float4 NormalMapLightMapPS(TNORMALMAP_2TEXTURED_VERTEX_PS IN):COLOR 
{
	float4 l_Ambient = float4(g_LightAmbient.xyz * g_LightAmbient.w, 1.0);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV);
	float4 l_LightmapColor = g_LightMapFactor * tex2D(LightmapTextureSampler2, IN.UV2);
	
	float4 colour = l_DiffuseColor * (l_Ambient + l_LightmapColor);
	//return float4(1,1,1,1);
	//return float4(IN.UV2.y, 0.0, 0.0,1.0);
	//return tex2D(LightmapTextureSampler2, IN.UV2);
	 // return 3*l_LightmapColor;
		
	
	//--- bumpmap -------
	float3x3 worldToTangentSpace;
    worldToTangentSpace[0] = IN.WorldTangent;
    worldToTangentSpace[1] = IN.WorldBinormal;
    worldToTangentSpace[2] = IN.WorldNormal;
	
	float3 normalmap = (2 * tex2D(NormalTextureSampler2, IN.UV)) - 1.0;
	
	float3 l_TotalDiffuseValue = 0;
	float3 l_TotalSpecularValue = 0;
	CalcLightingForBumMap(normalmap, IN.WorldPosition, worldToTangentSpace, l_TotalDiffuseValue, l_TotalSpecularValue);
	//-------
	
	//return 1*float4(l_TotalDiffuseValue,1);
	return colour * (l_Ambient + float4(l_TotalDiffuseValue + l_TotalSpecularValue, 1));
	
}
technique NormalMapLightMapTechnique
{
    pass p0
    {
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
	    AlphaTestEnable = false;
		CullMode = CCW;
        VertexShader = compile vs_3_0 NormalMapLightMapVS();
        PixelShader = compile ps_3_0 NormalMapLightMapPS();
    }
}
//--------------------------------------------------
