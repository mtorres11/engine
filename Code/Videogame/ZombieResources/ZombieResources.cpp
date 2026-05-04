#include "ZombieResources.h"

bool CZombieUserData::Init(std::string name)
{
  m_sUserData = name;
  m_bIsOk = true;

  if (!m_bIsOk){
    Release();
  }

  return m_bIsOk;
}

void CZombieUserData::Done()
{
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}  

void CZombieUserData::Release()
{
//free memory

}
  //bool CZombieResources::InicializaUserData(std::string _Name)
//{
//    CZombieUserData* l_pUserData = new CZombieUserData(_Name);
//    if(!l_pUserData)
//    {
//      LOGGER->AddNewLog(ELL_WARNING,"a CZombieUserData instace couldnt allocate memory");
//      m_bIsOk = false;
//    }
//
//}

