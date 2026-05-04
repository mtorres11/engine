#include "Effect.h"
#include "RenderManager.h"
#include "Lights/LightManager.h"
#include "Lights/Light.h"
#include "Lights/DirectionalLight.h"
#include "Lights/SpotLight.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CDirectionalLight *g_defaultLight = 0;

///<summary>
/// CEffect:: Constructor
///</summary>
CEffect::CEffect()
: m_FileName(""),
	m_Effect(0),
	m_WorldMatrixParameter(0),
	m_ViewMatrixParameter(0),
	m_ProjectionMatrixParameter(0),
	m_WorldViewMatrixParameter(0),
	m_ViewProjectionMatrixParameter(0),
	m_WorldViewProjectionMatrixParameter(0),
	m_ViewToLightProjectionMatrixParameter(0),
	m_LightEnabledParameter(0),
	m_LightsTypeParameter(0),
	m_LightsPositionParameter(0),
	m_LightsDirectionParameter(0),
	m_LightsAngleParameter(0),
	m_LightsColorParameter(0),
  m_AmbientLightParameter(0),
  m_SelfIlluminationColorParameter(0),
	m_LightsFallOffParameter(0),
	m_LightsStartRangeAttenuationParameter(0),
	m_LightsEndRangeAttenuationParameter(0),
	m_CameraPositionParameter(0),
	m_BonesParameter(0),
	m_TimeParameter(0)
{
	for(int i=0; i< MAX_LIGHTS_BY_SHADER; i++)
	{
		m_LightsEnabled[i] = false;
		m_LightsType[i] = 0;
		m_LightsAngle[i] = 0.0f;
		m_LightsFallOff[i] = 0.0f;
		m_LightsStartRangeAttenuation[i] = 0.0f;
		m_LightsEndRangeAttenuation[i] = 0.0f;
		m_LightsPosition[i] = 0.0f;
		m_LightsDirection[i] = 0.0f;
		m_LightsColor[i] = 0.0f;
	}
}

///<summary>
/// CEffect:: Destructor
///</summary>
CEffect::~CEffect()
{
  if(g_defaultLight != 0) delete(g_defaultLight);
	//CHECKED_DELETE(m_Effect);
  //delete m_Effect;
	SetNullParameters();
}

///<summary>
/// CEffect:: Release : Libera memoria y coloca todos los parametros en null.
///</summary>
void CEffect::Release()
{
	CHECKED_RELEASE(m_Effect);
  SetNullParameters();

}

///<summary>
/// CEffect:: Load : Carga un efecto segun el Filename.
///</summary>
///<param name="&FileName">Nombre del fichero XML</param>
///<returns name="l_bIsOk">Variable que nos indica si se ha leído bien el fichero XML</returns>
bool CEffect::Load(const std::string &FileName)
{

	CRenderManager *RM = CORE->GetRenderManager();
  m_FileName = FileName;
	std::string l_FileName = FileName;
	LPD3DXBUFFER l_ErrorBuffer=NULL;
  
  bool flag = false;
  if(m_Effect != NULL)
  {
    D3DXCreateEffectFromFile(RM->GetDevice(), l_FileName.c_str(), NULL, NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, NULL, &m_Effect, &l_ErrorBuffer);
    flag = true;
  }
  else
  {
    HRESULT l_HR=D3DXCreateEffectFromFile(RM->GetDevice(), l_FileName.c_str(), NULL, NULL, D3DXSHADER_USE_LEGACY_D3DX9_31_DLL, NULL, &m_Effect, &l_ErrorBuffer);
    flag = true;
  }
	if(l_ErrorBuffer!=NULL)
	{
		LOGGER->AddNewLog(ELL_ERROR, "CEffect::Load-> Error creating effect(%s) Error: %s", l_FileName.c_str(), l_ErrorBuffer->GetBufferPointer());
		CHECKED_RELEASE(l_ErrorBuffer);
		return false;
	}
	else
	{
    if(flag)
		  SetNullParameters();
		
    return true;
	}
}

///<summary>
/// CEffect:: Reload : Vuelve a cargar un efecto, primero liberando memoria.
///</summary>
///<returns name="l_bIsOk">Nos indica si se ha leído bien el fichero XML</returns>
bool CEffect::Reload()
{
	//Release();
  //Unload();
  return Load(m_FileName);
}

///<summary>
/// CEffect:: LoadEffect : Nos dice si esta cargado un Efecto.
///</summary>
///<returns name="varBool">Nos indica si se ha cargado bien el efecto</returns>
bool CEffect::LoadEffect()
{  
/*
	if(m_Effect)
	{ 
		return true;
	}
	else
	{
		return false;
	}
*/
  return 0;
}

///<summary>
/// CEffect:: SetLights : Establece las propiedades de las luces según el parámetro del número de luces.
///</summary>
///<param name="NumOfLights">Número de luces</param>
///<returns name="varBool">Nos indica si se han establecido bien las luces</returns>
bool CEffect::SetLights(size_t NumOfLights)
{
	CLightManager* l_pLightManager = CORE->GetLigthManager();
	std::vector<CLight*> l_vLights = l_pLightManager->GetLights();

  if (l_vLights.empty())
  {
    g_defaultLight = new CDirectionalLight();
//    defaultLight->SetYaw(0.0);
//    defaultLight->SetPitch(0.0);
//    defaultLight->SetRoll(0.0);
    g_defaultLight->SetName("luzdefecto");
    g_defaultLight->SetType(CDirectionalLight::DIRECTIONAL);
    g_defaultLight->SetColor(colWHITE);

    g_defaultLight->SetPosition(Vect3f(-5.0f,5.0f,-5.0f));
    g_defaultLight->SetVisible(true);
    g_defaultLight->SetDirection(Vect3f(0.0f,0.0f,0.0f));
    g_defaultLight->SetStartRangeAttenuation(80.0);
    g_defaultLight->SetEndRangeAttenuation(200.0);
    l_vLights.push_back((CLight*)g_defaultLight);
    NumOfLights = 1;
  }

  if (l_vLights.size() < NumOfLights)
    NumOfLights = l_vLights.size();

	for(size_t i = 0; i < NumOfLights; ++i)
	{
		CLight *l_Light = l_vLights[i];
		m_LightsEnabled[i] = true;
    m_LightsType[i] = l_Light->GetType();

		m_LightsStartRangeAttenuation[i] = l_Light->GetStartRangeAttenuation();
		m_LightsEndRangeAttenuation[i] = l_Light->GetEndRangeAttenuation();
		m_LightsPosition[i] = l_Light->GetPosition();
		CColor l_LightColor = l_Light->GetColor() * l_Light->GetMultiplier();
		m_LightsColor[i] = Vect3f(l_LightColor.GetRed(),l_LightColor.GetGreen(),l_LightColor.GetBlue());

		if(m_LightsType[i] == CLight::DIRECTIONAL)
		{
			CDirectionalLight* l_pDirectional = (CDirectionalLight*) l_Light;
			m_LightsDirection[i] = l_pDirectional->GetDirection();
		}else
		{
			if(m_LightsType[i] == CLight::SPOT)
				{
					CSpotLight* l_pSpot = (CSpotLight*) l_Light;
					m_LightsAngle[i]   = l_pSpot->GetAngle();
					m_LightsFallOff[i] = l_pSpot->GetFallOff();
					m_LightsDirection[i] = l_pSpot->GetDirection();
				}
		}
	}

	for(int i = NumOfLights; i < MAX_LIGHTS_BY_SHADER; i++)
	{
		m_LightsEnabled[i] = false;
	}

	m_Effect->SetBoolArray(m_LightEnabledParameter, m_LightsEnabled, MAX_LIGHTS_BY_SHADER);	
	m_Effect->SetIntArray(m_LightsTypeParameter, m_LightsType, NumOfLights);
	m_Effect->SetFloatArray(m_LightsAngleParameter, m_LightsAngle, NumOfLights);

	m_Effect->SetFloatArray(m_LightsFallOffParameter, m_LightsFallOff, NumOfLights);
	m_Effect->SetFloatArray(m_LightsStartRangeAttenuationParameter, m_LightsStartRangeAttenuation, NumOfLights);
	m_Effect->SetFloatArray(m_LightsEndRangeAttenuationParameter, m_LightsEndRangeAttenuation, NumOfLights);

	m_Effect->SetFloatArray(m_LightsPositionParameter, (float*)m_LightsPosition, NumOfLights*3);
	m_Effect->SetFloatArray(m_LightsColorParameter, (float*)m_LightsColor, NumOfLights*4);
	m_Effect->SetFloatArray(m_LightsDirectionParameter, (float*)m_LightsDirection, NumOfLights*3);

  m_AmbientLight = l_pLightManager->GetAmbientLight();
	const float l_fAmbientLight[4] = {m_AmbientLight.x, m_AmbientLight.y, m_AmbientLight.z, m_AmbientLight.w};
	m_Effect->SetFloatArray(m_AmbientLightParameter, (float*)l_fAmbientLight, 4);
	
  return true;
}

///<summary>
/// CEffect:: GetParameterBySemantic : Para encontrar una variable HLSL dentro del effect utilizaremos el método de DirectX GetParameterBySemantic, 
/// el cual nos devolverá un D3DXHANDLE con la dirección de memoria dónde se encuentra nuestra variable en el shader.
///</summary>
///<param name="&SemanticName">Nombre de la semántica</param>
///<param name="&l_Handle">Parámetro de DirectX D3DXHANDLE</param>

void CEffect::GetParameterBySemantic(const std::string &SemanticName, D3DXHANDLE &l_Handle)
{
	l_Handle=m_Effect->GetParameterBySemantic(NULL,SemanticName.c_str());

	if(l_Handle==NULL)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CEffect::GetParameterBySemantic-> Parameter by semantic (%s) wasn't found on effect (%s)", SemanticName.c_str(), m_FileName.c_str());
	}
}

///<summary>
/// CEffect:: Unload : CHECKED_RELEASE del efecto.
///</summary>
void CEffect::Unload()
{
	CHECKED_RELEASE(m_Effect);
}

///<summary>
/// CEffect:: SetNullParameters : Establece todos los parametros en NULL.
///</summary>
void CEffect::SetNullParameters()
{
	m_WorldMatrixParameter = 0;
	m_ViewMatrixParameter = 0;
	m_ProjectionMatrixParameter = 0;
	m_WorldViewMatrixParameter = 0;
	m_ViewProjectionMatrixParameter = 0;
	m_WorldViewProjectionMatrixParameter = 0;
	m_ViewToLightProjectionMatrixParameter = 0;
	m_LightEnabledParameter = 0;
	m_LightsTypeParameter = 0;
	m_LightsPositionParameter = 0;
	m_LightsDirectionParameter = 0;
	m_LightsAngleParameter = 0;
	m_LightsColorParameter = 0;
	m_LightsFallOffParameter = 0;
	m_LightsStartRangeAttenuationParameter = 0;
	m_LightsEndRangeAttenuationParameter = 0;
	m_CameraPositionParameter = 0;
	m_BonesParameter = 0;
	m_TimeParameter = 0;

	GetParameterBySemantic("WORLDVIEWPROJECTION",m_WorldViewProjectionMatrixParameter);
	GetParameterBySemantic("WORLD",m_WorldMatrixParameter);
	GetParameterBySemantic("VIEW",m_ViewMatrixParameter);
	GetParameterBySemantic("PROJECTION",m_ProjectionMatrixParameter);
	GetParameterBySemantic("WORLDVIEW",m_WorldViewMatrixParameter);
	GetParameterBySemantic("VIEWPROJECTION",m_ViewProjectionMatrixParameter);
	GetParameterBySemantic("VIEWTOLIGHTPROJECTION",m_ViewToLightProjectionMatrixParameter);
	GetParameterBySemantic("TIME",m_TimeParameter);
	GetParameterBySemantic("CAMERAPOSITION",m_CameraPositionParameter);
	GetParameterBySemantic("BONES",m_BonesParameter);
	GetParameterBySemantic("LIGHTSDIRECTION", m_LightsDirectionParameter);
	GetParameterBySemantic("LIGHTSCOLOR", m_LightsColorParameter);
	GetParameterBySemantic("AMBIENT", m_AmbientLightParameter);
	GetParameterBySemantic("SELFILLUMINATION", m_SelfIlluminationColorParameter);
	GetParameterBySemantic("LIGHTSENABLED", m_LightEnabledParameter);
	GetParameterBySemantic("LIGHTSPOSITION", m_LightsPositionParameter);
	GetParameterBySemantic("LIGHTSTYPE", m_LightsTypeParameter);
	GetParameterBySemantic("LIGHTSSTARTATT", m_LightsStartRangeAttenuationParameter);
	GetParameterBySemantic("LIGHTSENDATT", m_LightsEndRangeAttenuationParameter);
	GetParameterBySemantic("LIGHTSFALLOFF", m_LightsFallOffParameter);
	GetParameterBySemantic("LIGHTSANGLE", m_LightsAngleParameter);
	GetParameterBySemantic("INSTANCESPOSITION", m_InstancesPositionParameter);
}

///<summary>
/// CEffect:: GetD3DEffect : Devuelve el handle de DirectX del efecto.
///</summary>
///<returns name="m_Effect">Devuelve el efecto de tipo LPD3DXEFFECT</returns>
LPD3DXEFFECT CEffect::GetD3DEffect() const
{
	return m_Effect;
}

///<summary>
/// CEffect:: GetTechniqueByName : Devuelve el handle de DirectX de la technique.
///</summary>
///<param name="&TechniqueName">Nombre de la técnica</param>
///<returns name="l_EffectTechnique">Devuelve el handle de DirectX D3DXHANDLE</returns>
D3DXHANDLE CEffect::GetTechniqueByName(const std::string &TechniqueName)
{
  D3DXHANDLE l_EffectTechnique=m_Effect->GetTechniqueByName(TechniqueName.c_str());
	return l_EffectTechnique;
}


//Vect3f&	CEffect::GetLightsDirection()
//{
//	return m_LightsDirection;
//}
//
//Vect3f&	CEffect::GetLightsColor		()
//{
//	return m_LightsColor;
//}
//
//Vect3f&	CEffect::GetLightsPosition	()
//{
//	return m_LightsPosition;
//}


