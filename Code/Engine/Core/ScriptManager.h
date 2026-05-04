#ifndef SCRIPTMANAGER_H
#define SCRIPTMANAGER_H

#include <string>
#include <vector>

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

class CScriptManager
{
private:
  lua_State *m_LS;
public:
  CScriptManager();
  ~CScriptManager();
  void Initialize();
  void Destroy();
  void RunCode(const std::string &Code) const;
  void RunFile(const std::string &FileName) const;
  //void RunScript(const std::string &LuaCode) const;

  void LoadFile(const std::string &FileName) const;
  void CallChunk();


  void Load(const std::string &XMLFile);
  lua_State * GetLuaState() const {return m_LS;}
  void RegisterLUAFunctions();
  
private:
  void RegisterEngineFunctions();
  void RegisterCoreFunctions();
  void RegisterGUIFunctions();
  void RegisterCameraManagerFunctions();
  void RegisterCameraFunctions();
  void RegisterAnimatedModelManagerFunctions();
  void RegisterAnimatedCoreModelFunctions();
  void RegisterAnimatedInstanceModelFunctions();
	void RegisterPhysicsFunctions();
	void RegisterSoundFunctions();
	void RegisterParticleManagerFunctions();
	void RegisterEmitterFunctions();
	void RegisterItemFunctions();
	void RegisterItemManagerFunctions();
	void RegisterLightManagerFunctions();
	void RegisterBillboardManagerFunctions();
	void RegisterStaticMeshManagerFunctions();
	void RegisterGestionNivelManagerFunctions();
	void RegisterBillboardAnimationFunctions();
};

#endif
