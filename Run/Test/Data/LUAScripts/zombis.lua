-- //----------------------------------------------------------------------------------
-- // Logica Zombie.lua
-- // Author: Miquel Rojas
-- //
-- // Description:
-- // archivo con la logica de funcionamiento de los zombies.
-- // 
-- //----------------------------------------------------------------------------------


function z_char_center(_zombi_name)
	corefih():get_z_manager():get_resource(_zombi_name):z_get_physic_controller():set_position(Vect3f(0.0,0.0,0.0))
	log("zombie " .. _zombi_name .. " at (0,0,0) " )
end

function p_char_center()
--no vaç
--	get_core():get_physics_manager():get_resource("mi_amigo"):get_character_controller():set_position(Vect3f(0.0,0.0,0.0))
	player:set_position(Vect3f(0,0,0))
	log("zombie al centro")
end

function z_recibe_golpe(_zombi_name, danyo)
	local z = corefih():get_z_manager():get_resource(_zombi_name)
	-- log("zombie " .. _zombi_name .. " vida: " .. z:z_get_health() .. " estado: " .. z:z_get_state())
	if z:z_get_state() == 10 or z:z_get_state() == 23 then-- inactivo
		log("zombie " .. _zombi_name .. " estado desactivado ")
	else
--		if z:z_get_health() > 0 then
			--z:z_receive_damage(danyo
			if z:z_get_state() < 50 then -- no es zombie boss
				z:z_receive_damage(danyo)
				z:z_set_state(2)--recibegolpe-
				z:z_set_fase_estado(0) --init		
				_soundM:play_sound("zombieMoan")
			else --es zombi boss
				log("entra a golpear".. boss:z_get_state())
				log("status golpeado: "..glob_istatus)
				-- if boss:z_get_state() == 53 and (glob_istate == 0 or glob_istate == 1 or glob_istate == 2 or glob_istate == 3 )then
				
				if boss:z_get_state() == 53 and glob_istate == 0 then --embestida
					log("embestida".. boss:z_get_state() .. " " .. glob_istatus)
				elseif  boss:z_get_state() == 53 and glob_istatus == 1 then
					log("embestida1".. boss:z_get_state() .. " " .. glob_istate)
				elseif  boss:z_get_state() == 53 and glob_istatus == 2 then 
					log("embestida2".. boss:z_get_state() .. " " .. glob_istate)
				elseif  boss:z_get_state() == 53 and glob_istatus == 3 then
					log("embestida3".. boss:z_get_state() .. " " .. glob_istatus)
				else 
					log("ha golpeado".. boss:z_get_state() .. " " .. glob_istatus)
					boss:z_set_state(57)--recibegolpe zombie boss
					if not boss:b_get_invencible() then			
						boss:z_receive_damage(danyo)
					end
				end
			end
--		end
		log("zombie " .. _zombi_name .. " vida: " .. z:z_get_health() .. " estado: " .. z:z_get_state())
			
	end
end

function z_restart(_zombi_name)
	local z = corefih():get_z_manager():get_resource(_zombi_name)
	z:z_set_health(100)
	--z:z_get_instance():get_animated_instance_model():clear_cycle(3)
	--z:z_get_instance():get_animated_instance_model():remove_action(3)
	z:z_get_instance():get_animated_instance_model():execute_action(0,0.5)
	z:z_set_state(0)
	z:z_get_physic_controller():get_userdata():set_paint(true)
	z:z_get_physic_controller():set_position(Vect3f(10.0,0.0,0.0))
	log("zombie " .. _zombi_name .. " restart " )
	z:z_get_instance():set_visible(true)
end

--obsoleta
-- function sub_life_char(_zombi_name)
-- --	gui_hi:hit(g_health, g_damage)
	 -- local z = corefih():get_z_manager():get_resource(_zombi_name)

	 -- local vida_act = z:z_get_health()
	 
	 -- vida_act = vida_act - 20
	 -- z:z_set_health(vida_act)
	 -- log("zombie " .. _zombi_name .. " vida: " .. z:z_get_health() )

-- end

function golpea_mano_derecha(_trigger_name, _perjudicado)
	log(_trigger_name .. "-" .. _perjudicado)
	 --local z = corefih():get_z_manager():get_resource(_perjudicado)
	 local trigger = get_trigger(_trigger_name)

	if trigger:get_active() == true then
		--local vida_act = player:c_get_health()
	 
		--vida_act = vida_act - 20
		--player:c_set_health(vida_act)
		if player:c_get_health() > 0 then 
			get_core():get_sound_manager():play_sound("zombieAttack")
			player:hit(10.0)
		end
		log("el perjudicado " .. _perjudicado .. " tiene vida: " .. player:c_get_health() .. " se la han quitado con la mano derecha")
	end
end


function golpea_mano_izquierda(_trigger_name, _perjudicado)
	log(_trigger_name .. "-" .. _perjudicado)	
	--local z = corefih():get_z_manager():get_resource(_perjudicado)
	 local trigger = get_trigger(_trigger_name)

	if trigger:get_active() == true then
		--local vida_act = player:c_get_health()
	 
		--vida_act = vida_act - 20
		--player:c_set_health(vida_act)
		if player:c_get_health() > 0 and player:collide_enemies() == true then 
			_soundM:play_sound("zombieAttack")
			player:hit(10.0)
		end
		log("el perjudicado " .. _perjudicado .. " tiene vida: " .. player:c_get_health() .. " se la han quitado con la mano izquierda")
	end 
end

function z_animacion(_zombi_name, _id_anim, _time_anim)
	corefih():get_z_manager():get_resource(_zombi_name):z_get_instance():get_animated_instance_model():blend_cycle(_id_anim, _time_anim)
	log("zombie " .. _zombi_name .. " animacion: " .. _id_anim)
end

function ataque_zombie(_zombi_name, _tiempo_anim, _duracion)

	local z = corefih():get_z_manager():get_resource(_zombi_name)
    -- // 0 --anim--------- get_duration
    -- // 0 ----x---------- frames_total

	-- valor aproximado lo tiene que dar javi
	local frames_total = 77
	
	--calculo frame actual
	local frames_anim_calculado = _tiempo_anim * frames_total
	frames_anim_calculado = frames_anim_calculado / _duracion
	
	--intervalo trigger dependiendo de los fps
--	local tiempo_disparo_trigger = 0.5

	-- empieza golpe derecha
	if frames_anim_calculado >= 20.0 and frames_anim_calculado <= 30.0 then
		log("mano derecha activa")
		local nombre = _zombi_name .. " ManoDerecha"
		get_trigger(nombre):set_active(true)
		z:z_posiciona_mano_derecha()	
	end
	-- finaliza golpe derecha	
	if frames_anim_calculado >= 30.0 and frames_anim_calculado <= 50.0 then
		log("mano derecha desactiva")
		local nombre = _zombi_name .. " ManoDerecha"
		get_trigger(nombre):set_active(false)
		z:z_esconde_punyos()
	end
	-- empieza golpe izquierda
	if frames_anim_calculado >= 53.0 and frames_anim_calculado <= 70.0 then
		log("mano izquierda activa")
		local nombre = _zombi_name .. " ManoIzquierda"
		get_trigger(nombre):set_active(true)
		z:z_posiciona_mano_izquierda()
	end
	-- finaliza golpe izquierda
	if frames_anim_calculado >= 70.0 and frames_anim_calculado <= 75.0 then
		log("mano izquierda desactiva")
		local nombre = _zombi_name .. " ManoIzquierda"
		get_trigger(nombre):set_active(false)
		z:z_esconde_punyos()
	end
end

function activa_zombi(_zombi_name)
	-- si no esta muerto lo activa
	local estado = corefih():get_z_manager():get_resource(_zombi_name):z_get_state()
	if estado > -1 then 
		corefih():get_z_manager():get_resource(_zombi_name):z_set_state(5) -- state = idle
	end
end

function desactiva_zombi(_zombi_name)
	-- si no esta muerto lo desactiva
	local estado = corefih():get_z_manager():get_resource(_zombi_name):z_get_state()
	if  estado > -1 then 
		corefih():get_z_manager():get_resource(_zombi_name):z_get_instance():
		corefih():get_z_manager():get_resource(_zombi_name):z_set_state(10) -- state = inactivo
	end
end

function crea_zombie(_type, _name, _initPos, _initRot)
	corefih():get_z_manager():create_zombie(_type, _name, _initPos, _initRot)
	log("creado zombi " .. _name .. " de tipo: ".. _type)
end

function destruye_zombie( _zombi_name)
	corefih():get_z_manager():destroy_zombie(_zombie_name)
	log("destruido zombi " .. _zombie_name)
end

