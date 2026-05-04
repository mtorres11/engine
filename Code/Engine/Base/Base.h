//-------------------------------------
// includes básicos
//-------------------------------------
#ifndef _BASE_H
#define _BASE_H

#include "Utils/Types.h"
#include "Logger/Logger.h"

#include <assert.h>

//--- BASIC INCLUDES ---------
#define CHECKED_DELETE(x)					if(x) delete x; x=0;
#define CHECKED_RELEASE(x)				if(x) x->Release(); x=0;
#define CHECKED_DELETE_ARRAY(x)		if(x) delete [] x; x=0;
//------------------------------

//Estructura con los parametros iniciales
struct SInitParams
{
  bool		m_bFullScreen;
	uint16  m_uWidthScreen;
  uint16  m_uHeightScreen;

	bool		m_bConsoleEnabled;
  bool    m_bLogRender;
  bool		m_bMouseEnabled;
	bool		m_bGuiEnabled;
};

//Singleton del CORE y del LOGGER
#include "Core.h"
#define CORE CCore::GetSingletonPtr()
#define LOGGER CLogger::GetSingletonPtr()
#define DEVINPUT CORE->GetInputManager()
#endif //_BASE_H


