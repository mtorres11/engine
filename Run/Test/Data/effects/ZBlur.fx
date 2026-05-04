#ifndef INC_ZBLUR_H_
#define INC_ZBLUR_H_

#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"
/*
//distribucion de poisson
float2 TwelveKernelBase[12] =
{
    { 1.0f,  0.0f},
    { 0.5f,  0.8660f},
    {-0.5f,  0.8660f},
    {-1.0f,  0.0f},
    {-0.5f, -0.8660f},
    { 0.5f, -0.8660f},
   
    { 1.5f,  0.8660f},
    { 0.0f,  1.7320f},
    {-1.5f,  0.8660f},
    {-1.5f, -0.8660f},
    { 0.0f, -1.7320f},
    { 1.5f, -0.8660f},
};*/

//distribucion de poisson
float2 TwelveKernelBase[12] =
{
    { 10.0f,  0.0f},
    { 5.0f,  8.660f},
    {-5.0f,  8.660f},
    {-10.0f,  0.0f},
    {-5.0f, -8.660f},
    { 5.0f, -8.660f},
   
    { 15.0f,  8.660f},
    { 0.0f,  17.320f},
    {-15.0f,  8.660f},
    {-15.0f, -8.660f},
    { 0.0f, -17.320f},
    { 15.0f, -8.660f},
};

// ZBlur
// vDofParams coefficients:
// x = near blur depth; y = focal plane depth; z = far blur depth
// w = blurriness cutoff constant for objects behind the focal plane

ZBLUR_TEXTURED_VERTEX_PS NormalTexturedVertexVS(TNORMAL_TEXTURED_VERTEX_VS IN)
{
 ZBLUR_TEXTURED_VERTEX_PS OUT=(ZBLUR_TEXTURED_VERTEX_PS)0; 				//init
 OUT.WorldNormal = mul(IN.Normal,(float3x3)g_WorldMatrix); 				//valor del pixel <- transformacion de la normal 
 OUT.UV = IN.UV.xy; 													//copia de la textura 
 float4 vPos = mul(IN.Position, g_WorldViewMatrix); 		//vector posicion hacia donde apunta la camara
 OUT.HPosition = mul( vPos, g_ProyectionMatrix ); 						//valor del pixel <- posicion x,y,z,w segun la proyeccion de la imagen dada por la camara.
 OUT.WorldPosition = mul(IN.Position, g_WorldMatrix).xyz; 		//valor del pixel <- posicion x,y,z,w del mundo ¿pa qué?
 OUT.PosLight=mul(vPos, g_WorldToLightProjectionMatrix); 				//valor del pixel <- distancia segun la luz ¿pa qué?
 OUT.Depth.xy=OUT.HPosition.zw; 										//valor del pixel <- profundidad (Z,1.0) con referencia a la proyeccion de la imagen dada por la camara
 return OUT; 															//devuelve pixel
}

float ComputeDepthBlur(float depth /* in view space */)
{
 float f;
 if(depth<vDofParams.y)
 {
  //scale depth value between near blur distance and focal distance to [-1,0] range
  f=(depth-vDofParams.y)/(vDofParams.y-vDofParams.x); 
 }
 else
 {
  //scale depth value between foca distance and far blur distance to [0,1] range
  f=(depth-vDofParams.y)/(vDofParams.z-vDofParams.y);
  f=clamp(f,0,vDofParams.w);
 }
 //scale and bias into [0, 1] range
 return f*0.5f + 0.5f;
}

float4 ZBlurPS(ZBLUR_TEXTURED_VERTEX_PS IN) : COLOR
{
 float l_Depth=ComputeDepthBlur(IN.Depth.x);
 //parece que no entra aqui 
 //return float4(0.0,1.0,1.0,1.0); //¿por que no devuelve la escena de este color que quiero si alphablendingenable = false y post-render desactivado? 
	
 return float4(l_Depth,l_Depth,l_Depth,1.0);
}

technique RenderZBlurTechnique //creada en  effects.xml y llamada en prerender para el efecto zblur 
{
 pass p0
 {
 ZEnable = true;
 ZWriteEnable = true;
 ZFunc = LessEqual;
 AlphaBlendEnable = false;
 CullMode = CCW;
 VertexShader = compile vs_3_0 NormalTexturedVertexVS();
 PixelShader = compile ps_3_0 ZBlurPS();
 }
}

//-----------------------------------------------------------------------------
// Name: DepthOfFieldManySamples
// Type: Pixel shader
// Desc: This post process pixel shader uses an array of values to offset into the
// render-target texture to blur the image based on the value of alpha channel.
//-----------------------------------------------------------------------------
float4 DepthOfFieldManySamples( in float2 OriginalUV : TEXCOORD0) : COLOR
{
 float3 OriginalRGB = tex2D(NormalTextureSampler, OriginalUV).xyz;  
 float OriginalA = tex2D(DiffuseTextureSampler, OriginalUV).x;
 //return float4(OriginalA,OriginalA,OriginalA,1.0);	
 float3 Blurred = 0;
 int NumSamples=12;
 for(int i = 0; i < NumSamples; i++)
 {
  // Lookup into the rendertarget based by offsetting the
  // original UV by KernelArray[i]. See the TwelveKernelBase[] above
  // and UpdateTechniqueSpecificVariables() for how this array is created
//  float3 CurrentRGB = tex2D(NormalTextureSampler, OriginalUV).xyz;//mio +  TwelveKernelBase[i]/float2(RENDER_TARGET_WIDTH,RENDER_TARGET_HEIGHT)).xyz;
//  float3 CurrentA = tex2D(DiffuseTextureSampler, OriginalUV).x;//mio + TwelveKernelBase[i]/float2(ZBLUR_WIDTH,ZBLUR_HEIGHT)).x;
  float3 CurrentRGB = tex2D(NormalTextureSampler, OriginalUV +  TwelveKernelBase[i]*0.3/float2(RENDER_TARGET_WIDTH,RENDER_TARGET_HEIGHT)).xyz;
  float3 CurrentA = tex2D(DiffuseTextureSampler, OriginalUV + TwelveKernelBase[i]*0.3/float2(ZBLUR_WIDTH,ZBLUR_HEIGHT)).x;

  // Lerp between original rgb and the jitter rgb based on the alpha value
  Blurred += lerp(OriginalRGB, CurrentRGB, saturate(OriginalA*CurrentA));
 }
 return float4(Blurred / NumSamples, 1.0f);
}
technique UsePS20ThirteenLookups
{
 pass P0
  {
  PixelShader = compile ps_3_0 DepthOfFieldManySamples();
  }
}

#endif