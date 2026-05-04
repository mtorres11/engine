#include "Globals.fx"
#include "Samplers.fx"
#include "VertexType.fx"

//-------------Lista de Efectos--------------------
//--- Glow ----------------------------------------
//--- NormalMap -----------------------------------
//--- LightMap ------------------------------------
//--- RenderNoiseAndVignetting --------------------
//--- ShadowMap -----------------------------------
//--- Difuminado ----------------------------------
//--- Ondulatorio ---------------------------------
//--- Difuminado ----------------------------------
//--- Ondulatorio ---------------------------------
//--- Relieve -------------------------------------
//--- Cal3D ---------------------------------------
//--- Blur ----------------------------------------

//Efecto Glow
//--------------------------------------------------
ZBLUR_TEXTURED_VERTEX_PS GlowVS(TNORMAL_TEXTURED_VERTEX_VS IN)
{
	ZBLUR_TEXTURED_VERTEX_PS l_Out=(ZBLUR_TEXTURED_VERTEX_PS)0;
	l_Out.HPosition=mul(float4(IN.Position,1.0),g_WorldViewProjectionMatrix);
	l_Out.UV=IN.UV;
	return l_Out;
}
float4 GlowPS(ZBLUR_TEXTURED_VERTEX_PS IN):COLOR
{
	return tex2D(GlowTextureSampler, IN.UV);
}
float4 RenderGlowPostFXPS(in float4 Color: COLOR, in float2 UV : TEXCOORD0) : COLOR
{
	float4 l_Total=0;
	int l_Counter=0;
	for(int i=-(GLOWVTXCOUNTER-1);i<(GLOWVTXCOUNTER);++i)
	{
		float l_XTextureInc=i*g_IncrementTexture;
		for(int j=-(GLOWVTXCOUNTER-1);j<(GLOWVTXCOUNTER);++j)
		{
			float l_YTextureInc=j*g_IncrementTexture;
			float l_KT=(g_KT[abs(i)]+g_KT[abs(j)])/2.0;
			float4 l_Color=tex2D( GlowTextureSampler, UV + float2(l_XTextureInc, l_YTextureInc));
			if(l_Color.a>0)
			{
				l_Total+=l_Color;//(l_Color/**l_Color.a*/)*l_KT;
				l_Color.a=1.0;
			}
			else
				l_Color.a=0.0;
			l_Counter=l_Counter+1*l_Color.a;
		}
	}
	return l_Total/max(1,l_Counter);
}
technique RenderGlowTechnique
{
	pass p0
	{
		VertexShader = compile vs_3_0 GlowVS();
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		PixelShader = compile ps_3_0 GlowPS();
	}
}
//--------------------------------------------------



//Efecto NormalMap
//--------------------------------------------------
TNORMAL_TEXTURED_VERTEX_PS NormalTexturedVertexVS(TNORMAL_TEXTURED_VERTEX_VS IN)
{
	TNORMAL_TEXTURED_VERTEX_PS OUT = (TNORMAL_TEXTURED_VERTEX_PS)0;
	OUT.WorldNormal = mul(IN.Normal,(float3x3)g_WorldMatrix);
	OUT.UV = IN.UV.xy;
	
	float3 vPos = mul( float4(IN.Position.xyz,1.0), g_WorldViewMatrix ).xyz;
	OUT.PosLight = mul( float4(vPos, 1.0), g_WorldToLightProjectionMatrix );
	
	//OUT.HPosition=OUT.PosLight;
	OUT.HPosition = mul(float4(IN.Position,1.0),g_WorldViewProjectionMatrix);
	return OUT;
}
float4 NormalTexturedVertexPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
	//En el pixel shader deberemos introducir el siguiente código
	float2 ShadowTexC = 0.5 * IN.PosLight.xy / IN.PosLight.w + float2( 0.5, 0.5 );
	ShadowTexC.y = 1.0f - ShadowTexC.y;
	// PETA EN ESTA LINEA
	float LightAmount = (tex2D( ShadowTextureSampler, ShadowTexC ) + SHADOW_EPSILON) < (IN.PosLight.z / IN.PosLight.w) ? 0.0f: 1.0f;
	//float LightAmount = (tex2D( ShadowTextureSampler, ShadowTexC ) + SHADOW_EPSILON < IN.PosLight.z / IN.PosLight.w)? 0.0f: 1.0f;
	// 1 – Píxel iluminado
	// 0 – Píxel en sombra
	//return float4(LightAmount,LightAmount,LightAmount,1.0);
	//return tex2D(ShadowTextureSampler,IN.UV);
	
	return LightAmount*tex2D(DiffuseTextureSampler,IN.UV);

}
float4 ShowNormalPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
	float3 Nn=normalize(IN.WorldNormal);
	return float4(Nn, 1.0);
}
technique NormalTextureVertexTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_2_0 NormalTexturedVertexVS();
		PixelShader = compile ps_2_0 NormalTexturedVertexPS();
	}
}
//--------------------------------------------------



//Efecto LightMap
//--------------------------------------------------
TNORMAL_2TEXTURED_VERTEX_PS LightmapVS(TNORMAL_2TEXTURED_VERTEX_VS IN)
{
	TNORMAL_2TEXTURED_VERTEX_PS OUT = (TNORMAL_2TEXTURED_VERTEX_PS)0;
	OUT.WorldNormal = mul(IN.Normal,(float3x3)g_WorldMatrix);
	OUT.UV = IN.UV.xy;
	OUT.UV2 = IN.UV2.xy;
	OUT.HPosition = mul(float4(IN.Position,1.0),g_WorldViewProjectionMatrix);
	return OUT;
}
float4 LightmapPS(TNORMAL_2TEXTURED_VERTEX_PS IN): COLOR
{
	return float4(1,1,1,1);
	float4 l_DiffuseColor = tex2D(DiffuseTextureSampler, IN.UV);
	float4 l_LightmapColor = tex2D(LightmapTextureSampler, IN.UV2);
	
	return l_DiffuseColor*l_LightmapColor*20;
}
technique LightmapTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_2_0 NormalTexturedVertexVS();
		PixelShader = compile ps_2_0 LightmapPS();
	}
}
//--------------------------------------------------



//Efecto RenderNoiseAndVignetting
//--------------------------------------------------
float4 RenderNoiseAndVignettingPS(in float2 UV : TEXCOORD0) : COLOR
{
	float2 l_Offset=float2(g_Time*0.39*0.001,g_Time*0.79*0.001);
	float2 l_UV=UV+l_Offset;
	float4 l_VignettingColor=tex2D(NormalTextureSampler, l_UV);
	float4 l_NoiseColor=tex2D(DiffuseTextureSampler, l_UV);
	//return float4(l_VignettingColor.xyz,l_VignettingColor.a*10);
	//float l_C=l_NoiseColor.a;
	//return float4(l_C.x,l_C.x,l_C.x,0.8);
	return float4(l_NoiseColor.xyz*l_VignettingColor.xyz, l_NoiseColor.a*2+l_VignettingColor.a*13);
}
technique RenderNoiseAndVignettingTechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 RenderNoiseAndVignettingPS();
	}
}
//--------------------------------------------------


//Efecto ShadowMap
//--------------------------------------------------
void VertShadow(float4 Pos : POSITION,float3 Normal : NORMAL,out float4 oPos : POSITION,out float2 Depth : TEXCOORD0 )
{
	// Compute the projected coordinates
	oPos = mul( Pos, g_WorldViewProjectionMatrix );
	
	// Store z and w in our spare texcoord
	Depth.xy = oPos.zw;
}
void PixShadow( float2 Depth : TEXCOORD0,out float4 Color : COLOR)
{
	// Depth is z / w
	//float l_Color = (Depth.x / Depth.y)/100.0;
	Color = Depth.x / Depth.y;
	//Color=float4(l_Color,l_Color,l_Color,1.0);
}
technique StaticMeshesTechniqueShadowMap
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}
//----------------------------------------



//Efecto Difuminado
//--------------------------------------------------
float4 DifuminadoPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR 
{
	float4 color = tex2D(DifuminadoTextureSampler, IN.UV);//todos
	color += tex2D(DifuminadoTextureSampler, IN.UV-0.01);//difuminado
	color += tex2D(DifuminadoTextureSampler, IN.UV+0.01);//difuminado
	color = color * 0.333;
	return color;
}
technique RenderDifuminadoTechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 DifuminadoPS();
	}
}
//----------------------------------------


//Efecto Ondulatorio
//--------------------------------------------------
float4 OndulatorioPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR 
{
	IN.UV.y = IN.UV.y + (sin(IN.UV.x*30.0)*0.015*sin(g_Time));
	float4 color = tex2D(DifuminadoTextureSampler, IN.UV);//todos

	return color;
}
technique RenderOndulatorioTechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 OndulatorioPS();
	}
}
//----------------------------------------



// Efecto Relieve
//--------------------------------------------------
 float4 RelievePS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR
{
  // Sharpening the image
  float sharpAmount = 15.0f;
  float4 color = tex2D( DifuminadoTextureSampler, IN.UV);
  color += tex2D( DifuminadoTextureSampler, IN.UV - 0.0001) * sharpAmount;
  color -= tex2D( DifuminadoTextureSampler, IN.UV + 0.0001) * sharpAmount;
  return color;
}
technique RenderRelieveTechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = false;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 RelievePS();
	}
}
//----------------------------------------




// Cal3D
//--------------------------------------------------
technique Cal3DTechniqueShadow
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CCW;
		VertexShader = compile vs_3_0 VertShadow();
		PixelShader = compile ps_3_0 PixShadow();
	}
}
float3 CalcAnimtedPos(float4 Position, float4 Indices, float4 Weight)
{
	float3 l_Position=0;
	l_Position = mul(g_Bones[Indices.x], Position) * Weight.x;
	l_Position += mul(g_Bones[Indices.y], Position) * Weight.y;
	l_Position += mul(g_Bones[Indices.z], Position) * Weight.z;
	l_Position += mul(g_Bones[Indices.w], Position) * Weight.w;
	return l_Position;
}

void CalcAnimatedNormalTangent(float3 Normal, float3 Tangent, float4 Indices, float4 Weight, out float3 OutNormal, out float3 OutTangent)
{
	OutNormal = 0;
	OutTangent =0;
	float3x3 m;
	m[0].xyz = g_Bones[Indices.x][0].xyz;
	m[1].xyz = g_Bones[Indices.x][1].xyz;
	m[2].xyz = g_Bones[Indices.x][2].xyz;
	OutNormal += mul(m, Normal.xyz)* Weight.x;
	OutTangent += mul(m, Tangent.xyz)* Weight.x;
	m[0].xyz = g_Bones[Indices.y][0].xyz;
	m[1].xyz = g_Bones[Indices.y][1].xyz;
	m[2].xyz = g_Bones[Indices.y][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.y);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.y);
	m[0].xyz = g_Bones[Indices.z][0].xyz;
	m[1].xyz = g_Bones[Indices.z][1].xyz;
	m[2].xyz = g_Bones[Indices.z][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.z);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.z);
	m[0].xyz = g_Bones[Indices.w][0].xyz;
	m[1].xyz = g_Bones[Indices.w][1].xyz;
	m[2].xyz = g_Bones[Indices.w][2].xyz;
	OutNormal += normalize(mul(m, Normal.xyz)* Weight.w);
	OutTangent += normalize(mul(m, Tangent.xyz)* Weight.w);
	OutNormal = normalize(float3(-OutNormal.x,OutNormal.y,OutNormal.z));
	OutTangent = normalize(float3(-OutTangent.x,OutTangent.y,OutTangent.z));
}

CAL3D_HW_VERTEX_PS RenderCal3DHWVS(CAL3D_HW_VERTEX_VS IN)
{
	CAL3D_HW_VERTEX_PS OUT=(CAL3D_HW_VERTEX_PS)0;
	float3 l_Normal= 0;
	float3 l_Tangent=0;
	CalcAnimatedNormalTangent(IN.Normal.xyz, IN.Tangent.xyz, IN.Indices, IN.Weight, l_Normal, l_Tangent);
	float3 l_Position=CalcAnimtedPos(float4(IN.Position.xyz,1.0), IN.Indices, IN.Weight);
	float4 l_WorldPosition=float4(-l_Position.x,l_Position.y,l_Position.z,1.0f);
	OUT.WorldPosition=mul(l_WorldPosition,g_WorldMatrix);
	OUT.WorldNormal=normalize(mul(float4(l_Normal,0),g_WorldMatrix));
	OUT.WorldTangent=normalize(mul(float4(l_Tangent,0),g_WorldMatrix));
	OUT.WorldBinormal=mul(cross(l_Tangent,l_Normal),(float3x3)g_WorldMatrix);
	OUT.UV = IN.TexCoord.xy;
	OUT.HPosition = mul(l_WorldPosition, g_WorldViewProjectionMatrix );
	return OUT;
}

float4 RenderCal3DHWPS(CAL3D_HW_VERTEX_PS IN) : COLOR
{
	float3 Nn=normalize(IN.WorldNormal);//CalcBumpMap(IN.WorldPosition, IN.WorldNormal, IN.WorldTangent, IN.WorldBinormal, IN.UV);
	float4 l_SpecularColor = 1.0;
	float4 l_DiffuseColor=tex2D(DiffuseTextureSampler, IN.UV);
	return l_DiffuseColor;
	return float4(1,1,1,1);//CalcLighting (IN.WorldPosition, Nn, l_DiffuseColor, l_SpecularColor);
}
technique Cal3DTechnique
{
	pass p0
	{
		ZEnable = true;
		ZWriteEnable = true;
		ZFunc = LessEqual;
		AlphaBlendEnable = false;
		CullMode = CW;
		VertexShader = compile vs_3_0 RenderCal3DHWVS();
		PixelShader = compile ps_3_0 RenderCal3DHWPS();
	}
}
//---------------------------------------------------------



//----------------------------------------
// Scene effect Noise
//float g_Time = 0.05;
//----------------------------------------


// Efecto Blur
//--------------------------------------------------
float4 BlurPS(TNORMAL_TEXTURED_VERTEX_PS IN) : COLOR 
{
	float4 color = (1.0,1.0,1.0,0.95);
	return tex2D(DifuminadoTextureSampler, IN.UV)*color;
}

//----------------------------------------
// Technique Blur

technique RenderGUITechnique
{
	pass p0
	{
		CullMode = CCW;
		ZEnable = false;
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend=SrcAlpha;
		DestBlend=InvSrcAlpha;
		PixelShader = compile ps_3_0 BlurPS();
	}
}








