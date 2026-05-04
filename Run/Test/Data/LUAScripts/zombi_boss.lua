-- //----------------------------------------------------------------------------------
-- // Logica ZombieBoss.lua
-- // Author: Miquel Rojas
-- //
-- // Description:
-- // archivo con la logica de funcionamiento del zombie Boss relativa a su IA.
-- // 
-- //----------------------------------------------------------------------------------

v_visible = true
glob_bgolpe_suelo = false
glob_fcontrol_parpadeo = 0.0
glob_istatus = -1

function flash_effect( _dt )
	glob_fcontrol_parpadeo = glob_fcontrol_parpadeo + _dt
	if glob_fcontrol_parpadeo > 0.10 then
		v_visible = not v_visible
		boss:z_get_instance():set_visible(v_visible)
		glob_fcontrol_parpadeo = 0.0
	end
end

function boss_visible()
	v_visible = true
	boss:z_get_instance():set_visible(v_visible)
end


function res_pos()
	local p = boss:z_get_pos_ini()
	local s = p.y + 10.0
	
	log ("p " ..p.y)
	log ("p+ " ..s)
	
	local v = Vect3f(p.x, s, p.z)
	boss:z_get_physic_controller():set_position(v)
end

---------------------------------------------------------------------------------
-- ESTADO: idle
---------------------------------------------------------------------------------

function update_idle( _dt )
	--log("ESTADO IN: idle")
	
	posiciona_instancia()  

	--control de invencibilidad
	if boss:b_get_invencible() then
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect( _dt )
			boss:b_acum_dt_invencible( _dt )
		end
	end
	
	if g_state == "init" then
		--inicia eje ia
--		boss:z_get_eje_ia():reset_datos_inicializacion()
--		boss:z_persigue( player:get_pos(), _dt, 1)
		--esconde trigger
		boss:b_esconde_trigger_arma()	

		local p = player:get_pos()
		boss:z_get_instance():get_animated_instance_model():blend_cycle( 0 , _dt,1.0)	
		if boss:b_fin_t_encarando() then 
			boss:b_reset_t_encarando()
			g_state = "update"
		else
			boss:b_acum_dt_encarando( _dt )
			boss:z_encarando( p, _dt, 1)
--			log("ESTADO IN: ataque con arma (Encarando)")
		end

		boss:b_reset_t_tras_golpe()
		g_state = "update"
		--por si viene de la embestida
		boss:z_set_rad_view( boss:b_get_radius_view_original() )
		log("radius original: "..boss:b_get_radius_view_original())
	elseif g_state == "update" then
	  if boss:b_fin_t_tras_golpe() then
		 local a = boss:z_get_eje_ia():get_vector_director()
		 local b = boss:z_get_eje_ia():get_vector_director_optimo()
		
		  if a * b > 0 then
		  --TODO..CAMBIA ESTADO EN RETIRADA
			  log("ESTADO OUT: Perseguir")
			  boss:z_set_state(59) --Estado Perseguir
			  boss:z_get_userdata():set_current_action(59)--perseguir
			  g_state = "init"
		  else
			  log("ESTADO OUT: Retirada")
			  boss:z_set_state(60) --Estado Retirada
			  boss:z_get_userdata():set_current_action(60)--retirada
			  g_state = "init"
		  end
	  else 
		  --log("acumula dt tras golpe")	
		  posiciona_instancia()  
		  boss:b_acum_dt_tras_golpe( _dt )
		  boss:z_get_instance():get_animated_instance_model():blend_cycle( 0 , _dt,1.0)	
	  end	
	end
end


---------------------------------------------------------------------------------
-- ESTADO: ataque con arma
---------------------------------------------------------------------------------
function update_ataque_con_arma( _dt )
	posiciona_instancia()

	--control de invencibilidad
	if boss:b_get_invencible() then
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect( _dt )
			boss:b_acum_dt_invencible( _dt )
		end
	end
	
	if boss:z_get_eje_ia():comprueba_dentro_area_vision(boss:z_get_rad_view(), 45.0) then
--		log("ESTADO IN: ataque con arma (Atacando)")
		if g_state == "init" then
			--animacion ataque con arma
			boss:z_get_instance():get_animated_instance_model():execute_action(1 ,0.0)
			boss:z_get_userdata():set_current_action(51)--ataque arma
			_soundM:play_sound("ataqueArma")
			g_state = "update"
		elseif g_state == "update" then
			-- posiciona trigger arma
			boss:b_posiciona_arma()
			-- comprueba fin animacion ataque con arma
			if boss:b_fin_t_anim_ataque_arma() then 
				--esconde trigger
				boss:b_esconde_trigger_arma()
				boss:b_reset_t_anim_ataque_arma()
				log("ESTADO OUT: idle")
				boss:z_set_state(50)
				boss:z_get_userdata():set_current_action(50)--idle
				g_state = "init"
				glob_bgolpe_suelo = false

			else	
--				log("acumula dt anim ataque arma")
				boss:b_acum_dt_anim_ataque_arma( _dt )
				if boss:b_momento_golpe() and glob_bgolpe_suelo == false then
					temblorCamara()
					glob_bgolpe_suelo = true
				end
			end
		end
	else
		local p = player:get_pos()
		boss:z_get_instance():get_animated_instance_model():blend_cycle( 0 , _dt,1.0)	
		if boss:b_fin_t_encarando() then 
			log("ESTADO OUT: idle")
			boss:b_reset_t_encarando()
			boss:z_set_state(50)
			boss:z_get_userdata():set_current_action(50)--idle
			g_state = "init"
		else
			boss:b_acum_dt_encarando( _dt )
			boss:z_encarando( p, _dt, 1)
--			log("ESTADO IN: ataque con arma (Encarando)")
		end
	end
end

---------------------------------------------------------------------------------
-- ESTADO: embestida
---------------------------------------------------------------------------------
function update_embestida( _dt )
	log("status: "..glob_istatus)
	--controller en la posicion del hueso cuando la animacion es 'crashed'
	if g_state == "crashed2idle" then
		boss:b_posiciona_torso()
		local aux = boss:b_get_p_torso()
		-- log("instance x: ".. aux.x .. " y: ".. aux.y .. " z: " .. aux.z)
		boss:z_get_physic_controller():set_position(aux)	
		
	else
		posiciona_instancia()
	end

	--control de invencibilidad
	if boss:b_get_invencible() then
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect( _dt )
			boss:b_acum_dt_invencible( _dt )
		end
	end

	--log("g_State" .. g_state)
	--log("action: " .. boss:z_get_userdata():get_is_action_finalized())
	boss:z_set_rad_view( boss:z_get_rad_smell())
	if boss:z_get_eje_ia():comprueba_dentro_area_vision(boss:z_get_rad_view(), 25.0) then
		if g_state == "init" then
			glob_istatus = 0
--			log("ESTADO IN: embestida (Atacando)")
			--animacion pre run embestida
			boss:z_get_instance():get_animated_instance_model():execute_action(2 ,0.2) -- prerun
			boss:z_get_userdata():set_is_action_finalized(2)
			boss:z_get_userdata():set_current_action(52)--prerun
			g_state = "prerun"
			boss:b_reset_t_anim_prerun()
		elseif g_state == "prerun" then
			glob_istatus = 1
			if boss:b_fin_t_anim_prerun() then
				boss:z_get_instance():get_animated_instance_model():execute_action(3 ,0.0) --correr (se hace una accion previa por que a veces parece que corre sin mover piernas)
				boss:z_get_userdata():set_current_action(53)--embestida
				boss:b_reset_t_anim_embestida()
				g_state = "prerun2"--"update"
			else
				boss:b_acum_dt_anim_prerun( _dt )
				--si toca el player en este estado le quita vida
				-- if boss:z_get_userdata():get_is_collided() then
					-- log("tocado!!")
					-- player:go_back( _dt )
					-- player:hit(0.5)
					-- boss:z_get_userdata():set_is_collided(false)
				-- end
			end
		elseif g_state == "prerun2" then
			glob_istatus = 2
			if boss:b_fin_t_anim_embestida() then --fin accion correr
				g_state = "update"
			else
				boss:b_acum_dt_anim_embestida( _dt )
				--si toca el player en este estado le quita vida
				-- if boss:z_get_userdata():get_is_collided() then
					-- log("tocado!!")
					-- player:go_back( _dt )
					-- player:hit(0.5)
					-- boss:z_get_userdata():set_is_collided(false)
				-- end

			end
		elseif g_state == "update" then
			glob_istatus = 3
			--comprobar choque con barrera para animacion crash. Se hace desde C++ !!!
			--reconocer si el boss a chocado con el coliseo
			if boss:z_get_userdata():get_is_action_finalized() == 53 then
				g_state = "crashed"
--				boss:z_get_userdata():set_is_action_finalized(2)
				--obtener vector direccion y hacer move en esa direccion hasta chocar
			else
				local vect_director = boss:z_get_eje_ia():get_vector_director()
				vect_director.x =  vect_director.x * boss:b_get_v_embestida()
				vect_director.y =  vect_director.y + 0.5
				vect_director.z =  vect_director.z * boss:b_get_v_embestida()
				boss:z_get_physic_controller():move( vect_director , _dt ) 
				--blendcycle de embestida
				boss:z_get_instance():get_animated_instance_model():blend_cycle( 3 , _dt,1.0)
				boss:z_get_userdata():set_current_action(53)--embestida
				--comprobar si choca con player para restarle vida			
				if boss:z_get_userdata():get_is_collided() then
					log("tocado!!")
					player:go_back( _dt )
					player:hit(0.5)
					boss:z_get_userdata():set_is_collided(false)
				end
			-- else
				-- boss:b_acum_dt_anim_prerun( _dt )
			end
		elseif g_state == "crashed" then
			glob_istatus = 4
			--execute action del crash
			log("choca y animacion crash")
			boss:z_get_instance():get_animated_instance_model():blend_cycle( 0, _dt, 0.0 )
			boss:z_get_instance():get_animated_instance_model():execute_action_blocked(4 ,0.0)--
			boss:z_get_userdata():set_current_action(54)--crashed
			g_state = "crashed2idle"
			boss:b_reset_t_anim_crash()
			temblorCamara()
		elseif g_state == "crashed2idle" then
			glob_istatus = 5
			if boss:b_fin_t_anim_crash() then
				   log("crash2idle")
				   g_state = "exit"
				   boss:z_get_instance():get_animated_instance_model():blend_cycle( 0, _dt, 1.0 )
				   boss:z_get_instance():get_animated_instance_model():remove_action( 4 )
				   boss:z_get_userdata():set_current_action(54)--crashed
				   boss:z_get_eje_ia():reset_datos_inicializacion()
				   boss:z_persigue( player:get_pos(), _dt, 1)
		    else
				   boss:b_acum_dt_anim_crash( _dt )
			end
		elseif g_state =="exit" then
			glob_istatus = 6
			--boss:z_get_userdata():set_current_action(81)
				g_state = "init"
				boss:z_set_state(50)--idle
				boss:z_get_userdata():set_current_action(50)--idle
				boss:z_get_instance():get_animated_instance_model():blend_cycle( 0, _dt, 1.0 )
			    boss:z_get_instance():get_animated_instance_model():remove_action( 4 )
		end
	else
		local p = player:get_pos()
		boss:z_get_instance():get_animated_instance_model():blend_cycle( 0 , _dt,1.0)	
		boss:z_get_userdata():set_current_action(50)--idle
		
		if boss:b_fin_t_encarando() then 
			log("ESTADO OUT: idle")
			boss:b_reset_t_encarando()
			boss:z_set_state(50)
			boss:z_get_userdata():set_current_action(50)--idle
			g_state = "init"
		else
			boss:b_acum_dt_encarando( _dt )
			boss:z_encarando( p, _dt, 1)
--			log("ESTADO IN: embestida (Encarando)")
		end
	end
end

---------------------------------------------------------------------------------
-- ESTADO: pisoton
---------------------------------------------------------------------------------

function update_pisoton( _dt )
--	log("ESTADO IN: pisoton")
	posiciona_instancia()

	--control de invencibilidad
	if boss:b_get_invencible() then
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect( _dt )
			boss:b_acum_dt_invencible( _dt )
		end
	end
	
	if g_state == "init" then
		boss:z_get_instance():get_animated_instance_model():execute_action_blocked( 5 ,0.0) --pisoton
		boss:z_get_userdata():set_current_action(55)--pisoton
		g_state = "update"
		boss:b_reset_t_anim_pisoton()
		--genera lanzas, ESPERAR A TENER LA ANIMACION DE LAS LANZAS
		boss:b_inicia_lanzas( 1 )
	elseif g_state == "update" then
		if boss:b_fin_t_anim_pisoton() then
			boss:z_get_instance():get_animated_instance_model():remove_action( 5 ) --pisoton
			boss:z_set_state(50)
			boss:z_get_userdata():set_current_action(50)--idle
			g_state = "init"			
		else
			temblorCamara()
			boss:b_acum_dt_anim_pisoton( _dt )
			--log("pisoton")
		end 
	end
end


---------------------------------------------------------------------------------
-- ESTADO: ataque reverso
---------------------------------------------------------------------------------
function update_ataque_reverso( _dt )
	posiciona_instancia()

	--control de invencibilidad
	if boss:b_get_invencible() then
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect( _dt )
			boss:b_acum_dt_invencible( _dt )
		end
	end

	--	log("ESTADO IN: ataque_reverso" )
	if g_state == "init" then
		boss:z_get_instance():get_animated_instance_model():execute_action_blocked( 6 ,0.0) --ataque reverso
		boss:z_get_userdata():set_current_action(56)--ataque reverso
		g_state = "update"
		boss:b_reset_t_anim_ataque_reverso()
	elseif g_state == "update" then
		-- posiciona trigger arma
		boss:b_posiciona_arma()
		if boss:b_fin_t_anim_ataque_reverso() then
			--esconde trigger arma
			boss:b_esconde_trigger_arma()
			--inicializa eje
			boss:z_get_eje_ia():reset_datos_inicializacion()
			boss:z_get_instance():get_animated_instance_model():remove_action( 6 ) --ataque reverso
			--log("fin at.reverso")
			boss:z_set_state(50)
			boss:z_get_userdata():set_current_action(50)--idle
			g_state = "init"			
			temblorCamara()
		else
			boss:b_acum_dt_anim_ataque_reverso( _dt )
			log("at.reverso")
		end 
	end
end

---------------------------------------------------------------------------------
-- ESTADO: recibiendo golpe
---------------------------------------------------------------------------------
function update_recibiendo_golpe( _dt )
	log("ESTADO IN: recibiendo_golpe" )
	posiciona_instancia()
	if boss:z_get_state() == 63 then --muerto
		log("ya esta muerto,bestia!, para!")
	else
		if boss:z_get_health() <= 0 then
			log("muriendo")
			_soundM:play_sound("MuerteBoss")
			g_state = "init"
			boss:z_set_state(58)--muriendo
			boss:z_get_userdata():set_current_action(58)--muriendo
		end
			--control de invencibilidad
		if boss:b_get_invencible() then			
			if boss:b_fin_t_invencible() then 
				boss_visible()
				boss:b_set_invencible(false)
			else 
				flash_effect(_dt)
				boss:b_acum_dt_invencible( _dt )
				boss:z_set_state(boss:z_get_userdata():get_current_action())
			end
		else
			boss:b_set_invencible(true)
			boss:b_reset_t_invencible()

			inicializa_animaciones()
			
			boss:z_get_instance():get_animated_instance_model():execute_action( 7 ,0.0) --recibe golpe	
			boss:z_get_userdata():set_current_action(57)--recibe golpe
		
			--pasan a inactivo
			boss:z_set_state(62)--estado inactivo
			boss:z_get_userdata():set_current_action(62)  --??
			g_state = "init"
			_soundM:play_sound("bossRecibeGolpe")
		end
	end
end


---------------------------------------------------------------------------------
-- ESTADO: muriendo
---------------------------------------------------------------------------------
function go_muriendo()
--	log("ESTADO IN: muriendo")
end
---------------------------------------------------------------------------------
-- ESTADO: muriendo
---------------------------------------------------------------------------------
function update_muriendo( _dt )

	boss:b_esconde_trigger_arma()
	--control de invencibilidad
	boss_visible()
	boss:b_set_invencible(false)

	if g_state == "init" then
		local aux = boss:z_get_physic_controller():get_position()
		posiciona_instancia()

--		log(" x: ".. aux.x .. " z: ".. aux.z)

--		local aux_x = aux.x - (aux.x / 1.5)
--		local aux_z = aux.z - (aux.z / 1.5)

--		log(" xx: ".. aux_x .. " zz: ".. aux_z)
		
--		boss:z_get_physic_controller():set_position(Vect3f(aux_x, aux.y, aux_z))
		boss:z_get_instance():get_animated_instance_model():execute_action_blocked( 8 ,0.0) --muriendo
		boss:z_get_userdata():set_current_action(58)--muriendo
		g_state = "update"
		boss:b_reset_t_anim_muriendo()
		temblorCamara()
	elseif g_state == "update" then
		if boss:b_fin_t_anim_muriendo() then		
		
			boss:z_get_eje_ia():reset_datos_inicializacion()
			--boss:z_get_instance():get_animated_instance_model():remove_action( 7 ) --ataque reverso
			log("muerto")
			boss:z_set_state(63)
			g_state = "init"			
			
			--_soundM:play_sound('MuerteBoss')
			g_videogame:game_over()
			
			g_state = "end"
			--fin del juego
		else
			--pone controller en la cabeza del boss
			boss:b_posiciona_torso()
			local aux = boss:b_get_p_torso()
			-- aux.y = aux.y - 1.0
			boss:z_get_physic_controller():set_position(aux)	
		   -- boss:b_posiciona_torso()
		   -- local aux = boss:b_get_p_torso()
		   -- boss:z_get_physic_controller():set_position(aux)	

			boss:b_acum_dt_anim_muriendo( _dt )
			-- local a = boss:z_get_eje_ia():get_vector_director()
			-- local b = boss:z_get_eje_ia():get_vector_director_optimo()
		
			-- if a * b > 0 then
				-- boss:z_evadir(player:get_pos(), _dt, boss:b_get_v_retirada() * -0.5, 1)
			-- else
				-- boss:z_evadir(player:get_pos(), _dt, boss:b_get_v_retirada() * 0.5, 1)
			-- end
--			log("muriendo")
		end 
	end
end


function go_perseguir()
end

---------------------------------------------------------------------------------
-- ESTADO: perseguir
---------------------------------------------------------------------------------
function update_perseguir( _dt )
--	log("ESTADO IN: perseguir")

	posiciona_instancia()

	--control de invencibilidad
	if boss:b_get_invencible() then			--esto en todas las funciones
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect(_dt)
			boss:b_acum_dt_invencible( _dt )
		end
	end

	if g_state == "init" then
		boss:b_calc_t_entre_golpes()
		log("el tiempo para dar el golpe es ".. boss:b_get_t_entre_golpes())
		g_state = "update"
	elseif g_state == "update" then
		boss:z_get_instance():get_animated_instance_model():blend_cycle(9, _dt ,1.0) -- caminar
		boss:z_get_userdata():set_current_action(59)
		local a = boss:z_get_eje_ia():get_vector_director()
		local b = boss:z_get_eje_ia():get_vector_director_optimo()
	
		if a * b < 0 then
			log("ESTADO OUT: Retirada")
			boss:z_set_state(60) --Estado Retirada
			boss:z_get_userdata():set_current_action(60)
			--g_state = "init"
			--boss:b_reset_t_entre_golpes()
		end
		
		if boss:z_get_eje_ia():comprueba_dentro_area_vision(boss:z_get_rad_view(), 60.0) then
			boss:z_set_state(51) --ataque arma
			boss:z_get_userdata():set_current_action(51)
			g_state = "init"
			boss:b_reset_t_entre_golpes()
		end 
		
		
		if boss:b_fin_t_entre_golpes() then
			log("ESTADO OUT: Encarar")
			boss:z_set_state(61) --Estado Encarar
			boss:z_get_userdata():set_current_action(61)
			g_state = "init"
			boss:b_reset_t_entre_golpes()
		end 
	end
end


---------------------------------------------------------------------------------
-- ESTADO: retirada
---------------------------------------------------------------------------------
function update_retirada (_dt)
--	log("ESTADO IN: retirada")
	posiciona_instancia()
	--control de invencibilidad
	if boss:b_get_invencible() then			--esto en todas las funciones
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect(_dt)
			boss:b_acum_dt_invencible( _dt )
		end
	end
	
	local a = boss:z_get_eje_ia():get_vector_director()
	local b = boss:z_get_eje_ia():get_vector_director_optimo()
	
	 if a * b > 0 then
		 log("ESTADO OUT: Perseguir")
		 boss:z_set_state(59) --Estado Perseguir
		 boss:z_get_userdata():set_current_action(59)
			-- no hay que resetear para no volver a pedir tiempo de golpe
			--g_state = "init"
			--boss:b_reset_t_entre_golpes()
	 end
	 if boss:b_fin_t_entre_golpes() then
		log("ESTADO OUT: Encarar")
		boss:z_set_state(61) --Estado Encarar
		boss:z_get_userdata():set_current_action(61)
		g_state = "reverse"
		boss:b_reset_t_entre_golpes()
	end 
end 

---------------------------------------------------------------------------------
-- ESTADO: encarar para ataque
---------------------------------------------------------------------------------

function update_encarar(_dt)
	posiciona_instancia()
	--control de invencibilidad
	if boss:b_get_invencible() then			
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect(_dt)
			boss:b_acum_dt_invencible( _dt )
		end
	end

	--	log("ESTADO IN: encarar")
	if g_state == "reverse" then
		log("ESTADO OUT: Ataque Reverso")
	    boss:z_set_state(56) --Estado Ataque Reverso
		boss:z_get_userdata():set_current_action(56)
	else
	  local ataque = boss:b_genera_ataque()
	  if ataque == 1 then
		  log("ESTADO OUT: Ataque Arma")
		  boss:z_set_state(51) --Estado Ataque Arma
		  boss:z_get_userdata():set_current_action(51)
	  elseif ataque == 2 then
		  log("ESTADO OUT: Ataque Embestida")
		  boss:z_set_state(53) --Estado Embestida
		  boss:z_get_userdata():set_current_action(53)
	  else --if ataque == 3
		  log("ESTADO OUT: Ataque Pisoton")
		  boss:z_set_state(55) --Estado Pisoton
		  boss:z_get_userdata():set_current_action(55)
	  end
	end
	g_state = "init"
end



---------------------------------------------------------------------------------
-- ESTADO: inactivo
---------------------------------------------------------------------------------
function update_inactivo( _dt )
	posiciona_instancia()

	--control de invencibilidad
	if boss:b_get_invencible() then			
		if boss:b_fin_t_invencible() then 
			boss_visible()
			boss:b_set_invencible(false)
		else 
			flash_effect(_dt)
			boss:b_acum_dt_invencible( _dt )
		end
	end
	
	boss:b_esconde_trigger_arma()
	
	if boss:z_get_eje_ia():comprueba_dentro_area_vision(boss:z_get_rad_view(), 45.0) then
		if g_state == "init" then
			g_state = "update"
			boss:b_reset_t_inactivo()
		elseif g_state == "update" then
			if boss:b_fin_t_inactivo() then
				g_state = "init"
				boss:z_set_state(50)--estado idle
				boss:z_get_userdata():set_current_action(50)
			else
				boss:b_acum_dt_inactivo( _dt )
				 boss:z_evadir(player:get_pos(), _dt, boss:b_get_v_retirada() * -0.5, 1)
				log("INACTIVO! "..boss:b_get_v_retirada())
			end
		end
	else
		local p = player:get_pos()

		if boss:b_fin_t_encarando() then 
			log("ESTADO OUT: idle")
			boss:b_reset_t_encarando()
			boss:z_set_state(50) --idle
			boss:z_get_userdata():set_current_action(50)			
			g_state = "init"
		else
			boss:b_acum_dt_encarando( _dt )
			boss:z_encarando( p, _dt, 1)
--			log("ESTADO IN: ataque con arma (Encarando)")
		end
	end


end

function inicializa_animaciones()
		--finaliza los executeactions si corta la animacion a medias
		if boss:z_get_userdata():get_current_action() == 50 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 0 )
		elseif boss:z_get_userdata():get_current_action() == 51 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 1 )
		elseif boss:z_get_userdata():get_current_action() == 52 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 2 )
		elseif boss:z_get_userdata():get_current_action() == 53 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 3 )
		elseif boss:z_get_userdata():get_current_action() == 54 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 4 )
		elseif boss:z_get_userdata():get_current_action() == 55 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 5 )
		elseif boss:z_get_userdata():get_current_action() == 56 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 6 )
		elseif boss:z_get_userdata():get_current_action() == 57 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 7 )
		elseif boss:z_get_userdata():get_current_action() == 58 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 8 )
		elseif boss:z_get_userdata():get_current_action() == 59 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 9 )
		elseif boss:z_get_userdata():get_current_action() == 60 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 10 )
		elseif boss:z_get_userdata():get_current_action() == 61 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 11 )
		elseif boss:z_get_userdata():get_current_action() == 62 then
			boss:z_get_instance():get_animated_instance_model():remove_action( 12 )
		
		boss:z_get_userdata():set_current_action(0)
		end	
end


function revive_boss()
	res_pos()
	boss_visible()
	boss:z_get_instance():get_animated_instance_model():remove_action( 8 ) --muriendo
	boss:z_set_health(1400)
	boss:z_set_state(50)
	boss:z_get_userdata():set_current_action(50)
	g_state = "init"
end

function posiciona_instancia()
	local aux_position = boss:z_get_physic_controller():get_position()
	aux_position.y = aux_position.y - 1.8	
	boss:z_get_instance():set_position(aux_position)
end

function debug_info_boss(fontmanager, _font)
--log("hola")
end