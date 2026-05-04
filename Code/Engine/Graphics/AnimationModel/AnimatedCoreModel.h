//----------------------------------------------------------------------------------
// CRenderableObjectsManager class
// Author: Andrés
// Última modificación: Katherine y Andrea
// Description:
// 
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_ANIMATED_CORE_MODEL_H_
#define INC_ANIMATED_CORE_MODEL_H_

//------Forward declarations---------
class CRenderableVertexs;
//----------------------------------

#include "cal3d/cal3d.h"
#include "Vertex/RenderableVertex.h"
#include "Named/Named.h"
#include "Math/MathTypes.h"


class CAnimatedCoreModel : public CNamed
{
	private:
		CalCoreModel *								m_CalCoreModel;
		std::string										m_Name;
		std::string										m_MeshFilename;
		std::string										m_SkeletonFilename;
		std::vector<std::string>			m_TextureFilenameList;
		std::string										m_Technique;
		CalHardwareModel *						m_CalHardwareModel;
		CRenderableVertexs *					m_RenderableVertexs;
		std::string										m_Path;
		int														m_NumVtxs;
		int														m_NumFaces;
    int                           m_NumAnimations;


		bool														LoadMesh								();
		bool														LoadSkeleton						();
		bool														LoadAnimation						(const std::string &Name, const std::string &Filename);
		

	public:
		CAnimatedCoreModel(std::string _sName);
		~CAnimatedCoreModel();

		

		CalHardwareModel *							GetCalHardwareModel			() const;
		bool														LoadVertexBuffer				();
		CRenderableVertexs *						GetRenderableVertexs		() const;
		CalCoreModel *									GetCoreModel						();
		const std::string &							GetTextureName					(size_t id);
		size_t													GetNumTextures					() const{ return m_TextureFilenameList.size(); }
		bool														Load										(const std::string &Path);
    int                             GetNumVertex            ()const  {return m_NumVtxs;}
    int                             GetNumFaces             ()const  {return m_NumFaces;}
    int                             GetNumAnimations        ()const  {return m_NumAnimations;}
		const std::string &							GetTechniqueName				()const  {return m_Technique;};
		const std::string &							GetAnimatedCoreModelName()const  {return m_Name;};

		bool														LoadInstances						(const std::string &Path);

		
		std::vector<Vect3f>						m_vInstancePosition;
		std::vector<Vect3f>						GetVector								(){return	m_vInstancePosition;}
		std::vector<float>						m_vInstanceYaw;
		std::vector<float>						GetInstanceYaw					(){return	m_vInstanceYaw;}


};

#endif
