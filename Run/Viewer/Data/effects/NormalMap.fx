#ifndef INC_NORMALMAP_H_
#define INC_NORMALMAP_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"
#include "lights.fx"

//Efecto NormalMap
//--------------------------------------------------
TNORMALMAP_TEXTURED_VERTEX_PS NormalmapVS(TNORMALMAP_TEXTURED_VERTEX_VS IN)
{
	TNORMALMAP_TEXTURED_VERTEX_PS l_OUT=(TNORMALMAP_TEXTURED_VERTEX_PS)0;
	//inicializamos los valores de l_OUT que vamos a retornar 
	l_OUT.WorldTangent=mul(IN.Tangent.xyz,(float3x3)g_WorldMatrix);
	l_OUT.WorldBinormal = mul(normalize(cross(IN.Tangent.xyz,IN.Normal)),(float3x3)g_WorldMatrix);
	l_OUT.HPosition = mul(float4(IN.Position,1.0),g_WorldViewProjectionMatrix);
    //l_OUT.HPosition = mul(IN.HPosition, g_WorldViewProjectionMatrix); 
	l_OUT.WorldNormal=mul(IN.Normal, g_WorldMatrix);
	l_OUT.WorldPosition = mul(float4(IN.Position,1.0),g_WorldMatrix).xyz;
	//l_OUT.WorldPosition=mul(IN.HPosition.xyz, g_WorldMatrix);
	l_OUT.UV=IN.UV;
	
	float3 vPos = mul( float4(IN.Position.xyz,1.0), g_WorldViewMatrix ).xyz;
	l_OUT.PosLight = mul( float4(vPos, 1.0), g_WorldToLightProjectionMatrix );
	
	return l_OUT;
}
float4 NormalmapPS(TNORMALMAP_TEXTURED_VERTEX_PS IN):COLOR 
{
    float3 Tn=normalize(IN.WorldTangent);
	float3 Bn=normalize(IN.WorldBinormal);

	float3 bump=g_Bump*(tex2D(NormalTextureSampler,IN.UV).rgb - float3(0.5,0.5,0.5)); //bump es el que nos da la profundidad
	//float4 color = tex2D(NormalTextureSampler, IN.UV); //Calculamos el color de la textura en cada punto
    float4 l_DiffuseColor=tex2D(DiffuseTextureSampler, IN.UV); //Calculamos el difuso
	float3 l_Ambient=g_LightAmbientColor*g_LightAmbient;
    float3 Nn=normalize(IN.WorldNormal);
	Nn = normalize(Nn + bump.x*Tn + bump.y*Bn);

	float3 l_TotalDiffuseValue=0;
	float3 l_TotalSpecularValue=0;
	CalcLighting(g_LightAmbient, l_DiffuseColor, Nn, IN.WorldPosition,l_TotalDiffuseValue, l_TotalSpecularValue);
	//return tex2D(DiffuseTextureSampler,IN.UV)*float4(l_Ambient+(l_TotalDiffuseValue+l_TotalSpecularValue), 1.0);
	return l_DiffuseColor+float4(bump,1.0)*float4(l_Ambient+(l_TotalDiffuseValue+l_TotalSpecularValue), 1.0);
	
	// float4 lightPos = (1.0f,1.0f,1.0f,1.0f);
	// float att = 1/( 1 + ( 0.005 * distance(lightPos.xyz, IN.WorldPosition) ) );//calculamos la atenuación de la luz
	// float4 color = tex2D(DiffuseTextureSampler, IN.UV); //Calculamos el color de la textura en cada punto
    // //La normal se extrae del nuestra textura NormalTexture de la siguiente manera:
    // float3 normal = 2.0f * tex2D(NormalTextureSampler, IN.UV).rgb - 1.0f;
    // float3 light = normalize( g_LightAmbient*g_LightAmbientColor); //Normalizamos el valor de la luz
    // float diffuse = saturate(dot(normal, light)); //Calculamos el color difuso
    // //return color * diffuse; //Obtenemos el color final
    // return att * color * diffuse; //Obtenemos el color final
}
technique NormalMapTechnique
{
    pass p0
    {
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
        CullMode = CCW;
        VertexShader = compile vs_3_0 NormalmapVS();
        PixelShader = compile ps_3_0 NormalmapPS();
    }
}
//--------------------------------------------------

#endif
