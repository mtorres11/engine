#include "MapManager/MapManager.h"
#include "Vertex/VertexType.h"
#include "RenderManager.h"
#include "Light.h"
#include "LightManager.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include <string>

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CSpotLight::~CSpotLight()
{

}
CSpotLight::CSpotLight()
: m_fAngle(0.0),
	m_fFallOff(0.0)
{}

void CSpotLight::SetAngle(float _fAngle)
{
	m_fAngle = _fAngle;
}
float CSpotLight::GetAngle() const
{
  return m_fAngle; 
}
void CSpotLight::SetFallOff(const float _fFallOff)
{
	m_fFallOff = _fFallOff;
}
float CSpotLight::GetFallOff() const
{
	return m_fFallOff;
}
