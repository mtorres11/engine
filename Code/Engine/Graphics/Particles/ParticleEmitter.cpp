#include "ParticleEmitter.h"
#include "Vertex/VertexType.h"
#include "Logger/Logger.h"
#include "Math/Random.h"
#include "Texture/TextureManager.h"
#include "Texture/Texture.h"
#include "RenderManager.h"
#include "Particles/Particle.h"
#include "XML/XMLTreeNode.h"
#include "Timer/Timer.h"
#include <string>
#include "ParticleManager.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

///<summary>
/// CParticleEmitter:: Constructor : Inicialización de las variables miembro.
///</summary>
CParticleEmitter::CParticleEmitter()
:m_Particles(NUMPARTICLES), 
 m_vPos(D3DXVECTOR3(0.0f,0.0f,0.0f)), 
 m_fMinEmitRate(10.0f),
 m_fMaxEmitRate(30.0f),
 m_Color1(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),
 m_Color2(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)),
 m_fMinSize(0.1f),
 m_fMaxSize(0.3f),
 m_vSpawnDir1(D3DXVECTOR3(0.0f,1.0f,0.0f)),
 m_vSpawnDir2(D3DXVECTOR3(10.0f,5.0f,10.0f)),
 m_fLifeMin(0.3f),
 m_fLifeMax(3.0f),
 m_bGravity(false),
 m_pd3dDevice(NULL),
 m_iVBSize(NUMPARTICLES/10),
 m_fNumNewPartsExcess(0.0f),
 m_vbParticles(NULL),
 m_fAngle(0.0f)
{
}

///<summary>
/// CParticleEmitter:: Init : Inicialización.
///</summary>
///<param name="void"></param>
///<returns name="result">Booleano que indica si la inicialización se ha realizado con éxito.</returns>
bool CParticleEmitter::Init ()
{
	m_bIsOk = true;
  m_pd3dDevice = CORE->GetRenderManager()->GetDevice();
	return m_bIsOk;
}

///<summary>
/// CParticleEmitter:: Done : Finalize data.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CParticleEmitter::Done()
{
  if (IsOk())
  {
    DeInit();
    m_bIsOk = false;
  }
}

///<summary>
/// CParticleEmitter:: Update : Actualiza cada partícula viva y crea partículas nuevas cuando toque.
///</summary>
///<param name="fElapsedTime">Tiempo transcurrido.</param>
///<returns name="void"></returns>
void CParticleEmitter::Update(float fElapsedTime)
{
	//Actualizar cada particula viva
	for (uint32 cont = 0; cont < m_Particles.GetTotalElements(); cont++)
	{
		if (!m_Particles.IsFree(cont))
		{		
			CParticle* par = m_Particles.GetAt(cont);
			if (!par->Update(fElapsedTime))
			{
				m_Particles.Free(cont);
			}
		}

		//¿Toca crear una nueva particula?
		float fEmitRateThisFrame = RandomNumber(m_fMinEmitRate, m_fMaxEmitRate);
		int iNumNewParts = (int) (fEmitRateThisFrame * fElapsedTime);
		m_fNumNewPartsExcess += (float)(fEmitRateThisFrame * fElapsedTime)-iNumNewParts;
		if (m_fNumNewPartsExcess > 1.0f)
		{
			iNumNewParts += (int)m_fNumNewPartsExcess;
			m_fNumNewPartsExcess -= (int)m_fNumNewPartsExcess;
		}
		for (int q=0; q < iNumNewParts; q++)
		{
			//Si hay espacio para una nueva partícula:
			if (m_Particles.GetNumFreeElements() > 0)
			{
				CParticle* part = m_Particles.New(); 

				//Calculamos la dirección, posición, color, tamaño, tiempo de vida y velocidad de la partícula
				part->SetTimeColor(m_vColorTime);
				part->SetColorVect(m_vectColor);

				part->SetTimeDir(m_vDirTime);
				part->SetDirVect(m_vectDir);

				float fRandPosX = RandomNumber(m_vPos1.x, m_vPos2.x);
				float fRandPosY = RandomNumber(m_vPos1.y, m_vPos2.y);
				float fRandPosZ = RandomNumber(m_vPos1.z, m_vPos2.z);
				part->SetPos(D3DXVECTOR3(fRandPosX, fRandPosY, fRandPosZ));

				part->SetTimeSize(m_vSizeTime);
				part->SetSizeVect(m_vectSize);

				float fRandLT = RandomNumber(m_fLifeMin, m_fLifeMax); 
				part->SetLifetime(fRandLT);

				part->SetGravity(m_bGravity);
				
				part->SetTimeAngle(m_vAngleTime);
				part->SetAngleVect(m_vectAngle);

				part->SetTimeVel(m_vVelocityTime);
				part->SetVelVect(m_vectVelocity);
			}
		}	
	}
}

///<summary>
/// CParticleEmitter:: Init : Inicialización. Crea el Vertex Buffer.
///</summary>
///<param name="rm" name2=textureFileName>El rm para crear el VB.</param>
///<returns name="void"></returns>
void CParticleEmitter::Init(CRenderManager* rm)
{
	//Eliminar todas las posiciones del array:
	m_Particles.DeleteAllElements();

	//Crear el vertex buffer:
	m_pd3dDevice = rm->GetDevice();
	
	HRESULT hr=S_FALSE;

	//billboard
	if(FAILED (hr = m_pd3dDevice->CreateVertexBuffer(	NUMPARTICLES * sizeof(VERTEX_TEXTURED), 
																										D3DUSAGE_DYNAMIC|D3DUSAGE_WRITEONLY|D3DUSAGE_POINTS,
																										VERTEX_TEXTURED::GetFVFBillboard(),
																										D3DPOOL_DEFAULT, &m_vbParticles, NULL)))
	{
		LOGGER->AddNewLog(ELL_ERROR, "ParticleEmitter::Init ---Error al crear el vertex buffer---");
	}
}

///<summary>
/// CParticleEmitter:: DeInit : Liberación de memoria.
///</summary>
///<param name="void"></param>
///<returns name="void"></returns>
void CParticleEmitter::DeInit()
{
	m_Particles.DeleteAllElements();
	if (m_vbParticles!=NULL)
	{
		m_vbParticles->Release();
	}

	m_vectColor.clear();
	m_vColorTime.clear();
	m_vectDir.clear();
	m_vDirTime.clear();
	m_vSizeTime.clear();
	m_vectSize.clear();
}

///<summary>
/// CParticleEmitter:: Render : Dibujado de las partículas.
///</summary>
///<param name="void"></param>
///<returns name="result">Booleano que indica si el render se ha realizado con éxito.</returns>
bool CParticleEmitter::Render()
{
	//PreRender
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
  m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//niebla
	//float fInicioNiebla = 2.0f;
	//float fFinNiebla = 10.0f;
	////float fDensidadNiebla = 0.1f;

	//m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE,true);
	//m_pd3dDevice->SetRenderState(D3DRS_RANGEFOGENABLE,true);
	//m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR,D3DCOLOR_XRGB(128,128,128));
	//m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_LINEAR);
	//m_pd3dDevice->SetRenderState(D3DRS_FOGSTART,*(DWORD*)(&fInicioNiebla));
	//m_pd3dDevice->SetRenderState(D3DRS_FOGEND,*(DWORD*)(&fFinNiebla));
	//m_pd3dDevice->SetRenderState(D3DRS_FOGCOLOR,D3DCOLOR_XRGB(255,255,255));
	//m_pd3dDevice->SetRenderState(D3DRS_FOGVERTEXMODE,D3DFOG_EXP2);
	//m_pd3dDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&fDensidadNiebla));

	m_pd3dDevice->SetTexture(0, m_texParticle);

	//DWORD dwNumParticlesToRender = 0;
	for (int q=0; q < NUMPARTICLES; q++)
	{
		if (!m_Particles.IsFree(q)) {

			CParticle* part = m_Particles.GetAt(q);
			//Billboard
			m_Billboard.SetPointA(part->GetPointA());
			m_Billboard.SetPointB(part->GetPointB());
			m_Billboard.SetPointC(part->GetPointC());
			m_Billboard.SetPointD(part->GetPointD());
			m_Billboard.SetSize(part->GetSize(), part->GetSize());
			m_Billboard.SetColor(part->GetColor());
			m_Billboard.Render(m_pd3dDevice, m_texParticle);
		}
	}

	//PostRender
	m_pd3dDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//end PostRender

	return true;
}

///<summary>
/// CParticleEmitter:: RandomNumber : Función para calcular el Random entre dos números de tipo float.
///</summary>
///<param name="fMin" name2="fMax"></param>
///<returns name="result">Número aleatorio calculado.</returns>
float CParticleEmitter::RandomNumber(float fMin, float fMax)
{
  float l_fNumber;
  if(fMin> fMax)
  {
    float l_fAux = fMax;
    fMax = fMin;
    fMin=l_fAux;
  }
  l_fNumber=((rand()/static_cast<float>(RAND_MAX)) * (fMax - fMin) + fMin);

  return l_fNumber;
}

void CParticleEmitter::SetTexture (CTexture* texture)
{
	m_texParticle = texture->GetD3DTexture();
}
