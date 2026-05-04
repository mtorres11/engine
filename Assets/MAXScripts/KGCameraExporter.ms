filein "functions.ms" quiet:true

function KGIsKeyCamera obj =
(
	return ((classof obj)==Targetcamera and obj.pos.controller.keys.count!=0)
)

function KGExportFixedCamera obj file =
(
	if (classof obj)==Targetcamera then
	(
		local pos=RHTranslationToLH obj.pos
		local current_target_pos=RHTranslationToLH obj.target.pos
		local current_fov=obj.fov*3.1416/180.0
		local current_near_clip=obj.nearclip 
		local current_far_clip=obj.farclip
		format "\t<fixed_camera name=\"%\" position=\"% % %\" look_at=\"% % %\" fov=\"%\" aspect=\"%\" near=\"%\" far=\"%\"/>\n" obj.name pos.x pos.y pos.z current_target_pos.x current_target_pos.y current_target_pos.z current_fov (16.0/9.0) current_near_clip current_far_clip to:file 
		--format "\t<fixed_camera name=\"%\" position=\"% % %\" look_at=\"% % %\" fov=\"%\" aspect=\"%\" near=\"%\" far=\"%\"/>\n" obj.name pos.x pos.y pos.z current_target_pos.x current_target_pos.y current_target_pos.z current_fov current_near_clip current_far_clip to:file
	)
)

function KGExportKeyCamera obj filename =
(
	if (classof obj)==Targetcamera then
	(
		/*format "class %\n" (classof obj)
		format "Camera % (% % %)\n" obj.name obj.pos.x obj.pos.y obj.pos.z
		format "FOV % NearP % FarP %\n" obj.fov obj.nearclip obj.farclip
		format "Target (% % %)\n" obj.target.pos.x obj.target.pos.y obj.target.pos.z
		for t = animationrange.start to animationrange.end do (
			
			at time t current_pos = obj.pos
			--at time t current_camera=obj
			format "Time (% / % - % secs) Target (% % %)\n" t animationrange.end (t/frameRate) current_pos.x current_pos.y current_pos.z
			--format "R (% % %) \n" current_pos.x current_pos.y current_pos.z
			--format "R (% % %) \n" current_camera.pos.x current_camera.pos.y current_camera.pos.z
		)*/
		local file=createfile filename
		if file!=undefined then
		(
			format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
			format "<camera_key_controller name=\"%\">\n" obj.name to:file
			local keys=obj.pos.controller.keys
			for b=1 to keys.count do
			(
				local current_frame=keys[b].time
				local current_time=float(current_frame)/frameRate
				at time current_frame current_pos = RHTranslationToLH obj.pos
				at time current_frame current_target_pos = RHTranslationToLH obj.target.pos
				at time current_frame current_fov = obj.fov 
				at time current_frame current_near_clip = obj.nearclip 
				at time current_frame current_far_clip = obj.farclip
				--format "Time (% / % - % secs) Target (% % %)\n" current_frame animationrange.end (current_frame/frameRate) current_pos.x current_pos.y current_pos.z
				format "\t<key time=\"%\" pos=\"% % %\" look_at=\"% % %\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" \
					current_time current_pos.x current_pos.y current_pos.z current_target_pos.x current_target_pos.y current_target_pos.z \
					current_fov current_near_clip current_far_clip to:file
			)
			format "</camera_key_controller>\n" to:file
			close file
			return true
		)
		else
		(
			KGShowError "File '"+filename+"' could'nt be created"
		)
	)
	else if (classof obj)==Freecamera then
	(
		local file=createfile filename
		if file!=undefined then
		(
			format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
			format "<camera_key_controller name=\"%\">\n" obj.name to:file
			local keys=obj.pos.controller.keys
			for b=1 to keys.count do
			(
				local current_frame=keys[b].time
				local current_time=float(current_frame)/frameRate
				
			
				at time current_frame matrix = obj.transform
				at time current_frame current_yaw = -(GetYaw matrix + 90)
				at time current_frame current_pitch = -(GetPitch matrix + 90)
				at time current_frame current_roll = GetRoll matrix
					
				
				at time current_frame current_pos = RHTranslationToLH obj.pos
				--at time current_frame current_target_pos = RHTranslationToLH obj.target.pos
				at time current_frame current_fov = obj.fov 
				at time current_frame current_near_clip = obj.nearclip 
				at time current_frame current_far_clip = obj.farclip
				--format "Time (% / % - % secs) Target (% % %)\n" current_frame animationrange.end (current_frame/frameRate) current_pos.x current_pos.y current_pos.z
				format "\t<key time=\"%\" pos=\"% % %\" yaw=\"%\" pitch=\"%\" roll=\"%\" fov=\"%\" near_plane=\"%\" far_plane=\"%\"/>\n" \
					current_time current_pos.x current_pos.y current_pos.z current_yaw current_pitch current_roll \
					current_fov current_near_clip current_far_clip to:file
			)
			format "</camera_key_controller>\n" to:file
			close file
			return true
		)
		else
		(
			KGShowError "File '"+filename+"' could'nt be created"
		)
	)
	return false
)

--KGExportCamera $
KGExportKeyCamera $ "D:/proy/Run/Test/Data/CameraPath.xml"