#include "RenderToTextureSceneEffect.h"
#include "XML/XMLTreeNode.h"
#include "Cameras/Camera.h"
#include "Process.h"
#include "RenderManager.h"
#include "Effect.h"
#include "EffectManager.h"
#include "Object3D/Object3D.h"
#include "Texture/Texture.h"
#include "Texture/TextureManager.h"
#include "Timer/Timer.h"
#include "Cameras/CameraManager.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CRenderToTextureSceneEffect:: Constructor.
///</summary>
///<param name="atts">Gestiona la lectura y escritura de ficheros XML</param>
CRenderToTextureSceneEffect::CRenderToTextureSceneEffect(CXMLTreeNode &atts)
: CRenderTextureSceneEffect (atts)
, m_StaticMeshTechnique (NULL)
, m_AnimatedModelTechnique (NULL)
{
	std::string l_sStaticMeshTechnique = atts.GetPszProperty("static_mesh_technique");
	std::string l_sAnimatedModelTechnique = atts.GetPszProperty("animated_model_technique");

  m_StaticMeshTechnique = (CEffectTechnique*)CORE->GetEffectManager()->GetEffectTechnique(l_sStaticMeshTechnique);
  m_AnimatedModelTechnique = (CEffectTechnique*)CORE->GetEffectManager()->GetEffectTechnique(l_sAnimatedModelTechnique);

}

///<summary>
/// CRenderToTextureSceneEffect:: Destructor.
///</summary>
CRenderToTextureSceneEffect::~CRenderToTextureSceneEffect ()
{
	//CHECKED_DELETE(m_StaticMeshTechnique);
  //CHECKED_DELETE(m_AnimatedModelTechnique);
}

///<summary>
/// CRenderToTextureSceneEffect:: PreRender.
///</summary>
///<param name="Proc"></param>
///<param name="RM"></param>
void CRenderToTextureSceneEffect::PreRender(CRenderManager *RM, CProcess *Proc)
{
	//CCamera *l_Camera = Proc->GetCamera();
	CEffectManager *l_EffectManager=CORE->GetEffectManager();
	CObject3D *l_pObject = CORE->GetCameraManager()->GetCamera()->GetObject3D();
	l_EffectManager->ActivateCamera(l_EffectManager->GetViewMatrix(),l_EffectManager->GetProjectionMatrix(), l_pObject->GetPosition());
	m_Texture->SetAsRenderTarget();
	RM->BeginRendering();
	l_EffectManager->SetStaticMeshTechnique(m_StaticMeshTechnique);
	l_EffectManager->SetAnimatedModelTechnique(m_AnimatedModelTechnique);
	// TODO.. método Render3DScene()

  Proc->RenderScene(RM);
/*
  std::string  pathFoto = "C:\\texturasjuego\\prerend_zblur";
  pathFoto += ::timeGetTime();
  pathFoto += ".bmp";
  this->m_Texture->Save(pathFoto);
*/
  
  RM->EndRendering();


	m_Texture->UnsetAsRenderTarget();
}