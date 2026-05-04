//----------------------------------------------------------------------------------
// CObject3D class
// Author: Marco Torres
//
// Description:
// Esta clase será la base de toda instancia que pueda existir en la escena
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_REND_VERTEX_H_
#define INC_REND_VERTEX_H_

#include "Math\MathTypes.h"
#include "Math\Vector3.h"

#include "Base.h"
#include <d3dx9.h>

class CEffectTechnique;

class CRenderableVertexs
{
protected:
  LPDIRECT3DVERTEXBUFFER9 m_VB;
	LPDIRECT3DINDEXBUFFER9	m_IB;
	size_t m_IndexCount, m_VertexCount;

public:
	CRenderableVertexs():
			m_IB(NULL),
			m_VB(NULL),
			m_IndexCount(0),
			m_VertexCount(0)
			{};
	virtual ~CRenderableVertexs()
	{
    CHECKED_RELEASE(m_VB);
		CHECKED_RELEASE(m_IB);
	}
	virtual bool Render(CRenderManager *RM) const = 0;
	bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique)
	{
    return Render(RM, EffectTechnique,0,0,-1,0,-1);
  }
	
	virtual bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique,D3DXHANDLE &_InstancesPositionParameter, std::vector<Vect3f>  _vInstancePosition, std::vector<float>  _vInstanceYaw, int BaseVertexIndex=0, int MinVertexIndex=0, int NumVertices=-1, int StartIndex=0, int PrimCount=-1) const=0; //Instancing
	virtual bool Render(CRenderManager *RM, CEffectTechnique *EffectTechnique, int BaseVertexIndex, int MinVertexIndex, int NumVertices, int StartIndex, int PrimCount) const = 0;
	virtual inline size_t GetFacesCount() const {return (m_IndexCount/3);};
	virtual inline size_t GetVertexsCount() const {return m_VertexCount;};
	virtual inline unsigned short GetVertexType() const = 0;
	virtual inline size_t GetVertexSize() const = 0;
	virtual inline size_t GetIndexSize() const = 0;
};

#endif