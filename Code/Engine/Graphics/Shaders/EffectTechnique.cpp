#include <d3dx9.h>
#include "EffectTechnique.h"
#include "EffectManager.h"
#include "xml/XMLTreeNode.h"
#include "Effect.h"
#include "Logger/Logger.h"
#include "Exceptions/Exception.h"
#include <string>
#include "Timer/Timer.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


///<summary>
/// CEffectTechnique:: Constructor: Encapsula la información que utilizaremos para cada una de las techniques de nuestro effect
///</summary>
///<param name="TreeNode">Gestiona la lectura y escritura en ficheros XML</param>
CEffectTechnique::CEffectTechnique ()
:m_UseCameraPosition(false)
,m_UseInverseProjMatrix(false)
,m_UseInverseViewMatrix(false)
,m_UseInverseWorldMatrix(false)
,m_UseLights(false)
,m_NumOfLights(0)
,m_UseLightAmbientColor(false)
,m_UseProjMatrix(false)
,m_UseViewMatrix(false)
,m_UseWorldMatrix(false)
,m_UseWorldViewMatrix(false)
,m_UseWorldViewProjectionMatrix(false)
,m_UseViewProjectionMatrix(false)
,m_UseViewToLightProjectionMatrix(false)
,m_UseTime(false)
,m_UseLightsDirection(false)
,m_UseLightsColor(false)
,m_UseLightsPosition(false)
,m_UseLightsEnabled(false)
,m_UseLightsType(false)
,m_UseLightsStartRange(false)
,m_UseLightsEndRange(false)
,m_UseSelfIlluminationColor(false)
,m_TimeParameter(0.f)
,m_Effect(0)
,m_D3DTechnique(0)
,m_TechniqueName("")
,m_EffectName("")
{
}
void CEffectTechnique::Init (CXMLTreeNode &TreeNode)
{
	m_UseInverseProjMatrix = false;
  m_UseInverseViewMatrix = false;
  m_UseInverseWorldMatrix = false;
  m_UseLightAmbientColor = false;
  m_UseViewMatrix = false;
  m_UseViewProjectionMatrix = false;
  m_UseLightsDirection = false;
  m_UseLightsColor = false;
  m_UseLightsPosition = false;
  m_UseLightsEnabled = false;
  m_UseLightsType = false;
  m_UseLightsStartRange = false;
  m_UseLightsEndRange = false;


	m_EffectName = TreeNode.GetPszProperty("effect");
	m_UseWorldMatrix = TreeNode.GetBoolProperty("use_world_matrix", false);
	m_UseWorldViewProjectionMatrix = TreeNode.GetBoolProperty("use_world_view_projection_matrix", false);
	m_UseLights = TreeNode.GetBoolProperty("use_lights", false);
	m_NumOfLights = TreeNode.GetIntProperty("num_of_lights", 0);
	m_UseCameraPosition = TreeNode.GetBoolProperty("use_camera_position", false);
	m_UseProjMatrix = TreeNode.GetBoolProperty("use_projection_matrix", false);
	m_UseWorldViewMatrix = TreeNode.GetBoolProperty("use_world_view_matrix", false);
	m_UseViewToLightProjectionMatrix = TreeNode.GetBoolProperty("use_view_to_light_projection_matrix", false);
	m_TechniqueName=TreeNode.GetPszProperty("name");
  m_UseTime = TreeNode.GetBoolProperty("use_time", false);
  m_UseSelfIlluminationColor = TreeNode.GetBoolProperty("use_self_illumination_color", false);
	m_UseInstancingPos = TreeNode.GetBoolProperty("use_instancing", false);

	Refresh();
}

///<summary>
/// CEffectTechnique:: Destructor.
///</summary>
CEffectTechnique::~CEffectTechnique()
{
  if (m_D3DTechnique)
	  m_D3DTechnique=NULL;
}

///<summary>
/// CEffectTechnique:: GetD3DTechnique : Devuelve m_D3DTechnique.
///</summary>
///<returns name="m_D3DTechnique">Devuelve D3DTechnique de tipo D3DXHANDLE</returns>
D3DXHANDLE CEffectTechnique::GetD3DTechnique()
{
	return m_D3DTechnique;
}


///<summary>
/// Preparará las variables del effect según las variables booleanas
///</summary>
///<returns name="varBool">Indica si se ha renderizado bien</returns>
bool CEffectTechnique::BeginRender(Vect3f* _pV3fSelfIlluminationColor)
{
	CEffectManager *EM=CORE->GetEffectManager();
	
	 //_pMaterial->ActivateTextures(l_pRM);

	//Position
	if(m_UseCameraPosition)
	{
		Vect3f l_Pos = EM->GetCameraEye();
		const float l_fPos[3] = {l_Pos.x,l_Pos.y,l_Pos.z};
		m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->m_CameraPositionParameter, l_fPos, 3);
	}

	//Lights
	if(m_UseLights)
	{
		//en SetLights ya fija todos los parámetros de las luces (Direction, Position, Color, Type, StartRangeAtt, EndRangeAtt, Enabled)
		m_Effect->SetLights(m_NumOfLights);
	}

	//Matrix Projection
	if(m_UseProjMatrix)
	{
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->m_ProjectionMatrixParameter,&EM->GetProjectionMatrix().GetD3DXMatrix());
	}

	if(m_UseInverseProjMatrix)
	{
		const Mat44f &l_MatWorld=EM->GetViewMatrix();
		D3DXMATRIX l_D3DMatWorld=l_MatWorld.GetD3DXMatrix();
		D3DXMatrixInverse(&l_D3DMatWorld, NULL, &l_D3DMatWorld);
	}

  //Matrix World
	if(m_UseWorldMatrix)
	{
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->m_WorldMatrixParameter,&EM->GetWorldMatrix().GetD3DXMatrix());
	}

	if(m_UseInverseWorldMatrix) 
	{
		D3DXMATRIX l_WorldMatrix=EM->GetWorldMatrix().GetD3DXMatrix();
		D3DXMatrixInverse(&l_WorldMatrix, NULL, &l_WorldMatrix);
	}

	//Matrix View
  if(m_UseViewMatrix)    
	{
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->m_ViewMatrixParameter,&EM->GetViewMatrix().GetD3DXMatrix());
  } 

	if(m_UseInverseViewMatrix) 
	{
		D3DXMATRIX l_ViewMatrix=EM->GetViewMatrix().GetD3DXMatrix();
		D3DXMatrixInverse(&l_ViewMatrix, NULL, &l_ViewMatrix);
	}
  //Matrix WorldView
	if(m_UseWorldViewMatrix)
	{
		D3DXMATRIX l_WorldMatrix=EM->GetWorldMatrix().GetD3DXMatrix();
		D3DXMATRIX l_ViewMatrix=EM->GetViewMatrix().GetD3DXMatrix();
		D3DXMATRIX l_Worldview=l_WorldMatrix*l_ViewMatrix;
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->m_WorldViewMatrixParameter,&l_Worldview);
	}
  //Matrix ViewProjection
	if(m_UseViewProjectionMatrix)
	{
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->m_ViewProjectionMatrixParameter,&EM->GetViewProjectionMatrix().GetD3DXMatrix());
	}
  //Matrix WorldViewProjection
	if(m_UseWorldViewProjectionMatrix) 
	{
		D3DXMATRIX l_World=EM->GetWorldMatrix().GetD3DXMatrix();
		D3DXMATRIX l_View=EM->GetViewMatrix().GetD3DXMatrix();
		D3DXMATRIX l_Projection=EM->GetProjectionMatrix().GetD3DXMatrix();
		D3DXMATRIX l_WorldViewProjection=l_World*l_View*l_Projection;
		m_Effect->GetD3DEffect()->SetMatrix(m_Effect->m_WorldViewProjectionMatrixParameter,&l_WorldViewProjection);
	}
  
	//Light
  if(m_UseViewToLightProjectionMatrix) 
  {
	  D3DXMATRIX l_ViewToLightProjectionMatrix=EM->GetViewMatrix().GetD3DXMatrix();
	  D3DXMatrixInverse(&l_ViewToLightProjectionMatrix,NULL,&l_ViewToLightProjectionMatrix);
	  //l_ViewToLightProjectionMatrix.Invert();
	  l_ViewToLightProjectionMatrix=l_ViewToLightProjectionMatrix*EM->GetLightViewMatrix().GetD3DXMatrix();
	  l_ViewToLightProjectionMatrix=l_ViewToLightProjectionMatrix*EM->GetShadowProjectionMatrix().GetD3DXMatrix();
	  m_Effect->GetD3DEffect()->SetMatrix(m_Effect->m_ViewToLightProjectionMatrixParameter, &l_ViewToLightProjectionMatrix);
	}

	if(m_UseTime)
  {
    CTimer *t = CORE->GetTimer();
    m_Effect->GetD3DEffect()->SetFloat(m_Effect->m_TimeParameter, (float)t->GetTotalTime());
  }

	if(m_UseSelfIlluminationColor) 
	{
    const float l_fSelfIlluminationColor[3] = {(*_pV3fSelfIlluminationColor).x, (*_pV3fSelfIlluminationColor).y, (*_pV3fSelfIlluminationColor).z};
    m_Effect->GetD3DEffect()->SetFloatArray(m_Effect->m_SelfIlluminationColorParameter, (float*)l_fSelfIlluminationColor, 3);
	}

	//if(m_UseInstancingPos)
	//{
	//}

	//if(m_UseLightAmbientColor) 
	//{
    
	//}
  return true;
}


///<summary>
/// Recogerá la technique del efecto según el nombre de ésta
///</summary>
///<returns name="varBool">Indica si Refresh() se realizó con éxito</returns>
bool CEffectTechnique::Refresh()
{
	m_D3DTechnique=NULL;
	m_Effect=CORE->GetEffectManager()->GetEffect(m_EffectName);
	m_D3DTechnique=m_Effect->GetTechniqueByName(m_TechniqueName.c_str());

	return m_D3DTechnique!=NULL;
}