#include "lights.fx"

//Efecto NormalMap
//--------------------------------------------------
TNORMALMAP_TEXTURED_VERTEX_PS NormalmapVS(TNORMALMAP_TEXTURED_VERTEX_VS IN)
{
	TNORMALMAP_TEXTURED_VERTEX_PS l_OUT=(TNORMALMAP_TEXTURED_VERTEX_PS)0;
	//inicializamos los valores de l_OUT que vamos a retornar 
	l_OUT.WorldTangent = mul(IN.Tangent, g_WorldMatrix);
	l_OUT.WorldBinormal = mul(cross(IN.Tangent, IN.Normal), g_WorldMatrix);
	l_OUT.WorldNormal = mul(IN.Normal, g_WorldMatrix);
	l_OUT.UV = IN.UV;
	
    float4 vPos = mul(IN.Position, g_WorldViewMatrix);
	l_OUT.PosLight = mul(vPos, g_WorldToLightProjectionMatrix);
	l_OUT.WorldPosition = mul(IN.Position, g_WorldMatrix).xyz;
	l_OUT.HPosition = mul(IN.Position, g_WorldViewProjectionMatrix);
	
	return l_OUT;
}
float4 NormalmapPS(TNORMALMAP_TEXTURED_VERTEX_PS IN):COLOR 
{
	float3x3 worldToTangentSpace;
    worldToTangentSpace[0] = IN.WorldTangent;
    worldToTangentSpace[1] = IN.WorldBinormal;
    worldToTangentSpace[2] = IN.WorldNormal;
	
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV); //Calculamos el color de la textura en cada punto
    float4 l_Ambient = float4(g_LightAmbient.xyz * g_LightAmbient.w, 1.0);
    
	//La normal se extrae del nuestra textura NormalTexture de la siguiente manera:
	float3 normalmap = (2 * tex2D(NormalTextureSampler, IN.UV)) - 1.0;
	
	float3 l_TotalDiffuseValue = 0;
	float3 l_TotalSpecularValue = 0;
	CalcLightingForBumMap(normalmap, IN.WorldPosition, worldToTangentSpace, l_TotalDiffuseValue, l_TotalSpecularValue);
	
	return l_DiffuseColor * (l_Ambient + float4(l_TotalDiffuseValue + l_TotalSpecularValue, 1));
}
technique NormalMapTechnique
{
    pass p0
    {
		AlphaBlendEnable = false;
        //SrcBlend=SrcAlpha;
        //DestBlend=InvSrcAlpha;
        //CullMode = CCW;
        VertexShader = compile vs_3_0 NormalmapVS();
        PixelShader = compile ps_3_0 NormalmapPS();
    }
}
//--------------------------------------------------
