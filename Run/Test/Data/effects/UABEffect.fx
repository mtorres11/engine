/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

float4x4 WorldViewProj : WorldViewProjection;
float4x4 g_World : World;
float3 g_CameraEye: CameraPosition;

float g_Bump
<
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 20.0;
	float UIStep = 0.01;
	string UIName =  "g_Bump";
> = 0.5;
	

float3 Lamp1Direction : DIRECTION
<
	string Object = "Directional Light 1";
	string UIName =  "Lamp 1 Direction";
	string Space = "World";
> = {-0.5f,2.0f,1.25f};

float3 Lamp_1_color : COLOR <
	string Object = "Directional Light 1";
	string UIName =  "Lamp 1 Color";
	string UIWidget = "Color";
> = {1.0f,1.0f,1.0f};
	

texture Diffuse  <
	string ResourceName = "";//Optional default file name
	string UIName =  "Diffuse Texture";
	string ResourceType = "2D";
>;

sampler2D DiffuseSampler = sampler_state {
	Texture = <Diffuse>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};
	

texture NormalMap  <
	string ResourceName = "";//Optional default file name
	string UIName =  "NormalMap Texture";
	string ResourceType = "2D";
>;

sampler2D NormalMapSampler = sampler_state {
	Texture = <NormalMap>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
};
	
struct TNORMAL_TEXTURED_VERTEX_VS
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV : TEXCOORD0;
	float3 Tangent: TANGENT;
};
//Para declarar una estructura de vértice que utilizaremos como salida para el Vertex Shader y entrada para el Píxel Shader
struct TNORMAL_TEXTURED_VERTEX_PS 
{
	float4 Hposition : POSITION;
	float2 UV : TEXCOORD0;
	float3 WorldNormal : TEXCOORD1;
	float3 WorldPosition : TEXCOORD2;
	float3 WorldTangent: TEXCOORD3;
	float3 WorldBinormal: TEXCOORD4;
};

TNORMAL_TEXTURED_VERTEX_PS mainVS(TNORMAL_TEXTURED_VERTEX_VS IN)
{
	TNORMAL_TEXTURED_VERTEX_PS l_OUT=(TNORMAL_TEXTURED_VERTEX_PS)0;
	l_OUT.Hposition=mul(float4(IN.Position.xyz, 1.0), WorldViewProj);
	l_OUT.UV=IN.UV;
	l_OUT.WorldNormal=mul(IN.Normal, g_World);
	l_OUT.WorldPosition=mul(IN.Position.xyz, g_World);
	
	l_OUT.WorldTangent=mul(IN.Tangent.xyz,(float3x3)g_World);
	l_OUT.WorldBinormal = mul(normalize(cross(IN.Tangent.xyz,IN.Normal)),(float3x3)g_World);
	return l_OUT;
}

float CalcAttenuation(int IdLight, float3 WorldPosition)
{
	float l_StartRangeAttenuation=20; 	//g_LightsStartRange[IdLight]
	float l_EndRangeAttenuation=50;		//g_LightsEndRange[IdLight]
	
	if(true)//if(g_LightsType[IdLight]==LIGHT_DIRECTIONAL)
		return 1.0;
	else
	{
		float l_DistanceToLight=0.0;//length(WorldPosition-g_LightsPosition[IdLight]);
		if(l_DistanceToLight<l_StartRangeAttenuation)
			return 1.0;
		else
			return saturate((l_DistanceToLight-l_StartRangeAttenuation)/(l_EndRangeAttenuation-l_StartRangeAttenuation));
	}
}


// void CalcLightingByLight(int IdLight, float LightAmbient, float4 DiffuseColor, float3 Nn, float3 WorldPosition, out float3 DiffuseValue, out float3 SpecularValue)
// {
	// //if(g_LightsEnabled[IdLight])
	// if(true)
	// {
		// float g_SpecularPower=120;
		// float g_Ks=0.80;

		// float3 l_LightDirection=Lamp1Direction;//g_LightsDirection[IdLight];
		// float3 l_LightColor=Lamp_1_color;//g_LightsColor[IdLight];
		// float l_Att=CalcAttenuation(IdLight, WorldPosition);
		
		// float l_DiffuseIntensity=saturate(dot(Nn,-l_LightDirection))*l_Att;	
		
		// float3 Vn=normalize(g_CameraEye-WorldPosition);
		// float3 Hn=normalize(Vn+Nn);
		
		// float l_SpecularIntensity=pow(saturate(dot(Hn,-l_LightDirection)),g_SpecularPower)*g_Ks*l_Att;

		// DiffuseValue=saturate((1.0-LightAmbient)*l_DiffuseIntensity*DiffuseColor.xyz);
		// SpecularValue=l_SpecularIntensity*l_LightColor.xyz;
	// }
	// else
	// {
		// DiffuseValue=0;
		// SpecularValue=0;
	// }
// }

// #define MAX_LIGHTS 1

// void CalcLighting(float LightAmbient, float4 DiffuseColor, float3 Nn, float3 WorldPosition, out float3 TotalDiffuseValue, out float3 TotalSpecularValue)
// {
	// TotalDiffuseValue=0;
	// TotalSpecularValue=0;
	
	// //for(int i=0;i<MAX_LIGHTS;++i)
	// int i=0;
	// {
		// float3 l_DiffuseValue;
		// float3 l_SpecularValue;
		
		// CalcLightingByLight(i, LightAmbient, DiffuseColor, Nn, WorldPosition, l_DiffuseValue, l_SpecularValue);
		// TotalDiffuseValue+=l_DiffuseValue;
		// TotalSpecularValue=l_SpecularValue;
	// }
// }

// float3 CalcParallaxMap(float3 Vn, float3 WorldNormal, float3 WorldTangent, float3 WorldBinormal, float2 UV, out float2 OutUV)
// {
	// // view directions
	// //float3 Vn = normalize(WorldPosition);
	// float2 l_UV = UV;
	// // parallax code
	// float3x3 tbnXf = float3x3(WorldTangent,WorldBinormal,WorldNormal);
	// float4 l_NormalMapColor = tex2D(NormalMapSampler,l_UV);
	// float height = l_NormalMapColor.w * 0.06 - 0.03;
	// //float height = reliefTex.w * g_Bump - g_Bump*0.5;
	// OutUV=l_UV + height * mul(tbnXf,Vn).xy;
	// // normal map
	// float3 tNorm = l_NormalMapColor.xyz - float3(0.5,0.5,0.5);
	// // transform tNorm to world space
	// tNorm = normalize(tNorm.x*WorldTangent -
	// tNorm.y*WorldBinormal +
	// tNorm.z*WorldNormal);
	// return tNorm;
// }

float3 CalcNormalMap(float3 Nn, float3 WorldTangent, float3 WorldBinormal, float2 UV)
{
	float3 Tn=normalize(WorldTangent);
	float3 Bn=normalize(WorldBinormal);
	float3 bump=g_Bump*(tex2D(NormalMapSampler,UV).rgb - float3(0.5,0.5,0.5));
	float3 l_Nn = Nn + bump.x*Tn + bump.y*Bn;
	return normalize(l_Nn);
}

// float4 mainPS(TNORMAL_TEXTURED_VERTEX_PS IN):COLOR 
// {
	// float g_LightAmbient=0.3;
	// float3 g_LightAmbientColor={1.0,1.0,1.0};
	
	// float3 Nn=normalize(IN.WorldNormal);
	
	
	// float3 Tn=normalize(IN.WorldTangent);
	// float3 Bn=normalize(IN.WorldBinormal);
	
	// //Nn=CalcNormalMap(Nn, IN.WorldTangent, IN.WorldBinormal, IN.UV);
	// float2 l_UV;
	// float3 Vn=normalize(g_CameraEye-IN.WorldPosition);
	
	// Nn=CalcParallaxMap(Vn, Nn, Tn, Bn, IN.UV, l_UV);
	
	// float4 l_DiffuseColor=tex2D(DiffuseSampler, l_UV);
	
	// float3 l_TotalDiffuseValue=0;
	// float3 l_TotalSpecularValue=0;

	// CalcLighting(g_LightAmbient, l_DiffuseColor, Nn, IN.WorldPosition, l_TotalDiffuseValue, l_TotalSpecularValue);
	
	// float3 l_Ambient=g_LightAmbient*g_LightAmbientColor*l_DiffuseColor.xyz;
	
	
	// return float4(l_Ambient+l_TotalDiffuseValue+l_TotalSpecularValue,l_DiffuseColor.a);
	// /*float l_Att=CalcAttenuation();
	
	
	// float l_DiffuseIntensity=saturate(dot(Nn,-Lamp1Direction))*l_Att;
	
	
	
	// float3 Vn=normalize(g_CameraEye-IN.WorldPosition);
	// float3 Hn=normalize(Vn+Nn);
	
	// float l_SpecularIntensity=pow(saturate(dot(Hn,-Lamp1Direction)),g_SpecularPower)*g_Ks*l_Att;
	
	
	// float3 l_DiffuseValue=saturate((1.0-g_LightAmbient)*l_DiffuseIntensity*l_DiffuseColor.xyz)*Lamp_1_color;
	// float3 l_SpecularValue=l_SpecularIntensity*Lamp_1_color.xyz;
	// */
// }

technique technique0 {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
