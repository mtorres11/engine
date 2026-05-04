#include "Process.h"
#include "Core.h"
#include "InputManager.h"
#include "ActionToInput.h"
#include "FontManager/FontManager.h"
#include "Cameras/Camera.h"
#include "Object3D/Object3D.h"
#include "Shaders/SceneEffectManager.h"
#include "Timer/Timer.h"
#include "Utils/LogRender.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

void CProcess::Init()
{
  
}
void CProcess::Update(float elapsedTime) 
{
}
void CProcess::Render(CRenderManager * rm)
{

}
void CProcess::RenderScene(CRenderManager * rm)
{

}
void CProcess::RenderDebugInfo()
{
  CFontManager* fm = CORE->GetFontManager();
  CTimer *t = CORE->GetTimer();
 
  uint32 l_uiFontId = fm->GetTTF_Id("arial");
  //fm->DrawTextA (10,130,colWHITE,l_uiFontId,"Elapsed Time: %f",t->GetElapsedTime());
  fm->DrawTextA (10, 20, colWHITE, l_uiFontId, "FPS: %f", t->GetFPS());
  //fm->DrawTextA (10,170,colWHITE,l_uiFontId,"Total Processing Time: %f",t->GetTotalTime());
}

void CProcess::PreRender(CRenderManager* rm) 
{
//	m_SceneEffectManager->PreRender(rm,this);
}