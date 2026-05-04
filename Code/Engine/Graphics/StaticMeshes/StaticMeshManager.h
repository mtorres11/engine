//----------------------------------------------------------------------------------
// CStaticMeshManager class
// Author: Miquel Rojas
//
// Description:
// clase que controla todas las mallas (cores) del juego, se introducen dentro de un
// mapa, con load se lee el StaticMeshManager.xml donde estan todas las cores
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_MESH_MANAGER_H_
#define INC_MESH_MANAGER_H_

#include "MapManager/MapManager.h"
#include "StaticMeshes/StaticMesh.h"
#include <string>

class CStaticMeshManager : public CMapManager<CStaticMesh>
{
protected:
  std::string m_FileName;
  unsigned long m_TotalObjectsFaces;
public:
  CStaticMeshManager(){}
  ~CStaticMeshManager();
  bool Load(const std::string &FileName);
  bool Reload();
  unsigned long GetTotalObjectsFaces()const {return m_TotalObjectsFaces;}
};

#endif //INC_RENDER_MANAGER_H_