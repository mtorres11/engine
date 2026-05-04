#include "ActionToInput.h"
#include "XML/XMLTreeNode.h"
#include "InputDefs.h"
#include "InputManager.h"


#if defined( _DEBUG )  
#include "Memory/MemLeaks.h" 
#endif

/// <summary>
/// Inicializa el mapa con los valores numericos asociados a cada literal del XML.
/// </summary>
///<returns>Boleano que representa si la operación se realizó exitosamente.</returns>
bool CActionToInput::Init(){
  m_bIsOk = true;

  InitString2Input();

  if (!m_bIsOk){
    Release();
  }

  return m_bIsOk;
}

/// <summary>
/// Finalizes data.
/// </summary>
void CActionToInput::Done(){
  if (IsOk())
  {
    Release();
    m_bIsOk = false;
  }
}

/// <summary>
/// Libera la memoria usada por la clase
/// </summary>
void CActionToInput::Release(){
  Unload();
}

/// <summary>
/// Devuelve los inputs, de una accion dada, en forma de cadena preparada para imprimir.
/// De momento solo retorna las teclas (o axis) asociados a la accion pasada.
/// </summary>
///<param name="config_">Cadena en la que se retornara el resultado</param>
///<param name="_action">Accion de la que se quieren conocer sus inputs asociados</param>
void CActionToInput::GetActionConfig(std::string &config_,  std::string _action)
{
  config_ = "";
  TMapActions::iterator iter = m_InputsPerAction.find(_action);

  if(iter != m_InputsPerAction.end())
	{
    TInputs::iterator itVEnd = iter->second.end();
    bool l_bFirst = true;
    for(TInputs::iterator it = iter->second.begin(); it != itVEnd; ++it)
    {
      if (l_bFirst) l_bFirst = false;
      else
      {
        config_ += "+";
      }
      config_ += (*it)->str_id;
    }
	}
}

/// <summary>
/// Retorna el codigo asociado al literal dado.
/// </summary>
///<param name="_lit">Literal. ej:"IDV_KEYBOARD"</param>
///<returns>Codigo int asociado al literal o -1 en caso de no tener un codigo asociado</returns>
int CActionToInput::GetValue(std::string _lit)
{
  std::map<std::string, uint32>::iterator iter = m_String2Code.find(_lit);

  if(iter == m_String2Code.end())
	{
    return -1;
	}
	else {
		return iter->second;
	}
}

/// <summary>
/// Recononce si se han introducido los inputs asociados a la accion dada (se ha ejecutado la accion)
/// </summary>
///<param name="_myAction">Accion por la que se consulta</param>
///<returns>float: 0.0f si no se ha ejecutado la accion
/// 1.0f si se ha presionado una tecla
/// el valor del MouseDelta si se ha consultado por el axis
///</returns>
float CActionToInput::DoAction(std::string _myAction)
{
  float l_fDelta = 0.0f;

  TMapActions::iterator iter = m_InputsPerAction.find(_myAction);

  if (iter != m_InputsPerAction.end())
	{
    TInputs::iterator itVEnd = iter->second.end();
    for (TInputs::iterator it = iter->second.begin(); it != itVEnd; ++it)
    {
      switch ((*it)->dispositivo)
      {
      case IDV_MOUSE:
        switch((*it)->accion)
        {
        case AXIS_MOUSE_X:
          l_fDelta = (float)DEVINPUT->GetMouseDelta().x;         
          if (!l_fDelta) 
            return l_fDelta;
          break;

        case AXIS_MOUSE_Y:
          l_fDelta = (float)DEVINPUT->GetMouseDelta().y;         
          if (!l_fDelta) 
            return l_fDelta;
          break;
        case   AXIS_MOUSE_Z:
          l_fDelta = (float)DEVINPUT->GetMouseDelta().z;         
          if (!l_fDelta) 
            return l_fDelta;
          break;
        case   AXIS_LEFT_THUMB_X:

          break;
        case   AXIS_LEFT_THUMB_Y:
          break;
        case   AXIS_RIGHT_THUMB_X:
          break;
        case   AXIS_RIGHT_THUMB_Y:
          break;
        case   AXIS_DELTA_TRIGGER_RIGHT:
          break;
        case   AXIS_DELTA_TRIGGER_LEFT:
          break;
        case   AXIS_NOTHING:
          break;
        }
      case IDV_KEYBOARD:
        switch((*it)->accion)
        {
        case   EVENT_DOWN:
          l_fDelta = (float) DEVINPUT->IsDown((INPUT_DEVICE_TYPE)(*it)->dispositivo, (*it)->valor);
          if (!l_fDelta) 
            return l_fDelta;
          break;
        case   EVENT_UP_DOWN:
          l_fDelta = (float) DEVINPUT->IsUpDown((INPUT_DEVICE_TYPE)(*it)->dispositivo, (*it)->valor);                
          if (!l_fDelta) 
            return l_fDelta;
          break;
        case   EVENT_DOWN_UP:
          l_fDelta = (float) DEVINPUT->IsDownUp((INPUT_DEVICE_TYPE)(*it)->dispositivo, (*it)->valor);
          if (!l_fDelta) 
            return l_fDelta;
          break;
        case   EVENT_NOTHING:
          break;
        }
        break;
      //aca falta poner los case para otros dispositivos, como los gamepads 
      }
    }
	}
	
  return l_fDelta;
}

/// <summary>
/// Carga las acciones e inputs asociados del fichero XML.
/// </summary>
///<param name="_file">Filepath del XML.</param>
///<returns>Boleano que representa si la operación se realizó exitosamente.</returns>
bool CActionToInput::Load(std::string _file)
{
  CXMLTreeNode l_XMLReadActions;

  if (!l_XMLReadActions.LoadFile(_file.c_str()))
  {
    LOGGER->AddNewLog(ELL_ERROR, "CActionToInput::Load->Error al cargar el archivo: %s", _file.c_str());
    m_bIsOk = false;
  }
  else
  {
    m_filename = _file;
    CXMLTreeNode  act = l_XMLReadActions["Actions"];
    if(!act.IsComment())
    {
      if (act.Exists())
      {
        std::string evt = "" , cod = "" , axs = "" , dlt = "";

        int32 numNodes = act.GetNumChildren();		
        for(int i=0; i < numNodes; i++)
        {
          if (!act(i).IsComment())
          {
            std::string l_szAction = (act(i).GetPszProperty("name"));
            CXMLTreeNode inputs = act(i)["action"];
            if (inputs.Exists())
            {
              int32 numNodesHijo = inputs.GetNumChildren();
              for(int j=0; j < numNodesHijo; j++)
              {
                if (inputs(j).IsComment())
                  continue;

                std::string dev= inputs(j).GetPszProperty("deviceType");        
                if(dev == "IDV_KEYBOARD")
                {
                  evt= inputs(j).GetPszProperty("EventType");        
                  cod= inputs(j).GetPszProperty("Code");
                  axs = "";
                }
                else if(dev == "IDV_MOUSE")
                {
                  if(inputs(j).GetPszProperty("EventType")!= NULL) 
                  {
                    evt= inputs(j).GetPszProperty("EventType");        
                    cod= inputs(j).GetPszProperty("Code");
                    axs = "";
                  }
                  if(inputs(j).GetPszProperty("AxisType")!= NULL) 
                  {
                    axs= inputs(j).GetPszProperty("AxisType");        
                    dlt= inputs(j).GetPszProperty("Delta");
                    evt = "";
                    cod = "";
                  }
                }
                else if(dev == "IDV_GAMEPAD")
                {

                }

                signed int l_iDev = GetValue(dev);
                signed int l_iAxi = GetValue(axs);
                float l_fValue = (float)atof(dlt.c_str());
                signed int l_iEvt = -1;
                signed int l_iCod = -1;
                if(l_iAxi == -1)
                {
                  l_iEvt = GetValue(evt);
                  l_iCod = GetValue(cod);
                }


                SInput *input = new SInput;
                input->dispositivo = l_iDev;

                if (cod != "")
                {
                  input->str_id = cod;
                }
                else
                {
                  input->str_id = axs;
                }

                if(l_iEvt != -1)//si hay evento
                {
                  input->accion = l_iEvt;
                  input->valor = l_iCod;
                  input->deltaRaton = 0;
                }
                else
                {
                  input->accion = l_iAxi;
                  input->valor = -1;
                  input->deltaRaton = l_fValue;
                }
                AddInput(l_szAction, input);
              }
            }
          }
        }  
      }
    }
  } 
  return m_bIsOk;
}

/// <summary>
/// Libera las accioens cargadas y las carga nuevamente.
/// </summary>
///<returns>Boleano que representa si la operación se realizó exitosamente.</returns>
bool CActionToInput::Reload ()
{
  if(Unload())
  {
    Load(m_filename);
    return true;
  }

  return false;
}

/// <summary>
/// Recorre el mapa de acciones liberando de memoria los inputs asociados-
/// </summary>
///<returns>Boleano que representa si la operación se realizó exitosamente.</returns>
bool CActionToInput::Unload ()
{
  TMapActions::iterator iter = m_InputsPerAction.begin();
  TMapActions::iterator itEnd = m_InputsPerAction.end();
  for(; iter != itEnd; ++iter)
  {
    TInputs::iterator itVEnd = iter->second.end();
    for(TInputs::iterator it = iter->second.begin(); it != itVEnd; ++it)
    {
      CHECKED_DELETE(*it);
    }
    iter->second.clear();
  }
  m_InputsPerAction.clear();

  return true;
}

/// <summary>
/// TODO: Guarda la configuracion de acciones (actual).
/// </summary>
///<returns>Boleano que representa si la operación se realizó exitosamente.</returns>
bool CActionToInput::SaveActions()
{
  return true;
}

/// <summary>
/// Mapea todos los posibles literales del XML (ej:"IDV_GAMEPAD1", "KEY_F12", ...)
/// asociandoles un codigo numerico int
/// </summary>
void CActionToInput::InitString2Input ()
{
  m_String2Code["IDV_KEYBOARD"] = IDV_KEYBOARD;
  m_String2Code["IDV_MOUSE"] = IDV_MOUSE;
  m_String2Code["IDV_GAMEPAD1"] = IDV_GAMEPAD1;
  m_String2Code["IDV_GAMEPAD2"] = IDV_GAMEPAD2;
  m_String2Code["IDV_GAMEPAD3"] = IDV_GAMEPAD3;
  m_String2Code["IDV_GAMEPAD4"] = IDV_GAMEPAD4;
  m_String2Code["IDV_NOTHING"] = IDV_NOTHING;

  //----------------INPUT_AXIS_TYPE-----------------//
  m_String2Code["AXIS_MOUSE_X"] = AXIS_MOUSE_X;
  m_String2Code["AXIS_MOUSE_Y"] = AXIS_MOUSE_Y;
  m_String2Code["AXIS_MOUSE_Z"] = AXIS_MOUSE_Z;
  m_String2Code["AXIS_LEFT_THUMB_X"] = AXIS_LEFT_THUMB_X;
  m_String2Code["AXIS_LEFT_THUMB_Y"] = AXIS_LEFT_THUMB_Y;
  m_String2Code["AXIS_RIGHT_THUMB_X"] = AXIS_RIGHT_THUMB_X;
  m_String2Code["AXIS_RIGHT_THUMB_Y"] = AXIS_RIGHT_THUMB_Y;
  m_String2Code["AXIS_DELTA_TRIGGER_RIGHT"] = AXIS_DELTA_TRIGGER_RIGHT;
  m_String2Code["AXIS_DELTA_TRIGGER_LEFT"] = AXIS_DELTA_TRIGGER_LEFT;
  m_String2Code["AXIS_NOTHING"] = AXIS_NOTHING;

  //----------------INPUT_EVENT_TYPE-----------------//
  m_String2Code["EVENT_DOWN"] = EVENT_DOWN;
  m_String2Code["EVENT_UP_DOWN"] = EVENT_UP_DOWN;
  m_String2Code["EVENT_DOWN_UP"] = EVENT_DOWN_UP;
  m_String2Code["EVENT_NOTHING"] = EVENT_NOTHING;

  //----------------MOUSE_BUTTON---------------------//
  m_String2Code["MOUSE_BUTTON_LEFT"] = MOUSE_BUTTON_LEFT;
  m_String2Code["MOUSE_BUTTON_RIGHT"] = MOUSE_BUTTON_RIGHT;
  m_String2Code["MOUSE_BUTTON_MIDDLE"] = MOUSE_BUTTON_MIDDLE;
  m_String2Code["MOUSE_BUTTON_MIDDLE"] = MOUSE_BUTTON_MIDDLE;
  m_String2Code["MOUSE_BUTTON_NOTHING"] = MOUSE_BUTTON_NOTHING;


  //----------------Gamepad codes from XInput.h------//
  m_String2Code["PAD_DPAD_UP"] = PAD_DPAD_UP;
  m_String2Code["PAD_DPAD_DOWN"] = PAD_DPAD_DOWN;
  m_String2Code["PAD_DPAD_LEFT"] = PAD_DPAD_LEFT;
  m_String2Code["PAD_DPAD_RIGHT"] = PAD_DPAD_RIGHT;
  m_String2Code["PAD_DPAD_START"] = PAD_DPAD_START;
  m_String2Code["PAD_DPAD_BACK"] = PAD_DPAD_BACK;
  m_String2Code["PAD_BUTTON_LEFT_THUMB"] = PAD_BUTTON_LEFT_THUMB;
  m_String2Code["PAD_BUTTON_RIGHT_THUMB"] = PAD_BUTTON_RIGHT_THUMB;
  m_String2Code["PAD_BUTTON_LEFT_SHOULDER"] = PAD_BUTTON_LEFT_SHOULDER;
  m_String2Code["PAD_BUTTON_RIGHT_SHOULDER"] = PAD_BUTTON_RIGHT_SHOULDER;
  m_String2Code["PAD_BUTTON_A"] = PAD_BUTTON_A;
  m_String2Code["PAD_BUTTON_B"] = PAD_BUTTON_B;
  m_String2Code["PAD_BUTTON_X"] = PAD_BUTTON_X;
  m_String2Code["PAD_BUTTON_Y"] = PAD_BUTTON_Y;

  //-----------Keyboard scan codes from dinput.h ------//
  m_String2Code["KEY_ESCAPE"] = KEY_ESCAPE;
  m_String2Code["KEY_TAB"] = KEY_TAB;
  m_String2Code["KEY_SPACE"] = KEY_SPACE;
  m_String2Code["KEY_RETURN"] = KEY_RETURN;
  m_String2Code["KEY_BACK"] = KEY_BACK;
  m_String2Code["KEY_CAPITAL"] = KEY_CAPITAL;
  m_String2Code["KEY_MINUS"] = KEY_MINUS;
  m_String2Code["KEY_EQUALS"] = KEY_EQUALS;
  m_String2Code["KEY_LBRACKET"] = KEY_LBRACKET;
  m_String2Code["KEY_RBRACKET"] = KEY_RBRACKET;
  m_String2Code["KEY_SEMICOLON"] = KEY_SEMICOLON;
  m_String2Code["KEY_APOSTROPHE"] = KEY_APOSTROPHE;
  m_String2Code["KEY_GRAVE"] = KEY_GRAVE;
  m_String2Code["KEY_BACKSLASH"] = KEY_BACKSLASH;
  m_String2Code["KEY_COMMA"] = KEY_COMMA;
  m_String2Code["KEY_PERIOD"] = KEY_PERIOD;
  m_String2Code["KEY_SLASH"] = KEY_SLASH;
  m_String2Code["KEY_A"] = KEY_A;
  m_String2Code["KEY_S"] = KEY_S;
  m_String2Code["KEY_D"] = KEY_D;
  m_String2Code["KEY_F"] = KEY_F;
  m_String2Code["KEY_G"] = KEY_G;
  m_String2Code["KEY_H"] = KEY_H;
  m_String2Code["KEY_J"] = KEY_J;
  m_String2Code["KEY_K"] = KEY_K;
  m_String2Code["KEY_L"] = KEY_L;
  m_String2Code["KEY_X"] = KEY_X;
  m_String2Code["KEY_C"] = KEY_C;
  m_String2Code["KEY_B"] = KEY_B;
  m_String2Code["KEY_N"] = KEY_N;
  m_String2Code["KEY_M"] = KEY_M;
  m_String2Code["KEY_Q"] = KEY_Q;
  m_String2Code["KEY_W"] = KEY_W;
  m_String2Code["KEY_E"] = KEY_E;
  m_String2Code["KEY_R"] = KEY_R;
  m_String2Code["KEY_T"] = KEY_T;
  m_String2Code["KEY_Y"] = KEY_Y;
  m_String2Code["KEY_U"] = KEY_U;
  m_String2Code["KEY_I"] = KEY_I;
  m_String2Code["KEY_O"] = KEY_O;
  m_String2Code["KEY_P"] = KEY_P;
  m_String2Code["KEY_1"] = KEY_1;
  m_String2Code["KEY_2"] = KEY_2;
  m_String2Code["KEY_3"] = KEY_3;
  m_String2Code["KEY_4"] = KEY_4;
  m_String2Code["KEY_5"] = KEY_5;
  m_String2Code["KEY_6"] = KEY_6;
  m_String2Code["KEY_7"] = KEY_7;
  m_String2Code["KEY_8"] = KEY_8;
  m_String2Code["KEY_9"] = KEY_9;
  m_String2Code["KEY_0"] = KEY_0;
  m_String2Code["KEY_F1"] = KEY_F1;
  m_String2Code["KEY_F2"] = KEY_F2;
  m_String2Code["KEY_F3"] = KEY_F3;
  m_String2Code["KEY_F4"] = KEY_F4;
  m_String2Code["KEY_F5"] = KEY_F5;
  m_String2Code["KEY_F6"] = KEY_F6;
  m_String2Code["KEY_F7"] = KEY_F7;
  m_String2Code["KEY_F8"] = KEY_F8;
  m_String2Code["KEY_F9"] = KEY_F9;
  m_String2Code["KEY_F10"] = KEY_F10;
  m_String2Code["KEY_F11"] = KEY_F11;
  m_String2Code["KEY_F12"] = KEY_F12;
  m_String2Code["KEY_NPPLUS"] = KEY_NPPLUS;
  m_String2Code["KEY_NPMINUS"] = KEY_NPMINUS;
  m_String2Code["KEY_NPDECIMAL"] = KEY_NPDECIMAL;
  m_String2Code["KEY_NPCOMMA"] = KEY_NPCOMMA;
  m_String2Code["KEY_NPDIVIDE"] = KEY_NPDIVIDE;
  m_String2Code["KEY_NPMULTIPLY"] = KEY_NPMULTIPLY;
  m_String2Code["KEY_NPENTER"] = KEY_NPENTER;
  m_String2Code["KEY_NUMLOCK"] = KEY_NUMLOCK;
  m_String2Code["KEY_NP1"] = KEY_NP1;
  m_String2Code["KEY_NP2"] = KEY_NP2;
  m_String2Code["KEY_NP3"] = KEY_NP3;
  m_String2Code["KEY_NP4"] = KEY_NP4;
  m_String2Code["KEY_NP5"] = KEY_NP5;
  m_String2Code["KEY_NP6"] = KEY_NP6;
  m_String2Code["KEY_NP7"] = KEY_NP7;
  m_String2Code["KEY_NP8"] = KEY_NP8;
  m_String2Code["KEY_NP9"] = KEY_NP9;
  m_String2Code["KEY_NP0"] = KEY_NP0;
  m_String2Code["KEY_RSHIFT"] = KEY_RSHIFT;
  m_String2Code["KEY_LSHIFT"] = KEY_LSHIFT;
  m_String2Code["KEY_RCTRL"] = KEY_RCTRL;
  m_String2Code["KEY_LCTRL"] = KEY_LCTRL;
  m_String2Code["KEY_RALT"] = KEY_RALT;
  m_String2Code["KEY_LALT"] = KEY_LALT;
  m_String2Code["KEY_LWIN"] = KEY_LWIN;
  m_String2Code["KEY_RWIN"] = KEY_RWIN;
  m_String2Code["KEY_UP"] = KEY_UP;
  m_String2Code["KEY_DOWN"] = KEY_DOWN;
  m_String2Code["KEY_LEFT"] = KEY_LEFT;
  m_String2Code["KEY_RIGHT"] = KEY_RIGHT;
  m_String2Code["KEY_INSERT"] = KEY_INSERT;
  m_String2Code["KEY_DELETE"] = KEY_DELETE;
  m_String2Code["KEY_HOME"] = KEY_HOME;
  m_String2Code["KEY_END"] = KEY_END;
  m_String2Code["KEY_PGDOWN"] = KEY_PGDOWN;
  m_String2Code["KEY_PGUP"] = KEY_PGUP;
  m_String2Code["KEY_PAUSE"] = KEY_PAUSE;
  m_String2Code["KEY_SCROLL"] = KEY_SCROLL;
}

/// <summary>
/// Asocia un input a la accion dada.
/// (se pueden asociar varios inputs a una accion haciendo varios llamados a esta funcion)
/// </summary>
///<param name="name">Nombre de la accion.</param>
///<param name="_psInput">Puntero al input a asociar.</param>
void CActionToInput::AddInput(const std::string &name, SInput *_psInput)
{
  TMapActions::iterator iter = m_InputsPerAction.find(name);

  if(iter == m_InputsPerAction.end())
	{
    TInputs l_vNewInputs;
    l_vNewInputs.push_back(_psInput);
    m_InputsPerAction[name] = l_vNewInputs;
	}
	else {
		iter->second.push_back(_psInput);
	}
}