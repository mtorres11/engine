#include "AnimatedInstanceModel.h"
#include "Base.h"
#include "AnimatedCoreModel.h"
#include "Vertex/VertexType.h"
#include "AnimatedInstanceModel.h"
#include "RenderManager.h"
#include "Shaders/EffectTechnique.h"
#include "Shaders/EffectManager.h"
#include "Shaders/Effect.h"
#include "Texture/TextureManager.h"
#include "Texture/Texture.h"

#if defined( _DEBUG )
  #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CAnimatedInstanceModel:: Constructor
///</summary>
CAnimatedInstanceModel::CAnimatedInstanceModel()
: m_CalModel(NULL)
, m_AnimatedCoreModel(NULL)
, m_CurrentCycle(-1)
, m_CurrentAnimationId(-1)
, m_CurrentAnimationTime(0.0f)
, m_pVB(NULL)
, m_pIB(NULL)
{
}

///<summary>
/// CAnimatedInstanceModel:: Destructor
///</summary>
CAnimatedInstanceModel::~CAnimatedInstanceModel()
{
	Destroy();
}

///<summary>
/// CAnimatedInstanceModel:: Render : Llamada al render del model por software
///</summary>
///<param name="RM"></param>
void CAnimatedInstanceModel::Render(CRenderManager *RM)
{
  //RenderModelBySoftware(RM);
	RenderModelByHardware(RM);
}

///<summary>
/// CAnimatedInstanceModel:: RenderModelBySoftware : Render del modelo en el software 
///</summary>
///<param name="RM"></param>
void CAnimatedInstanceModel::RenderModelBySoftware(CRenderManager *RM)
{
  //leer renderModel
  LPDIRECT3DDEVICE9 l_pDevice = RM->GetDevice();
  
  // get the renderer of the model
  CalRenderer * pCalRenderer;
  pCalRenderer = m_CalModel->getRenderer();

  // begin the rendering loop
  if(!pCalRenderer->beginRendering()) return;

  int l_iVBCursor=0;
  int l_iIBCursor=0;

  DWORD dwVBLockFlags=D3DLOCK_NOOVERWRITE;
  DWORD dwIBLockFlags=D3DLOCK_NOOVERWRITE;

  l_pDevice->SetStreamSource( 0, m_pVB, 0,  sizeof(TNORMAL_TEXTURE1_VERTEX) );
  l_pDevice->SetFVF(TNORMAL_TEXTURE1_VERTEX::GetFVF());
  l_pDevice->SetIndices(m_pIB);
  
  // get the number of meshes
  int l_iMeshCount;
  l_iMeshCount = pCalRenderer->getMeshCount();

  // render all meshes of the model
  int l_iMeshId;
  for(l_iMeshId = 0; l_iMeshId < l_iMeshCount; l_iMeshId++)
  {
    // get the number of submeshes
    int l_iSubmeshCount;
    l_iSubmeshCount = pCalRenderer->getSubmeshCount(l_iMeshId);

    // render all submeshes of the mesh
    int l_iSubmeshId;
    for(l_iSubmeshId = 0; l_iSubmeshId < l_iSubmeshCount; l_iSubmeshId++)
    {
      // select mesh and submesh for further data access
      if(pCalRenderer->selectMeshSubmesh(l_iMeshId, l_iSubmeshId))
      {
		   
		    // Get vertexbuffer from the model		  		  
		    TNORMAL_TEXTURE1_VERTEX* l_pVertices;

		    m_pVB->Lock(l_iVBCursor*sizeof(TNORMAL_TEXTURE1_VERTEX), pCalRenderer->getVertexCount()*sizeof(TNORMAL_TEXTURE1_VERTEX), (void**)&l_pVertices, dwVBLockFlags);

        int l_iVertexCount = pCalRenderer->getVerticesNormalsAndTexCoords((float*)l_pVertices);
		    m_pVB->Unlock();
		  
		    CalIndex *meshFaces;

        int l_iFaceCount;

		    m_pIB->Lock(l_iIBCursor* 3*sizeof(CalIndex), pCalRenderer->getFaceCount()*3* sizeof(CalIndex), (void**)&meshFaces,dwIBLockFlags);

		    l_iFaceCount = pCalRenderer->getFaces(meshFaces);
		    m_pIB->Unlock();


        l_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
        l_pDevice->SetTexture(0,(LPDIRECT3DTEXTURE9)m_TextureList[0]);

		    l_pDevice->DrawIndexedPrimitive(
			    D3DPT_TRIANGLELIST,
			    l_iVBCursor,
			    0,
			    l_iVertexCount,
			    l_iIBCursor*3,
			    l_iFaceCount
			  );

		    l_iVBCursor+=l_iVertexCount;
		    l_iIBCursor+=l_iFaceCount;

		    dwIBLockFlags=D3DLOCK_NOOVERWRITE;
		    dwVBLockFlags=D3DLOCK_NOOVERWRITE;

      }
    }
  }
  // end the rendering
  pCalRenderer->endRendering();
}

///<summary>
/// CAnimatedInstanceModel:: Update
///</summary>
///<param name="ElapsedTime"></param>
void CAnimatedInstanceModel::Update(float ElapsedTime)
{
  m_CalModel->update(ElapsedTime);
  m_CurrentAnimationTime += ElapsedTime;
}

///<summary>
/// CAnimatedInstanceModel:: Initialize
///</summary>
///<param name="AnimatedCoreModel"></param>
void CAnimatedInstanceModel::Initialize(CAnimatedCoreModel *AnimatedCoreModel)
{
  m_AnimatedCoreModel = AnimatedCoreModel;
  m_iNumAnimations = m_AnimatedCoreModel->GetNumAnimations();

  m_CalModel = new CalModel(m_AnimatedCoreModel->GetCoreModel());
  int meshId;
  for(meshId = 0; meshId < AnimatedCoreModel->GetCoreModel()->getCoreMeshCount(); meshId++)
  {
    m_CalModel->attachMesh(meshId);
  }
  
  LoadTextures();
  
  BlendCycle(1, 0.0f);
	
	CEffectManager *l_EffectManager=CORE->GetEffectManager();
	std::string &l_EffectTechniqueName=l_EffectManager->GetTechniqueEffectNameByVertexDefault(CAL3D_HW_VERTEX::GetVertexType());
	const std::string &l_ModelName= m_AnimatedCoreModel->GetAnimatedCoreModelName();

	if(l_ModelName=="bot")
	{
		m_EffectTechnique=l_EffectManager->GetEffectTechnique(m_AnimatedCoreModel->GetTechniqueName());
	}
	else
	{
		m_EffectTechnique=l_EffectManager->GetEffectTechnique(l_EffectTechniqueName);
	}
}

///<summary>
/// CAnimatedInstanceModel:: InitD3D
///</summary>
///<param name="RM"></param>
void CAnimatedInstanceModel::InitD3D(CRenderManager *RM)
{
  int l_Indx = 0;
  int l_Vrtx = 0;
  for(int i = 0; i < m_CalModel->getCoreModel()->getCoreMeshCount(); i++)
  {
    for(int j = 0; j < m_CalModel->getCoreModel()->getCoreMesh(i)->getCoreSubmeshCount(); j++)
    {
      l_Indx += m_CalModel->getCoreModel()->getCoreMesh(i)->getCoreSubmesh(j)->getFaceCount()*3;
      l_Vrtx += m_CalModel->getCoreModel()->getCoreMesh(i)->getCoreSubmesh(j)->getVertexCount();
    }
  }
}

///<summary>
/// CAnimatedInstanceModel:: Destroy
///</summary>
void CAnimatedInstanceModel::Destroy()
{
  CHECKED_DELETE(m_CalModel);
  CHECKED_RELEASE(m_pVB);
  CHECKED_RELEASE(m_pIB);
	m_TextureList.clear();
}

///<summary>
/// CAnimatedInstanceModel:: ExecuteAction
///</summary>
///<param name="Id"></param>
///<param name="Time"></param>
void CAnimatedInstanceModel::ExecuteAction(int Id, float Time)
{
  m_CurrentAnimationId = Id;
  m_CurrentAnimationTime = 0.0f;
  m_CalModel->getMixer()->executeAction(Id, 0.0f, Time);
}
void CAnimatedInstanceModel::ExecuteAction(int Id, float inTime, float outTime)
{
  m_CurrentAnimationId = Id;
  m_CurrentAnimationTime = 0.0f;
  m_CalModel->getMixer()->executeAction(Id, inTime, outTime);
}

///<summary>
/// CAnimatedInstanceModel:: ExecuteActionBlocked
///</summary>
///<param name="Id"></param>
///<param name="Time"></param>
void CAnimatedInstanceModel::ExecuteActionBlocked(int Id, float Time)
{
  m_CurrentAnimationId = Id;
  m_CurrentAnimationTime = 0.0f;
  m_CalModel->getMixer()->executeAction(Id, 0.0f, Time, 1.0f, true);
}

bool CAnimatedInstanceModel::RemoveAction(int Id)
{
  m_CurrentAnimationId = -1;
  m_CurrentAnimationTime = 0.0f;
  return m_CalModel->getMixer()->removeAction(Id);
}


///<summary>
/// CAnimatedInstanceModel:: BlendCycle
///</summary>
///<param name="Id"></param>
///<param name="Time"></param>
void CAnimatedInstanceModel::BlendCycle(int Id, float Time, float W)
{
	if(m_CurrentCycle!=-1)
		m_CalModel->getMixer()->clearCycle(m_CurrentCycle, Time);
  m_CurrentCycle = Id;
  m_CalModel->getMixer()->blendCycle(Id, W, Time);
}

///<summary>
/// CAnimatedInstanceModel:: ClearCycle
///</summary>
///<param name="Time"></param>
void CAnimatedInstanceModel::ClearCycle(float Time)
{
	if(m_CurrentCycle!=-1)
		m_CalModel->getMixer()->clearCycle(m_CurrentCycle, Time);
}

///<summary>
/// CAnimatedInstanceModel:: GetCurrentCycle
///</summary>
///<returns name="m_CurrentCycle"></returns>
int CAnimatedInstanceModel::GetCurrentCycle() const
{
  return m_CurrentCycle;
}

///<summary>
/// CAnimatedInstanceModel:: GetCurrentAction
///</summary>
///<param name="RM"></param>
int CAnimatedInstanceModel::GetCurrentAction() const
{
  return m_CurrentAnimationId;
}

///<summary>
/// CAnimatedInstanceModel:: LoadTextures
///</summary>
void CAnimatedInstanceModel::LoadTextures()
{
	m_TextureList.clear();
  LPDIRECT3DDEVICE9 l_device = CORE->GetRenderManager()->GetDevice();
	CTextureManager* l_TM = CORE->GetTextureManager();
  int count = m_AnimatedCoreModel->GetNumTextures();

  for(int i = 0; i < count; i++)
  {   	
    std::string l_sCoreTextureName = m_AnimatedCoreModel->GetTextureName(i);
		CTexture *myTexture = l_TM->GetTexture(l_sCoreTextureName);
		m_TextureList.push_back(myTexture);
  }
}

///<summary>
/// CAnimatedInstanceModel:: RenderModelByHardware
///</summary>
///<param name="RM"></param>
void CAnimatedInstanceModel::RenderModelByHardware(CRenderManager *RM)
{
	CEffectManager *l_EffectManager=CORE->GetEffectManager();
	CEffectTechnique *l_EffectTechnique = l_EffectManager->GetAnimatedModelTechnique();
	if(l_EffectTechnique==NULL)
	{
		l_EffectTechnique=m_EffectTechnique;
	}
	if(l_EffectTechnique==NULL)
	{
		return;
	}
	//l_EffectManager->SetWorldMatrix(GetTransform());
	CEffect *m_Effect=l_EffectTechnique->GetEffect();
	if(m_Effect==NULL)
	{
		return;
	}
	LPD3DXEFFECT l_Effect=m_Effect->GetD3DEffect();
	if(l_Effect)
	{
		l_EffectTechnique->BeginRender();
		CalHardwareModel *l_CalHardwareModel=m_AnimatedCoreModel->GetCalHardwareModel();

		D3DXMATRIX transformation[MAXBONES];
		for(int hardwareMeshId=0;hardwareMeshId<l_CalHardwareModel->getHardwareMeshCount(); hardwareMeshId++)
		{
			l_CalHardwareModel->selectHardwareMesh(hardwareMeshId);
			for(int boneId = 0; boneId < l_CalHardwareModel->getBoneCount(); boneId++)
			{
				D3DXMatrixRotationQuaternion(&transformation[boneId],(D3DXQUATERNION*)&l_CalHardwareModel->getRotationBoneSpace(boneId, m_CalModel->getSkeleton()));
				CalVector translationBoneSpace =l_CalHardwareModel->getTranslationBoneSpace(boneId,m_CalModel->getSkeleton());
				transformation[boneId]._14 =translationBoneSpace.x;
				transformation[boneId]._24 =translationBoneSpace.y;
				transformation[boneId]._34 =translationBoneSpace.z;
			}
			float l_Matrix[MAXBONES*3*4];
			
			for(int b=0;b<l_CalHardwareModel->getBoneCount();++b)
			{
				memcpy(&l_Matrix[b*3*4], &transformation[b], sizeof(float)*3*4);
			}
			l_Effect->SetFloatArray(m_Effect->m_BonesParameter,(float *)l_Matrix,(l_CalHardwareModel->getBoneCount())*3*4);
			//m_TextureList[1]->Activate(0);
			
      m_TextureList[hardwareMeshId]->Activate(0);
			//m_NormalTextureList[0]->Activate(1);

			const std::string &l_EffectTechniqueName=l_EffectManager->GetTechniqueEffectNameByVertexDefault(CAL3D_HW_VERTEX::GetVertexType());

			if(m_AnimatedCoreModel->GetTechniqueName()=="Cal3DInstancingTechnique")
			{
				m_EffectTechnique=l_EffectManager->GetEffectTechnique(m_AnimatedCoreModel->GetTechniqueName());

				
				//ShaderInstancing
				 m_AnimatedCoreModel->GetRenderableVertexs()->Render( RM, l_EffectTechnique, 
																														  m_Effect->m_InstancesPositionParameter,
																															m_AnimatedCoreModel->GetVector(),
																															m_AnimatedCoreModel->GetInstanceYaw(),
																															0,0,-1,0,-1);
			}
			else
			{
				m_EffectTechnique=l_EffectManager->GetEffectTechnique(l_EffectTechniqueName);
				m_AnimatedCoreModel->GetRenderableVertexs()->Render(RM, l_EffectTechnique, 
																													l_CalHardwareModel->getBaseVertexIndex(), 0, 
																													l_CalHardwareModel->getVertexCount(), 
																													l_CalHardwareModel->getStartIndex(),
																													l_CalHardwareModel->getFaceCount());
			}
		}
	}
}