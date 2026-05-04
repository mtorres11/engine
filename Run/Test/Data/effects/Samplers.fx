#ifndef INC_SAMPLERS_H_
#define INC_SAMPLERS_H_

sampler NormalTextureSampler : register( s1 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

// Sampler para bumpMap cuando tiene lightmap+bumpMap
sampler NormalTextureSampler2 : register( s2 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};
// Sampler para lightMap cuando tiene lightmap+bumpMap
sampler LightmapTextureSampler2 : register( s1 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

sampler DiffuseTextureSampler : register( s0 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler DifuminadoTextureSampler : register( s0 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

sampler LightmapTextureSampler : register( s1 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

sampler EnvironmentTextureSampler : register( s3 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler GlowTextureSampler : register( s4 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler ShadowTextureSampler : register( s5 ) = sampler_state
{
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

#endif