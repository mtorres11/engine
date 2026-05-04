#include "ItemManager.h"
#include "Core.h"
#include "PhysicsManager.h"
#include "RenderablePhysX/RenderablePhysXManager.h"
#include "Utils/BaseUtils.h"
#include "ScriptManager.h"
#include "Actor/PhysicActor.h"
#include "../Sound/SoundManager.h"


#if defined( _DEBUG )
 #include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

bool CItemManager::Init()
{
	m_bIsOk = true;
	m_iNumItems=1;
	if (!m_bIsOk)
  {
    Release();
  }
	return m_bIsOk;

}

void CItemManager::AddItem(Vect3f _vPosition)
{
	std::string l_sFinalItemName="", l_sItemName="item";
	baseUtils::FormatSrting(l_sFinalItemName, "%s%d", l_sItemName.c_str (), m_iNumItems);

	CItem *	l_pItemNew = new CItem();
	l_pItemNew->CreateItem(l_sFinalItemName,_vPosition);

	if(GetResource(l_sFinalItemName) == NULL)
	{
		AddResource(l_sFinalItemName, l_pItemNew);
		m_iNumItems=m_iNumItems+1;
	}
	else
	{
		CHECKED_DELETE(l_pItemNew);
	}
}

void CItemManager::Update(float elapsedTime)
{
	std::map<std::string, CItem*>::iterator it_1	= CMapManager::m_Resources.begin();
  std::map<std::string, CItem*>::iterator itEnd_1	= CMapManager::m_Resources.end();
  while (it_1 != itEnd_1)
  {
    it_1->second->Update(elapsedTime);
    ++it_1;
  }
}

void CItemManager::CheckColisions()
{
	std::string l_sName;
	std::map<std::string, CItem*>::iterator it_1	= CMapManager::m_Resources.begin();
  std::map<std::string, CItem*>::iterator itEnd_1	= CMapManager::m_Resources.end();
  bool l_bFind = false;

  while (it_1 != itEnd_1)
  {
		l_sName=it_1->first;
		CItem* l_pItem = it_1->second;
		
		Vect3f l_PosSphere=CORE->GetRenderableObjectsManager()->GetInstance(l_sName)->GetPosition();
		std::vector<CPhysicUserData*>	m_vImpactObjects;
		CORE->GetPhysicsManager()->OverlapSphereActor(0.5f,l_PosSphere,m_vImpactObjects);

		if(!m_vImpactObjects.empty())
		{
      std::vector<CPhysicUserData*>::iterator it(m_vImpactObjects.begin());
			std::vector<CPhysicUserData*>::iterator itEnd(m_vImpactObjects.end());
			 
			while (it!=itEnd)
			{
				if(l_pItem->GetStatus()==true)
				{
			
					m_pPhysicUserdata=*it;
					if( m_pPhysicUserdata->GetName() == "mi_amigo")
					{
						l_bFind = true;
					  l_pItem->SetStatus(false);
						m_pPhysicUserdata->SetPaint(false);
					
					}
				}

				if(l_bFind)
				{
					l_pItem->SetPosition(Vect3f(0.0f,-20.0f,0.0f));
					CORE->GetItemManager()->GetResource(l_sName)->SetVisible(false);
					CORE->GetRenderableObjectsManager()->GetInstance(l_sName)->SetVisible(false);
					CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject(l_sName)->GetPhysXActor()->SetMat44(l_pItem->GetMat44());
					CORE->GetRenderablePhysXManager()->GetRenderablePhysXObject(l_sName)->GetUserData()->SetPaint(false);
					
					CORE->GetSoundManager()->PlaySound("powerUpPickup");
					
					std::string l_sNameLlave = "item0";
					if (l_pItem->GetName() != l_sNameLlave)
					{
						CORE->GetScriptManager()->RunCode("player:health_up(10.0)");
					}
					
					l_bFind=false;

				}
				++it;
			}
			
		}
   ++it_1;
  }
}

void CItemManager::CheckTimer()
{
	std::string l_name;
	std::map<std::string, CItem*>::iterator it_2	= CMapManager::m_Resources.begin();
	std::map<std::string, CItem*>::iterator itEnd_2	= CMapManager::m_Resources.end();
	CItem* l_item;
	bool l_timerOff= false;
	int time;
	
	while (it_2 != itEnd_2)
	{
		l_name = it_2->first;
		l_item = it_2->second;
		time=l_item->GetTimer();

		if(time>15)
		{
			l_timerOff = true;
		}
		++it_2;
	}

	if(l_timerOff)
	{
		CORE->GetItemManager()->GetResource(l_name)->SetVisible(false);
		CORE->GetRenderableObjectsManager()->GetInstance(l_name)->SetVisible(false);
	}
}


void CItemManager::Done()
{
  Release();
  m_bIsOk = false;
}

void CItemManager::Release()
{
  Destroy();
}

