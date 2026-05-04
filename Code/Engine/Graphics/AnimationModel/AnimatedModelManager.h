//----------------------------------------------------------------------------------
// CRenderableObjectsManager class
// Author: Andrés
// Description:
// Mánager de Modelos Animados que deriva de CMapManager
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_ANIMATED_MODEL_MANAGER_H_
#define INC_ANIMATED_MODEL_MANAGER_H_

//------Forward declarations---------
//class CAnimatedCoreModel;
class CAnimatedInstanceModel;
//----------------------------------

#include "AnimatedCoreModel.h"
#include "MapManager/MapManager.h"

class CAnimatedModelManager : public CMapManager<CAnimatedCoreModel>
{
private:
  unsigned long m_TotalNumFaces;
  public:
		CAnimatedModelManager();
		~CAnimatedModelManager();

		CAnimatedCoreModel *				GetCore						(const std::string &Name, const std::string &Path);
		CAnimatedInstanceModel *		GetInstance				(const std::string &Name);
    bool												Load							(const std::string &FileName);
    unsigned long               GetTotalNumFaces  ()const   {return m_TotalNumFaces;}

};

#endif
