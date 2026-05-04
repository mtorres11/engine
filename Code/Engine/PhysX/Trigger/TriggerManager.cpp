#include "PhysicsManager.h"
#include "Actor/PhysicActor.h"
#include "TriggerManager.h"
#include "Trigger.h"
#include "TriggerManager.h"
#include "XML/XMLTreeNode.h"
#include "Exceptions/Exception.h"
#include "ScriptManager.h"
#include "Math/Vector3.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

/// <summary>
/// Inicializa el manager.
/// </summary>
bool CTriggerManager::Init()
{
  m_bIsOk = true;
  return m_bIsOk;
}

/// <summary>
/// Finalize data.
/// </summary>
void CTriggerManager::Done()
{
  Release();
  m_bIsOk = false;
}

/// <summary>
/// Libera memoria.
/// </summary>
void CTriggerManager::Release()
{
//free memory
  std::vector<CPhysicUserData*>::iterator l_iter = m_vUserDatas.begin();
  for(;l_iter != m_vUserDatas.end();++l_iter)
  {
    CHECKED_DELETE(*l_iter);
  }

  std::vector<CPhysicActor*>::iterator l_iter2 = m_vTriggerActors.begin();
  for(;l_iter2 != m_vTriggerActors.end();++l_iter2)
  {
    CHECKED_DELETE(*l_iter2);
  }

  /*std::vector<CTrigger*>::iterator l_iter3 = m_vTriggers.begin();
  for(;l_iter3 != m_vTriggers.end();++l_iter3)
  {
    //CHECKED_DELETE(*l_iter3);
    if(*l_iter3) delete *l_iter3; *l_iter3=0;
  }*/


  m_vUserDatas.clear();
  m_vTriggers.clear();
	m_vTriggerActors.clear();
  
  Destroy();
}

/// <summary>
/// Carga el .xml de triggers.
/// </summary>
///<param name="FileName">Ubicación del archivo xml.</param>
bool CTriggerManager::Load(const std::string &FileName)
{
	m_bIsOk = true;

  m_FileName = FileName;

	CXMLTreeNode filexml;
	if (!filexml.LoadFile(FileName.c_str()))
	{
    //Guardar el mensaje de error en el LOGGER
		LOGGER->AddNewLog(ELL_ERROR, "CTriggerManager::Load ---Error al cargar el XML---");
		m_bIsOk = false;
	}
	else
	{
		CXMLTreeNode triggers = filexml["triggers"];
		int numNodes = triggers.GetNumChildren();
		if (triggers.Exists())
		{
		//<triggers>
			//<trigger name="Trigger_EntradaPuerta" entity="character" position="13.4664 0.0 -3.93624" max="-0.600954 4.70389 -1.79946" 
			//min="0.600954 0.0 1.79946" active="true" onEnter="testEnter" onLeave="testLeave" rotation="" size="1.0"/>
		//</triggers> 
      CTrigger* l_Trigger;

			for (int i = 0; i < numNodes; ++i) 
			{
				if (!triggers(i).IsComment())
        {
          //crea trigger info
          l_Trigger = new CTrigger();
          l_Trigger->Init();

        	l_Trigger->SetName(triggers(i).GetPszProperty("name", ""));
          l_Trigger->SetEntity(triggers(i).GetPszProperty("entity", ""));
          l_Trigger->SetPosition(triggers(i).GetVect3fProperty("position", Vect3f(0.0f,0.0f,0.0f)));
					l_Trigger->SetMax(triggers(i).GetVect3fProperty("max", Vect3f(0.0f,0.0f,0.0f)));
					l_Trigger->SetMin(triggers(i).GetVect3fProperty("min", Vect3f(0.0f,0.0f,0.0f)));
					l_Trigger->SetActive(triggers(i).GetBoolProperty("active", false));
					l_Trigger->SetTriggerActive(triggers(i).GetBoolProperty("active", false));
					l_Trigger->SetOnEnterCode(triggers(i).GetPszProperty("onEnter", ""));
          l_Trigger->SetOnLeaveCode(triggers(i).GetPszProperty("onLeave", ""));
					l_Trigger->SetRotationX(triggers(i).GetFloatProperty("rotationX"));
					l_Trigger->SetRotationY(triggers(i).GetFloatProperty("rotationY"));
					l_Trigger->SetRotationZ(triggers(i).GetFloatProperty("rotationZ"));
					l_Trigger->SetSize(triggers(i).GetVect3fProperty("size", Vect3f(0.0f,0.0f,0.0f)));
					l_Trigger->SetDimensions(triggers(i).GetVect3fProperty("size", Vect3f(0.0f,0.0f,0.0f)));
					GeneraTrigger(l_Trigger->GetName(), l_Trigger, false);
          m_vTriggers.push_back(l_Trigger);
        }
      }
    }
    else
    {
      m_bIsOk = false;
    }
  }
  return m_bIsOk;
}

/// <summary>
/// Recarga del .xml de triggers.
/// </summary>
/*bool CTriggerManager::Reload()
{
  Release();
  return Load(m_FileName);
}*/

/// <summary>
///  
/// </summary>
///<param name="entity_trigger1"></param>
///<param name="other_shape"></param>
void CTriggerManager::OnEnter(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape)
{
		CScriptManager* scriptManager = CORE->GetScriptManager();

    CTrigger *l_pTriggerActivado = GetResource(entity_trigger1->GetName());
    if(l_pTriggerActivado)
    {
      if(!l_pTriggerActivado->GetEntity().compare(other_shape->GetName()))
      {
				m_bActivo = true;
        scriptManager->RunCode(l_pTriggerActivado->GetOnEnterCode());
      }
    }
}

/// <summary>
///  
/// </summary>
///<param name="entity_trigger1"></param>
///<param name="other_shape"></param>
void CTriggerManager::OnLeave(CPhysicUserData* entity_trigger1, CPhysicUserData* other_shape)
{
 		CScriptManager* scriptManager = CORE->GetScriptManager();
    CTrigger *l_pTriggerActivado = GetResource(entity_trigger1->GetName());
    if(l_pTriggerActivado)
    {
      if(!l_pTriggerActivado->GetEntity().compare(other_shape->GetName()))
      {
        scriptManager->RunCode(l_pTriggerActivado->GetOnLeaveCode());
      }
    }
}


CPhysicActor* CTriggerManager::GeneraTrigger(const std::string& _name, CTrigger* _trigger, bool _esPunyo = false) 
{
  //genera el PhysicActor
  CPhysicUserData *userdata = new CPhysicUserData(_name);
  
  if(!userdata)
  {
    std::string msg_error = "CTriggerManager::GeneraTrigger->Error al intentar generar un userdata para triggers: " + _name;
    LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
    return false;
  }
  userdata->Init();
  userdata->SetName(_name);
  userdata->SetPaint(true);

  CPhysicActor* aTrigger = new CPhysicActor(userdata);

  ////guarda el userdata para luego borrarlo correctamente
  m_vUserDatas.push_back(userdata);

	//calculamos la matriz con la rotación
	_trigger->CalculateMatriz44f();
	Mat44f l_RotationTrigger = _trigger->GetMatrizTrigger();

  //se le asigna el rol de trigger y se posiciona
  aTrigger->CreateBoxTrigger(_trigger->GetDimensions(),PX_CG_TRIGGER);
  aTrigger->SetGlobalPosition(_trigger->GetPosition());

  //se añade el trigger en el vector y en el mapa
  AddResource(_name, _trigger);

  //lo añadimos al manager
  if(!CORE->GetPhysicsManager()->AddPhysicActor(aTrigger))
  {
    //Guardar el mensaje de error en el LOGGER
    std::string msg_error = "CTriggerManager::Load->Error al intentar generar un trigger en physics: " + _name;
    //msg_error.append(m_File_name.c_str());
    LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
    return 0;
  }
  aTrigger->SetMat44(l_RotationTrigger);
    
  if(!_esPunyo)
  {
    m_vTriggerActors.push_back(aTrigger);
  }

  std::string msg_info = "CTriggerManager::Load->Se ha generado el trigger: " + _name;
  LOGGER->AddNewLog(ELL_INFORMATION, msg_info.c_str());

  return aTrigger;
}

void CTriggerManager::RegistraTriggers()
{
  //recibe llamadas de los eventos de triggers
  CORE->GetPhysicsManager()->SetTriggerReport((CPhysicTriggerReport*)this);
}

void CTriggerManager::Render()
{
	std::vector<CTrigger*>::const_iterator l_Iter = m_vTriggers.begin();
  for(;l_Iter != m_vTriggers.end();++l_Iter) 
  {
		Vect3f l_fSize = (*l_Iter)->GetSize();
		Vect3f l_pPosition = (*l_Iter)->GetPosition();
    (*l_Iter)->Render(l_fSize, l_pPosition);
  }
}

CTrigger* CTriggerManager::GetTrigger(std::string name)
{
	return GetResource(name);
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
