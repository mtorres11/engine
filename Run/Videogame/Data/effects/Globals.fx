float4x4 g_WorldMatrix : WORLD;
float4x4 g_ViewMatrix : VIEW;
float4x4 g_ProyectionMatrix : PROJECTION;
float4x4 g_WorldViewMatrix : WORLDVIEW;
float4x4 g_WorldViewProjectionMatrix : WORLDVIEWPROJECTION;
float4x4 g_ViewProjectionMatrix : VIEWPROJECTION;
float4x4 g_WorldToLightProjectionMatrix : VIEWTOLIGHTPROJECTION;
float g_Time : TIME;
float3 g_CameraPosition : CAMERAPOSITION;
float g_ShadowMapTextureSampler : SHADOWMAPTEXTURE;
float g_IncrementTexture=1.0/512.0; //1.0 dividido por el tamaño de la textura
float g_KT[4]={1.0,0.8,0.6,0.4};
float4 vDofParams=float4(1.0,3.0,2.0,0.75);

#define GLOWVTXCOUNTER 4 //Número de vértices que cogemmos para hacer el degradado
#define MAXBONES 40
float3x4 g_Bones[MAXBONES] : BONES;

#define SHADOW_EPSILON 0.00005f

#define RENDER_TARGET_WIDTH 1024
#define RENDER_TARGET_HEIGHT 1024
#define ZBLUR_WIDTH 256 
#define ZBLUR_HEIGHT 256
