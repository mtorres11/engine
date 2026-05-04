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
	AddressU = WRAP;
	AddressV = WRAP;
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