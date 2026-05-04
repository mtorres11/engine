#include "XML/XMLTreeNode.h"
#include "ShadowMapRenderToTextureSceneEffect.h"
#include "RenderToTextureSceneEffect.h"
#include "RenderManager.h"
#include "Process.h"
#include "Lights/Light.h"
#include "EffectManager.h"
#include "Math/MathUtils.h"
#include "Lights/DirectionalLight.h"
#include "Texture/Texture.h"
#include "Object3D/Object3D.h"
#include "Lights/LightManager.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraManager.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CShadowMapRenderToTextureSceneEffect::CShadowMapRenderToTextureSceneEffect(CXMLTreeNode &atts)
: CRenderToTextureSceneEffect (atts)
{
	//Inicialización de m_LightShadowCast
	std::string l_sLightShadowCast = atts.GetPszProperty("light_shadow_cast");;
	m_LightShadowCast = (CLight*)CORE->GetLigthManager()->GetResource(l_sLightShadowCast);

}

CShadowMapRenderToTextureSceneEffect::~CShadowMapRenderToTextureSceneEffect()
{
	//CHECKED_DELETE(m_LightShadowCast);
}

void CShadowMapRenderToTextureSceneEffect::PreRender(CRenderManager *RM, CProcess *Proc)
{
	//Render ShadowMap
	if(m_LightShadowCast!=NULL)
	{
		//Nos debemos guardar las matrices de View y de Proyección actualmente activas en el EffectManager
		const Vect3f &l_Position=m_LightShadowCast->GetPosition();
		D3DXVECTOR3 l_D3DPosition(l_Position.x,l_Position.y,l_Position.z);
		Vect3f l_VDir=((CDirectionalLight *)m_LightShadowCast)->GetDirection();
		D3DXVECTOR3 l_D3DVDir=D3DXVECTOR3(l_VDir.x,l_VDir.y,l_VDir.z);

		//Creamos las matrices de View y de Proyección según la luz
		CEffectManager *l_EffectManager=CORE->GetEffectManager();
		D3DXMATRIX l_LightViewMatrix, l_LightProjectionMatrix;
		D3DXVECTOR3 l_VUP=D3DXVECTOR3(0.0f,1.0f,0.0f);

		D3DXMatrixLookAtLH( &l_LightViewMatrix, &l_D3DPosition, &(l_D3DPosition+l_D3DVDir*10.0f), &l_VUP);

		//Setup Matrix projection
		D3DXMatrixPerspectiveFovLH( &l_LightProjectionMatrix, 60.0f*D3DX_PI/180.0f, 1.0f, 1.0f, 1000.0f );
		Mat44f l_Mat44fViewMatrix(l_LightViewMatrix), l_Mat44fProjectionMatrix(l_LightProjectionMatrix);
		l_EffectManager->ActivateCamera(l_Mat44fViewMatrix,l_Mat44fProjectionMatrix, l_Position);

		//La matriz Projection la crearemos mediante el Fov de la luz, Aspect ratio, near y far plane.
		CCamera *l_Camera = CORE->GetCameraManager()->GetCamera();
		//Mat44f LightProjectionMatrix = (Mat44f(l_Camera->GetFov(), l_Camera->GetAspectRatio(), l_Camera->GetZn(), l_Camera->GetZf()));
		l_EffectManager->SetShadowProjectionMatrix(l_Mat44fProjectionMatrix);
		//Mat44f LightViewMatrix = CORE->GetEffectManager()->GetLightViewMatrix();
		l_EffectManager->SetLightViewMatrix(l_Mat44fViewMatrix);
		m_Texture->SetAsRenderTarget();

		RM->BeginRendering();

		l_EffectManager->SetStaticMeshTechnique(m_StaticMeshTechnique);
		l_EffectManager->SetAnimatedModelTechnique(m_AnimatedModelTechnique);
		Proc->RenderScene(RM);

		RM->EndRendering();
		m_Texture->UnsetAsRenderTarget();
		//Debemos establecer las matrices de View y de Proyección guardadas previamente en el EffectManager
	}
	/*if(::GetAsyncKeyState(VK_F8)&0x8000)
		m_Texture->Save("D:/SHADOWMAP.BMP");*/
}
