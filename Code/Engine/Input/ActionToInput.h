//----------------------------------------------------------------------------------
// ActionToInput Class
// Author: Miquel Rojas
//
// Description:
// Lectura y tratamiento de acciones realizadas por dispositivos de entrada.
//----------------------------------------------------------------------------------
#pragma once

#ifndef ACTIONTOINPUT_H_
#define ACTIONTOINPUT_H_

#include "Base.h"
#include <vector>
#include <string>
#include <map>

/// <summary>
/// Almacena la información de cada input del XML
/// </summary>
typedef struct
{
  signed short  dispositivo;  // Teclado, raton o gamepad
  signed short  accion;       // Evento o axis
  signed short  valor;        // Tecla
  float         deltaRaton;   // MouseDelta
  std::string   str_id;       // Campo code leido del XML (ej: "KEY_K"). Si no tiene code uso el AxisType, como en el caso del mouse
}SInput;


/// <summary>
/// Gestiona la correspondencia entre nombres de acciones e inputs
/// </summary>
class CActionToInput
{
  public:
    // Init and End protocols
    CActionToInput() : m_bIsOk(false), m_filename(""){}
    virtual ~CActionToInput () { Done(); }

    virtual	bool				Init								();
    virtual void				Done               	();
    bool								IsOk               	() const { return m_bIsOk; }
    bool                Load                (std::string file);//carga el xml y lee/convierte acciones
    bool                Reload              ();             //libera y load
    bool                Unload              ();             //libera
    bool                SaveActions         ();             //graba la configuracion actual
    void                GetActionConfig     (std::string &config, std::string action);//devuelve la cadena con la configuracion de una accion
    float               DoAction            (std::string _myAction);

  private:
    typedef std::vector<SInput*>            TInputs;
    typedef std::map<std::string, TInputs>	TMapActions;
		  
    bool                                    m_bIsOk;           // Initialization boolean control
    TMapActions	  												  m_InputsPerAction;  // Mapa con los inputs asociados a cada accion
    std::string                             m_filename;         // Nombre del fichero XML
    std::map<std::string, uint32>           m_String2Code;      // Mapa con los valores numericos asociados a cada literal del XML
    
    void        				Release   					();
    void                InitString2Input    ();
    int                 GetValue            (std::string lit);  //devuelve el valor del literal de m_String2Code
    void                AddInput            (const std::string &name, SInput* input);
};

#endif