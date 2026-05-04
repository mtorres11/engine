filein "functions.ms"

function GetLight Luz Archivo =
(
	local linea ="\t<light name =\""
	linea += Luz.name
	linea += "\" type=\""
	local dir=point3 0 0 0
	local cLuz = classof Luz
	if cLuz == targetSpot then
	(
		if Luz.target != undefined then
		(
			dir = Luz.target.pos
		)
		linea +="spot\" pos=\""
		linea += GetVect3fAsString (RHTranslationToLH (Luz.pos))
		linea += "\" dir=\""
		linea += GetVect3fAsString (RHTranslationToLH (dir))
		linea += "\" color=\""
		linea += GetColorAsString (Luz.color/255)
		linea += "\" angle=\""
		linea += (Luz.hotspot As String)
		linea += "\" falloff=\""
		linea += (Luz.falloff As String)
		linea += "\" att_start_range=\""
		linea += (Luz.farAttenStart As String)
		linea += "\" att_end_range=\""
		linea += (Luz.farAttenEnd As String)
		--dir=(Luz.target.pos - Luz.pos) --Debe ir esto ultimo
		--dir=normalize dir
	)
	else if cLuz == targetDirectionalLight then
	(
		if Luz.target != undefined then
		(
			dir = Luz.target.pos
		)
		linea +="directional\" pos=\""
		linea += GetVect3fAsString (RHTranslationToLH (Luz.pos))
		linea += "\" dir=\""
		linea += GetVect3fAsString (RHTranslationToLH (dir))
		linea += "\" color=\""
		linea += GetColorAsString (Luz.color/255)
		linea += "\" render_shadows=\""
		linea += "false"--de momento falso
		linea += "\" att_start_range=\""
		linea += (Luz.farAttenStart As String)
		linea += "\" att_end_range=\""
		linea += (Luz.farAttenEnd As String)		
		--dir=(Luz.target.pos - Luz.pos) --Debe ir esto ultimo
		--dir=normalize dir
	)
	else if cLuz == Omnilight then
	(
		linea +="omni\" pos=\""
		linea += GetVect3fAsString (RHTranslationToLH (Luz.pos))
		linea += "\" color=\""
		linea += GetColorAsString (Luz.color/255)
--		linea += "\" falloff=\""
--		linea += (Luz.falloff As String)
		linea += "\" att_start_range=\""
		linea += (Luz.farAttenStart As String)
		linea += "\" att_end_range=\""
		linea += (Luz.farAttenEnd As String)
		
	)
	linea += "\"/>"
	--format "%\n"linea
	format"%\n" linea to:Archivo
)


function CreateLightsManager SaveInPath = 
(
	format "DEBUG: Info: Export Lights."
	local filename = "lights.xml"
	-- Para crear un fichero de tipo carácter stream
	local file = createfile (SaveInPath+"/"+filename)
	
	if file != undefined then
	(
		local ObjLuminicos = Lights
		-- Para escribir sobre el fichero de tipo carácter stream utilizando la función format
		format "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n" to:file
		format "<lights>\n" to:file
		
		for i = 1 to ObjLuminicos.count do
		(
			local cLuz = classof ObjLuminicos[i]
			--if cLuz == targetSpot or cLuz == targetDirectionalLight or cLuz == Omnilight then
			--(
				GetLight ObjLuminicos[i] file --(SaveInPath+"Lights/")
			--)
		)
		format "</lights>\n" to:file
		-- Para cerrar el fichero de tipo carácter stream
		format "DEBUG: Info: Lights Exported."
		close file
	)
	else
	(
		DisplayWarning "No se ha podido crear Lights.xml"
	)
)

--CreateLightsManager "D:/vjuab2010/Run/Test/Data"