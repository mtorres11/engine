#include "RandomArray.h"
#include "Math/Random.h"


#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

unsigned int CRandomArray::m_iCurrentIndex = 0;

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CRandomArray::Init(){
  m_bIsOk = true;

  if (!m_bIsOk){
    Release();
  }

  return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CRandomArray::Done(){
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CRandomArray::Release(){
//free memory
  std::vector<float>::iterator l_iter = m_vRandomArray.begin();
  for(;l_iter != m_vRandomArray.end();++l_iter)
  {
    m_vRandomArray.pop_back();
  }

  m_vRandomArray.clear();
}

//----------------------------------------------------------------------------
// Generate Array
//----------------------------------------------------------------------------
void CRandomArray::GenerateArray(unsigned int nElems)
{
    m_iNumElems = nElems;
    
    CRandom v;
    SYSTEMTIME ahora;
    GetLocalTime(&ahora);
    v.init((unsigned int)ahora.wMilliseconds);
    
    for(unsigned int i = 0 ; i < m_iNumElems ; i++)
    {
      m_vRandomArray.push_back(v.getRandFloat(0.0f,1.0f));
    }
}

//----------------------------------------------------------------------------
// Advance Index
//----------------------------------------------------------------------------
void CRandomArray::AdvanceIndex(void)
{
  if(m_iCurrentIndex < (m_iNumElems - 1))
  {
    m_iCurrentIndex ++;
  }
  else
  {
    m_iCurrentIndex = 0;
  }
}

  //inicializacion y pedida de valores.
  //CRandomArray *randomArray = new CRandomArray();
  //randomArray->Init();
  //randomArray->GenerateArray(100);
  //float myvalue = randomArray->GetValue();
  //float myvalue2 = randomArray->GetValue();
  //float myvalue3 = randomArray->GetValue();
  //float myvalue4 = randomArray->GetValue();


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------