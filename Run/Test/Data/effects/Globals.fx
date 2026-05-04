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

// colores (para debug)
#define RED float4(1.0, 0.0, 0.0, 1.0)
#define GREEN float4(0.0, 1.0, 0.0, 1.0)
#define BLUE float4(0.0, 0.0, 1.0, 1.0)

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
	
float4 g_LightAmbient : AMBIENT;

float3 selfIlluminationColor : SELFILLUMINATION;

float g_SpecularPower = 25.0; //50.0
float g_Ks = 0;//2.0;

//ParallaxMap-------------------------------------
float g_Bump = 0.02;

//LightMap----------------------------------------
float g_LightMapFactor = 1;

//Glow--------------------------------------------
float g_IncrementTexture=1.0/512.0; //1.0 dividido por el tamaño de la textura
float g_KT[4]={1.0,0.8,0.6,0.4};
float4 vDofParams=float4(1.0,3.0,2.0,0.75);

//Instancing---------------------------------------
float4 g_InstancePosition : INSTANCESPOSITION;

//Fog----------------------------------------------
float g_FogStart=0.0;
float g_FogEnd=100.0;
float4 g_fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);

#endif







