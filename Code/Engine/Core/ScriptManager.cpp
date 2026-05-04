#include "ScriptManager.h"
#include "Base.h"
#include <luabind/luabind.hpp>
#include <luabind/function.hpp>
#include <luabind/class.hpp>
#include <luabind/operator.hpp>
#include "Engine.h"
#include "Core.h"
#include "MapManager/MapManager.h"
#include "Object3D/Object3D.h"
#include "Lights/LightManager.h"
//#include "Math/Color.h"
#include "Lights/Light.h"
#include "Math/Vector3.h"
#include "RenderableObjects/RenderableObjectsManager.h"
#include "RenderableObjects/RenderableObject.h"
#include "RenderableObjects/RenderableAnimatedInstanceModel.h"
#include "GUIManager.h"
#include "RenderManager.h"
#include "FontManager/FontManager.h"
#include "LanguageManager/LanguageManager.h"
#include "InputManager.h"
#include "StaticMeshes/StaticMeshManager.h"
#include "AnimationModel/AnimatedModelManager.h"
#include "Texture/TextureManager.h"
#include "Shaders/EffectManager.h"
#include "Timer/Timer.h"
#include "Cameras/CameraManager.h"
#include "Cameras/Camera.h"
#include "GUIManager.h"
#include "AnimationModel/AnimatedCoreModel.h"
#include "AnimationModel/AnimatedInstanceModel.h"
#include "SoundManager.h"
#include "Particles/ParticleManager.h"
#include "PhysicsManager.h"
#include "CharacterController/PhysicController.h"
#include "Actor/PhysicActor.h"
#include "TriggerReport/PhysicTriggerReport.h"
#include "Trigger/TriggerManager.h"
#include "Trigger/Trigger.h"
#include "Item.h"
#include "ItemManager.h"
#include "BillBoard/BillboardManager.h"
#include "BillBoard/BillboardAnimation.h"

#if defined( _DEBUG )
#include "Memory/MemLeaks.h"
#endif // defined( _DEBUG )

using namespace luabind;

#define REGISTER_LUA_FUNCTION(LUA_STATE, FunctionName,AddrFunction){luabind::module(LUA_STATE) [ luabind::def(FunctionName,AddrFunction) ];}



CScriptManager::CScriptManager() :m_LS(NULL) {}


CScriptManager::~CScriptManager()
{
  Destroy();
}

//Código de la función Alert que se llamará al generarse algún error de LUA
int Alert(lua_State * State)
{
  std::string l_Text;
  int n = lua_gettop(State);
  int i;
  lua_getglobal(State, "tostring");
  for (i=1; i<=n; i++) 
  {
    const char *s;
    lua_pushvalue(State, -1);
    lua_pushvalue(State, i);
    lua_call(State, 1, 1);
    s = lua_tostring(State, -1);

    if (s == NULL)
    {
      return luaL_error(State, "`tostring' must return a string to `print'");
    }

    if (i>1) 
    {
      l_Text += '\t';
    }

    l_Text += s;
    lua_pop(State, 1);
  }
  l_Text += '\n';
  LOGGER->AddNewLog(ELL_WARNING, l_Text.c_str());
  return true;
}

//Para inicializar el motor de LUA
void CScriptManager::Initialize()
{
  m_LS=lua_open();
  luaopen_base(m_LS);
  luaopen_string(m_LS);
  luaopen_table(m_LS);
  luaopen_math(m_LS);
  //luaopen_io(m_LS);
  luaopen_debug(m_LS);
  luabind::open(m_LS);
  //Sobreescribimos la función _ALERT de LUA cuando se genere algún error al ejecutar código LUA
  RegisterLUAFunctions();
  RegisterGUIFunctions();
  RegisterCameraManagerFunctions();
  RegisterCameraFunctions();
  RegisterAnimatedModelManagerFunctions();
	RegisterPhysicsFunctions();
  RegisterCoreFunctions();
  RegisterEngineFunctions();
  RegisterSoundFunctions();
	RegisterParticleManagerFunctions();
	RegisterEmitterFunctions();
	RegisterItemManagerFunctions();
	RegisterItemFunctions();
	RegisterLightManagerFunctions();
	RegisterBillboardManagerFunctions();
	RegisterStaticMeshManagerFunctions();
	RegisterBillboardAnimationFunctions();
}

//Para desinicializar el motor de LUA
void CScriptManager::Destroy()
{
  lua_close(m_LS);
}

//Para ejecutar un fragmento de código LUA
void CScriptManager::RunCode(const std::string &Code) const
{
  if(luaL_dostring(m_LS,Code.c_str()))
  {
    const char *l_Str = lua_tostring(m_LS, -1);
    LOGGER->AddNewLog(ELL_WARNING, "%s",l_Str);
  }
}

//Para cargar y ejecutar un fichero de código LUA
void CScriptManager::RunFile(const std::string &FileName) const
{
  if(luaL_dofile(m_LS, FileName.c_str()))
  {
    const char *l_Str=lua_tostring(m_LS, -1);
    LOGGER->AddNewLog(ELL_WARNING, "%s",l_Str);
  }
  else
  {
    LOGGER->AddNewLog(ELL_INFORMATION, "Se ejecutó el script: %s", FileName.c_str());
  }
}
//Para cargar un fichero de código LUA
void CScriptManager::LoadFile(const std::string &FileName) const
{
  if(luaL_loadfile(m_LS, FileName.c_str()))
  {
    const char *l_Str = lua_tostring(m_LS, -1);
    LOGGER->AddNewLog(ELL_WARNING, "%s",l_Str);
  }
}
//Para llamar el código LUA cargado con LoadFile y ejecutarlo
void CScriptManager::CallChunk() 
{
  lua_pushvalue(m_LS, -1); // Duplicate top item
  if(lua_pcall(m_LS,0, 0,0))
  {
    const char *l_Str=lua_tostring(m_LS, -1);
    LOGGER->AddNewLog(ELL_WARNING, "%s",l_Str);
  }
}

void LogText(const std::string &Text)
{
  LOGGER->AddNewLog(ELL_INFORMATION, "%s", Text.c_str());
  //const std::string &c = lua_tostring(_State,1);
  //return 1;
}
CCore * GetCore()
{
   return CORE;
}
CLight* GetLight(const std::string &LightName)
{
  return CORE->GetLigthManager()->GetResource(LightName);
}

CTrigger* GetTrigger(const std::string &TriggerName)
{
  return CORE->GetTriggerManager()->GetResource(TriggerName);
}

int GetNewId()
{
  static int l_KKId=0;
  l_KKId++;
  return l_KKId;
}
void CScriptManager::RegisterLUAFunctions()
{
  //para cuando hay un error en LUA
  lua_register(m_LS,"_ALERT",Alert);
  REGISTER_LUA_FUNCTION(m_LS, "log", LogText);
	REGISTER_LUA_FUNCTION(m_LS, "get_core", GetCore);
	REGISTER_LUA_FUNCTION(m_LS, "get_trigger", GetTrigger);
  REGISTER_LUA_FUNCTION(m_LS, "get_light", GetLight);

  //LUA Utils
  module (m_LS) [
    class_<Vect3f>("Vect3f")
      .def(constructor<const float, const float, const float>())
      .def(const_self + const_self)
      .def(const_self - const_self)
      .def(const_self * const_self)
      .def(const_self ^ const_self)
//      .def(const_self / const_self)
 //     .def(const_self = const_self)
      .def("v3f_rotatex", &Vect3f::RotateX)
      .def("v3f_get_rotatex", &Vect3f::GetRotatedX)
      .def("v3f_rotatey", &Vect3f::RotateY)
      .def("v3f_get_rotatey", &Vect3f::GetRotatedY)
      .def("v3f_rotatez", &Vect3f::RotateZ)
      .def("v3f_get_rotatez", &Vect3f::GetRotatedZ)
      .def("v3f_get_anglex", &Vect3f::GetAngleX)
      .def("v3f_get_angley", &Vect3f::GetAngleY)
      .def("v3f_get_anglez", &Vect3f::GetAngleZ)
      .def("v3f_get_angles", &Vect3f::GetAngles)
      .def_readwrite("x",&Vect3f::x)
      .def_readwrite("y",&Vect3f::y)
      .def_readwrite("z",&Vect3f::z)
  ];

  //module(m_LS) [
  //  class_< CLight, CObject3D>("CLight")
  //];

  module (m_LS) [
    class_<CMapManager<CLight>>("CMapManager")
      .def("get_resource", &CMapManager< CLight >::GetResource)
  ];

  //module(m_LS) [
  //  class_<CLightManager,CMapManager<CLight>>("CLightManager")
  //];

  //module(m_LS)
  //  [
  //    class_<CRenderableObject>("CRenderableObject"),class_<CRenderableAnimatedInstanceModel, CRenderableObject>("CRenderableAnimatedInstanceModel")
  //     .def("get_animated_instance_model", &CRenderableAnimatedInstanceModel::GetAnimatedInstanceModel)
  //  ];

  REGISTER_LUA_FUNCTION(m_LS,"get_new_id", &GetNewId);
}

void CScriptManager::RegisterEngineFunctions()
{
  module(m_LS)
	[
		// registramos la clase CEngine
    class_<CEngine>("CEngine")
      .def("get_timer", &CEngine::GetTimer)
  ];
}
void CScriptManager::RegisterCoreFunctions()
{
  module(m_LS)
	[
		// registramos la clase CCore
    class_<CCore>("CCore")
		.def("get_light_manager", &CCore::GetLigthManager)
      .def("get_gui_manager", &CCore::GetGUIManager)
      .def("get_renderable_objects_manager", &CCore::GetRenderableObjectsManager)
      .def("get_render_manager", &CCore::GetRenderManager)
      .def("get_font_manager", &CCore::GetFontManager)
      .def("get_language_manager", &CCore::GetLanguageManager)
      .def("get_input_manager", &CCore::GetInputManager)
      .def("get_static_mesh_manager", &CCore::GetStaticMeshManager)
      .def("get_animated_model_manager", &CCore::GetAnimatedModelManager)
      .def("get_texture_manager", &CCore::GetTextureManager)
      .def("get_effect_manager", &CCore::GetEffectManager)
      .def("get_timer", &CCore::GetTimer)
      .def("get_script_manager", &CCore::GetScriptManager)
      .def("get_camera_manager", &CCore::GetCameraManager)
			.def("get_physics_manager", &CCore::GetPhysicsManager)
			.def("get_sound_manager", &CCore::GetSoundManager)
			.def("get_particle_manager", &CCore::GetParticleManager)
      .def("get_trigger_manager", &CCore::GetTriggerManager)      
			.def("set_process", &CCore::SetProcess)
      .def("exit", &CCore::Exit)
			.def("get_item_manager", &CCore::GetItemManager)
			.def("get_billboard_manager", &CCore::GetBillboardManager)
  ];
}
void CScriptManager::RegisterGUIFunctions()
{
  module(m_LS)
    [
      // registramos la clase CGUIManager
      class_<CGUIManager>("CGUIManager")
      // registramos su constructor
      .def(constructor<const Vect2i&>())
      // registramos sus funciones publicas
      .def("active_windows", &CGUIManager::ActiveWindows)
      .def("active_windows_with_effect",&CGUIManager::ActiveWindowsWithEffect)
      .def("push_windows",&CGUIManager::PushWindows)
      .def("pop_windows",&CGUIManager::PopWindows)
      .def("set_screen_resolution",&CGUIManager::SetScreenResolution)
      .def("load_gui_files",&CGUIManager::LoadGuiFiles)
      .def("reload_gui",&CGUIManager::ReloadGuiFiles)
      .def("set_message_box",&CGUIManager::SetMessageBox)
      .def("is_visible_message",&CGUIManager::IsVisibleMessage)
      .def("set_visible_pointer_mouse",&CGUIManager::SetVisiblePointerMouse)
      //-------Funciones para modificar los GuiElements--------------------------		
      .def("set_active_gui_element",&CGUIManager::SetActiveGuiElement)
      .def("get_visible_gui_element",&CGUIManager::GetVisibleGuiElement)
      .def("set_visible_gui_element",&CGUIManager::SetVisibleGuiElement)
      .def("get_progress_bar_value",&CGUIManager::GetProgressBarValue)
      .def("set_progress_bar_value",&CGUIManager::SetProgressBarValue)
      .def("next_block_in_radio_box",&CGUIManager::NextBlockInRadioBox)
      .def("prev_block_in_radio_box",&CGUIManager::PrevBlockInRadioBox)
      .def("get_button_check_in_radio_box",&CGUIManager::GetButtonCheckInRadioBox)
      .def("set_button_check_in_radio_box",&CGUIManager::SetButtonCheckInRadioBox)
      .def("set_state_check_button",&CGUIManager::SetStateCheckButton)
      .def("get_state_check_button",&CGUIManager::GetStateCheckButton)
      .def("set_state_slider",&CGUIManager::SetStateSlider)
      .def("get_state_slider",&CGUIManager::GetStateSlider)
      .def("set_literal_in_static_text",&CGUIManager::SetLiteralInStaticText)
      .def("set_editable_text_box",&CGUIManager::SetEditableTextBox)
      .def("get_editable_text_box",&CGUIManager::GetEditableTextBox)
      .def("set_image",&CGUIManager::SetImage)
      .def("get_image",&CGUIManager::GetImage)			
      .def("play_image",&CGUIManager::PlayImage)
      .def("set_alpha_image",&CGUIManager::SetAlphaImage)			
      .def("fade_out_image",&CGUIManager::FadeOutImage)
    ];
}

void CScriptManager::RegisterPhysicsFunctions()
{
  module(m_LS)
    [
      // registramos la clase CPhysicsManager
      class_<CPhysicsManager>("CPhysicsManager")
			//--- Intersection Functions:
			.def(	"_raycast_closest_actor", &CPhysicsManager::RaycastClosestActor)
			.def(	"overlap_sphere_actor", &CPhysicsManager::OverlapSphereActor)
			//--- Get CookingMesh
//			.def(	"get_cooking_mesh", &CPhysicsManager::GetCookingMesh)
			//--- Rendering Stuff
			.def(	"debug_render", &CPhysicsManager::DebugRender)
			.def(	"set_debug_render", &CPhysicsManager::SetDebugRenderMode)
			.def(	"get_debug_render", &CPhysicsManager::GetDebugRenderMode)
			//----Update
			.def(	"update", &CPhysicsManager::Update)
			//--- Add/Release Actors
			.def(	"add_physic_actor", &CPhysicsManager::AddPhysicActor)
			.def(	"release_physic_actor", &CPhysicsManager::ReleasePhysicActor)
			//--- Add/Release CharacterControllers
			.def(	"add_physic_controller", &CPhysicsManager::AddPhysicController)
			.def(	"release_physic_controller", &CPhysicsManager::ReleasePhysicController)
			//--- Add/Release Joints
			.def(	"add_physic_spherical_joint", &CPhysicsManager::AddPhysicSphericalJoint)
			.def(	"release_spherical_joint", &CPhysicsManager::RelasePhysicSphericalJoint)
			.def(	"add_physic_revolute_joint", &CPhysicsManager::AddPhysicRevoluteJoint)
			.def(	"release_physic_revolute_joint", &CPhysicsManager::RelasePhysicRevoluteJoint)
			.def(	"add_physic_fixed_joint", &CPhysicsManager::AddPhysicFixedJoint)
			.def(	"release_physic_fixed_joint", &CPhysicsManager::RelasePhysicFixedJoint)
			//--- Create CCDSkeleton
//			.def(	"create_ccd_skeleton", &CPhysicsManager::CreateCCDSkeleton)
			//--- Triggers
			.def(	"set_trigger_report", &CPhysicsManager::SetTriggerReport)
      //--- InvisibleBoxes
      .def(	"set_invisible_box_position", &CPhysicsManager::SetInvisbleBoxPosition)
    ];

  module(m_LS)[
    class_<CNamed>("CNamed"),
    class_<CPhysicUserData, CNamed>("CPhysicUserdata")
      .def("get_paint", &CPhysicUserData::GetPaint)
      .def("set_paint", &CPhysicUserData::SetPaint)
      .def("get_color", &CPhysicUserData::GetColor)
      .def("set_color", &CPhysicUserData::SetColor)
  ];

  module(m_LS)[
    class_<CActive>("CActive"),
    class_<CTrigger, CActive>("CTrigger")
      .def("get_active", &CActive::GetActive)
      .def("set_active", &CActive::SetActive)
      .def("get_position", &CTrigger::GetPosition)
      .def("get_dimensions", &CTrigger::GetDimensions)
      .def("get_max", &CTrigger::GetMax)
      .def("get_min", &CTrigger::GetMin)
      .def("get_entity", &CTrigger::GetEntity)
      .def("get_on_enter_code", &CTrigger::GetOnEnterCode)
      .def("get_on_leave_code", &CTrigger::GetOnLeaveCode)
      .def("get_transformation", &CTrigger::GetTransformation)
      .def("set_position", &CTrigger::SetPosition)    
      .def("set_max", &CTrigger::SetMax)
      .def("set_min", &CTrigger::SetMin)
      .def("set_dimensions", &CTrigger::SetDimensions)
      .def("set_entity", &CTrigger::SetEntity)
      .def("set_on_enter_code", &CTrigger::SetOnEnterCode)
      .def("set_on_leave_code", &CTrigger::SetOnLeaveCode)
			.def("is_ok", &CTrigger::IsOk)
			.def("set_trigger_active", &CTrigger::SetTriggerActive)
			.def("get_trigger_active", &CTrigger::GetTriggerActive)
			.def("move_renderable_object", &CTrigger::MoveRenderableObject)
  ];

  //no consigo registrarla, preguntar a Jordi
  module(m_LS)[
    class_<CMapManager<CTrigger>>("CMapManager<CTrigger>")
      .def("get_resource", &CMapManager<CTrigger>::GetResource)
  ];

  module(m_LS)[
     //class_<CPhysicTriggerReport>("CPhysicTriggerReport"), 
     //class_<CTriggerManager, bases<CMapManager<CTrigger>/*, CPhysicTriggerReport*/>("CTriggerManager")
    class_<CTriggerManager,CMapManager<CTrigger>>("CTriggerManager")
			.def("get_trigger", &CTriggerManager::GetTrigger)
  ];  

  module(m_LS)[
      class_<CAnimatedInstanceModel,CNamed>("CAnimatedInstanceModel")
        .def("remove_action", &CAnimatedInstanceModel::RemoveAction)
        .def("execute_action", (void(CAnimatedInstanceModel::*)(int, float))&CAnimatedInstanceModel::ExecuteAction)
        .def("execute_action", (void(CAnimatedInstanceModel::*)(int, float, float))&CAnimatedInstanceModel::ExecuteAction)
        .def("execute_action_blocked", &CAnimatedInstanceModel::ExecuteActionBlocked)
        .def("blend_cycle", &CAnimatedInstanceModel::BlendCycle)
        .def("clear_cycle", &CAnimatedInstanceModel::ClearCycle)
        .def("get_current_cycle", &CAnimatedInstanceModel::GetCurrentCycle)
        .def("get_current_action", &CAnimatedInstanceModel::GetCurrentAction)
        .def("get_num_animations", &CAnimatedInstanceModel::GetNumAnimations)
        .def("get_model_cal3d", &CAnimatedInstanceModel::GetModelCal3DHW)
  ];

  module(m_LS)[
	    class_<CPhysicActor>("CPhysicActor")
        .def(constructor<CPhysicUserData*>())
        .def("get_user_data", &CPhysicActor::GetUserData)
        .def("set_linear_velocity", &CPhysicActor::SetLinearVelocity)
        .def("get_position", &CPhysicActor::GetPosition)
        .def("create_body", &CPhysicActor::CreateBody)
        .def("set_global_position", &CPhysicActor::SetGlobalPosition)
	    //---AddShape Functions-----
        .def("add_sphere_shape", &CPhysicActor::AddSphereShape)
        .def("add_box_shape", &CPhysicActor::AddBoxSphape)
        .def("add_capsule_shape", &CPhysicActor::AddCapsuleShape)
        .def("add_mesh_shape", &CPhysicActor::AddMeshShape)
        .def("add_plane_shape", &CPhysicActor::AddPlaneShape)
	    //---Trigger Function---
        .def("create_box_trigger", &CPhysicActor::CreateBoxTrigger)
	    //---Get Info-------
        .def("get_mat44", &CPhysicActor::GetMat44)
        .def("set_mat44", &CPhysicActor::SetMat44)
	    //---Get PhsX Info---
        .def("create_actor", &CPhysicActor::CreateActor) 
//        .def("get_phx_actor", &CPhysicActor::GetPhXActor)
//        .def("get_actor_desc", &CPhysicActor::GetActorDesc)
    ];

  module(m_LS)[
     class_<NxController>("NxController") 
  ];

  module(m_LS)[
     class_<CPhysicController>("CPhysicController") 
       .def("get_userdata", &CPhysicController::GetUserData)
       .def("set_collision", &CPhysicController::SetCollision)
       .def("move", &CPhysicController::Move)
       .def("get_position", &CPhysicController::GetPosition)
       .def("set_position", &CPhysicController::SetPosition)
       .def("jump", &CPhysicController::Jump)
       //UpdateCharacterExtents	(bool bent, float ammount);
	     //GetGravity							() const {return m_fGravity;}
	     // SetGravity							(float gravity)  {m_fGravity = gravity;}
       // GetTotalHeight          () { return m_fHeight_Capsule + m_fRadius_Capsule * 2; }
       //GetHeight               () { return m_fHeight_Capsule; }
       // GetRadius               () { return m_fRadius_Capsule; }
	//---PhsX Info---

       //.def("get_phx_controller", &CPhysicController::GetPhXController)
       //.def("get_phx_controller_desc", &CPhysicController::GetPhXControllerDesc)

       //CreateController				(NxController* controller, NxScene* scene);
  ];
}

void CScriptManager::RegisterCameraManagerFunctions()
{
  module(m_LS)
    [
      // registramos la clase CCameraManager
      class_<CCameraManager>("CCameraManager")
      .def("init", &CCameraManager::Init)
      .def("load", &CCameraManager::Load)
      .def("reload", &CCameraManager::Reload)
      .def("cleanup", &CCameraManager::CleanUp)
      .def("add_camera", &CCameraManager::AddCamera)
      .def("set_camera", &CCameraManager::SetCamera)
      .def("reset_to_default_camera", &CCameraManager::ResetToDefaultCamera)
      .def("draw_cameras", &CCameraManager::DrawCameras)
      .def("get_camera", &CCameraManager::GetCamera)
      .def("get_camera_by_name", &CCameraManager::GetCameraByName)
    ];
}
void CScriptManager::RegisterCameraFunctions()
{
  module(m_LS)
    [
      // registramos la clase CCamera
      class_<CCamera>("CCamera")
      .def("get_object3d", &CCamera::GetObject3D)
      .def("get_camera_name", &CCamera::GetName)
      .def("get_direction", &CCamera::GetDirection)
      .def("get_look_at", &CCamera::GetLookAt)
      .def("get_get_eye", &CCamera::GetEye)
      .def("get_get_vect_up", &CCamera::GetVecUp)
      .def("camera_shake", &CCamera::CameraShake)
      .def("init_shake", &CCamera::InitShake)
    ];
}

void CScriptManager::RegisterAnimatedModelManagerFunctions()
{
  module(m_LS) [
    class_<CRenderableObjectsManager>("CRenderableObjectsManager")
      .def("add_mesh_instance", &CRenderableObjectsManager::AddMeshInstance)
			.def("get_instance_as_animated_instance_model", &CRenderableObjectsManager::GetInstanceAsAnimatedInstanceModel)
			.def("release", &CRenderableObjectsManager::Release)
  ];

  module(m_LS)
    [
      // registramos la clase CAnimatedModelManager
      class_<CAnimatedModelManager>("CAnimatedModelManager")
      .def("get_core", &CAnimatedModelManager::GetCore)
      .def("get_instance", &CAnimatedModelManager::GetInstance)
      .def("load", &CAnimatedModelManager::Load)
      .def("get_total_num_faces", &CAnimatedModelManager::GetTotalNumFaces)
			.def("release", &CAnimatedModelManager::Destroy)
    ];
  
  module(m_LS)
    [
      class_<CObject3D>("CObject3D"),
      class_<CRenderableObject, CObject3D>("CRenderableObject"),
      class_<CRenderableAnimatedInstanceModel, CRenderableObject>("CRenderableAnimatedInstanceModel")
       .def("set_yaw", &CObject3D::SetYaw)
       .def("set_pitch", &CObject3D::SetPitch)
       .def("set_roll", &CObject3D::SetRoll)
       .def("get_yaw", &CObject3D::GetYaw)
       .def("get_pitch", &CObject3D::GetPitch)
       .def("get_roll", &CObject3D::GetRoll)
       .def("set_position", &CObject3D::SetPosition)
       .def("get_position", &CObject3D::GetPosition)
       .def("get_animated_instance_model", &CRenderableAnimatedInstanceModel::GetAnimatedInstanceModel)
       .def("set_visible", &CObject3D::SetVisible)
       .def("get_visible", &CObject3D::GetVisible)
    ];
}

void CScriptManager::RegisterSoundFunctions()
{
  module(m_LS)
    [
     class_<CSoundManager>("CSoundManager")
      //.def("play_sample", &CSoundManager::PlaySample)
      //.def("play_music", &CSoundManager::PlayMusic)
      
      .def("fade_out", &CSoundManager::FadeOut)
      .def("fade_in", &CSoundManager::FadeIn)
			.def("release", &CSoundManager::Release)
			.def("test_sound", &CSoundManager::TestSound)
			.def("play_sound", &CSoundManager::PlaySound)
			.def("play_sound_volume", &CSoundManager::PlaySoundVolume)
      .def("cross_fade", &CSoundManager::CrossFade)
			.def("pause", &CSoundManager::Pause)	
			.def("resume", &CSoundManager::Resume)	

    ];
}
void CScriptManager::RegisterAnimatedCoreModelFunctions()
{
  module(m_LS)
    [
      // registramos la clase CAnimatedCoreModel
      class_<CAnimatedCoreModel>("CAnimatedCoreModel")
      .def("get_cal_hd_model", &CAnimatedCoreModel::GetCalHardwareModel)
      .def("load_vertex_buffer", &CAnimatedCoreModel::LoadVertexBuffer)
      .def("get_rederable_vertex", &CAnimatedCoreModel::GetRenderableVertexs)
      .def("get_core_model", &CAnimatedCoreModel::GetCoreModel)
      .def("get_texture_name", &CAnimatedCoreModel::GetTextureName)
      .def("get_num_textures", &CAnimatedCoreModel::GetNumTextures)
      .def("load", &CAnimatedCoreModel::Load)
      .def("get_num_vertex", &CAnimatedCoreModel::GetNumVertex)
      .def("get_num_faces", &CAnimatedCoreModel::GetNumFaces)
    ];
}

void CScriptManager::RegisterParticleManagerFunctions()
{
	module(m_LS)
		[
			//registramos la clase CParticleManager
			class_<CParticleManager>("CParticleManager")
			.def("get_emitter", &CParticleManager::GetEmitter)
			.def("release", &CParticleManager::Release)
		];
}

void CScriptManager::RegisterEmitterFunctions()
{
	module(m_LS)
		[
			//registramos la clase CParticleEmitter
			class_<CParticleEmitter>("CParticleEmitter")
			.def("get_pos", &CParticleEmitter::GetPos)
			.def("get_pos1", &CParticleEmitter::GetPos1)
			.def("get_pos2", &CParticleEmitter::GetPos2)
			.def("get_color1", &CParticleEmitter::GetColor1)
			.def("get_color2", &CParticleEmitter::GetColor2)
			.def("get_min_size", &CParticleEmitter::GetMinSize)
			.def("get_max_size", &CParticleEmitter::GetMaxSize)
			.def("get_dir1", &CParticleEmitter::GetSpawnDir1)
			.def("get_dir2", &CParticleEmitter::GetSpawnDir2)
			.def("get_min_emit_rate", &CParticleEmitter::GetMinEmitRate)
			.def("get_max_emit_rate", &CParticleEmitter::GetMaxEmitRate)
			.def("get_min_life", &CParticleEmitter::GetMinLife)
			.def("get_max_life", &CParticleEmitter::GetMaxLife)
			.def("get_texture", &CParticleEmitter::GetTexture)
			.def("set_pos", &CParticleEmitter::SetPos)
			.def("set_pos1", &CParticleEmitter::SetPos1)
			.def("set_pos2", &CParticleEmitter::SetPos2)
			.def("set_color1", &CParticleEmitter::SetColor1)
			.def("set_color2", &CParticleEmitter::SetColor2)
			.def("set_min_size", &CParticleEmitter::SetMinSize)
			.def("set_max_size", &CParticleEmitter::SetMaxSize)
			.def("set_dir1", &CParticleEmitter::SetSpawnDir1)
			.def("set_dir2", &CParticleEmitter::SetSpawnDir2)
			.def("set_min_emit_rate", &CParticleEmitter::SetMinEmitRate)
			.def("set_max_emit_rate", &CParticleEmitter::GetMaxEmitRate)
			.def("set_min_life", &CParticleEmitter::SetMinLife)
			.def("set_max_life", &CParticleEmitter::SetMaxLife)
			.def("set_texture", &CParticleEmitter::SetTexture)
			.def("de_init", &CParticleEmitter::DeInit)
			.def("done", &CParticleEmitter::Done)
			.def("render", &CParticleEmitter::Render)
			.def("update", &CParticleEmitter::Update)
		];
}


void CScriptManager::RegisterItemManagerFunctions()
{
		module(m_LS)
		[
			//registramos la clase CItemManager
			class_<CItemManager>("CItemManager")
			.def("check_colisions", &CItemManager::CheckColisions)
			.def("add_item", &CItemManager::AddItem)
			.def("update", &CItemManager::Update)
		];
}

void CScriptManager::RegisterItemFunctions()
{
		module(m_LS)
		[
			//registramos la clase CItem
			class_<CItem>("CItem")
			.def("create_item", &CItem::CreateItem)
			.def("get_name", &CItem::GetName)
			.def("update", &CItem::Update)
			.def("set_name", &CItem::SetName)
		];
}

void CScriptManager::RegisterLightManagerFunctions()
{
  module(m_LS)
    [
      // registramos la clase CLightManager
      class_<CLightManager>("CLightManager")
			.def("turn_light", &CLightManager::TurnLight)
			.def("on_off_lights", &CLightManager::OnOffLights)
    ];
}

void CScriptManager::RegisterBillboardManagerFunctions()
{
  module(m_LS)
    [
      // registramos la clase CBillboardManager
      class_<CBillboardManager>("CBillboardManager")
			.def("release", &CBillboardManager::Release)
			.def("get_billboard_animation", &CBillboardManager::GetBillboardAnimation)
			.def("render_by_id", &CBillboardManager::RenderById)
			.def("get_b_animation_by_instance", &CBillboardManager::GetBAnimationByInstance)
			.def("set_active", &CBillboardManager::SetActive)
    ];
}

void CScriptManager::RegisterBillboardAnimationFunctions()
{
	module(m_LS)
		[
			//registramos la clase CBillboardAnimation
			class_<CBillboardAnimation>("CBillboardAnimation")
			.def("set_pos", &CBillboardAnimation::SetPos)
			.def("set_end", &CBillboardAnimation::SetEnd)
			.def("get_texture_act", &CBillboardAnimation::GetTextureAct)
		];
}
				
void CScriptManager::RegisterStaticMeshManagerFunctions()
{
  module(m_LS)
    [
      // registramos la clase CStaticMeshManager
      class_<CStaticMeshManager>("CStaticMeshManager")
			.def("release", &CStaticMeshManager::Destroy)
    ];
}						
