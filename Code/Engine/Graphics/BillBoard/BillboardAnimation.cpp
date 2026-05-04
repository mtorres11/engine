#include "BillboardAnimation.h"
#include "Vertex/VertexType.h"
#include "Core.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraManager.h"
#include "Math/Matrix33.h"
#include "Texture/TextureManager.h"
#include "Texture/Texture.h"
#include "Timer/Timer.h"
#include "RenderManager.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CBillboardAnimation::CBillboardAnimation()
: m_vbD3DBillboardAnimation(NULL)
, m_pd3dDevice(NULL)
, m_iTextureAct(0)
, m_fTime(0.0f)
, m_fTimePerImage(0.4f)
, m_bLoop(true)
, m_iNumTextures(0)
, m_bEnd(false)
, m_fSizeX(1.0f)
, m_fSizeY (1.0f)
, m_Position (0.0f, 0.0f, 0.0f)
{}

CBillboardAnimation::~CBillboardAnimation()
{
  CHECKED_RELEASE(m_vbD3DBillboardAnimation);
}


///<summary>
/// CBillboardAnimation:: Init : Inicialización. Crea el Vertex Buffer.
///</summary>
///<param name="rm" name2=textureFileName>El rm para crear el VB.</param>
///<returns name="void"></returns>
void CBillboardAnimation::Init(CRenderManager* rm)
{
	m_pd3dDevice = rm->GetDevice();
	HRESULT hr=S_FALSE;
	if(FAILED (hr = m_pd3dDevice->CreateVertexBuffer( NUMPARTICLES * sizeof(VERTEX_TEXTURED), 
																										D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
																										VERTEX_TEXTURED::GetFVFBillboard(),
																										D3DPOOL_DEFAULT, &m_vbD3DBillboardAnimation, NULL)))
	{
		LOGGER->AddNewLog(ELL_ERROR, "BillboardAnimation::Init ---Error al crear el vertex buffer---");
	}
}


///<summary>
/// CBillboardAnimation:: Update : Calcula la textura actual del billboard en cada Update
///</summary>
///<param name=elapsedTime>El elapsedTime para cada update.</param>
///<returns name="void"></returns>
void CBillboardAnimation::Update(float elapsedTime)
{	
	CCamera * camera = CORE->GetCameraManager()->GetCamera();
	Vect3f l_vectUp = camera->GetVecUp();
	Vect3f l_vDirection = camera->GetDirection();
	
	Vect3f l_vRight = l_vectUp^l_vDirection; 
	l_vRight.Normalize();
	m_PointA = m_Position - (l_vRight*m_fSizeX*0.5f) - (l_vectUp*m_fSizeY*0.5f);
	m_PointB = m_Position + (l_vRight*m_fSizeX*0.5f) - (l_vectUp*m_fSizeY*0.5f);
	m_PointC = m_Position - (l_vRight*m_fSizeX*0.5f) + (l_vectUp*m_fSizeY*0.5f);
	m_PointD = m_Position + (l_vRight*m_fSizeX*0.5f) + (l_vectUp*m_fSizeY*0.5f);
}

///<summary>
/// CBillboardAnimation:: Render : dibuja el billboard animado con la textura correspondiente
///</summary>
///<param name="rm" name2=vTextures>Llamamos al render del billboard con la textura actual.</param>
///<returns name="void"></returns>
void CBillboardAnimation::Render(const std::vector<LPDIRECT3DTEXTURE9> vTextures)
{
	LPDIRECT3DDEVICE9 m_pd3dDevice = CORE->GetRenderManager()->GetDevice();

	//PreRender
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	CTimer *t = CORE->GetTimer();
	float totalTime = t->GetElapsedTime();

	//calculamos la textura actual según el totalTime
	m_fTime += totalTime;
	if(m_fTime > m_fTimePerImage)
	{
		//para q solo se haga una vez
		if( m_bLoop == false && m_iTextureAct+1 == m_iNumTextures )
		{
			m_bEnd = true;
		}
		else if (m_iTextureAct == m_iNumTextures)
		{
			m_iTextureAct = 0;
		}
		m_iTextureAct = (m_iTextureAct+1) % m_iNumTextures;
		m_fTime=0;
	}

	//calculamos la textura actual según el elapsedTime
	LPDIRECT3DTEXTURE9 texture = vTextures[m_iTextureAct];

	VERTEX_TEXTURED l_Points[4];
	unsigned short l_Indexes[6]={0,2,1,1,2,3};

	l_Points[0].x=m_PointA.x;
	l_Points[0].y=m_PointA.y;
	l_Points[0].z=m_PointA.z;
	l_Points[0].u=0.0f;
	l_Points[0].v=1.0f;
	l_Points[0].color=m_Color;
	
	l_Points[1].x=m_PointB.x;
	l_Points[1].y=m_PointB.y;
	l_Points[1].z=m_PointB.z;
	l_Points[1].u=1.0f;
	l_Points[1].v=1.0f;
	l_Points[1].color=m_Color;

	l_Points[2].x=m_PointC.x;
	l_Points[2].y=m_PointC.y;
	l_Points[2].z=m_PointC.z;
	l_Points[2].u=0.0f;
	l_Points[2].v=0.0f;
	l_Points[2].color=m_Color;

	l_Points[3].x=m_PointD.x;
	l_Points[3].y=m_PointD.y;
	l_Points[3].z=m_PointD.z;
	l_Points[3].u=1.0f;
	l_Points[3].v=0.0f;		
	l_Points[3].color=m_Color;


	m_pd3dDevice->SetTexture(0,texture);
	m_pd3dDevice->SetFVF(D3DFVF_VERTEX_TEXTURED);
	m_pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(VERTEX_TEXTURED));

	//PostRender
	m_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//end PostRender
}


void CBillboardAnimation::SetVTextures (std::vector<LPDIRECT3DTEXTURE9>	vD3DTextures)
{
		m_vTexturesTest = vD3DTextures;
}
