//----------------------------------------------------------------------------------
// CObject3D class
// Author: Marco Torres
//
// Modificado por: Katherine Capdevila
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_IND_VERTEX_H_
#define INC_IND_VERTEX_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"
#include "Vertex/VertexType.h"
#include "RenderManager.h"
#include "Vertex/RenderableVertex.h"
#include "Shaders/Effect.h"
#include <d3dx9.h>
#include "Shaders/EffectTechnique.h"

template<class T>
class CIndexedVertex : public CRenderableVertexs
{
protected:
	inline size_t GetVertexSize() const {return sizeof(T);}
	inline size_t GetIndexSize() const {return sizeof(unsigned short);}

public:
	CIndexedVertex(CRenderManager * RM, void *VertexAddress, void *IndexAddres, size_t VertexCount, size_t IndexCount)
	{
		m_IndexCount = IndexCount;
		m_VertexCount = VertexCount;

		VOID * pVertices, * pIndices;

		//VertexBuffer
		RM->GetDevice()->CreateVertexBuffer(VertexCount*GetVertexSize(), 0, CAL3D_HW_VERTEX::GetFVF(), D3DPOOL_DEFAULT, &m_VB, NULL);
		m_VB->Lock(0, VertexCount*GetVertexSize(),&pVertices, 0 );
		memcpy(pVertices, VertexAddress, VertexCount*GetVertexSize());
		m_VB->Unlock();

		//IndexBuffer
		RM->GetDevice()->CreateIndexBuffer(IndexCount*GetIndexSize(), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_IB, NULL);
		m_IB->Lock(0, IndexCount*GetIndexSize(),&pIndices, 0 );
		memcpy(pIndices, IndexAddres, IndexCount*GetIndexSize());
		m_IB->Unlock();

	};

	~CIndexedVertex()
  {
    CHECKED_RELEASE(m_IB);
    CHECKED_RELEASE(m_VB);

  }

	bool Render(CRenderManager *RM) const {
		RM->GetDevice()->SetStreamSource(0,m_VB,0, GetVertexSize());
		RM->GetDevice()->SetIndices(m_IB);
		RM->GetDevice()->SetFVF(T::GetFVF());
		RM->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_VertexCount, 0, GetFacesCount());
    return true;
	}

	virtual inline unsigned short GetVertexType() const
	{
		return T::GetVertexType();
	}

	bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique, int BaseVertexIndex=0, int MinVertexIndex=0, int NumVertices=-1, int StartIndex=0, int PrimCount=-1) const
	{
		LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
		UINT l_NumPasses;
		LPD3DXEFFECT l_Effect=EffectTechnique->GetEffect()->GetD3DEffect();
		l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());
		if(SUCCEEDED(l_Effect->Begin(&l_NumPasses,0)))
		{
			l_Device->SetVertexDeclaration(T::GetVertexDeclaration());
			l_Device->SetStreamSource(0,m_VB,0,sizeof(T));
			l_Device->SetIndices(m_IB);
			for (UINT b=0;b<l_NumPasses;++b)
			{
				l_Effect->BeginPass(b);
				l_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, BaseVertexIndex, MinVertexIndex, (NumVertices==-1) ? (UINT)m_VertexCount : NumVertices, StartIndex, (PrimCount==-1) ? (UINT)m_IndexCount/3 : PrimCount);
				l_Effect->EndPass();
			}
			l_Effect->End();
		}
		return true;
	}

	//Shader Instancing
	bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique,D3DXHANDLE &_InstancesPositionParameter, std::vector<Vect3f> _vInstancePosition, std::vector<float>  _vInstanceYaw, int BaseVertexIndex=0, int MinVertexIndex=0, int NumVertices=-1, int StartIndex=0, int PrimCount=-1) const
	{
		LPDIRECT3DDEVICE9 l_Device=RM->GetDevice();
		HRESULT hr;
    UINT iPass, cPasses;

		l_Device->SetVertexDeclaration(CAL3D_HW_VERTEX::GetVertexDeclaration());

		// Stream zero is our model
		l_Device->SetStreamSource( 0, m_VB, 0, sizeof(CAL3D_HW_VERTEX));
		l_Device->SetIndices(m_IB);

		// Render the scene with this technique as defined in the .fx file
		LPD3DXEFFECT l_Effect=EffectTechnique->GetEffect()->GetD3DEffect();
		l_Effect->SetTechnique(EffectTechnique->GetD3DTechnique());

		l_Effect->Begin(&cPasses, 0);

		for(iPass = 0; iPass < cPasses; iPass++)
    {
			l_Effect->BeginPass(iPass);

			int numInstances = _vInstancePosition.size();
			int i=0;
      
			while( numInstances > 0 )
			{
				// set the instance instancing array
				float l_fInstances[4] = {_vInstancePosition[i].x, _vInstancePosition[i].y, _vInstancePosition[i].z, _vInstanceYaw[i]};
				l_Effect->SetFloatArray(_InstancesPositionParameter,(float*)l_fInstances, 4);
				i++;

				l_Effect->CommitChanges();

				l_Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0,numInstances*(NumVertices==-1) ? (UINT)m_VertexCount : NumVertices, 
																						0, numInstances*(PrimCount==-1) ? (UINT)m_IndexCount/3 : PrimCount);

				numInstances--;
      }
      l_Effect->EndPass() ;
    }
    l_Effect->End();
		return true;
	}
};

#endif