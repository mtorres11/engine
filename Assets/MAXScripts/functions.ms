global VERTEX_TYPE_GEOMETRY = 0x0001
global VERTEX_TYPE_NORMAL = 0x0002
global VERTEX_TYPE_TANGENT = 0x0004
global VERTEX_TYPE_BINORMAL = 0x0008
global VERTEX_TYPE_TEXTURE1 = 0x0010
global VERTEX_TYPE_TEXTURE2 = 0x0020
global VERTEX_TYPE_DIFFUSE = 0x0040

-- mascaras para alpha
global ALPHA_TEST = 0x0200
global ALPHA_BLEND = 0x0400

global BOUNDING_BOX = 0x0001
global BOUNDING_SPHERE = 0x0002
global BOUNDING_CAPSULE = 0x0004
global BOUNDING_MESH = 0x0008
global BOUNDING_PLANE = 0x0010
global NO_BOUNDING_OBJECT = 0x0020

global HEADER = 0xFF77
global FOOTER = 0x77FF

global UABVtxsNormals=#()

global totalmateriales=0

function UABClearNormalsArray =
(
	while UABVtxsNormals.count>0 do
	(
		deleteItem UABVtxsNormals 1
	)
)

function IsSmoothingGroupEnabled IdGroup Value =
(
	local ValueMask=2^(IdGroup-1)
	return (bit.and Value ValueMask)==ValueMask
)

function UABGetVertexNormal obj IdVertex SmoothValue FaceNormal =
(
	local HasNormal=false
	local Normal=point3 0 0 0
	for b=1 to 32 do
	(
		if((IsSmoothingGroupEnabled b SmoothValue)==true) then
		(
			Normal=Normal+UABVtxsNormals[IdVertex][b]
			HasNormal=true
		)
	)
	if HasNormal==false then
	(
		Normal=FaceNormal
	)
	return Normal
)
function UABCalcVertexsNormals obj =
(
	UABClearNormalsArray()

	local NumVtxs=getNumVerts obj
	for b=1 to NumVtxs do
	(
		UABVtxsNormals[b]=#()
		for t=1 to 32 do
		(
			UABVtxsNormals[b][t]=point3 0 0 0
		)
	)
	local NumFaces=getNumFaces obj
	local InvTransform=inverse obj.transform
	for IdFace=1 to NumFaces do
	(
		local IdxsFace=getFace obj IdFace
		local Vtx1=(getVert obj IdxsFace.x)*InvTransform
		local Vtx2=(getVert obj IdxsFace.y)*InvTransform
		local Vtx3=(getVert obj IdxsFace.z)*InvTransform
		local FaceNormal=getFaceNormal obj IdFace
		local SmoothValue=getFaceSmoothGroup obj IdFace
		for b=1 to 32 do
		(
			if((IsSmoothingGroupEnabled b SmoothValue)==true) then
			(
				UABVtxsNormals[IdxsFace.x][b]=UABVtxsNormals[IdxsFace.x][b] + FaceNormal
				UABVtxsNormals[IdxsFace.y][b]=UABVtxsNormals[IdxsFace.y][b] + FaceNormal
				UABVtxsNormals[IdxsFace.z][b]=UABVtxsNormals[IdxsFace.z][b] + FaceNormal
			)
		)
	)
)

function ExportMesh Object3D filename CopyTextures RelativeTexturesPath ExportPath TexturesPath=
(
	format "DEBUG: Path de las Texturas a copiar:%\n"TexturesPath
	local vertices = #()
	local indices = #()
	file=fopen filename "wb"
	WriteShort file HEADER #unsigned
	-- Si es un material Standard
	countMaterials = 1
	clase = classof(Object3D.material)

	--Creacion del vector del "mesh"
	CreateMesh Object3D vertices indices
	
	-----------------------------------------Prueba-------------------------------------------------------
	totalmateriales = 0
	for k=1 to vertices.count do
	(		
		if vertices[k].count > 0 then
		(
			totalmateriales=totalmateriales+1
			format "TOTALMat %" totalmateriales
		)
	)
	
	if clase == Standardmaterial then
	(
		-- numero de materiales
		WriteShort file countMaterials #unsigned
		ExportMaterial Object3D.material file CopyTextures RelativeTexturesPath ExportPath TexturesPath
	)
	else --Si es Multimaterial
	(
		-- numero de materiales
		--countMaterials=Object3D.material.count
		countMaterials=totalmateriales
		WriteShort file countMaterials #unsigned
		for i = 1 to vertices.count do
		(
			if vertices[i].count > 0 then
			(
				format "DEBUG: Material % en posicion %\n" Object3D.material[i] i
			    ExportMaterial Object3D.material[i] file CopyTextures RelativeTexturesPath ExportPath TexturesPath
			)
		)
	)
	
	---------------------------------------PRINT IN FILE-----------------------------------------------
	format "VERTEX LOCO %\n" vertices.count
	for k=1 to vertices.count do
	(
		cantVertices = vertices[k].count
		
		if cantVertices == 0 then
		(
			format "INFO: material (%) no tiene vertices\n" k
			continue
		)
		--WriteShort file cantVertices #unsigned
		cantIndices = indices[k].count
		--format "Cantidad de Valores: %\n" cantValores
		--format "Cantidad de Vertices: %\n" cantVertices
		WriteShort file cantVertices #unsigned
		--format "vtx count %" vertices[1].count
		for i = 1 to vertices[k].count do
		(
			if i==1 then
			(
				format "material % vertex size %\n" k vertices[k][i].count
				
			)
			for j = 1 to vertices[k][i].count do
			(
				(
					WriteFloat file vertices[k][i][j]
				)
			)
			--format " %\n" vertices[i]
			--format "vertices count %\n" vertices[i].count
		)
		WriteShort file cantIndices #unsigned
		format "DEBUG: Cantidad de Vertices para % es: %\n" Object3D.name cantVertices
		format "DEBUG: Cantidad de Indices para % es: %\n" Object3D.name cantIndices
		for i = 1 to cantIndices do
		(
			WriteShort file indices[k][i] #unsigned
		)
		--format "Indices: %\n" indices
	)
	----------------------------------------------------------------------------------------------------------
	WriteShort file FOOTER #unsigned
	--Para hacer un flush de los bytes no escritos en el fichero
	fflush file
	-- Para cerrar el fichero
	fclose file
)

function ExportMaterial material file CopyTextures RelativeTexturesPath ExportPath SaveTextPath=
(
	format "DEBUG: Export Material %\n" material
	--Vertex Type
	local vertexType = GetVertexType material
	
	-- verifico si tiene opacity para agregarlo a la mascara del vertexType
	if material.opacityMapEnable and material.opacityMap != undefined then
	(
		-- si esta marcado el refractionMap, indica que tiene Alpha Blend
		if material.refractionMapEnable then
		(
			format "DEBUG: Tiene Alpha Blend\n"
			vertexType = bit.or vertexType ALPHA_BLEND
		)
		else
		(
			format "DEBUG: Tiene Alpha Test\n"
			vertexType = bit.or vertexType ALPHA_TEST
		)
		
	)
	format "DEBUG: Vertex Type (ConAlpha): %\n" vertexType
	WriteShort file vertexType #unsigned
	
	--Cantidad de texturas
	local cantTextures = (CountTextures material)
	WriteShort file cantTextures #unsigned
	--Exporta materiales
	local FileName = ""
	if material.diffuseMapEnable and material.diffuseMap != undefined then
	(
		format "DEBUG: Info: Diffuse\n"
		FileName = openBitMap material.diffuseMap.bitmap.filename
		format "DEBUG: Material DiffuseMap Filename: %\n" FileName
		ExportTexturesPath file FileName CopyTextures ExportPath RelativeTexturesPath SaveTextPath
	)
	if material.selfIllumMapEnable and material.selfIllumMap != undefined then
	(
		format "DEBUG: Info: SelfIlum\n"
		FileName = openBitMap material.selfIllumMap.bitmap.filename
		format "DEBUG: Material LightMap Filename: %\n" FileName
		ExportTexturesPath file FileName CopyTextures ExportPath RelativeTexturesPath SaveTextPath
	)
	if material.bumpMapEnable and material.bumpMap  != undefined then
	(
		format "DEBUG: Info: Bump\n"
		FileName = openBitMap material.bumpMap.bitmap.filename
		format "DEBUG: Material BumpMap Filename: %\n" FileName
		ExportTexturesPath file FileName CopyTextures ExportPath RelativeTexturesPath SaveTextPath
	)
	if material.reflectionMapEnable and material.reflectionMap != undefined then
	(
		format "DEBUG: Info: Reflection\n"
		FileName = openBitMap material.reflectionMap.bitmap.filename
		format "DEBUG: Material ReflectionMap Filename: %\n" FileName
		ExportTexturesPath file FileName CopyTextures ExportPath RelativeTexturesPath SaveTextPath
	)
	if material.filterMapEnable and material.filterMap != undefined then
	(
		format "DEBUG: Info: Filter\n"
		FileName = openBitMap material.filterMap.bitmap.filename
		format "DEBUG: Material ReflectionMap Filename: %\n" FileName
		ExportTexturesPath file FileName CopyTextures ExportPath RelativeTexturesPath SaveTextPath
	)
	if material.glossinessMapEnable and material.glossinessMap != undefined then
	(
		format "DEBUG: Info: Glow\n"
		FileName = openBitMap material.glossinessMap.bitmap.filename
		format "DEBUG: Material Glossiness Map Filename: %\n" FileName
		ExportTexturesPath file FileName CopyTextures ExportPath RelativeTexturesPath SaveTextPath
	)
)

function CreateVertex position normal diffuseColor tuv1 tuv2  vertexType VertOut =
(
	local vertexOut =#()
	if (bit.and vertexType VERTEX_TYPE_TEXTURE1) == VERTEX_TYPE_TEXTURE1 then
	(
		if (bit.and vertexType VERTEX_TYPE_BINORMAL) == VERTEX_TYPE_BINORMAL then
		(
			vertexOut = #(position.x, position.y, position.z, normal.x, normal.y, normal.z, 0, 0, 0, 0, 0, 0, 0, 0, 0, tuv1.x, 1.0-tuv1.y)
		)
		else
		(
			--vertexOut = #(position.x, position.y, position.z, normal.x, normal.y, normal.z, tuv1.x, 1.0-tuv1.y)
			vertexOut = #(position.x, position.y, position.z, normal.x, normal.y, normal.z, tuv1.x, tuv1.y*(-1))
		)
	)
	else if (bit.and vertexType VERTEX_TYPE_TEXTURE2) == VERTEX_TYPE_TEXTURE2 then
	(
		if (bit.and vertexType VERTEX_TYPE_BINORMAL) == VERTEX_TYPE_BINORMAL then
		(
			vertexOut = #(position.x, position.y, position.z, normal.x, normal.y, normal.z, 0, 0, 0, 0, 0, 0, 0, 0, 0, tuv1.x, 1.0-tuv1.y, tuv2.x, 1.0-tuv2.y)
		)
		else
		(
			vertexOut = #(position.x, position.y, position.z, normal.x, normal.y, normal.z, tuv1.x, 1.0-tuv1.y, tuv2.x, 1.0-tuv2.y)
		)
	)
	else if (bit.and vertexType VERTEX_TYPE_DIFFUSE) == VERTEX_TYPE_DIFFUSE then
	(
		vertexOut = #(position.x, position.y, position.z, normal.x, normal.y, normal.z, diffuseColor)
	)
	
	join VertOut vertexOut
)

function CreateMesh Object3D vertices indices =
(
	local numFaces = getNumFaces Object3D
	local vertType=#()
	local IsStandardMaterial=(classof(Object3D.material) == Standardmaterial)
	if IsStandardMaterial then
	(
		format "DEBUG: Info: Standard Material\n"
		append vertType (GetVertexType Object3D.material)
	)
	else --Si es Multimaterial
	(
		format "DEBUG: Info: MultiMaterial\n"
		for i = 1 to Object3D.material.count do
		(
			append vertType (GetVertexType Object3D.material[i])
		)
	)
	
	UABCalcVertexsNormals Object3D
	
	for i=1 to numFaces do
	(
		local ldsxFace = getFace Object3D i
		local vtx1 = (getVert Object3D ldsxFace.x)
		local vtx2 = (getVert Object3D ldsxFace.y)
		local vtx3 = (getVert Object3D ldsxFace.z)
		
		--MULTIPLICAR VERTICES POR LA INVERSA DE LA MATRIS DE TRANSFORMACION
		local invTransform = inverse Object3D.transform --Inversa
		vtx1 = vtx1 * invTransform
		vtx2 = vtx2 * invTransform
		vtx3 = vtx3 * invTransform
		
		--cara
		vtx1 = RHTranslationToLH (vtx1)
		vtx2 = RHTranslationToLH (vtx2)
		vtx3 = RHTranslationToLH (vtx3)
		
		--normal
		--local normal1 = RHTranslationToLH (getNormal Object3D ldsxFace.x)
		--local normal2 = RHTranslationToLH (getNormal Object3D ldsxFace.y)
		--local normal3 = RHTranslationToLH (getNormal Object3D ldsxFace.z)
		local SmoothValue = getFaceSmoothGroup Object3D i
		local FaceNormal=getFaceNormal Object3D i
		
		local normal1 = RHTranslationToLH ( normalize(UABGetVertexNormal Object3D ldsxFace.x SmoothValue FaceNormal))
		local normal2 = RHTranslationToLH ( normalize(UABGetVertexNormal Object3D ldsxFace.y SmoothValue FaceNormal))
		local normal3 = RHTranslationToLH ( normalize(UABGetVertexNormal Object3D ldsxFace.z SmoothValue FaceNormal))
		--color
		--sustituir en la version final
		--local dColor = 0x7FFFFFFF --Maximo valor que lee C++ sin dar error.
		local dColor = 0xFFFFFF01
		
		--Coordenadas de textura
		local text1 = meshop.getMapFace Object3D 1 i
		--TUV1
		local TUV1Map1 = meshop.getMapVert Object3D 1 text1.x
		local TUV1Map2 = meshop.getMapVert Object3D 1 text1.y
		local TUV1Map3 = meshop.getMapVert Object3D 1 text1.z
		--TUV2
		local TUV2Map1 = TUV1Map1
		local TUV2Map2 = TUV1Map2
		local TUV2Map3 = TUV1Map3
		
		
		local IdMapMaterial=1
		if IsStandardMaterial==false then -- si es MutiMaterial
		(
			IdMapMaterial=getFaceMatID Object3D i
		)
		
		if (bit.and vertType[IdMapMaterial] VERTEX_TYPE_TEXTURE2)==VERTEX_TYPE_TEXTURE2 then
		(
			--Si tiene el TUV2
			local text2 = meshop.getMapFace Object3D 2 i
			TUV2Map1 = meshop.getMapVert Object3D 2 text2.x
			TUV2Map2 = meshop.getMapVert Object3D 2 text2.y
			TUV2Map3 = meshop.getMapVert Object3D 2 text2.z
		)
		
		local VertOut1 = #()
		local VertOut2 = #()
		local VertOut3 = #()
		
		CreateVertex vtx1 normal1 dColor TUV1Map1 TUV2Map1 vertType[IdMapMaterial] VertOut1
		format "1 - %: %\n" i VertOut1
		CreateVertex vtx2 normal2 dColor TUV1Map2 TUV2Map2 vertType[IdMapMaterial] VertOut2
		format "2 - %: %\n" i VertOut2
		CreateVertex vtx3 normal3 dColor TUV1Map3 TUV2Map3 vertType[IdMapMaterial] VertOut3
		format "3 - %: %\n" i VertOut3
		
		AddVertex IdMapMaterial vertices VertOut1 indices
		AddVertex IdMapMaterial vertices VertOut3 indices
		AddVertex IdMapMaterial vertices VertOut2 indices
		--uab_add_vtx_on_vertex_buffer IdMapMaterial vertices.count 
	)
)
function GetVect3fAsString Position =
(
	local Value=(Position.x As String)+" "+ (Position.y As String)+" "+(Position.z As String)
)
function GetColorAsString Color =
(
	local Value=(Color.r As String)+" "+ (Color.g As String)+" "+(Color.b As String)
)
function RHTranslationToLH translation =
(
	return point3 translation.x translation.z translation.y
)
function GetVertexType material =
(
	local mascara = bit.or VERTEX_TYPE_GEOMETRY VERTEX_TYPE_NORMAL
	if material.diffuseMapEnable AND material.diffuseMap != undefined then
	(
		if material.selfIllumMapEnable AND material.selfIllumMap != undefined then
		(
			mascara = bit.or mascara VERTEX_TYPE_TEXTURE2
		)
		else
		(
			mascara = bit.or mascara VERTEX_TYPE_TEXTURE1
		)
		if material.bumpMapEnable AND material.bumpMap != undefined then
		(
			mascara = bit.or mascara VERTEX_TYPE_TANGENT
			mascara = bit.or mascara VERTEX_TYPE_BINORMAL
		)
	)
	else
	(
		mascara = bit.or mascara VERTEX_TYPE_DIFFUSE
	)
	format "DEBUG: Vertex Type: %\n" mascara
	return mascara
)

function AddVertex IdMapMaterial VertexBuffer vtx1  indices =
(
	while VertexBuffer.count<IdMapMaterial do
	(
		append VertexBuffer (#())
	)
	local exist = FindVertex IdMapMaterial VertexBuffer vtx1
	if exist == -1 then
	(
		exist = VertexBuffer[IdMapMaterial].count
		append VertexBuffer[IdMapMaterial] vtx1
	)
	AddIndex IdMapMaterial indices exist
)

function AddIndex IdMapMaterial indices indice =
(
	while indices.count<IdMapMaterial do
	(
		append indices (#())
	)
	append indices[IdMapMaterial] indice
)

function FindVertex IdMapMaterial Vertices vtx =
(
	local size = Vertices[IdMapMaterial].count
	for i = 1 to size do
	(
		local IdFound=true
		for j=1 to Vertices[IdMapMaterial][i].count do
		(
			if Vertices[IdMapMaterial][i][j]!=vtx[j] then
				IdFound=false
		)
		if IdFound then
		(
			return i-1
		)
	)
	return -1
)
function CountTextures material =
(
	local cant = 0
	if material.diffuseMapEnable then
	(
		cant = cant + 1
	)
	if material.selfIllumMapEnable then
	(
		cant = cant + 1
	)
	if material.bumpMapEnable then
	(
		cant = cant + 1
	)
	if material.reflectionMapEnable then
	(
		cant = cant + 1
	)
	if material.filterMapEnable then
	(
		cant = cant + 1
	)
	if material.glossinessMapEnable then
	(
		cant = cant + 1
	)
	
	format "DEBUG: Cantidad de Texturas: %\n" cant
	return cant
)
function DisplayMessage message =
(
	messageBox message --title:"Exporting Mesh" beep:true
)
function DisplayWarning message =
(
	messageBox message title:"Warning Message" beep:true
)
function DisplayError message =
(
	messageBox message title:"Error Message" beep:true
)

function ExportTexturesPath file FileName CopyTextures ExportPath RelativeTexturesPath SaveTextPath=
(
	format "DEBUG: FileName:%\n" FileName
	format "DEBUG: ExportPath:%\n" ExportPath
	format "DEBUG: RelativeTexturesPath:%\n" RelativeTexturesPath
	format "DEBUG: SaveTextPath:%\n" SaveTextPath
	format "DEBUG: CopyTextures:%\n" CopyTextures
	
	local AbsoluteFileName=(FileName).fileName
	--local RelativeFileName=RelativeTexturesPath+filenameFromPath AbsoluteFileName
	local RelativeFileName=filenameFromPath AbsoluteFileName
	format "DEBUG: RelativeFileName:%\n" RelativeFileName
	local PathToImporter = "./"
	PathToImporter += GetRelativePath SaveTextPath
	PathToImporter += RelativeFileName
	local tmp = (PathToImporter.count)
	WriteShort file tmp #unsigned
	WriteString file PathToImporter
	format "DEBUG: Path to import %\n"  PathToImporter
	format "DEBUG: % - %\n" tmp PathToImporter
	if CopyTextures then
	(
		format "DEBUG: Copy from \"%\" to \"%\"\n" AbsoluteFileName (SaveTextPath+RelativeFileName)
		copyFile AbsoluteFileName (SaveTextPath+RelativeFileName)
	)	
)
function GetRelativePath AbsoluteFileName =
(
	local position = findString AbsoluteFileName "Data"
	local RelativePath = substring AbsoluteFileName position -1
	return RelativePath
)
function WriteStaticMeshElement Object3D file SaveInPath =
(
	local relativePathTemp = (SaveInPath+"Meshes/"+Object3D.name+".mesh")
	local pathTemp = GetRelativePath relativePathTemp
	format "\t<static_mesh name=\"%\" filename=\"%\" />\n" Object3D.name pathTemp to:file
)
function WriteXMLElement Object3D file SubDirectory=
(
	format "\t<static_mesh name=\"%\" filename=\"%\" />\n" Object3D.name (SubDirectory+Object3D.name+".mesh") to:file
)

function CreateRenderableObjXML  SaveInPath meshes =
(
	local filename = "RenderableObjects.xml"
	-- Para crear un fichero de tipo carácter stream
	local file = createfile (SaveInPath+"/"+filename)
	
	if file != undefined then
	(
		--local meshes = $*
		-- Para escribir sobre el fichero de tipo carácter stream utilizando la función format
		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<renderable_objs>\n" to:file
		
		for i = 1 to meshes.count do
		(
			if meshes[i].material != undefined then
			(
				WriteXMLObject meshes[i] file
			)
		)
		
		format "</renderable_objs>\n" to:file
		-- Para cerrar el fichero de tipo carácter streamç
		close file
	)
	else
	(
		DisplayWarning "No se ha podido crear el archivo %" filename
	)
)
function WriteXMLObject Object3D file =
(
	local RotationObject3D = Object3D.rotation
	Object3D.rotation=quat 0 0 1 0
	
	local invTransform = inverse Object3D.transform
	local maxS = "" as stringStream
	local MaxCoord = Object3D.max
	MaxCoord = MaxCoord * invTransform
	MaxCoord = RHTranslationToLH(MaxCoord)
	format "% % %" MaxCoord.x MaxCoord.y MaxCoord.z to:maxS
	local minS = "" as stringStream
	local MinCoord = Object3D.min
	MinCoord = MinCoord *invTransform
	MinCoord = RHTranslationToLH(MinCoord)
	format "% % %" MinCoord.x MinCoord.y MinCoord.z to:minS
	
	Object3D.rotation = RotationObject3D
	
	local matrix = Object3D.transform
	local ObjectName = Object3D.name
	local xml = "\t<object "
	
	xml += "name=\"" + ObjectName + "\" "
	xml += "core=\"" + ObjectName + "\" "
	xml += "pos=\"" + (matrix.translation.x as string)
	xml += " " + (matrix.translation.z as string)
	xml += " " + (matrix.translation.y as string) + "\" "
	--xml += "yaw=\"" + (GetYaw matrix as string) + "\" "
	--xml += "pitch=\"" + (GetPitch matrix as string) + "\" "
	--xml += "roll=\"" + (GetRoll matrix as string) + "\" "
	xml += "yaw=\"" + (GetYaw matrix as string) + "\" "
	xml += "pitch=\"" + (GetRoll matrix as string) + "\" "
	xml += "roll=\"" + (GetPitch matrix as string) + "\" "
	
	xml += "min=\"" + minS + "\" "
	xml += "max=\"" + maxS + "\" "
	
	if matchPattern ObjectName pattern:"nobox_*" then
	(
		if matchPattern ObjectName pattern:"*_no_visible" then
		(
			xml += "visible=\"false\" "
		)
		else
		(
			xml += "visible=\"true\" "
		)
		xml += "type=\"" + (NO_BOUNDING_OBJECT as string) + "\" "
	)-- if nobox_
	else
	(
		if matchPattern ObjectName pattern:"sphere_*" then
		(
			if matchPattern ObjectName pattern:"*_no_visible" then
			(
				xml += "visible=\"false\" "
			)
			else
			(
				xml += "visible=\"true\" "
			)
			xml += "type=\"" + (BOUNDING_SPHERE as string) + "\" "
		)-- if sphere_
		else
		(
			if matchPattern ObjectName pattern:"capsule_*" then
			(
				if matchPattern ObjectName pattern:"*_no_visible" then
				(
					xml += "visible=\"false\" "
				)
				else
				(
					xml += "visible=\"true\" "
				)
				xml += "type=\"" + (BOUNDING_CAPSULE as string) + "\" "
			)-- if capsule_
			else
			(
				if matchPattern ObjectName pattern:"mesh_*" then
				(
					if matchPattern ObjectName pattern:"*_no_visible" then
					(
						xml += "visible=\"false\" "
					)
					else
					(
						xml += "visible=\"true\" "
					)
					xml += "type=\"" + (BOUNDING_MESH as string) + "\" "
				)-- if mesh_
				else
				(
					-- box_
					/*En caso de que el objeto no sea explicitamente otro tipo de objeto fisico por defecto sera asignada una caja*/
					if matchPattern ObjectName pattern:"*_no_visible" then
					(
						xml += "visible=\"false\" "
					)
					else
					(
						xml += "visible=\"true\" "
					)
					xml += "type=\"" + (BOUNDING_BOX as string) + "\" "
					--default box_
				)-- else mesh_
			)-- else capsule_
		)-- else sphere_
	)--  else nobox_
	
	xml += "density=\"0.0\" "
	xml += "angularDamping=\"0.5\" "

	
	/*if matchPattern ObjectName pattern:"*fbs" then
	(
		xml += "min=\"" + minS + "\" "
		xml += "max=\"0.0f\" "
		--xml += "density=\"" + (Object3D.density  as string) + "\" "
		xml += "density=\"0.0\" "
		xml += "angularDamping=\"0.0\" "
		xml += "type=\"" + (BOUNDING_BOX as string) + "\" "-- Types: box, sphere, capsule, mesh
	)
	else
	(
		--Plane
		if matchPattern ObjectName pattern:"*fbp" then
		(
			xml += "min=\"0 1 0\" "
			xml += "max=\"" + (Object3D.length as string) + "\" "
			--xml += "density=\"" + (Object3D.density  as string) + "\" "
			xml += "density=\"0.0\" "
			xml += "angularDamping=\"0.0\" "
			xml += "type=\"" + (BOUNDING_BOX as string) + "\" "-- Types: box, sphere, capsule, mesh
		)
		else
		(
			--Box
			--if matchPattern ObjectName pattern:"*fbb" then
			--(
				xml += "min=\"" + minS + "\" "
				xml += "max=\"" + maxS + "\" "
				--xml += "density=\"" + (Object3D.density  as string) + "\" "
				xml += "density=\"0.0\" "
				xml += "angularDamping=\"0.0\" "
				xml += "type=\"" + (BOUNDING_BOX as string) + "\" "-- Types: box, sphere, capsule, mesh
			--)
		)
	)*/
	
	xml +=" />\n"
	
	format xml to:file
)

fn GetYaw transform =
(
	return (transform as eulerangles).z
)
fn GetPitch transform =
(
	--return (transform as eulerangles).x
	return -1 * (transform as eulerangles).x
)
fn GetRoll transform =
(
	return -1 * (transform as eulerangles).y
)

/*TRIGGERS*/
function ExportTriggersXML elements datapath =
(
	local file = createfile (datapath + "Triggers.xml")
	
	if file != undefined then
	(
		--local meshes = $*
		-- Para escribir sobre el fichero de tipo carácter stream utilizando la función format
		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<renderable_objs>\n" to:file
		for i = 1 to elements.count do
		(
			if matchPattern elements[i].name pattern:"Trigger_*" then
			(
				local yaw = GetYaw (elements[i].transform)
				local position = elements[i].position
				position = RHTranslationToLH(position)
				
				/*MAX & MIN*/
				local RotationObject3D = elements[i].rotation
				elements[i].rotation=quat 0 0 1 0
				local invTransform = inverse elements[i].transform
				local MaxCoord = elements[i].max
				MaxCoord = MaxCoord * invTransform
				MaxCoord = RHTranslationToLH(MaxCoord)
				local MinCoord = elements[i].min
				MinCoord = MinCoord *invTransform
				MinCoord = RHTranslationToLH(MinCoord)
				elements[i].rotation = RotationObject3D
				
				format "\t<Trigger " to:file
				format "name=\"%\" " elements[i].name to:file
				format "entity=\"character\" " to:file
				format "position=\"% % %\" " position.x position.y position.z to:file
				format "max=\"% % %\" " MaxCoord.x MaxCoord.y MaxCoord.z to:file
				format "min=\"% % %\" " MinCoord.x MinCoord.y MinCoord.z to:file
				format "active=\"true\" " minS to:file
				format "onEnter=\"\" " minS to:file
				format "onLeave=\"\" " minS to:file
				format "rotation=\"%\"" yaw to:file
				format " />\n" to:file
			)
		)
		format "</renderable_objs>\n" to:file
		-- Para cerrar el fichero de tipo carácter streamç
		close file
	)
)

/*CINEMATICAS*/
fn FindObjOnObjs Obj Objs =
(
    for b=1 to Objs.count do
    (
        if (areNodesInstances Obj Objs[b])==true then
            return Objs[b]
    )
    return undefined
)

fn GetFileName str = 
(
    local StrCopy=str
    local IdLast=0
    local IdFound
    do
    (
        IdFound=findString StrCopy "\\" 
        if IdFound!=undefined then
        (
            StrCopy=substring StrCopy (IdFound+1) (StrCopy.count-IdFound)
        )
    ) while (IdFound!=undefined)
    --format "%\n" (findString str "\\")
    return StrCopy
)

function ConvertPos3ToString pos = 
(
    local value_pos=(pos.x as String)+" "+(pos.y as String)+" "+(pos.z as String)
    return value_pos
)