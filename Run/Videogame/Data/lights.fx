/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/
//Para declarer una estructura de vértice que utilizaremos como entrada para el Vertex Shader
struct TNORMAL_TEXTURED_VERTEX_VS {
	float3 Position : POSITION;
	float3 Normal : NORMAL;
	float4 UV: TEXCOORD0;
};

//Para declarar una estructura de vértice que utilizaremos como salida para el Vertex Shader y
//entrada para el Píxel Shader
struct TNORMAL_TEXTURED_VERTEX_PS {
	float4 Hposition: POSITION;
	float2 UV: TEXCOORD0;
	float3 WorldNormal: TEXCOORD1;
	float3 WorldPosition: TEXCOORD2;
};

texture NAME  
<
  string ResourceName = "";//Optional default file name
  string UIName =  "NAME Texture";
  string ResourceType = "2D";
>;

sampler2D NAMESampler = sampler_state 
{
  Texture = <NAME>;
  MinFilter = Linear;
  MagFilter = Linear;
  MipFilter = Linear;
  AddressU = Wrap;
  AddressV = Wrap;
};
	
texture LightMap 
<
  string ResourceName = "";//Optional default file name
  string UIName =  "NAME Texture";
  string ResourceType = "2D";
>;
	
sampler2D LightMapSampler = sampler_state {
  Texture = <NAME>;
  MinFilter = Linear;
  MagFilter = Linear;
  MipFilter = Linear;
  AddressU = Wrap;
  AddressV = Wrap;
};
		
float4x4 WorldViewProj : WorldViewProjection;
float4x4 g_World: World;
float3 g_Camera : CameraPosition;

float g_LightAmbientIntensity = 0.3f; //luz ambiente
float3 g_LightAmbientColor = (0.9,0.9f,0.9f);
//luz especular es el brillo a objetos

float3 Lamp0Direction : DIRECTION
<
  string Object = "Directional Light INDEX";
  string UIName =  "Lamp INDEX Direction";
  string Space = "World";
> = {-0.5f,2.0f,1.25f};

float3 Lamp_0_color : COLOR 
<
  string Object = "Directional Light INDEX";
  string UIName =  "Lamp INDEX Color";
  string UIWidget = "Color";
> = {1.0f,1.0f,1.0f};
	


TNORMAL_TEXTURED_VERTEX_PS mainVS(TNORMAL_TEXTURED_VERTEX_VS IN)
{
    TNORMAL_TEXTURED_VERTEX_PS l_OUT=(TNORMAL_TEXTURED_VERTEX_PS)0;
    l_OUT.Hposition = mul(float4(IN.Position.xyz, 1.0), WorldViewProj);
	l_OUT.WorldPosition = mul(float4(IN.Position.xyz, 1.0), g_World).xyz;
    l_OUT.UV=IN.UV;
	l_OUT.WorldNormal = mul(float4(IN.Normal.xyz,1.0),g_World);
    return l_OUT;
	//return mul(float4(pos.xyz, 1.0), WorldViewProj);
}

float4 mainPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR {
	float3 Nn = normalize(IN.WorldNormal);
	//float DiffuseIntensity = saturate(dot(-Lamp0Direction, Nn));
	//ambiente = Ca * [Ga + Atti * Spoti * sum(Lai)]
	float3 l_LightAmbient=g_LightAmbientIntensity*g_LightAmbientColor;
	float l_LightDiffuseIntensity=saturate(dot(-Lamp0Direction, Nn));
	//float l_Light = g_LightAmbient*g_LightAmbientColor; 
	float3 l_LightDiffuse=l_LightDiffuseIntensity*Lamp_0_color;
	
	float Vn = normalize(g_Camera -IN.WorldPosition);
	float Hn = normalize (Vn + Lamp0Direction);
	float l_Specular=pow(dot(Nn,Hn),4)*0.3;
	
	
	float4 l_Diffusecolor=tex2D(NAMESampler,IN.UV);
	return float4(saturate(l_LightAmbient+l_LightDiffuse)*l_Diffusecolor.xyz,l_Diffusecolor.a)+float4(l_Specular*Lamp_0_color,1.0);
	return float4(l_Specular,l_Specular,l_Specular,1.0);
	
	/*return l_Light*(Lamp_0_color.xyz,1.0)*DiffuseIntensity * tex2D(NAMESampler,IN.UV);
	//especular= (Cs*sum[Ls (N.H)^p * Atten * spot])
	//H = (norm(norm(Cp - Vp) +Ldir))
	
	float Hn = normalize ((g_Camera - Vn) + Lamp0Direction);
	float Specular = pow(saturate(dot(Nn,Hn)),2000) * Lamp_0_color;
	
	return float4(IN.WorldNormal,1.0);
	return tex2D(NAMESampler,IN.UV);*/
	//return float4(0.5, 1.0, 1.0, 1.0);
}

float mainPSLightMap(TNORMAL_TEXTURED_VERTEX_PS IN):COLOR
{
	float l_LightMapColor = tex2D(LightMapSampler, IN.UV);
	return tex2D(NAMESampler, IN.UV)*l_LightMapColor;
	
//	return float4(0.5, 1.0, 1.0, 1.0);
}


technique technique0 {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
