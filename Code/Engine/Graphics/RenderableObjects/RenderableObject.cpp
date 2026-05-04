#include "RenderableObject.h"
#include "RenderManager.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool  CRenderableObject::Init()
{
  m_bIsOk = true;

  if (!m_bIsOk){
    Release();
  }

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CRenderableObject::Done()
{
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CRenderableObject::Release()
{
//free memory
}

void CRenderableObject::Update(float ElapsedTime) 
{

}
void CRenderableObject::Render(CRenderManager *RM)
{

}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------