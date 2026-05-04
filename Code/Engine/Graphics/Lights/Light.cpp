#include "MapManager/MapManager.h"
#include "RenderManager.h"
#include "Light.h"
#include "LightManager.h"
#include "LightEffects/LightEffect.h"
#include "Math/Matrix44.h"
#include <string>

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CLight::CLight()
: m_Color(colYELLOW),
	m_Type(OMNI),
	m_sName(""),
	m_bRenderShadows(false),
	m_fStartRangeAttenuation(0.0),
	m_fEndRangeAttenuation(0.0),
	m_bEnable(false),
  m_pLightEffect(0)
{ }

CLight::~CLight()
{  
  CHECKED_DELETE(m_pLightEffect)
}

void CLight::SetName(const std::string &_sName)
{  
	m_sName = _sName;
}

void CLight::SetColor(const CColor &_Color)
{
	m_Color = _Color;
}

void CLight::SetMultiplier(float _fMultiplier)
{
	m_fMultiplier = _fMultiplier;
}

const CColor &CLight::GetColor() const
{
	return m_Color;
}

float CLight::GetMultiplier()
{
	return m_fMultiplier;
}

void CLight::SetStartRangeAttenuation(const float _fStartRangeAttenuation)
{
	m_fStartRangeAttenuation = _fStartRangeAttenuation;
}

float CLight::GetStartRangeAttenuation()const
{
	return m_fStartRangeAttenuation;
}

void CLight::SetEndRangeAttenuation(const float _fEndRangeAttenuation)
{
	m_fEndRangeAttenuation = _fEndRangeAttenuation;
}

float CLight::GetEndRangeAttenuation()const
{
	return m_fEndRangeAttenuation;
}

void CLight::SetType(const TLightType _Type)
{
    m_Type = _Type;
}

CLight::TLightType CLight::GetType() const
{
    return m_Type;
}

void CLight::Render(CRenderManager *_pRM)
{
  Mat44f l_mat44;
  l_mat44.SetIdentity();
  l_mat44.Translate(m_Position);
  _pRM->SetTransform(l_mat44);
  _pRM->DrawAxis(3);
}

void CLight::SetEnable(bool _bEnable)
{
	m_bEnable = _bEnable;
}

bool CLight::GetEnable()
{
	return m_bEnable;
}



