#include "RenderableAnimatedInstanceModel.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "AnimationModel/AnimatedCoreModel.h"
#include "Math/Matrix44.h"
#include "RenderManager.h"

#if defined( _DEBUG )
  #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CRenderableAnimatedInstanceModel::CRenderableAnimatedInstanceModel(const std::string &Name, const std::string &CoreName)
{
  m_AnimatedInstanceModel = NULL;
  m_bIsOk = true;
  CAnimatedCoreModel * l_AnimatedCoreModel = CORE->GetAnimatedModelManager()->GetResource(CoreName);

  if (l_AnimatedCoreModel == NULL)
  {
    m_bIsOk = false;
  }
  else
  {
    m_AnimatedInstanceModel = new CAnimatedInstanceModel();
    m_AnimatedInstanceModel->Initialize(l_AnimatedCoreModel);
    m_AnimatedInstanceModel->InitD3D(CORE->GetRenderManager());
    
    SetName(Name);
    SetPitch(0.0);
    SetPosition(0.0);
    SetRoll(0.0);
    SetVisible(false);
    SetYaw(0.0);
  }
}

CRenderableAnimatedInstanceModel::~CRenderableAnimatedInstanceModel()
{
   CHECKED_DELETE(m_AnimatedInstanceModel);
}

bool CRenderableAnimatedInstanceModel::Init()
{
  return IsOk();
}

void CRenderableAnimatedInstanceModel::Done()
{
}

void CRenderableAnimatedInstanceModel::Render(CRenderManager *RM)
{
  if(GetVisible())
  {
    Mat44f l_Transform = GetTransform();
    RM->SetTransform(l_Transform);
    m_AnimatedInstanceModel->Render(RM);
  }
}

void CRenderableAnimatedInstanceModel::Update(float ElapsedTime)
{
  m_AnimatedInstanceModel->Update(ElapsedTime);
}

/*void CRenderableAnimatedInstanceModel::ExecuteAction(int Id, float Time)
{
  m_AnimatedInstanceModel->ExecuteAction(Id, Time);
}*/



