filein "Dummies.ms"
filein "functions.ms"
filein "Lights.ms"


utility UABUtility "UAB Utility" width:230 height:620
(
	button SetPathButton "Set Path" pos:[7,7] width:206 height:24
	label pathLabel "[PATH TO EXPORT]" pos:[8,40] width:184 height:22
	checkbox ExportMeshCheck "Export Mesh" pos:[8,64] width:144 height:18
	checkbox CopyTexturesCheck "Copy Textures" pos:[8,88] width:144 height:18
	GroupBox grp2 "Export" pos:[8,160] width:204 height:220
	button ExportLights "Lights" pos:[16,176] width:90 height:24
	button ExportMeshButton "Mesh" pos:[113,176] width:90 height:24
	button ExportSceneButton "Scene" pos:[16,272] width:187 height:24
	button CameraButton "Camera" pos:[16,208] width:90 height:24
	button ExportMeshToViewerButton "Mesh To Viewer" pos:[113,208] width:90 height:24
	button StaticMeshManagerButton "Static Mesh Manager" pos:[16,240] width:187 height:24
	button DummyExportButton "Dummys" pos:[16,300] width:187 height:24
	button TriggerExportButton "Triggers" pos:[16,330] width:187 height:24
	dropdownList LevelList "Levels" pos:[8,112] width:190 height:40
	groupBox groupBoxPhysX "PhysX Functions" pos:[8,380] width:204 height:55
	button BoundingBoxButton "Export Bounding Box" pos:[16,400] width:187 height:24
	
	/*FUNCTIONS*/
	function GetDataPath  = 
	(
		if LevelList.selection == 1 then
		(
			return pathLabel.text+"/"
		)
		else 
		(
			return pathLabel.text+"/"+LevelList.items[LevelList.selection]+"/"
		)
	)
	function GetTexturesPath =
	(
		return GetDataPath()+"Textures/"
	)
	function GetMeshesPath =
	(
		return GetDataPath()+"Meshes/"
	)
	function CreateStaticMeshManager ElementsInScene =
	(
		--local ElementsInScene = $*
		local DataPath = GetDataPath()
		local filename = DataPath + "StaticMeshManager.xml"
		deleteFile filename
		local file = createfile filename
		local LOGGER = DataPath + "LoggerMAX.log"
		deleteFile LOGGER
		local LogFile = createfile LOGGER
		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<static_meshes>\n" to:file
		for i=1 to ElementsInScene.count do
		(
			local original = ElementsInScene[i]
			local clase = classof original
			format "DEBUG: Clase del objeto: %\n" clase
			if original != targetSpot and original != targetDirectionalLight and original != Omnilight and original != Targetobject then
				--  and clase != Targetobject and clase != UndefinedClass then
			(
				format "DEBUG: Clase del objeto (Adentro): %\n" clase
				--local element = original
				element = copy original
				element.name = original.name
				ConvertToMesh element
				--element.mesh = original
				if element.material == undefined then
				(
					local msj = "El objeto " + element.name + " no tiene texturas y no se ha podido exportar"
					format "%\n" msj to:LogFile
					--DisplayWarning msj
				)
				else
				(
					if ExportMeshCheck.checked then
					(
						local TexturesPath = GetTexturesPath()		
						--local out_name = GetSaveFileName filename:(DataPath+element.name+".mesh") caption: "Select output file to export file" types:" mesh(*.mesh)|*.mesh|All Files(*.*)|*.*|"
						local out_name = DataPath+element.name+".mesh"
						if out_name!=undefined then
						(
							ExportMesh element out_name CopyTexturesCheck.checked "textures" DataPath TexturesPath
							deleteFile  (GetMeshesPath()+element.name+".mesh")
							copyFile (DataPath+element.name+".mesh") (GetMeshesPath()+element.name+".mesh")
							deleteFile (DataPath+element.name+".mesh")
						)
					)
					WriteStaticMeshElement element file DataPath
				)
				delete element
			)
		)
		format "</static_meshes>\n" to:file
		close LogFile
		close file
	)

	/*EVENTS*/

	on UABUtility open do
	(
		local LevelItems = #("","Level1","Level2", "Level3")
		LevelList.items = LevelItems
		pathLabel.text = "E:\vjuab2010\Run\Test\Data"
		--pathLabel.text = "E:\proy\Run\Test\Data"
	)
	on SetPathButton pressed do
	(
		local data_Path = getSavePath caption:"Data Path" initialDir:pathLabel.text
		if data_Path != undefined then
		(
			pathLabel.text = data_Path
		)
	)
	on ExportLights pressed do
	(
		local dataPath = GetDataPath()
		CreateLightsManager dataPath
	)
	on ExportMeshButton pressed do
	(
		if $!=undefined then
		(
			--DisplayMessage SubDirectory
			local OutputPath=GetDataPath()
			local TexturesPath = GetTexturesPath()
			--local out_name="E:/vjuab2010/Run/Test/Data/mi1ermesh.mesh"
			local out_name = GetSaveFileName filename:(OutputPath+$.name+".mesh") caption: "Select output file to export file" types:" mesh(*.mesh)|*.mesh|All Files(*.*)|*.*|"
			if out_name!=undefined then
			(
				element = copy $
				element.name = $.name
				ConvertToMesh element
				ExportMesh element out_name CopyTexturesCheck.checked "" OutputPath TexturesPath 
				delete element
				--copyFile (OutputPath+$.name+".mesh") (GetMeshesPath+$.name+".mesh")
				--deleteFile (OutputPath+$.name+".mesh")
			)
		)
		else
		(
			DisplayWarning "Debes seleccionar una malla para realizar esta operacion."
		)
	)
	on ExportSceneButton pressed do
	(
		format "Exporting complete scene"
		local dataPath = GetDataPath()
		local ElementsInScene = Geometry 
		CreateStaticMeshManager ElementsInScene
		CreateRenderableObjXML dataPath ElementsInScene
		CreateLightsManager dataPath
		ExportTriggersXML ElementsInScene dataPath
		DisplayMessage "Scene Exported Successfully!"
	)
	on ExportMeshToViewerButton pressed do
	(
		if $!=undefined then
		(
			format "Exporting object to viewer scene"
			local dataPath = GetDataPath()
			local ElementsInScene = $selection
			CreateStaticMeshManager ElementsInScene
			CreateRenderableObjXML dataPath ElementsInScene
			CreateLightsManager dataPath
			ExportTriggersXML ElementsInScene dataPath
			DisplayMessage "Mesh Exported Successfully!"
		)
		else
		(
			DisplayWarning "Debes seleccionar almenos una malla para realizar esta operacion."
		)
	)
	on StaticMeshManagerButton pressed do
	(
		local ElementsInScene = Geometry 
		CreateStaticMeshManager ElementsInScene 
	)
	
	/*PhysX Functions*/
	on BoundingBoxButton pressed do
	(
		local selectedItems = $selection
		ExportBoundingBox selectedItems
	)
	
	on DummyExportButton pressed do
	(
		local dataPath = GetDataPath()
		GetPathDummies dataPath	
	)
	
	on TriggerExportButton pressed do
	(
		local OutputPath=GetDataPath()
		local ElementsInScene = Geometry
		ExportTriggersXML ElementsInScene OutputPath
	)
)

