#include "Billboard.h"
#include "Vertex/VertexType.h"
#include "Core.h"
#include "Cameras/Camera.h"
#include "Cameras/CameraManager.h"
#include "Math/Matrix33.h"

#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CBillboard::CBillboard()
	: m_Position (1.0f,1.0f,1.0f),
		m_PointA	 (1.0f,1.0f,1.0f),
		m_PointB   (1.0f,1.0f,1.0f),
		m_PointC   (1.0f,1.0f,1.0f),
		m_PointD   (1.0f,1.0f,1.0f),
		m_fSizeX   (1.0f),
		m_fSizeY   (1.0f)
{}

CBillboard::~CBillboard()
{
	
}

///<summary>
/// CBillboard:: Update : Calcula el Billboard a partir de los 4 puntos que forman el Quad
///</summary>
///<param name="camera" name2=elapsedTime>La cámara para calcular la dirección del billboard y elapsedTime para cada update.</param>
///<returns name="void"></returns>
void CBillboard::Update(float elapsedTime, float angle)
{
	CCamera * camera = CORE->GetCameraManager()->GetCamera();
	Vect3f l_vectUp = camera->GetVecUp();
	Vect3f l_vDirection = camera->GetDirection();

	//para la rotación del billboard
	Mat33f l_rotation; 
	l_vectUp = l_rotation.FromAxisAngle(l_vDirection.Normalize(), angle)*l_vectUp.Normalize();
	
	Vect3f l_vRight = l_vectUp^l_vDirection; 
	l_vRight.Normalize();
	m_PointA = m_Position - (l_vRight*m_fSizeX*0.5f) - (l_vectUp*m_fSizeY*0.5f);
	m_PointB = m_Position + (l_vRight*m_fSizeX*0.5f) - (l_vectUp*m_fSizeY*0.5f);
	m_PointC = m_Position - (l_vRight*m_fSizeX*0.5f) + (l_vectUp*m_fSizeY*0.5f);
	m_PointD = m_Position + (l_vRight*m_fSizeX*0.5f) + (l_vectUp*m_fSizeY*0.5f);
}

///<summary>
/// CBillboard:: Render : Dibuja el Quad con la textura que le pasamos por parámetro
///</summary>
///<param name="device" name2="texture">Renderizamos la textura que le pasamos como parámetro.</param>
///<returns name="void"></returns>
void CBillboard::Render(LPDIRECT3DDEVICE9 device, LPDIRECT3DTEXTURE9 & texture)
{
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


		device->SetTexture(0,texture);
		device->SetFVF(D3DFVF_VERTEX_TEXTURED);
		device->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,6,2,l_Indexes,D3DFMT_INDEX16,l_Points,sizeof(VERTEX_TEXTURED));
}