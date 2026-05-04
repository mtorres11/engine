#include "LightManager.h"
#include "RenderManager.h"
#include "xml/XMLTreeNode.h"
#include "Logger/Logger.h"
#include "Exceptions/Exception.h"
#include "LightEffects/LightEffect1.h"
#include "LightEffects/LightEffect2.h"
#include "DirectionalLight.h"
#include "OmniLight.h"
#include "SpotLight.h"
#include "Object3D/Object3D.h"
#include "Math/Matrix44.h"
#include "Utils/BaseUtils.h"
#include <string>

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

CLightManager::CLightManager()
: m_vLights(NULL)
, m_sFileName("")
{
  m_V4fAmbientLight = Vect4f(0.0, 0.0, 0.0, 1.0);
}

CLightManager::~CLightManager()
{  
  Done();
}

void CLightManager::Done()
{  
  m_vLights.clear();
  Destroy();
}

bool CLightManager::Load(const std::string &_sPathFile)
{
	CXMLTreeNode parser;
	if (!parser.LoadFile(_sPathFile.c_str()))
	{
		std::string msg_error = "LightManager::Load->Error al intentar leer el archivo de luces: " + _sPathFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
	}
  
  m_sFileName = _sPathFile;

/*<lights>
	<light name ="Omni01" type="omni=" pos="37.8395 0.0 1.09377" color="255.0 255.0 255.0" att_start_range="80.0" att_end_range="200.0"/>
	<light name ="Direct01" type="directional=" pos="78.8884 0.0 -15.8936" dir="78.8884 0.0 -15.8936" color="255.0 255.0 255.0" render_shadows="false" att_start_range="80.0" att_end_range="200.0"/>
	<light name ="Spot01" type="spot=" pos="76.4187 0.0 36.7725" dir="76.4187 0.0 36.7725" color="255.0 255.0 255.0" angle="43.0" falloff="45.0" att_start_range="80.0" att_end_range="200.0"/>
  <light type="ambient" color="0.898039 0.843137 0.827451" multiplier="0.7" />
</lights>
*/

	LOGGER->AddNewLog(ELL_INFORMATION, "LightsManager::Load-> Parseando fichero de luces: %s", _sPathFile.c_str());
	
	CXMLTreeNode  l_treeNode = parser["lights"];
	if (l_treeNode.Exists())
	{
		if (!l_treeNode.IsComment())
		{
			int count = l_treeNode.GetNumChildren();

      CXMLTreeNode l_effectNode;

      CLight *l_Light; 
			int l_iEffect;
      std::string l_sTipo;
      std::string l_sName;
      Vect3f color;
      		
      for (int i = 0; i < count; ++i)
			{
				if (!l_treeNode(i).IsComment())
				{
					l_sTipo = l_treeNode(i).GetPszProperty("type");
		      
          if(!strcmp(l_sTipo.c_str(), "ambient"))
					{
							color = l_treeNode(i).GetVect3fProperty("color", Vect3f(0.f,0.f,0.f));
              m_V4fAmbientLight.x = color.x;
              m_V4fAmbientLight.y = color.y;
              m_V4fAmbientLight.z = color.z;
              m_V4fAmbientLight.w = l_treeNode(i).GetFloatProperty("multiplier", 1.0f);

              continue;
					}
					else if(!strcmp(l_sTipo.c_str(), "omni"))
					{
							l_Light = new COmniLight();
							l_Light->SetType(l_Light->OMNI);
					}
					else if(!strcmp(l_sTipo.c_str(), "directional"))
					{
							l_Light = new CDirectionalLight();
							l_Light->SetType(l_Light->DIRECTIONAL);
					}
          else if(!strcmp(l_sTipo.c_str(), "spot"))
					{
							l_Light= new CSpotLight();
							l_Light->SetType(l_Light->SPOT);
					}
					

					l_sName = l_treeNode(i).GetPszProperty("name");
					l_Light->SetName(l_sName);

					l_Light->SetPosition(l_treeNode(i).GetVect3fProperty("pos", Vect3f(0.f,0.f,0.f)));
					color = l_treeNode(i).GetVect3fProperty("color", Vect3f(0.f,0.f,0.f));
					CColor l_myColor(color.x,color.y,color.z);
					l_Light->SetColor(l_myColor);
          l_Light->SetMultiplier(l_treeNode(i).GetFloatProperty("multiplier", 1.0f));
					l_Light->SetStartRangeAttenuation(l_treeNode(i).GetFloatProperty("att_start_range"));
					l_Light->SetEndRangeAttenuation(l_treeNode(i).GetFloatProperty("att_end_range"));
					l_Light->SetEnable(l_treeNode(i).GetBoolProperty("enable"));

          l_effectNode = l_treeNode(i)["effect"];
          
          if (l_effectNode.Exists())
          {
            l_iEffect = l_effectNode.GetIntProperty("type", 0);
            
            switch(l_iEffect)
            {
              case 1: //intensidad parpadeante
              {
                CLightEffect1 * l_LightEffect1 = new CLightEffect1(l_Light);

                l_LightEffect1->SetMaxMultiplier(l_effectNode.GetFloatProperty("max", 0.0));
                l_LightEffect1->SetMinMultiplier(l_effectNode.GetFloatProperty("min", 0.0));
                l_LightEffect1->SetLapDuration( l_effectNode.GetFloatProperty("numframes", 0.0), l_effectNode.GetFloatProperty("fps", 0.0));

                l_Light->SetEffect(l_LightEffect1);

                l_LightEffect1->Init();

                break;
              }
              
              case 2: // usado para el destello de los relampagos
              {
                CLightEffect2 * l_LightEffect2 = new CLightEffect2(l_Light);

                l_LightEffect2->SetFPS(l_effectNode.GetFloatProperty("fps", 0.0));

                // keyframes
                int l_iNumFrames = l_effectNode.GetNumChildren();

                float l_fFinalMultiplier;
                uint16 l_wLength;

                for (int j = 0; j < l_iNumFrames; ++j)
			          {
                  l_fFinalMultiplier = l_effectNode(j).GetFloatProperty("finalMultiplier", 0.0f);
                  l_wLength = l_effectNode(j).GetIntProperty("length", 0.0f);
                  l_LightEffect2->AddKeyFrame(l_fFinalMultiplier, l_wLength);
                }

                l_Light->SetEffect(l_LightEffect2);

                l_LightEffect2->Init();

                break;
              }
            }
          }

					if(!strcmp(l_sTipo.c_str(),"omni"))
					{
					}
					else if(!strcmp(l_sTipo.c_str(),"directional"))
					{
							((CDirectionalLight *)l_Light)->SetDirection(l_treeNode(i).GetVect3fProperty("dir", Vect3f(0.f,0.f,0.f)));
					}
					else if(!strcmp(l_sTipo.c_str(),"spot"))
					{
						((CSpotLight *)l_Light)->SetDirection(l_treeNode(i).GetVect3fProperty("dir", Vect3f(0.f,0.f,0.f)));
						((CSpotLight *)l_Light)->SetAngle(l_treeNode(i).GetFloatProperty("angle"));
						((CSpotLight *)l_Light)->SetFallOff(l_treeNode(i).GetFloatProperty("falloff"));
					}
					this->AddResource(l_sName, l_Light);
					this->m_vLights.push_back(l_Light);

          LOGGER->AddNewLog(ELL_INFORMATION, "LightsManager::Load-> Añadida luz(%s)", l_sName.c_str());
				}//end if IsComment()
      }//for
		}//end if IsComment()
	}//if
  else
	{
		std::string msg_error = "LightManager::Load->Error al intentar leer el <lights> en archivo de luces: " + _sPathFile;
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		throw CException(__FILE__, __LINE__, msg_error);
    return false;
	}
  return true;
}

bool CLightManager::Reload()
{
  m_vLights.clear();
  Destroy();
  return Load(m_sFileName);
}

void CLightManager::Update(float elapsedTime)
{
  CLightEffect * l_pEffect;
  std::vector<CLight*>::const_iterator l_Iter = m_vLights.begin();
  for(;l_Iter != m_vLights.end();++l_Iter) 
  {
    l_pEffect = (*l_Iter)->GetEffect();

    if (l_pEffect != NULL)
    {
      switch (l_pEffect->GetType())
      {
        case 1:
        {
           if ((*l_Iter)->GetEnable())
            ((CLightEffect1*) l_pEffect)->Update(elapsedTime);
          break;
        }

        case 2:
        {
          ((CLightEffect2*) l_pEffect)->Update(elapsedTime);
          break;
        }
      }
    }
  }
}

void CLightManager::Render(CRenderManager *_pRM)
{
	std::vector<CLight*>::const_iterator l_Iter = m_vLights.begin();
  for(;l_Iter != m_vLights.end();++l_Iter) 
  {
    (*l_Iter)->Render(_pRM);
  }
  
}

std::vector<CLight *> CLightManager::GetLights()
{
	std::vector<CLight*>::const_iterator l_Iter = m_vLights.begin();
	std::vector<CLight*> l_vLights;
	
	for(int i=0;l_Iter != m_vLights.end();++l_Iter, ++i) 
  {
		if(m_vLights[i]->GetEnable())
		{
			l_vLights.push_back(m_vLights[i]);
		}
  }
	return l_vLights;
}

/// <summary>
/// Encender o apagar la luz con el nombre dado
/// </summary>
///<param name="_sLightName">Nombre de la luz.</param>
///<param name="_sLightOFF">(Bool) Flag que indica si la luz se debe encender (true) o apagar (false).</param>
void CLightManager::TurnLight(std::string _sLightName, bool _bFlag)
{
	CLight * l_pLight = GetResource(_sLightName);

	if (l_pLight != NULL)
	{
		l_pLight->SetEnable(_bFlag);
	}
	else
	{ 
		std::string msg_error;
    baseUtils::FormatSrting(msg_error, "LightManager::TurnLight->No se obtuvo la luz %s del mapa de luces.", _sLightName);
		LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
	}
}

/// <summary>
/// Enciende la primera luz y apaga la segunda
/// </summary>
///<param name="_sLightON">Nombre de la luz a encender.</param>
///<param name="_sLightOFF">Nombre de la luz a apagar.</param>
void CLightManager::OnOffLights(std::string _sLightON, std::string _sLightOFF)
{
	CLight *l_pLightOFF = GetResource(_sLightOFF);
	CLight *l_pLightON = GetResource(_sLightON);

	if((l_pLightOFF!=NULL)&&(l_pLightON!=NULL))
	{
		l_pLightOFF->SetEnable(false);
		l_pLightON->SetEnable(true);
	}
	else
	{ 
		std::string msg_error = "LightManager::OnOffLights->Error al intentar leer el mapa de luces.";
		LOGGER->AddNewLog(ELL_WARNING, msg_error.c_str());
	}
}
