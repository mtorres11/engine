#ifndef INC_LIGHTS_H_
#define INC_LIGHTS_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"

float CalcAttenuation(int IdLight, float3 WorldPosition)
{
	int l_StartRangeAttenuation=g_LightsStartRange[IdLight];
	int l_EndRangeAttenuation=g_LightsEndRange[IdLight];
	// int l_StartRangeAttenuation=5;
	// int l_EndRangeAttenuation=10;
	
	if(g_LightsType[IdLight]==1) //si es DIRECTIONAL
		return 1.0;
	else
	{
		float l_DistanceToLight=length(g_LightsPosition[IdLight]-WorldPosition);
		if(l_DistanceToLight<l_StartRangeAttenuation)
			return 1.0;
		else
			// return saturate((l_EndRangeAttenuation - l_DistanceToLight2)/(l_EndRangeAttenuation-l_StartRangeAttenuation));
			return saturate((l_EndRangeAttenuation-l_DistanceToLight)/(l_EndRangeAttenuation-l_StartRangeAttenuation));
	}
}
void CalcLightingByLight(int IdLight, float LightAmbient, float4 DiffuseColor, float3 Nn, float3 WorldPosition, out float3 DiffuseValue, out float3 SpecularValue)
 {
	float3 l_LightDirection=g_LightsDirection[IdLight];
	float3 l_LightColor=g_LightsColor[IdLight];

	float l_Att=CalcAttenuation(IdLight, WorldPosition);

	l_LightDirection=normalize(l_LightDirection);
	float l_DiffuseIntensity=saturate(dot(Nn,-l_LightDirection))*l_Att;	

	float3 Vn=normalize(g_CameraPosition-WorldPosition);
	float3 Hn=normalize(Vn+Nn);

	float l_SpecularIntensity=pow(saturate(dot(Hn,Nn)),g_SpecularPower)*l_Att*g_Ks;

	DiffuseValue=saturate(l_DiffuseIntensity*DiffuseColor.xyz*l_LightColor.xyz*g_Ks);
	SpecularValue=l_SpecularIntensity*l_LightColor.xyz;
	
	if(g_LightsType[IdLight]==DIRECTIONAL)
	{
		l_DiffuseIntensity = saturate(dot(Nn,-l_LightDirection));
		DiffuseValue = l_DiffuseIntensity*l_LightColor;
		
		float NL = saturate(dot(Nn, -l_LightDirection));//producto punto de la normal por luz
		
		SpecularValue = l_LightColor * g_Ks * pow(saturate(dot(Hn,Nn)),g_SpecularPower) * NL;
	}
	else if(g_LightsType[IdLight]==OMNI)
	{
		DiffuseValue = l_Att*l_LightColor;
		l_SpecularIntensity=pow(saturate(dot(Hn,Nn)),g_SpecularPower)*l_Att*g_Ks;
	    SpecularValue=l_SpecularIntensity*l_LightColor.xyz;
	}
	else if(g_LightsType[IdLight]==SPOT) //TODO Optimizar
	{   
		float3 L = normalize(-l_LightDirection);
		float3 Q =  normalize(g_LightsPosition[IdLight]-WorldPosition);
		float angle = acos(dot(L, Q));
		if (angle < g_LightsFallOff[IdLight]/90) 
		{
			if (angle < g_LightsAngle[IdLight]/90) {
				//float L2 = (g_LightsPosition[IdLight]-WorldPosition)/length(g_LightsPosition[IdLight]-WorldPosition);
				DiffuseValue = l_Att*l_LightColor;
			}
			else {
				DiffuseValue = (((g_LightsFallOff[IdLight]/90)-angle) / ((g_LightsFallOff[IdLight]/90)-g_LightsAngle[IdLight]/90))*l_Att*l_LightColor;
			}
		}
		else{
			DiffuseValue = float3 (0.0, 0.0, 0.0);
			l_SpecularIntensity=pow(saturate(dot(Hn,Nn)),g_SpecularPower)*l_Att*g_Ks;
			 SpecularValue=l_SpecularIntensity*l_LightColor.xyz*DiffuseValue;
		}
		
	}
 }
 void CalcLighting(float LightAmbient, float4 DiffuseColor, float3 Nn, float3 WorldPosition,out float3 TotalDiffuseValue, out float3 TotalSpecularValue)
 {
	 TotalDiffuseValue=0;
	 TotalSpecularValue=0;
	
	 for(int i=0;i<MAX_LIGHTS;i++)
	 {
		 float3 l_DiffuseValue;
		 float3 l_SpecularValue;
		
		if(g_LightsEnabled[i])
		{
			CalcLightingByLight(i, LightAmbient, DiffuseColor, Nn, WorldPosition, l_DiffuseValue, l_SpecularValue);
			TotalDiffuseValue += l_DiffuseValue;
			TotalSpecularValue += l_SpecularValue;
		}
	 }
 }
 
 #endif