#ifndef INC_GLOBALS_H_
#define INC_GLOBALS_H_

#define SHADOW_EPSILON 0.00005f
#define MAX_LIGHTS_BY_SHADER 4
#define RENDER_TARGET_WIDTH 1024
#define RENDER_TARGET_HEIGHT 1024
#define ZBLUR_WIDTH 256 
#define ZBLUR_HEIGHT 256
#define GLOWVTXCOUNTER 4 //Número de vértices que cogemos para hacer el degradado
#define MAXBONES 40
#define MAX_LIGHTS 4

// Tipos de luces
#define OMNI 0
#define DIRECTIONAL 1
#define SPOT 2
 
float4x4 g_WorldMatrix : WORLD;
float4x4 g_ViewMatrix : VIEW;
float4x4 g_ProyectionMatrix : PROJECTION;
float4x4 g_WorldViewMatrix : WORLDVIEW;
float4x4 g_WorldViewProjectionMatrix : WORLDVIEWPROJECTION;
float4x4 g_ViewProjectionMatrix : VIEWPROJECTION;
float4x4 g_WorldToLightProjectionMatrix : VIEWTOLIGHTPROJECTION;
float g_Time : TIME;
float3x4 g_Bones[MAXBONES] : BONES;
float3 g_CameraPosition : CAMERAPOSITION;
//Luces --------------------------------------
float3 g_LightsDirection[MAX_LIGHTS_BY_SHADER] : LIGHTSDIRECTION;
float3 g_LightsColor[MAX_LIGHTS_BY_SHADER] : LIGHTSCOLOR;
float3 g_LightsPosition[MAX_LIGHTS_BY_SHADER] : LIGHTSPOSITION;
float g_LightsType[MAX_LIGHTS_BY_SHADER] : LIGHTSTYPE;
float g_LightsStartRange[MAX_LIGHTS_BY_SHADER] : LIGHTSSTARTATT;
float g_LightsEndRange[MAX_LIGHTS_BY_SHADER]: LIGHTSENDATT;
bool g_LightsEnabled [MAX_LIGHTS]: LIGHTSENABLED;

float g_LightsFallOff[MAX_LIGHTS]: LIGHTSFALLOFF;
float g_LightsAngle[MAX_LIGHTS]: LIGHTSANGLE;
	
float3 g_LightAmbientColor={0.5,0.5,0.5};
float g_LightAmbient=0.07;
float g_SpecularPower=50.0;
float g_Ks=2;

//NormalMap-------------------------------------
float g_Bump = 2.4;

//Glow-----------------------------------------
float g_IncrementTexture=1.0/512.0; //1.0 dividido por el tamaño de la textura
float g_KT[4]={1.0,0.8,0.6,0.4};
float4 vDofParams=float4(1.0,3.0,2.0,0.75);

//Fog---------------------------------------------
float g_FogStart=0.0;
float g_FogEnd=10.0;
float4 g_fogColor = float4(0.3f, 0.3f, 0.3f, 1.0f);

#endif







