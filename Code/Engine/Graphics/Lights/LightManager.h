//----------------------------------------------------------------------------------
// CLightManager class
// Authors: Katherine Capdevilla y Miquel Rojas
// 
// Description:
// Mánager de Luces que deriva de CMapManager
//----------------------------------------------------------------------------------

#pragma once
#ifndef INC_LIGHT_MANAGER_H_
#define INC_LIGHT_MANAGER_H_

#include "Base.h"
#include "MapManager/MapManager.h"
#include "Lights/Light.h"
#include <string>

class CLightManager : public CMapManager<CLight>
{
	public:
		CLightManager();
		~CLightManager();

		bool Load(const std::string &_sFileName);
		bool Reload();
    void Update(float elapsedTime);
		void Render(CRenderManager *_pRenderManager);
    void Done(); 
		
    std::vector<CLight *> GetLights();
    void TurnLight(std::string _sLightName, bool _bFlag);
		void OnOffLights(std::string _sLightON, std::string _sLightOFF);

    Vect4f  GetAmbientLight() { return m_V4fAmbientLight; }

	protected:
		std::vector<CLight *>								m_vLights;
    std::string													m_sFileName;

  private:
    Vect4f         m_V4fAmbientLight;    
};

#endif //INC_LIGHT_MANAGER_H_