#include "DrawQuadSceneEffect.h"
#include "XML/XMLTreeNode.h"
#include "EffectTechnique.h"
#include "Effect.h"
#include "EffectManager.h"
#include "Texture/Texture.h"
#include "RenderManager.h"
//#include "InputManager.h"
//#include "ActionToInput.h"
//#include "Timer/Timer.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CDrawQuadSceneEffect:: Constructor
///</summary>
///<param name="atts">Gestiona la lectura y escritura en ficheros XML</param>
CDrawQuadSceneEffect::CDrawQuadSceneEffect(CXMLTreeNode &atts)
: CSceneEffect (atts)
, m_Technique (NULL)
, m_Color (NULL)
{
	//<post_render type="draw_quad" name="glow_post_effect" technique="RenderGlowPostFXTechnique" active="true" color="1.0 1.0 1.0 1.0">
	//  <texture stage_id="0" file="GlowTexture"/>
	//</post_render>

	CEffectManager *effm = CORE->GetEffectManager();
	//TODO.. no devuelve la effectTechnique
	m_Technique = effm->GetEffectTechnique(atts.GetPszProperty("technique"));
	Vect4f l_vColor = atts.GetVect4fProperty("color", Vect4f(0.f,0.f,0.f,0.f));
	m_Color.SetArgb(l_vColor);
}

///<summary>
/// CDrawQuadSceneEffect:: Destructor
///</summary>
CDrawQuadSceneEffect::~CDrawQuadSceneEffect()
{
//	CHECKED_DELETE(m_Technique);
}

///<summary>
/// CDrawQuadSceneEffect:: PostRender : renderizaremos un quad a tamaño completo con la textura y el color aplicado según la technique.
///</summary>
///<param name="RM">RenderManager</param>

void CDrawQuadSceneEffect::PostRender(CRenderManager *RM)
{
	if(m_Technique!=NULL)
	{
		RECT l_Rect;
		l_Rect.top=l_Rect.left=0;
    uint32 l_ScreenHeight,l_ScreenWidth;
    RM->GetWandH(l_ScreenWidth,l_ScreenHeight);
    l_Rect.bottom = l_ScreenHeight;
		l_Rect.right = l_ScreenWidth;
		m_Technique->BeginRender();
		ActivateTextures();

		LPD3DXEFFECT l_Effect = m_Technique->GetEffect()->GetD3DEffect();
		if(l_Effect!=NULL)
		{
			l_Effect->SetTechnique(m_Technique->GetD3DTechnique());
			UINT l_NumPasses;
			l_Effect->Begin(&l_NumPasses, 0);
			for (UINT iPass = 0; iPass < l_NumPasses; iPass++)
			{
				l_Effect->BeginPass(iPass);
				
				//RM->DrawColoredQuad2DTexturedInPixels( l_Rect, m_Color, m_StageTextures[0].m_Texture);
        RM->DrawTexturedQuad2D(Vect2i(0,0), l_ScreenWidth, l_ScreenHeight, CRenderManager::UPPER_LEFT,m_StageTextures[0].m_Texture, m_Color);

/*
        if(CORE->GetInputManager()->GetActionToInput()->DoAction("Screenshot") != 0.0f )
	      {
          char pathFoto[256];
          sprintf(pathFoto, "./Data/Screenshots/screen%d.bmp", (int) CORE->GetTimer()->GetTotalTime());
          this->m_StageTextures[0].m_Texture->Save(pathFoto);
	      }    

        
        std::string pathFoto = "C:\\texturasjuego\\postrend_zblur";
  pathFoto += ::timeGetTime();
  pathFoto += ".bmp";
  this->m_StageTextures[0].m_Texture->Save(pathFoto);
*/        l_Effect->EndPass();
			}
			l_Effect->End();
		}
	}
}