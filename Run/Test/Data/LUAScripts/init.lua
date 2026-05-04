--local value=Vect3f(50, 10, 0)
--get_core():get_light_manager():get_resource("Spot01"):set_position(value)

--require "variables"


function tumba()
	local l_position=Vect3f(-25+math.random()*50,0.0, -25+math.random()*50)
	local new_id=get_new_id()
	local scale_factor = math.random()*2
	get_core():get_renderable_objects_manager():add_mesh_instance("tumba","my_instance_"..new_id, l_position, math.random()*6.2831, 0.0, 0.0, scale_factor, scale_factor, scale_factor)
end

function triggered_sound()
	_soundM:play_sound("fireLoopMono")
	end

function tumbaAlta()
	local l_position=Vect3f(-25+math.random()*50,5, -25+math.random()*50)
	local new_id=get_new_id()
	get_core():get_renderable_objects_manager():add_mesh_instance("tumba","my_instance_"..new_id, l_position, 0.0, 0.0, 0.0, 1, 1, 1)
end

-- FUNCIONES DE CAMARAS
function temblorCamara()
	--local objeto_camara = get_core():get_camera_manager():get_camera():get_object3d()
	--objeto_camara:set_roll(0.5)
	_core:get_camera_manager():get_camera():init_shake()
end

-- FIN FUNCIONES DE CAMARAS

-- FUNCIONES DE GUI -------------------------------
function gui_show_dialog(id) 
	_guiM:set_image('gui_dialog', id)
	_guiM:set_visible_gui_element('gui_dialog', true)
	_guiM:set_alpha_image('gui_dialog', 1.0)
	_guiM:fade_out_image('gui_dialog', 2.3, 0.3)
end

function gui_show_blood() 
	if _guiM:get_visible_gui_element('gui_blood') then
		if g_gui_blood_state < 3 then
			g_gui_blood_state = g_gui_blood_state + 1
			gui_show_blood_by_id(tostring(g_gui_blood_state)) 
		end
	elseif g_gui_blood_state == 3 then
		g_gui_blood_state = 0
	else
		g_gui_blood_state = 1
		gui_show_blood_by_id(tostring(g_gui_blood_state)) 
	end
end

function gui_show_blood_by_id(id) 
	_guiM:set_image('gui_blood', id)
	_guiM:set_visible_gui_element('gui_blood', true)
	_guiM:set_alpha_image('gui_blood', 1.0)
	_guiM:fade_out_image('gui_blood', 0.3, 4.0)
end

