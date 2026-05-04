#include "RenderManager.h"
#include "Exceptions\Exception.h"
#include "Math\Matrix44.h"
#include "Cameras\Camera.h"
#include "Math/MathUtils.h"
#include "Object3D/Object3D.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "Shaders/EffectManager.h"
#include "Vertex/VertexType.h"
#include "texture/texture.h"
#include "InputManager.h"
#include "Cameras/CameraManager.h"
#include "Cameras/Camera.h"
#include "Cameras/Frustum.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )


#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
#define D3DFVF_CUSTOMVERTEX2 (D3DFVF_XYZ|D3DFVF_TEX1)

struct CUSTOMVERTEX
{
	float x, y, z;
	DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};
struct CUSTOMVERTEX2
{
	D3DXVECTOR3 pos;
	DWORD color;
	static unsigned int getFlags()
	{
		return D3DFVF_CUSTOMVERTEX;
	}
};

bool CRenderManager::Init(SInitParams _params, HWND hWnd)
{

	LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: Inicializando la libreria Direct3D");
	//CLogger * logger = CLogger::GetSingletonPtr();
	

	// Create the D3D object.
	m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
	
	m_bIsOk = m_pD3D != NULL;

	if (m_bIsOk) {
		// Set up the structure used to create the D3DDevice
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof(d3dpp) );

		if(_params.m_bFullScreen) {
      d3dpp.Windowed          = FALSE;
			d3dpp.BackBufferWidth   = _params.m_uWidthScreen;
			d3dpp.BackBufferHeight  = _params.m_uHeightScreen;
      d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		}
		else
		{
			d3dpp.Windowed          = TRUE;
      d3dpp.BackBufferFormat	= D3DFMT_UNKNOWN;
		}

		d3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D16;
		d3dpp.EnableAutoDepthStencil	= TRUE;
		d3dpp.AutoDepthStencilFormat	= D3DFMT_D24S8;
		d3dpp.Flags						= D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		d3dpp.PresentationInterval		= D3DPRESENT_INTERVAL_IMMEDIATE;

		// Create the D3DDevice
		m_bIsOk = !FAILED(	m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
												D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) );

		if (!m_bIsOk)
		{
			m_bIsOk = !FAILED(	m_pD3D->CreateDevice(	D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
													D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice ) );

			if (m_bIsOk)
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_SOFTWARE_VERTEXPROCESSING");
			}
		}
		else
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: D3DCREATE_HARDWARE_VERTEXPROCESSING");
		}

		if (m_bIsOk) {
			// Turn off culling, so we see the front and back of the triangle
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
			m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

			m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
			m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
			m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );


			m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
			m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
			m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

			// Turn off D3D lighting, since we are providing our own vertex colors
			m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

		
			if(_params.m_bFullScreen) {
				m_uWidth	= _params.m_uWidthScreen;
        m_uHeight	= _params.m_uHeightScreen;
			}
			else {
				GetWindowRect(hWnd);
			}
			LOGGER->AddNewLog(ELL_INFORMATION, "RenderManager:: La resolucion de pantalla es (%dx%d)",m_uWidth,m_uHeight);			
		}
	}

  if(m_bIsOk)
  {
    //CStaticMeshManager *sm = new CStaticMeshManager();
    //sm->Load("./Data/StaticMeshManager.xml");
  }

	TCOLORED_VERTEX::GetVertexDeclaration();
	TNORMAL_COLORED_VERTEX::GetVertexDeclaration();
	TNORMAL_COLORED_TEXTURE1_VERTEX::GetVertexDeclaration();
	TNORMAL_COLORED_TEXTURE2_VERTEX::GetVertexDeclaration();
	TTEXTURE1_VERTEX::GetVertexDeclaration();
	TTEXTURE2_VERTEX::GetVertexDeclaration();
	TNORMAL_TEXTURE1_VERTEX::GetVertexDeclaration();
	TNORMAL_TEXTURE2_VERTEX::GetVertexDeclaration();
	TNORMAL_TANGENT_BINORMAL_TEXTURED::GetVertexDeclaration();
	CAL3D_HW_VERTEX::GetVertexDeclaration();
  //VERTEX_INSTANCING_DATA::GetVertexType();

	if (!m_bIsOk) {
		std::string msg_error = "Rendermanager::Init-> Error al inicializar Direct3D";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		Relase();
		throw CException(__FILE__, __LINE__, msg_error);
	}
	
	return m_bIsOk;
}


void CRenderManager::GetWindowRect( HWND hwnd )
{
	RECT rec_window;
	GetClientRect(	hwnd, &rec_window);
	m_uWidth	= rec_window.right - rec_window.left;
	m_uHeight = rec_window.bottom - rec_window.top;
}


void CRenderManager::Relase(void)
{
	//Relase main devices of render

	TCOLORED_VERTEX::ReleaseVertexDeclaration();
	TNORMAL_COLORED_VERTEX::ReleaseVertexDeclaration();
	TNORMAL_COLORED_TEXTURE1_VERTEX::ReleaseVertexDeclaration();
	TNORMAL_COLORED_TEXTURE2_VERTEX::ReleaseVertexDeclaration();
	TTEXTURE1_VERTEX::ReleaseVertexDeclaration();
	TTEXTURE2_VERTEX::ReleaseVertexDeclaration();
	TNORMAL_TEXTURE1_VERTEX::ReleaseVertexDeclaration();
	TNORMAL_TEXTURE2_VERTEX::ReleaseVertexDeclaration();
	TNORMAL_TANGENT_BINORMAL_TEXTURED::ReleaseVertexDeclaration();
	CAL3D_HW_VERTEX::ReleaseVertexDeclaration();
	CHECKED_RELEASE(m_pD3DDevice);
	CHECKED_RELEASE(m_pD3D);
}


// RENDERING STUFF:
void CRenderManager::BeginRendering ()
{

#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
  uint32 red		= (uint32) (1.f * 255);
  uint32 green	= (uint32) (1.f * 0);
  uint32 blue		= (uint32) (1.f * 255);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#else // Clear the backbuffer to a black color in a Release mode
	uint32 red		= (uint32) (0.f * 255);
	uint32 green	= (uint32) (0.f * 255);
	uint32 blue		= (uint32) (0.f * 255);
	m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(red, green, blue), 1.0f, 0 );
#endif


	// Begin the scene
	HRESULT hr = m_pD3DDevice->BeginScene();
	assert( SUCCEEDED( hr ) );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );

	//if(m_bPaintSolid)
	if(true)
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
	}
	else
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME  );
	}
}

void CRenderManager::EndRendering    ()
{
	m_pD3DDevice->EndScene();
}
void CRenderManager::Present()
{
	// Present the backbuffer contents to the display
	m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}

void CRenderManager::SetupMatrices(CCamera* camera)
{
  D3DXMATRIX m_matView;
  D3DXMATRIX m_matProject;
  Vect3f eye;

  if(!camera)
  {
	  //Set default view and projection matrix

	  //Setup Matrix view
	  eye=Vect3f(0.0f,5.0f,-5.0f);
	  D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z), l_LookAt(0.0f,0.0f,0.0f), l_VUP(0.0f,1.0f,0.0f);
	  D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

	  //Setup Matrix projection
	  D3DXMatrixPerspectiveFovLH( &m_matProject, 45.0f * D3DX_PI / 180.0f, 1.0f, 1.0f, 100.0f );

  }
  else
  {
	  eye = camera->GetEye();
	  D3DXVECTOR3 l_Eye(eye.x, eye.y, eye.z);
	  Vect3f lookat = camera->GetLookAt();
	  D3DXVECTOR3 l_LookAt(lookat.x, lookat.y, lookat.z);
	  Vect3f vup = camera->GetVecUp();
	  D3DXVECTOR3 l_VUP(vup.x, vup.y, vup.z);
	  //Setup Matrix view
	  D3DXMatrixLookAtLH( &m_matView, &l_Eye, &l_LookAt, &l_VUP);

	  //Setup Matrix projection
	  D3DXMatrixPerspectiveFovLH(	&m_matProject, camera->GetFov(), camera->GetAspectRatio(),
		  camera->GetZn(), camera->GetZf());
  }

  //m_Frustum.Update( m_matView * m_matProject );
  m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
  m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

  CORE->GetEffectManager()->ActivateCamera(Mat44f(m_matView),Mat44f(m_matProject), eye);

  //TODO:
  //Revisar si realmente el objeto Frustum debe ir dentro de la camara
  //o si debe estar definido en otro lugar
  //Frustum
  //CORE->GetCameraManager()->GetCamera()->GetFrustum().Update(m_matView * m_matProject);
}

void CRenderManager::DrawLine ( const Vect3f &PosA, const Vect3f &PosB, CColor Color)
{
	DWORD color_aux = Color.GetUint32Argb();

	CUSTOMVERTEX v[2] =
	{
		{PosA.x, PosA.y, PosA.z, color_aux},
		{PosB.x, PosB.y, PosB.z, color_aux},
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST,1, v,sizeof(CUSTOMVERTEX));
}

void CRenderManager::DrawCube (float Size, CColor color)
{
	//suelo
	DrawLine(Vect3f(-Size*0.5f,-Size*0.5f,-Size*0.5f),    Vect3f(Size*0.5f,-Size*0.5f,-Size*0.5f),        color);
	DrawLine(Vect3f(-Size*0.5f,-Size*0.5f,Size*0.5f),    Vect3f(Size*0.5f,-Size*0.5f,Size*0.5f),        color);
	DrawLine(Vect3f(-Size*0.5f,-Size*0.5f,-Size*0.5f),    Vect3f(-Size*0.5f,-Size*0.5f,Size*0.5f),        color);
	DrawLine(Vect3f(Size*0.5f,-Size*0.5f,-Size*0.5f),    Vect3f(Size*0.5f,-Size*0.5f,Size*0.5f),        color);

	//tapa
	DrawLine(Vect3f(-Size*0.5f,Size*0.5f,-Size*0.5f),    Vect3f(Size*0.5f,Size*0.5f,-Size*0.5f),        color);
	DrawLine(Vect3f(-Size*0.5f,Size*0.5f,Size*0.5f),        Vect3f(Size*0.5f,Size*0.5f,Size*0.5f),        color);
	DrawLine(Vect3f(-Size*0.5f,Size*0.5f,-Size*0.5f),    Vect3f(-Size*0.5f,Size*0.5f,Size*0.5f),        color);
	DrawLine(Vect3f(Size*0.5f,Size*0.5f,-Size*0.5f),        Vect3f(Size*0.5f,Size*0.5f,Size*0.5f),        color);

	//lineas laterales
	DrawLine(Vect3f(-Size*0.5f,-Size*0.5f,-Size*0.5f),    Vect3f(-Size*0.5f,Size*0.5f,-Size*0.5f),        color);
	DrawLine(Vect3f(-Size*0.5f,-Size*0.5f,Size*0.5f),    Vect3f(-Size*0.5f,Size*0.5f,Size*0.5f),        color);
	DrawLine(Vect3f(Size*0.5f,-Size*0.5f,-Size*0.5f),    Vect3f(Size*0.5f,Size*0.5f,-Size*0.5f),        color);
	DrawLine(Vect3f(Size*0.5f,-Size*0.5f,Size*0.5f),        Vect3f(Size*0.5f,Size*0.5f,Size*0.5f),        color);
}

void CRenderManager::DrawCube (Vect3f Size, CColor color)
{
	//suelo
  DrawLine(Vect3f(-Size.x*0.5f,-Size.y*0.5f,-Size.z*0.5f),    Vect3f(Size.x*0.5f,-Size.y*0.5f,-Size.z*0.5f),        color);
	DrawLine(Vect3f(-Size.x*0.5f,-Size.y*0.5f,Size.z*0.5f),    Vect3f(Size.x*0.5f,-Size.y*0.5f,Size.z*0.5f),        color);
	DrawLine(Vect3f(-Size.x*0.5f,-Size.y*0.5f,-Size.z*0.5f),    Vect3f(-Size.x*0.5f,-Size.y*0.5f,Size.z*0.5f),        color);
	DrawLine(Vect3f(Size.x*0.5f,-Size.y*0.5f,-Size.z*0.5f),    Vect3f(Size.x*0.5f,-Size.y*0.5f,Size.z*0.5f),        color);

	//tapa
	DrawLine(Vect3f(-Size.x*0.5f,Size.y*0.5f,-Size.z*0.5f),    Vect3f(Size.x*0.5f,Size.y*0.5f,-Size.z*0.5f),        color);
	DrawLine(Vect3f(-Size.x*0.5f,Size.y*0.5f,Size.z*0.5f),        Vect3f(Size.x*0.5f,Size.y*0.5f,Size.z*0.5f),        color);
	DrawLine(Vect3f(-Size.x*0.5f,Size.y*0.5f,-Size.z*0.5f),    Vect3f(-Size.x*0.5f,Size.y*0.5f,Size.z*0.5f),        color);
	DrawLine(Vect3f(Size.x*0.5f,Size.y*0.5f,-Size.z*0.5f),        Vect3f(Size.x*0.5f,Size.y*0.5f,Size.z*0.5f),        color);

	//lineas laterales
	DrawLine(Vect3f(-Size.x*0.5f,-Size.y*0.5f,-Size.z*0.5f),    Vect3f(-Size.x*0.5f,Size.y*0.5f,-Size.z*0.5f),        color);
	DrawLine(Vect3f(-Size.x*0.5f,-Size.y*0.5f,Size.z*0.5f),    Vect3f(-Size.x*0.5f,Size.y*0.5f,Size.z*0.5f),        color);
	DrawLine(Vect3f(Size.x*0.5f,-Size.y*0.5f,-Size.z*0.5f),    Vect3f(Size.x*0.5f,Size.y*0.5f,-Size.z*0.5f),        color);
	DrawLine(Vect3f(Size.x*0.5f,-Size.y*0.5f,Size.z*0.5f),        Vect3f(Size.x*0.5f,Size.y*0.5f,Size.z*0.5f),        color);
}


void CRenderManager::DrawCamera (CCamera* camera)
{

	D3DXMATRIX matrix;
	D3DXMATRIX translation;
	if (camera->GetTypeCamera() == CCamera::TC_THPS)
	{
		Vect3f camera_CenterPos = camera->GetObject3D()->GetPosition();
		D3DXMatrixTranslation( &translation, camera_CenterPos.x ,camera_CenterPos.y ,camera_CenterPos.z );
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &translation );

		DrawCube(0.5f,colCYAN);
		D3DXMatrixTranslation( &matrix, 0, 0, 0 );
		m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
	}

	//---------PINTAMOS EL FRUSTUM-------------//
	D3DXMATRIX translation2;
	Vect3f eye_aux = camera->GetEye();
	D3DXVECTOR3 eye(eye_aux.x, eye_aux.y, eye_aux.z);
	D3DXMatrixTranslation( &translation, eye.x ,eye.y ,eye.z );

	D3DXMATRIX rotation;
	D3DXMATRIX rotation2;

	float yaw = camera->GetObject3D()->GetYaw();
	float pitch = camera->GetObject3D()->GetPitch();
	D3DXMatrixRotationY ( &rotation,  -yaw);
	D3DXMatrixRotationZ ( &rotation2, +pitch);

	matrix = rotation2 * rotation * translation; ;

	// Cambiar el sistema de coordenadas
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
	
	CUSTOMVERTEX2 pts[9];

	float fov = camera->GetFov();
	float aspect = camera->GetAspectRatio();
	float d = camera->GetViewD();
	float zNear = camera->GetZn();
	float zFar = camera->GetZf();



	float h_near = zNear * tan ( fov * 0.5f );
	float k_near = h_near * aspect;

	float h_far = d * tan ( fov * 0.5f );
	float k_far = h_far * aspect;

	pts[ 0 ].pos = D3DXVECTOR3( 0, 0,0 );
	pts[ 0 ].color = 0xffffffff;

	pts[ 1 ].pos = D3DXVECTOR3( d, h_far, k_far );
	pts[ 1 ].color = 0xffffffff;
	pts[ 2 ].pos = D3DXVECTOR3( d, h_far, -k_far );
	pts[ 2 ].color = 0xffffffff;
	pts[ 3 ].pos = D3DXVECTOR3( d,-h_far, -k_far );
	pts[ 3 ].color = 0xffffffff;
	pts[ 4 ].pos = D3DXVECTOR3( d, -h_far, k_far );
	pts[ 4 ].color = 0xffffffff;

	pts[ 5 ].pos = D3DXVECTOR3( zNear, h_near, k_near );
	pts[ 5 ].color = 0xffffffff;
	pts[ 6 ].pos = D3DXVECTOR3( zNear, h_near, -k_near );
	pts[ 6 ].color = 0xffffffff;
	pts[ 7 ].pos = D3DXVECTOR3( zNear,-h_near, -k_near );
	pts[ 7 ].color = 0xffffffff;
	pts[ 8 ].pos = D3DXVECTOR3( zNear, -h_near, k_near );
	pts[ 8 ].color = 0xffffffff;

	// Decimos el tipo de vertice que vamos a proporcionar...
	m_pD3DDevice->SetFVF( CUSTOMVERTEX2::getFlags() );

	// Desactivar la textura
	m_pD3DDevice->SetTexture (0, NULL);

	m_pD3DDevice->DrawPrimitiveUP( D3DPT_POINTLIST, 9, pts, sizeof( CUSTOMVERTEX2 ) );
	static short int indexes[] =  {
		0,1, 0,2, 0,3, 0,4,
		1,2, 2,3, 3,4, 4,1,
		5,6, 6,7, 7,8, 8,5
	};
	m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_LINELIST, 0, 9, 12, indexes, D3DFMT_INDEX16, pts, sizeof( CUSTOMVERTEX2 ) );

	D3DXMatrixTranslation( &matrix, 0, 0, 0 );
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );



	//---------PINTAMOS LA BOX Y LOS EJES------------------//
	D3DXMatrixTranslation( &translation2, -1.0f, 0.0f, 0.0f );
	matrix = translation2 * rotation2 * rotation * translation; 
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );

	CUSTOMVERTEX v[6] =
	{
		//EJE X
		{0, 0, 0, 0xffff0000},
		{3, 0, 0, 0xffff0000},
		//EJE Y
		{0, 0, 0, 0xff00ff00},
		{0, 3, 0, 0xff00ff00},
		//EJE Z
		{0, 0, 0, 0xff0000ff},
		{0, 0, 3, 0xff0000ff},
	};

	m_pD3DDevice->SetTexture(0,NULL);
	m_pD3DDevice->SetFVF(CUSTOMVERTEX::getFlags());
	m_pD3DDevice->DrawPrimitiveUP( D3DPT_LINELIST, 3, v,sizeof(CUSTOMVERTEX));

	DrawCube(0.5f, colWHITE);

	D3DXMatrixTranslation( &matrix, 0, 0, 0 );
	m_pD3DDevice->SetTransform( D3DTS_WORLD, &matrix );
}

void CRenderManager::SetTransform(D3DXMATRIX& matrix)
{
	assert(!"effect manager not implemented");
  m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);
}

void CRenderManager::SetTransform(Mat44f& m)
{
	CORE->GetEffectManager()->SetWorldMatrix(m);
	D3DXMATRIX aux(m.m00,m.m10,m.m20,m.m30,m.m01,m.m11,m.m21,m.m31,m.m02,m.m12,m.m22,m.m32,m.m03,m.m13,m.m23,m.m33);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &aux);
}

void CRenderManager::DrawSphere( float Radius, const CColor& Color, int Aristas)
{
	for(int t=0;t<Aristas;++t)
	{
		float l_RadiusRing=Radius*sin(mathUtils::Deg2Rad(180.0f*((float)t))/((float)Aristas));
		for(int b=0;b<Aristas;++b)
		{
			Vect3f l_PosA(	l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
											Radius*cos(mathUtils::Deg2Rad((180.0f*((float)t))/((float)Aristas))),
											l_RadiusRing*sin(mathUtils::Deg2Rad(((float)(360.0f*(float)b)/((float)Aristas)))));

			Vect3f l_PosB(	l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)(b+1))/((float)Aristas))),
											Radius*cos(mathUtils::Deg2Rad((180.0f*((float)t))/((float)Aristas))),
											l_RadiusRing*sin(mathUtils::Deg2Rad(((float)(360.0f*(float)(b+1))/((float)Aristas)))));

			DrawLine(l_PosA,l_PosB,Color);
			
			float l_RadiusNextRing=Radius*sin(mathUtils::Deg2Rad(180.0f*((float)(t+1)))/((float)Aristas));
			
			Vect3f l_PosC(	l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
											Radius*cos(mathUtils::Deg2Rad((180.0f*((float)t))/((float)Aristas))),
											l_RadiusRing*sin(mathUtils::Deg2Rad(((float)(360.0f*(float)b)/((float)Aristas)))));
			Vect3f l_PosD(	l_RadiusNextRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))),
											Radius*cos(mathUtils::Deg2Rad((180.0f*((float)(t+1)))/((float)Aristas))),
											l_RadiusNextRing*sin(mathUtils::Deg2Rad(((float)(360.0f*(float)b)/((float)Aristas)))));
			DrawLine(l_PosC,l_PosD,Color);
		}
	}
}

void CRenderManager::DrawGrid (float Size, CColor Color, int GridX, int32 GridZ )
{
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);		

	//GRID
	//LINEAS EN Z
	//....
	for(int i = (int)(-1*GridZ); i <= (int)(GridZ); i++){
		DrawLine(Vect3f((float)i,0,(-1*Size)),Vect3f((float)i,0,Size), Color);
	}
	//LINEAS EN X
	//....
	for(int i = (int)(-1*GridX); i <= (int)(GridX); i++){
		DrawLine(Vect3f((-1*Size),0,(float)i),Vect3f(Size,0,(float)i), Color);
	}
}

void CRenderManager::DrawAxis (float Size)
{
	/*D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);*/

	DrawLine(Vect3f(0.0f,0.0f,0.0f),Vect3f(Size,0.01f,0.0f),colRED);
  DrawLine(Vect3f(0.0f,0.0f,0.0f), Vect3f(0.0f,Size,0.0f),colGREEN);
	DrawLine(Vect3f(0.0f,0.0f,0.0f),Vect3f(0.0f,0.01f,Size),colBLUE);
}


struct SCREEN_COLOR_VERTEX
{
    float x,y,z,w;
    DWORD color;
    static unsigned int getFlags()
    {
        return D3DFVF_XYZRHW|D3DFVF_DIFFUSE;
    }
};

struct SCREEN_TEXTURED_COLOR_VERTEX
{
    float x,y,z,w;
    DWORD color;
    float u,v;
    static unsigned int getFlags()
    {
      return D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1;
    }
};


struct SCREEN_TEXTURE_VERTEX
{
   float x,y,z,w;
   float u,v;
   static unsigned int getFlags()
   {
       return (D3DFVF_XYZRHW|D3DFVF_TEX1);
   }
};

void CRenderManager::CalculateAlignment (uint32 w, uint32 h, ETypeAlignment alignment, Vect2i & finalPos)
{
    switch (alignment)
    {
      case CENTER:
      {
            finalPos.x -= (uint32)(w*0.5f);
            finalPos.y -= (uint32)(h*0.5f);
      }
      break;
  
      case UPPER_LEFT:
      {
        //Por defecto ya est alienado de esta manera :)
      }
      break;

      case UPPER_RIGHT:
      {
          finalPos.x -= w;
      }
      break;

      case LOWER_RIGHT:
      {
          finalPos.x -= w;
          finalPos.y -= h;
      }
      break;

      case LOWER_LEFT:
      {
         finalPos.y -= h;
      }
      break;

      default:

        {

          LOGGER->AddNewLog(ELL_ERROR, "RenderManager:: Se está intentado renderizar un quad2d con una alineacion desconocida");

        }

        break;

    }
}

void CRenderManager::DrawCircleV(float x, float y, float _radius,CColor _color = colWHITE)
{
	  Vect3f before;
	  Vect3f after;

	  before( mathUtils::Cos(0.f) * _radius + x, mathUtils::Sin(0.f) * _radius + y , 0.f );
		for(float i = 0.f; i <= e2PIf; i += 0.1f){
			after( mathUtils::Cos(i) * _radius + x, mathUtils::Sin(i) * _radius + y, 0.f ); 
			DrawLine( before,after,_color );
			before = after;
		}
		after( mathUtils::Cos(e2PIf) * _radius + x, mathUtils::Sin(e2PIf) * _radius + y, 0.f ); 
		DrawLine( before,after,_color );
		
}


void CRenderManager::DrawCircleH(float x, float y, float _radius,CColor _color = colWHITE)
{
	  Vect3f before;
	  Vect3f after;

	  before( ( mathUtils::Cos(0.f) * _radius) + x, 1.f, (mathUtils::Sin(0.f) * _radius) + y );
		for(float i = 0.f; i <= e2PIf; i += 0.1f){
			after( (mathUtils::Cos(i) * _radius) + x, 1.f ,(mathUtils::Sin(i) * _radius) + y); 
			DrawLine( before,after,_color );
			before = after;
		}
		after( (mathUtils::Cos(e2PIf) * _radius) + x,1.f , (mathUtils::Sin(e2PIf) * _radius) + y); 
		DrawLine( before,after,_color );
		
}


void CRenderManager::DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CColor color)
{
  Vect2i finalPos = pos;
  CalculateAlignment(w, h, alignment, finalPos);

  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]

  unsigned short indices[6]={0,2,1,1,2,3};
  SCREEN_COLOR_VERTEX v[4] =
  {
    { (float)finalPos.x,(float)finalPos.y,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) sup_esq.
    ,{ (float)finalPos.x,(float)finalPos.y+h,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) inf_esq.
    ,{ (float)finalPos.x+w,(float)finalPos.y,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) sup_dr.
    ,{ (float)finalPos.x+w,(float)finalPos.y+h,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) inf_dr.
  };

  m_pD3DDevice->SetFVF( SCREEN_COLOR_VERTEX::getFlags() );
  m_pD3DDevice->SetTexture(0, NULL);
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2,indices,D3DFMT_INDEX16, v, sizeof( SCREEN_COLOR_VERTEX ) );
}

void CRenderManager::DrawTexturedQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture *Texture, CColor color)
{
  Vect2i finalPos = pos;
  CalculateAlignment(w, h, alignment, finalPos);

  // finalPos = [0]
  //
  //  [0]------[2]
  //   |        |
  //   |        |
  //   |        |
  //  [1]------[3]

  unsigned short indices[6]={0,2,1,1,2,3};
  SCREEN_TEXTURED_COLOR_VERTEX v[4] =
  {
    { (float)finalPos.x,(float)finalPos.y,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()), 0.0f, 0.0f} //(x,y) sup_esq.
    ,{ (float)finalPos.x,(float)finalPos.y+h,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()), 0.0f, 1.0f} //(x,y) inf_esq.
    ,{ (float)finalPos.x+w,(float)finalPos.y,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()),1.0f, 0.0f} //(x,y) sup_dr.
    ,{ (float)finalPos.x+w,(float)finalPos.y+h,0,1, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha()),1.0f, 1.0f} //(x,y) inf_dr.
  };

  m_pD3DDevice->SetFVF( SCREEN_TEXTURED_COLOR_VERTEX::getFlags() );
  Texture->Activate(0);
  
  m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2,indices,D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURED_COLOR_VERTEX ) );
}



void CRenderManager::DrawQuad3D (const Vect3f& ul, const Vect3f& ur, const Vect3f& dl, const Vect3f& dr, CColor color)
{
	unsigned short indices[6]={0,2,1,1,2,3};
	CUSTOMVERTEX v[4] = 
	{
		{ ul.x, ul.y, ul.z, D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) up_left.
		, { dl.x, dl.y, dl.z,D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) down_left.
		, { ur.x, ur.y, ur.z,D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) up_right.
		, { dr.x, dr.y, dr.z,D3DCOLOR_COLORVALUE(color.GetRed(), color.GetGreen(), color.GetBlue(), color.GetAlpha())} //(x,y) down_right.
	};
	m_pD3DDevice->SetFVF( CUSTOMVERTEX::getFlags() );
	m_pD3DDevice->SetTexture(0, NULL);
	m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2,indices,D3DFMT_INDEX16, v, sizeof( CUSTOMVERTEX ) );
}

void CRenderManager::DrawQuad3D (const Vect3f& pos, const Vect3f& up, const Vect3f& right, float w, float h, CColor color)
{
	Vect3f upper_left, upper_right, down_right, down_left;
	upper_left = pos + up*h*0.5f - right*w*0.5f;
	upper_right = pos + up*h*0.5f + right*w*0.5f;
	down_left = pos - up*h*0.5f - right*w*0.5f;
	down_right = pos - up*h*0.5f + right*w*0.5f;
	DrawQuad3D (upper_left, upper_right, down_left, down_right, color);
}


          

void CRenderManager::DrawRectangle2D (const Vect2i& pos, uint32 w, uint32 h, CColor& backGroundColor,uint32 edge_w, uint32 edge_h, CColor& edgeColor)
{
	//Draw background quad2D:
	DrawQuad2D(pos, w, h, UPPER_LEFT, backGroundColor);
	//Draw the four edges:
	//2 Horizontal:
	Vect2i pos_aux = pos;
	pos_aux.y -= edge_h;
	DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);
	pos_aux = pos;
	pos_aux.y += h;
	DrawQuad2D(pos_aux, w, edge_h, UPPER_LEFT, edgeColor);
	//2 Vertical:
	pos_aux = pos;
	pos_aux.x -= edge_w;
	pos_aux.y -= edge_h;
	DrawQuad2D(pos_aux, edge_w, h + (2*edge_w), UPPER_LEFT, edgeColor);
	pos_aux.x = pos.x + w;
	DrawQuad2D(pos_aux, edge_w, h + (2*edge_w), UPPER_LEFT, edgeColor);   
}

void  CRenderManager::DrawQuad2D(const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, SRectangle2D& coordText)
{
   Vect2i finalPos = pos;
   CalculateAlignment(w, h, alignment,finalPos);
   

   // finalPos = [0]
   //
   //  [0]------[2]
   //   |        |
   //   |        |
   //   |        |
   //  [1]------[3]
   
   Vect2f coord_text[4];
   coord_text[0].x = coordText.m_vBottomLeft.x;    
   coord_text[0].y = coordText.m_vTopRight.y;
   coord_text[1]        = coordText.m_vBottomLeft;
   coord_text[2]        = coordText.m_vTopRight;
   coord_text[3].x = coordText.m_vTopRight.x;
   coord_text[3].y = coordText.m_vTopRight.y;
           
   unsigned short indices[6]={0,2,1,1,2,3};
   SCREEN_TEXTURE_VERTEX v[4] =
   {
           { (float)finalPos.x,        (float)finalPos.y,        0.f,1.f, coord_text[0].x,    coord_text[0].y} //(x,y) sup_esq.
       ,    { (float)finalPos.x,        (float)finalPos.y+h,    0.f,1.f, coord_text[1].x,    coord_text[1].y} //(x,y) inf_esq.
       , { (float)finalPos.x+w,    (float)finalPos.y,        0.f,1.f, coord_text[2].x,    coord_text[2].y} //(x,y) sup_dr.
       ,    { (float)finalPos.x+w,    (float)finalPos.y+h,    0.f,1.f, coord_text[3].x,    coord_text[3].y} //(x,y) inf_dr.
   };
 
   m_pD3DDevice->SetFVF( SCREEN_TEXTURE_VERTEX::getFlags() );
   m_pD3DDevice->SetTexture(0, texture->GetD3DTexture());
   m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2,indices,D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURE_VERTEX ) );
}

void CRenderManager::DrawQuad2D (const Vect2i& pos, uint32 w, uint32 h, ETypeAlignment alignment, CTexture* texture, ETypeFlip flip)
{
   Vect2i finalPos = pos;
   CalculateAlignment(w, h, alignment,finalPos);

   // finalPos = [0]
   //
   //  [0]------[2]
   //   |        |
   //   |        |
   //   |        |
   //  [1]------[3]
   
   Vect2f coord_text[4];
   uint32 w_aux, h_aux;
   switch(flip)
   {
   case NONE_FLIP:
       coord_text[0].x = 0.f;    coord_text[0].y = 0.f;
       coord_text[1].x = 0.f;    coord_text[1].y = 1.f;
       coord_text[2].x = 1.f;    coord_text[2].y = 0.f;
       coord_text[3].x = 1.f;    coord_text[3].y = 1.f;
       break;
   case FLIP_X:
       //Pintamos primero el quad de la izquierda:
       w_aux = (uint32)(w*0.5);
       DrawQuad2D(finalPos, w_aux, h, UPPER_LEFT, texture, NONE_FLIP);
       finalPos.x += w_aux;
       w = w - w_aux;
       coord_text[0].x = 0.99f;    coord_text[0].y = 0.f;
       coord_text[1].x = 0.99f;    coord_text[1].y = 1.f;
       coord_text[2].x = 0.f;        coord_text[2].y = 0.f;
       coord_text[3].x = 0.f;        coord_text[3].y = 1.f;
       break;
   case FLIP_Y:
       h_aux = (uint32)(h*0.5);
       DrawQuad2D(finalPos, w, h_aux, UPPER_LEFT, texture, NONE_FLIP);
       finalPos.y += h_aux + 1;
       h = h - h_aux -1;
       coord_text[0].x = 1.f;    coord_text[0].y = 0.f;
       coord_text[1].x = 1.f;    coord_text[1].y = 1.f;
       coord_text[2].x = 0.f;    coord_text[2].y = 0.f;
       coord_text[3].x = 0.f;    coord_text[3].y = 1.f;
       break;
   default:
       {
           LOGGER->AddNewLog(ELL_ERROR, "RenderManager:: Se está intentado renderizar un quad2d con un flip desconocido");
       }
       
   }
   
   
   unsigned short indices[6]={0,2,1,1,2,3};
   SCREEN_TEXTURE_VERTEX v[4] =
   {
           { (float)finalPos.x,        (float)finalPos.y,        0.f,1.f, coord_text[0].x,    coord_text[0].y} //(x,y) sup_esq.
       ,    { (float)finalPos.x,        (float)finalPos.y+h,    0.f,1.f, coord_text[1].x,    coord_text[1].y} //(x,y) inf_esq.
       , { (float)finalPos.x+w,    (float)finalPos.y,        0.f,1.f, coord_text[2].x,    coord_text[2].y} //(x,y) sup_dr.
       ,    { (float)finalPos.x+w,    (float)finalPos.y+h,    0.f,1.f, coord_text[3].x,    coord_text[3].y} //(x,y) inf_dr.
   };

   m_pD3DDevice->SetFVF( SCREEN_TEXTURE_VERTEX::getFlags() );
   m_pD3DDevice->SetTexture(0, texture->GetD3DTexture());
   m_pD3DDevice->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST,0, 4, 2,indices,D3DFMT_INDEX16, v, sizeof( SCREEN_TEXTURE_VERTEX ) );

}

void CRenderManager::EnableAlphaBlend ()
{
   m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
   // render el quad de difuminacion....
   m_pD3DDevice->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA  );
   m_pD3DDevice->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
   //// render el quad de difuminacion....
   m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
   m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
   m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
}

void CRenderManager::DisableAlphaBlend ()
{
   m_pD3DDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );    
}

void CRenderManager::DrawPlane(float size, const Vect3f& normal, float distance, CColor Color, int GridX, int GridZ )
{

	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);

	//Ax + By + Cz + D = 0
	//if (x,y) = (0,0) then z = -D/C
	//if (x,y) = (1,1) then z = (-D-A-B)/C
	//With two points we have a vector-->

	float A,B,C,D;
	A = normal.x;
	B = normal.y;
	C = normal.z;
	D = distance;
	//float pointAz;
	//float pointBz;

	Vect3f pointA, pointB;
	if( C!= 0)
	{
		pointA = Vect3f(0.f,0.f, -D/C);
		pointB = Vect3f(1.f,1.f, (D-A-B)/C);
	}
	else if( B!= 0)
	{
		pointA = Vect3f(0.f,-D/B, 0.f);
		pointB = Vect3f(1.f,(-D-A-C)/B,1.f);
	}
	else if( A != 0)
	{
		pointA = Vect3f(-D/A,0.f, 0.f);
		pointB = Vect3f((-D-B-C)/A,1.f,1.f);
	}
	else
	{
		//error.
	}


	Vect3f vectorA = pointB - pointA;
	vectorA.Normalize();
	Vect3f vectorB;
	vectorB = normal^vectorA;
	vectorB.Normalize();
	Vect3f initPoint = normal*distance;

	assert(GridX>0);
	assert(GridZ>0);
	//LINEAS EN Z
	Vect3f initPointA = initPoint - vectorB*size*0.5;
	for(int b=0;b<=GridX;++b)
	{        
		DrawLine(initPointA + vectorA*size*0.5, initPointA - vectorA*size*0.5, Color );

		initPointA += vectorB*size/(float)GridX;
	}
	initPointA = initPoint - vectorA*size*0.5;
	for(int b=0;b<=GridX;++b)
	{
		DrawLine(initPointA + vectorB*size*0.5, initPointA - vectorB*size*0.5, Color);
		initPointA += vectorA*size/(float)GridX;
	}
}

//void CRenderManager::DrawPlane (float Size, const Vect3f& normal, float distance,  CColor Color, int GridX, int32 GridZ, ETypeModePaint mode)
//{
//	 D3DXMATRIX matrix;
//   D3DXMatrixIdentity(&matrix);
//   m_pD3DDevice->SetTransform(D3DTS_WORLD, &matrix);
//
//   Ax + By + Cz + D = 0
//   if (x,y) = (0,0) then z = -D/C
//   if (x,y) = (1,1) then z = (-D-A-B)/C
//   With two points we have a vector-->
//
//   float A,B,C,D;
//   A = normal.x;
//   B = normal.y;
//   C = normal.z;
//   D = distance;
//
//   Vect3f pointA, pointB;
//   if( C!= 0)
//   {
//       pointA = Vect3f(0.f,0.f, -D/C);
//       pointB = Vect3f(1.f,1.f, (D-A-B)/C);
//   }
//   else if( B!= 0)
//   {
//       pointA = Vect3f(0.f,-D/B, 0.f);
//       pointB = Vect3f(1.f,(-D-A-C)/B,1.f);
//   }
//   else if( A != 0)
//   {
//       pointA = Vect3f(-D/A,0.f, 0.f);
//       pointB = Vect3f((-D-B-C)/A,1.f,1.f);
//   }
//   else
//   {
//       error.
//   }
//
//   Vect3f vectorA = pointB - pointA;
//   vectorA.Normalize();
//   Vect3f vectorB;
//   vectorB = vectorA ^ normal; //cross product
//   vectorB.Normalize();
//   Vect3f initPoint = normal*distance;
//
//   assert(GridX>0);
//   assert(GridZ>0);
//
//   if (mode != PAINT_WIREFRAME)
//   {
//       Vect3f initP = normal*(distance+0.1f);
//       DrawQuad3D(initP, vectorA, vectorB, Size, Size, Color);
//   }
//
//   if (mode != PAINT_SOLID)
//   {
//       if (mode == PAINT_BOTH)
//       {
//           si es solid las lineas las pintamos blancas:
//           Color = colWHITE;
//       }
//       LINEAS EN Z
//       Vect3f initPointA = initPoint - vectorB*Size*0.5;
//       for(int b=0;b<=GridX;++b)
//       {        
//           DrawLine(initPointA + vectorA*Size*0.5, initPointA - vectorA*Size*0.5, Color);
//           initPointA += vectorB*Size/(float)GridX;
//       }
//       initPointA = initPoint - vectorA*Size*0.5;
//       for(int b=0;b<=GridX;++b)
//       {
//           DrawLine(initPointA + vectorB*Size*0.5, initPointA - vectorB*Size*0.5, Color);
//           initPointA += vectorA*Size/(float)GridX;
//       }
//   }
//}
//
//void CRenderManager::DrawSphere (float Radius, CColor Color, uint32 Aristas,  ETypeModePaint mode,  EtypeSphere typeSphere)
//{
//	float l_fAngle = 180.f;
//	float l_fAngle2 = 180.f;
//
//	for(uint32 t=0;t<Aristas;++t)
//	{
//		float l_fLambda = ((float)t)/((float)Aristas);
//		float l_fLambda2 = ((float)(t+1))/((float)Aristas);
//		switch (typeSphere)
//		{
//		case  HALF_TOP:
//			l_fAngle = mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 90.f, l_fLambda));
//			l_fAngle2 = mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 90.f, l_fLambda2));
//			break;
//		case HALF_BOTTOM:
//			l_fAngle = mathUtils::Deg2Rad(mathUtils::Lerp(90.f, 180.f, l_fLambda));
//			l_fAngle2 = mathUtils::Deg2Rad(mathUtils::Lerp(90.f, 180.f, l_fLambda2));
//			break;
//		case COMPLETE:
//			l_fAngle = mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 180.f, l_fLambda));
//			l_fAngle2 = mathUtils::Deg2Rad(mathUtils::Lerp(0.f, 180.f, l_fLambda2));
//			break;
//		}
//
//		std::vector<Vect3f> l_ring;
//		float l_RadiusRing=Radius*sin(l_fAngle);
//		Vect3f l_PosAux = v3fZERO;
//		Vect3f l_PosAux2, l_PosAux3;
//		for(uint32 b=0;b<Aristas;++b)
//		{
//			Vect3f l_PosA(l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))), Radius*cos(l_fAngle), l_RadiusRing*sin(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))));
//			Vect3f l_PosB(l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)(b+1))/((float)Aristas))), Radius*cos(l_fAngle), l_RadiusRing*sin(mathUtils::Deg2Rad((float)(360.0f*(float)(b+1))/((float)Aristas))));
//  		float l_RadiusNextRing=Radius*sin(l_fAngle2);
//			Vect3f l_PosC(l_RadiusRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))), Radius*cos(l_fAngle), l_RadiusRing*sin(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))));
//			Vect3f l_PosD(l_RadiusNextRing*cos(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))), Radius*cos(l_fAngle2), l_RadiusNextRing*sin(mathUtils::Deg2Rad((float)(360.0f*(float)b)/((float)Aristas))));
//
//			if (mode != PAINT_WIREFRAME)
//			{
//				DrawTriangle3D(l_PosA, l_PosD, l_PosB, Color);
//				if(l_PosAux !=  v3fZERO )
//				{
//					DrawTriangle3D(l_PosA, l_PosAux, l_PosD,Color);
//				}
//				else
//				{
//					l_PosAux2 = l_PosA;
//					l_PosAux3 = l_PosD;
//				}
//
//				if(b == Aristas-1)
//				{
//					DrawTriangle3D(l_PosAux2, l_PosD, l_PosAux3,Color);
//				}
//				l_PosAux = l_PosD;
//			}
//
//			if( mode != PAINT_SOLID)
//			{
//				CColor color_aux = Color;
//				if (mode == PAINT_BOTH)
//				{
//					color_aux = colWHITE;
//				}
//				DrawLine(l_PosA,l_PosB,color_aux);
//				DrawLine(l_PosC,l_PosD,color_aux);
//			}
//		}
//	}
//}
