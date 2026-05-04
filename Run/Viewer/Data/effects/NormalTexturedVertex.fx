#ifndef INC_NORMALTEXTUREDVERTEX_H_
#define INC_NORMALTEXTUREDVERTEX_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"
#include "lights.fx"

//NormalTexturedVertex Shader
TNORMAL_TEXTURED_VERTEX_PS NormalTexturedVertexVS(TNORMAL_TEXTURED_VERTEX_VS IN)
{
	TNORMAL_TEXTURED_VERTEX_PS OUT = (TNORMAL_TEXTURED_VERTEX_PS)0;
	OUT.WorldNormal = normalize(mul(IN.Normal,(float3x3)g_WorldMatrix));
	OUT.UV = IN.UV.xy;
	
	float3 vPos = mul( float4(IN.Position.xyz,1.0), g_WorldViewMatrix ).xyz;
	OUT.PosLight = mul( float4(vPos, 1.0), g_WorldToLightProjectionMatrix );
	OUT.WorldPosition = mul(float4(IN.Position,1.0),g_WorldMatrix).xyz;
	OUT.HPosition = mul(float4(IN.Position,1.0),g_WorldViewProjectionMatrix);
		
	float4 Po = float4(IN.Position.xyz,1.0); // object coordinates float3 
	float3 Pw = mul(Po,g_WorldMatrix).xyz;
	OUT.SpotLightVec = OUT.PosLight - Pw.xyz;

	 // Calculate the camera position.
	float4 cameraPosition;
    cameraPosition = mul(IN.Position, g_WorldMatrix);
    cameraPosition = mul(cameraPosition, g_WorldViewMatrix);
	
	 // Calculate linear fog.    
    //OUT.fogFactor = saturate((g_FogEnd - cameraPosition.z) / (g_FogEnd - g_FogStart));
	OUT.fogFactor = 1.0f - ( length( IN.Position - cameraPosition.z) / 50.0f);
	
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
	float3 Nn=normalize(IN.WorldNormal);
	float3 l_TotalDiffuseValue=0;
	float3 l_TotalSpecularValue=0;

	CalcLighting(g_LightAmbient, l_DiffuseColor, Nn, IN.WorldPosition,l_TotalDiffuseValue, l_TotalSpecularValue);
	return LightAmount*tex2D(DiffuseTextureSampler,IN.UV)*float4(g_LightAmbientColor*g_LightAmbient+(l_TotalDiffuseValue+l_TotalSpecularValue), 1.0);
	// Calculate the final color using the fog effect equation.
	// float4 finalColor;  
    // finalColor = IN.fogFactor * tex2D(DiffuseTextureSampler,IN.UV) * float4(g_LightAmbientColor*g_LightAmbient+(l_TotalDiffuseValue+l_TotalSpecularValue), 1.0) * g_fogColor + (1.0 - IN.fogFactor) ;

    // return finalColor;

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
		//AlphaTestEnable = true; cambios para el glow
		AlphaFunc = Greater;
		AlphaRef = 0.2;
		//AlphaRef = 0.9; cambios para el glow		
		FogEnable=false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 NormalTexturedVertexVS();
		PixelShader = compile ps_3_0 NormalTexturedVertexPS();
	}
}
//--------------------------------------------------

#endif
