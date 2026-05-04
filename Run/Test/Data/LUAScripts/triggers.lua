--TRIGGER PUERTA ENTRADA
function puerta_entrada_in()
	local trigger = _triggerM:get_trigger('Trigger_EntradaPuerta')
	local triggerActivo = trigger:get_trigger_active()
	if triggerActivo == true then
		corefih():get_z_manager():set_active_zombies(true)
		trigger:set_trigger_active(false)

		crea_zombie("ZOMBIE_NORMAL","z_inst_1", Vect3f(15.0, 0.0, 8.8), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_3", Vect3f(24.5, 0.0, 0.3), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_4", Vect3f(26.6, 0.0, -5.7), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_5", Vect3f(24.4, 0.0, -11.3), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_6", Vect3f(24.4, 0.0, -16.0), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_7", Vect3f(21.0, 0.0, -15.5), Vect3f(90,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_8", Vect3f(29.8, 0.0, 6.8), Vect3f(180,0,0))		

		log("trigger puerta entrada activado")
	end
end
--------------------------------------------------------------------------
--TRIGGER ANTORCHAS ENTRADA
function antorchas_entrada_in()
	local trigger = _triggerM:get_trigger('Trigger_AntorchasEntrada')
	local triggerActivo = trigger:get_trigger_active()
	if triggerActivo == true then
		_soundM:play_sound('fireLoopMono')
		_soundM:play_sound('fireLoopMono2')
		_soundM:play_sound('fireLoopMono3')
		_soundM:play_sound('fireLoopMono4')
		_soundM:play_sound('fireLoopMono5')

		--_soundM:play_sound('game_music')
		trigger:set_trigger_active(false)
		log("trigger antorchas entrada activado")
	end
end
function antorchas_entrada_out()
	log("trigger antorchas entrada desactivado")
end
----------------------------------------------------------------------------
--TRIGGER LLAVE
function llave_in()
	local trigger = _triggerM:get_trigger('Trigger_Llave')
	local triggerActivo = trigger:get_trigger_active()
	if triggerActivo == true then
		trigger:move_renderable_object('llave')
		_soundM:play_sound('keyPickup')
		gui_show_dialog('got_key')
		trigger:set_trigger_active(false)
		--creando más zombies
		crea_zombie("ZOMBIE_RAPIDO","z_inst_18", Vect3f(36.4, 0.0, 3.7), Vect3f(90,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_20", Vect3f(30.0, 0.0, -8.0), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_21", Vect3f(30.0, 0.0, -1.9), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_22", Vect3f(33.8, 0.0, -15.7), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_23", Vect3f(33.9, 0.0, 4.8), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_24", Vect3f(31.1, 0.0, 2.7), Vect3f(90,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_25", Vect3f(31.1, 0.0, -8.7), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_26", Vect3f(49.1, 0.0, -8.7), Vect3f(90,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_29", Vect3f(61.1, 0.0, -14.9), Vect3f(90,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_30", Vect3f(61.1, 0.0, -3.9), Vect3f(120,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_31", Vect3f(61.1, 0.0, 3.7), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_32", Vect3f(62.0, 3.0, 6.8), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_33", Vect3f(62.1, 0.0, 2.6), Vect3f(120,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_34", Vect3f(61.4, 3.0, 4.2), Vect3f(90,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_35", Vect3f(57.5, 0.0, 3.5), Vect3f(120,0,0))

		log("trigger llave activado")
	end
end
function llave_out()
	log("trigger puerta llave desactivado")
end
----------------------------------------------------------------------------
--TRIGGER PALANCA
function palanca_in()
	local trigger = _triggerM:get_trigger('Trigger_Palanca')
	local triggerActivo = trigger:get_trigger_active()
	if triggerActivo == true then
		_core:get_renderable_objects_manager():get_instance_as_animated_instance_model('palanca'):get_animated_instance_model():execute_action_blocked(0,0.0)
		trigger:move_renderable_object('box_puerta palanca_visible')
		_soundM:play_sound('gateOpening')
		trigger:set_trigger_active(false)
		--dos zombies justo cuando se da la vuelta al activar la palanca
		crea_zombie("ZOMBIE_NORMAL","z_inst_9", Vect3f(49.9, 0.0, 2.7), Vect3f(80,0,0))
	    crea_zombie("ZOMBIE_NORMAL","z_inst_10", Vect3f(35.4, 0.0, 4.2), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_11", Vect3f(44.0, 0.0, -15.5), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_13", Vect3f(48.2, 0.0, -11.3), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_14", Vect3f(49.1, 0.0, -20.5), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_15", Vect3f(49.1, 0.0, -15.5), Vect3f(120,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_16", Vect3f(48.7, 0.0, 12.3), Vect3f(90,0,0))

		--crea_zombie("ZOMBIE_RAPIDO","z_inst_160", Vect3f(43.0, 0.0, 2.7), Vect3f(80,0,0))
	    -- crea_zombie("ZOMBIE_RAPIDO","z_inst_161", Vect3f(32.4, 0.0, 5.2), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_162", Vect3f(34.0, 0.0, -12.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_163", Vect3f(37.0, 0.0, -13.3), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_164", Vect3f(25.1, 0.0, -8.5), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_165", Vect3f(31.1, 0.0, -2.5), Vect3f(120,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_166", Vect3f(35.7, 0.0, -1.3), Vect3f(90,0,0))

		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_140", Vect3f(36.4, 0.0, -3.7), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_141", Vect3f(48.0, 0.0, -8.0), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_142", Vect3f(41.0, 0.0, -1.9), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_143", Vect3f(43.8, 0.0, -16.7), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_144", Vect3f(42.9, 0.0, 0.8), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_145", Vect3f(53.1, 0.0, 2.7), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_146", Vect3f(53.1, 0.0, -8.7), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_147", Vect3f(41.1, 0.0, 12.7), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_148", Vect3f(56.1, 0.0, 3.9), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_149", Vect3f(56.1, 0.0, -3.9), Vect3f(120,0,0))


		crea_zombie("ZOMBIE_RAPIDO","z_inst_180", Vect3f(58.0, 0.0, 2.7), Vect3f(80,0,0))
	    crea_zombie("ZOMBIE_NORMAL","z_inst_181", Vect3f(53.4, 0.0, -8.2), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_182", Vect3f(54.0, 0.0, 5.5), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_183", Vect3f(34.0, 0.0, -1.3), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_184", Vect3f(24.1, 0.0, -20.5), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_185", Vect3f(26.1, 0.0, -10.5), Vect3f(120,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_186", Vect3f(36.7, 0.0, -15.3), Vect3f(90,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_187", Vect3f(45.0, 0.0, -2.7), Vect3f(80,0,0))
	    crea_zombie("ZOMBIE_NORMAL","z_inst_188", Vect3f(50.4, 0.0, 0.2), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_189", Vect3f(54.0, 0.0, 5.5), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_190", Vect3f(37.0, 0.0, 1.3), Vect3f(180,0,0))
		--crea_zombie("ZOMBIE_RAPIDO","z_inst_191", Vect3f(39.1, 0.0, 6.5), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_192", Vect3f(24.1, 0.0, -15.5), Vect3f(120,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_193", Vect3f(31.7, 0.0, 2.3), Vect3f(90,0,0))
		--crea_zombie("ZOMBIE_NORMAL","z_inst_194", Vect3f(30.0, 0.0, 2.7), Vect3f(80,0,0))
	    --crea_zombie("ZOMBIE_NORMAL","z_inst_195", Vect3f(40.4, 0.0, 8.2), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_196", Vect3f(44.0, 0.0, -15.5), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_197", Vect3f(37.0, 0.0, -14.3), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_198", Vect3f(26.1, 0.0, -10.5), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_199", Vect3f(31.1, 0.0, 5.5), Vect3f(120,0,0))

		log("trigger palanca activado")
	end
end
function palanca_out()
	log("trigger palanca desactivado")
end
-- ----------------------------------------------------------------------------
-- --TRIGGER AUXILIAR
-- function aux_zombis1_in()
	-- local trigger = _triggerM:get_trigger('Trigger_Auxiliar1')
	-- local triggerActivo = trigger:get_trigger_active()
	-- if triggerActivo == true then
		-- trigger:set_trigger_active(false)
		-- --crea_zombie("ZOMBIE_NORMAL","z_inst_167", Vect3f(44.0, 0.0, 2.7), Vect3f(80,0,0))
	    -- crea_zombie("ZOMBIE_RAPIDO","z_inst_168", Vect3f(55.4, 0.0, -2.2), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_169", Vect3f(50.0, 0.0, 0.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_170", Vect3f(33.0, 0.0, 5.3), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_171", Vect3f(35.1, 0.0, -10.5), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_172", Vect3f(31.1, 0.0, 5.5), Vect3f(120,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_173", Vect3f(37.7, 0.0, -12.3), Vect3f(90,0,0))
		-- --crea_zombie("ZOMBIE_NORMAL","z_inst_174", Vect3f(42.0, 0.0, 2.7), Vect3f(80,0,0))
	    -- crea_zombie("ZOMBIE_RAPIDO","z_inst_175", Vect3f(52.4, 0.0, 8.2), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_176", Vect3f(54.0, 0.0, -12.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_177", Vect3f(37.0, 0.0, 4.3), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_178", Vect3f(30.1, 0.0, 5.5), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_179", Vect3f(31.1, 0.0, 3.5), Vect3f(120,0,0))

		-- log("trigger auxiliar activado")
	-- end
-- end
-- function aux_zombis1_out()
	-- log("trigger auxiliar desactivado")
-- end
-- ----------------------------------------------------------------------------
-- --TRIGGER AUXILIAR 2
-- function aux_zombis2_in()
	-- local trigger = _triggerM:get_trigger('Trigger_Auxiliar2')
	-- local triggerActivo = trigger:get_trigger_active()
	-- if triggerActivo == true then
		-- trigger:set_trigger_active(false)
		-- --crea_zombie("ZOMBIE_NORMAL","z_inst_167", Vect3f(44.0, 0.0, 2.7), Vect3f(80,0,0))

		-- log("trigger auxiliar activado")
	-- end
-- end
-- function aux_zombis2_out()
	-- log("trigger auxiliar desactivado")
-- end

-- ----------------------------------------------------------------------------
-- --TRIGGER AUXILIAR 3
-- function aux_zombis3_in()
	-- local trigger = _triggerM:get_trigger('Trigger_Auxiliar3')
	-- local triggerActivo = trigger:get_trigger_active()
	-- if triggerActivo == true then
		-- trigger:set_trigger_active(false)
		-- --crea_zombie("ZOMBIE_NORMAL","z_inst_167", Vect3f(44.0, 0.0, 2.7), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_120", Vect3f(15.5, 0.0, 0.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_121", Vect3f(50.5, 0.0, 4.5), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_122", Vect3f(40.5, 0.0, -8.5), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_123", Vect3f(40.5, 0.0, 0.0), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_124", Vect3f(35.5, 0.0, 4.5), Vect3f(80,0,0))  
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_125", Vect3f(27.5, 0.0, -4.5), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_126", Vect3f(32.5, 0.0, -0.5), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_127", Vect3f(38.5, 0.0, -7.0), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_128", Vect3f(24.5, 0.0, 0.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_129", Vect3f(47.5, 0.0, -4.5), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_130", Vect3f(42.5, 0.0, -3.5), Vect3f(90,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_131", Vect3f(30.5, 0.0, 0.0), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_132", Vect3f(37.5, 0.0, 6.5), Vect3f(80,0,0)) ------------------->
		-- -- crea_zombie("ZOMBIE_RAPIDO","z_inst_133", Vect3f(53.5, 0.0, 4.5), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_134", Vect3f(38.5, 0.0, 0.5), Vect3f(90,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_135", Vect3f(14.5, 0.0, 7.0), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_136", Vect3f(24.5, 0.0, 8.5), Vect3f(80,0,0))
		-- -- crea_zombie("ZOMBIE_RAPIDO","z_inst_137", Vect3f(32.5, 0.0, 2.5), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_138", Vect3f(12.5, 0.0, -8.5), Vect3f(90,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_139", Vect3f(34.5, 0.0, 0.0), Vect3f(180,0,0)) ------------------->

		-- log("trigger auxiliar activado")
	-- end
-- end
-- function aux_zombis3_out()
	-- log("trigger auxiliar desactivado")
-- end
-- ----------------------------------------------------------------------------
-- --TRIGGER AUXILIAR 4
-- function aux_zombis4_in()
	-- local trigger = _triggerM:get_trigger('Trigger_Auxiliar4')
	-- local triggerActivo = trigger:get_trigger_active()
	-- if triggerActivo == true then
		-- trigger:set_trigger_active(false)
		-- --crea_zombie("ZOMBIE_NORMAL","z_inst_167", Vect3f(44.0, 0.0, 2.7), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_42", Vect3f(24.5, 0.0, 1.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_43", Vect3f(22.5, 0.0, 2.5), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_44", Vect3f(32.5, 0.0, -8.5), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_45", Vect3f(48.5, 0.0, -3.5), Vect3f(180,0,0))

		
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_210", Vect3f(49.4, 0.0, -13.3), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_211", Vect3f(22.4, 0.0, -16.0), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_212", Vect3f(50.0, 0.0, -10.5), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_213", Vect3f(29.8, 0.0, -8.8), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_214", Vect3f(13.0, 0.0, -8.8), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_215", Vect3f(28.5, 0.0, 13.3), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_216", Vect3f(30.6, 0.0, -5.7), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_217", Vect3f(28.4, 0.0, -1.3), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_218", Vect3f(20.4, 0.0, -10.0), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_219", Vect3f(21.0, 0.0, -1.5), Vect3f(90,0,0))

		-- log("trigger auxiliar activado")
	-- end
-- end
-- function aux_zombis4_out()
	-- log("trigger auxiliar desactivado")
-- end
-- ----------------------------------------------------------------------------
-- --TRIGGER AUXILIAR 5
-- function aux_zombis5_in()
	-- local trigger = _triggerM:get_trigger('Trigger_Auxiliar5')
	-- local triggerActivo = trigger:get_trigger_active()
	-- if triggerActivo == true then
		-- trigger:set_trigger_active(false)
		-- --crea_zombie("ZOMBIE_NORMAL","z_inst_167", Vect3f(44.0, 0.0, 2.7), Vect3f(80,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_230", Vect3f(32.4, 0.0, -13.3), Vect3f(80,0,0))
		-- -- crea_zombie("ZOMBIE_RAPIDO","z_inst_231", Vect3f(32.4, 0.0, -16.0), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_232", Vect3f(31.0, 0.0, -10.5), Vect3f(90,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_233", Vect3f(39.8, 0.0, -8.8), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_234", Vect3f(23.0, 0.0, -8.8), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_235", Vect3f(38.5, 0.0, 1.3), Vect3f(80,0,0))
		-- -- crea_zombie("ZOMBIE_RAPIDO","z_inst_236", Vect3f(30.6, 0.0, -5.7), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_237", Vect3f(38.4, 0.0, -1.3), Vect3f(80,0,0))
		-- -- crea_zombie("ZOMBIE_RAPIDO","z_inst_238", Vect3f(30.4, 0.0, -10.0), Vect3f(180,0,0))
		-- -- crea_zombie("ZOMBIE_NORMAL","z_inst_239", Vect3f(31.0, 0.0, -1.5), Vect3f(90,0,0))

		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_220", Vect3f(37.5, 0.0, 3.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_221", Vect3f(50.5, 0.0, 4.5), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_222", Vect3f(31.5, 0.0, -8.5), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_223", Vect3f(45.5, 0.0, 0.0), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_224", Vect3f(35.5, 0.0, 13.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_225", Vect3f(50.5, 0.0, -4.5), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_226", Vect3f(42.5, 0.0, -0.5), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_227", Vect3f(44.5, 0.0, -7.0), Vect3f(180,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_228", Vect3f(39.5, 0.0, 0.5), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_229", Vect3f(52.5, 0.0, -4.5), Vect3f(180,0,0))


		-- log("trigger auxiliar activado")
	-- end
-- end
-- function aux_zombis5_out()
	-- log("trigger auxiliar desactivado")
-- end
-- ----------------------------------------------------------------------------
-- --TRIGGER AUXILIAR 6
-- function aux_zombis6_in()
	-- local trigger = _triggerM:get_trigger('Trigger_Auxiliar6')
	-- local triggerActivo = trigger:get_trigger_active()
	-- if triggerActivo == true then
		-- trigger:set_trigger_active(false)
		-- --crea_zombie("ZOMBIE_NORMAL","z_inst_167", Vect3f(44.0, 0.0, 2.7), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_150", Vect3f(50.1, 0.0, 3.7), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_151", Vect3f(47.0, 0.0, 13.8), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_152", Vect3f(62.1, 0.0, 1.6), Vect3f(120,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_153", Vect3f(61.4, 0.0, -1.2), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_NORMAL","z_inst_154", Vect3f(57.5, 0.0, -3.5), Vect3f(120,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_155", Vect3f(55.1, 0.0, 3.7), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_156", Vect3f(56.0, 0.0, 4.8), Vect3f(80,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_157", Vect3f(49.1, 0.0, 2.6), Vect3f(120,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_158", Vect3f(60.4, 0.0, 0.2), Vect3f(90,0,0))
		-- crea_zombie("ZOMBIE_RAPIDO","z_inst_159", Vect3f(57.5, 0.0, 1.5), Vect3f(120,0,0))

		-- log("trigger auxiliar activado")
	-- end
-- end
-- function aux_zombis6_out()
	-- log("trigger auxiliar desactivado")
-- end


----------------------------------------------------------------------------
--TRIGGER PUERTA FIN JUEGO
function puerta_salida_in()
	local triggerLlave = _triggerM:get_trigger('Trigger_Llave')
	local triggerPuerta = _triggerM:get_trigger('Trigger_SalidaPuerta')
	local triggerActivoLlave = triggerLlave:get_trigger_active()
	local triggerActivoPuerta = triggerPuerta:get_trigger_active()
	if triggerActivoLlave == false then
		if triggerActivoPuerta == true then
			_core:get_renderable_objects_manager():get_instance_as_animated_instance_model('Puerta_Salida'):get_animated_instance_model():execute_action_blocked(0,0.0)
--			trigger:move_renderable_object('puertasalida')
			triggerPuerta:set_trigger_active(false)
--			log("trigger puerta salida activado")
			
			corefih():get_z_manager():set_active_zombies(false) -- inactivar enemigos
			
			_soundM:fade_out(5)
			_soundM:play_sound('keyPickup')
			g_videogame:change_level(2)
		end
	else
		gui_show_dialog('closed_exit_gate')
	end
end
function puerta_salida_out()
	log("trigger puerta salida desactivado")
end
----------------------------------------------------------------------------
--TRIGGER PUERTA LLAVE
function puerta_llave_in()
	local triggerPalanca = _triggerM:get_trigger('Trigger_Palanca')
	local triggerActivoPalanca = triggerPalanca:get_trigger_active()
	if triggerActivoPalanca == false then
		--triggerLlave:set_trigger_active(false)
		log("trigger llave activado")
	else
		gui_show_dialog('closed_gate')
	end
end
function puerta_llave_out()
	log("trigger llave desactivado")
end
----------------------------------------------------------------------------
--TRIGGER FUENTE LAVA
function fuente_in()
	local trigger = _triggerM:get_trigger('Trigger_FuenteLava')
	local triggerActivo = trigger:get_trigger_active()
	if triggerActivo == true then
		_soundM:play_sound('waterFountain')
		trigger:set_trigger_active(false)
		log("trigger fuente lava activado")
		crea_zombie("ZOMBIE_NORMAL","z_inst_36", Vect3f(60.0, 0.0, -20.62), Vect3f(100,0,0))
	    crea_zombie("ZOMBIE_RAPIDO","z_inst_37", Vect3f(60.0, 0.0, -20.2), Vect3f(100,0,0))
	    crea_zombie("ZOMBIE_RAPIDO","z_inst_38", Vect3f(59.0, 0.0, 0.8), Vect3f(100,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_39", Vect3f(37.0, 0.0, -11.3), Vect3f(80,0,0))
	    crea_zombie("ZOMBIE_RAPIDO","z_inst_40", Vect3f(29.1, 0.0, -20.5), Vect3f(180,0,0))
	    crea_zombie("ZOMBIE_RAPIDO","z_inst_41", Vect3f(31.7, 0.0, -12.3), Vect3f(120,0,0))

		crea_zombie("ZOMBIE_NORMAL","z_inst_240", Vect3f(32.4, 0.0, 3.3), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_241", Vect3f(32.4, 0.0, 2.0), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_242", Vect3f(31.0, 0.0, 0.5), Vect3f(90,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_243", Vect3f(39.8, 0.0, 2.8), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_244", Vect3f(33.0, 0.0, -8.8), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_245", Vect3f(38.5, 0.0, 13.3), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_246", Vect3f(30.6, 0.0, -5.7), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_247", Vect3f(28.4, 0.0, -1.3), Vect3f(80,0,0))
		crea_zombie("ZOMBIE_RAPIDO","z_inst_248", Vect3f(30.4, 0.0, 2.0), Vect3f(180,0,0))
		crea_zombie("ZOMBIE_NORMAL","z_inst_249", Vect3f(31.0, 0.0, -1.5), Vect3f(90,0,0))
	end                                                
end
function fuente_out()
	log("trigger fuente lava desactivado")
end
----------------------------------------------------------------------------
--TRIGGER LLAVE PALANCA
function llave_palanca_in()

	--local trigger = _triggerM:get_trigger('Trigger_PuertaLlave')
	local trigger = _triggerM:get_trigger('Trigger_LlavePalanca')
	local triggerActivo = trigger:get_trigger_active()
	if triggerActivo == true then
		trigger:move_renderable_object('llave2')
		_soundM:play_sound('powerUpPickup')
		trigger:set_trigger_active(false)
		log("trigger llave palanca activado")
	end
end
function llave_palanca_out()
	log("trigger llave palanca desactivado")
end
----------------------------------------------------------------------------
--TRIGGER PUERTA PALANCA
function puerta_palanca_in()
	local triggerPuertaPalanca = _triggerM:get_trigger('Trigger_PuertaPalanca')
	local triggerLlave = _triggerM:get_trigger('Trigger_LlavePalanca')
	local triggerActivoLlave = triggerLlave:get_trigger_active()
	local triggerActivoPuerta = triggerPuertaPalanca:get_trigger_active()
	local myBillboard = _billboardM:get_b_animation_by_instance('pPuerta')
	if triggerActivoLlave == false then
		if triggerActivoPuerta == true then
			triggerPuertaPalanca:set_trigger_active(false)
			_billboardM:set_active(false)
			_soundM:play_sound('keyholeUnlock')
			triggerPuertaPalanca:move_renderable_object('box_puerta2 palanca_visible')
			myBillboard:set_pos(Vect3f(37.4, 1.5, -17.9))
			myBillboard:set_end(false)			
			log("trigger puerta palanca activado")
		end
	else --if triggerActivoPuerta == true
		gui_show_dialog('closed_gate_lever')
	end
end
function puerta_palanca_out()
	log("trigger puerta palanca desactivado")
end
----------------------------------------------------------------------------
--TRIGGERS SALTO
-- jumpDirection debe ser un vector normalizado de la dirección hacia donde debe saltar
function jump_in(boxName, boxPosition, jumpDirection)
	_physicsM:set_invisible_box_position(boxName, boxPosition)
	player:jump(boxName, jumpDirection)
	log(boxName .. ' in')
end
function jump_out(boxName, boxPosition)
	_physicsM:set_invisible_box_position(boxName, boxPosition)
	player:cancel_jump(boxName)
	log(boxName .. ' out')
end
----------------------------------------------------------------------------
--TRIGGERS MUERTE POR CAIDA
function Trigger_Fall(triggerName)
	local trigger = _core:get_trigger_manager():get_trigger(triggerName)
	
	if trigger:get_trigger_active() and player:collide_enemies() then
		player:fall()
		trigger:set_trigger_active(false)
	end
end
function Trigger_Fall_Out(triggerName)
	_core:get_trigger_manager():get_trigger(triggerName):set_trigger_active(true)
end

----------------------------------------------------------------------------
--TRIGGERS LUCES
function Trigger_TurnLight(lightName, _flag, triggerToInactivateName, triggerToActivateName)
	local triggerToInactivate = _core:get_trigger_manager():get_trigger(triggerToInactivateName)
	
	if triggerToInactivate:get_trigger_active() then
		local triggerToActivate = _core:get_trigger_manager():get_trigger(triggerToActivateName)
--[[		
		-- estos son logs para debug
		local flag
		if _flag then flag = "on"
		else flag = "off" end
		log(lightName .. " " .. flag)
--]]		
		_lightM:turn_light(lightName, _flag)
		triggerToInactivate:set_trigger_active(false)
		triggerToActivate:set_trigger_active(true)
	end
end

----------------------------------------------------------------------------
----------------------------------------------------------------------------