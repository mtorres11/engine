//----------------------------------------------------------------------------------
// CRenderableObjectsManager class
// Authors: Andrea Cristobo y Miquel Rojas
// Description:
// Clase que controla todas los elementos renderizables del videojuego
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_RENDER_OBJECTS_MANAGER_H_
#define INC_RENDER_OBJECTS_MANAGER_H_

//------Forward declarations---------
class CRenderableObject;
class CRenderableVertexs;
//----------------------------------

#include <d3dx9.h>
#include "MapManager/MapManager.h"
#include "Math/Vector3.h"

class CRenderableObject;
class CRenderableAnimatedInstanceModel;

class CRenderableObjectsManager : CMapManager<CRenderableObject>
{
	public:
		CRenderableObjectsManager(): m_RenderableObjects(0){};
		~CRenderableObjectsManager();

		void										Update								  (float ElapsedTime);
		void										Render								  (CRenderManager *RM);
		void										Release									();
		CRenderableObject *			AddMeshInstance				  (const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position, const float &Yaw, const float &Pitch, const float &Roll, const float &ScaleX=1.f, const float &ScaleY=1.f, const float &ScaleZ=1.f);
		CRenderableObject *			AddAnimatedModelInstance(const std::string &CoreMeshName, const std::string &InstanceName, const Vect3f &Position, const float &Yaw, const float &Pitch, const float &Roll, const float &ScaleX=1.f, const float &ScaleY=1.f, const float &ScaleZ=1.f);
		void										AddResource						  (const std::string &Name, CRenderableObject *RenderableObject);
		void										CleanUp								  ();
		bool										Load									  (const std::string &FileName);
		CRenderableObject *			GetInstance						  (const std::string &Name) ;
		CRenderableAnimatedInstanceModel * GetInstanceAsAnimatedInstanceModel(const std::string &Name);
		bool                    Reload                  ();
    std::vector<CRenderableObject *> GetVector      ()                {return	m_RenderableObjects;}

	protected:
		std::vector<CRenderableObject *>		m_RenderableObjects;
    std::string m_FileName;
};

#endif
