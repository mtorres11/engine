-- GUI -------------------------------------------
g_gui_blood_state = 0

--declaración de todas las variables mánager
_core = get_core()
_guiM = _core:get_gui_manager()
_soundM = _core:get_sound_manager()
_particleM = _core:get_particle_manager()
_lightM = _core:get_light_manager()
_physicsM = _core:get_physics_manager()
_billboardM = _core:get_billboard_manager()
_animatedModelM = _core:get_animated_model_manager()
_renderableObjM = _core:get_renderable_objects_manager()
_staticMeshM = _core:get_static_mesh_manager()

--[[_renderM = _core:get_render_manager()
_objectsM = _core:get_renderable_objects_manager()
_fontM = _core:get_font_manager()
_languageM = _core:get_language_manager()
_inputM = _core:get_input_manager()
_staticmeshM = _core:get_static_mesh_manager()
_animatedM = _core:get_animated_model_manager()
_textureM = _core:get_texture_manager()
_effectM = _core:get_effect_manager()
_timerM = _core:get_timer()
_scriptM = _core:get_script_manager()
_cameraM = _core:get_camera_manager()
--]]
_triggerM = _core:get_trigger_manager()

g_state = "init"