filein "functions.ms"
global HEADER 0xFF77
global FOOTER 0X77FF

function WriteDummyPositions Dummies file =
(
	for i=1 to Dummies.count do
	(
		if getUserProp Dummies[i] "type" == "\"PATH\"" then
		(		
			if Dummies[i].position.z == 0.0 then
			(	
				--Dummies[i].position = Dummies[i].position * (inverse Dummies[i].transform) 		
				Dummies[i].position = RHTranslationToLH(Dummies[i].position)
			)
			local id = getUserProp Dummies[i] "id"
			format "%:%\n" id Dummies[i].position
			
			local tmp = (id.count)
			WriteShort file tmp #unsigned	--id.count
			WriteString file id						--id
			
			WriteFloat file Dummies[i].position.x	 --posx
			WriteFloat file Dummies[i].position.y  --posy	
			WriteFloat file Dummies[i].position.z  --posz
		)
	)	
)

function WriteDummyLinks Dummies file =
(
	for i=1 to Dummies.count do
	(
		if getUserProp Dummies[i] "type" == "\"PATH\"" then
		(				
			local id 				= getUserProp Dummies[i] "id"
			--aDummy.position = RHTranslationToLH(aDummy.position)
			local numVecinos = getUserProp Dummies[i] "numVecinos"

			local vecino1 		= getUserProp Dummies[i] "a"	
			local vecino2 		= getUserProp Dummies[i] "b"	
			local vecino3 		= getUserProp Dummies[i] "c"	
			local vecino4 		= getUserProp Dummies[i] "d"	
			
			format "%:-%-%,%,%,%\n" id numVecinos vecino1 vecino2 vecino3 vecino4
			local tmp = id.count
			WriteShort file tmp #unsigned	--id.count
			WriteString file id						--id
			
			WriteShort file numVecinos #unsigned	--numVecinos
			
			tmp = vecino1.count
			WriteShort file tmp #unsigned	--vecino1.count
			WriteString file vecino1				--vecino1

			if vecino2 != undefined then
			(
				tmp = vecino2.count
				WriteShort file tmp #unsigned	--vecino2.count
				WriteString file vecino2				--vecino2
			)
			if vecino3 != undefined then
			(
				tmp = vecino3.count
				WriteShort file tmp #unsigned	--vecino3.count
				WriteString file vecino3				--vecino3
			)
			if vecino4 != undefined then
			(
				tmp = vecino4.count
				WriteShort file tmp #unsigned	--vecino4.count
				WriteString file vecino4				--vecino4
			)
		)
	)	
)

function GetPathDummies pathfile =
(
	format "creando fichero de paths...\n"
	local NumObjects = helpers
	format "%\n" NumObjects.count
	local NumNodos = NumObjects.count
	
	filename = pathfile + "/paths.dat"
	
	file=fopen filename "wb"
	WriteShort file HEADER #unsigned
	WriteShort file NumNodos #unsigned
	--selecciona dummies
	WriteDummyPositions NumObjects file
	
	WriteDummyLinks NumObjects file
	WriteShort file FOOTER #unsigned

	--Para hacer un flush de los bytes no escritos en el fichero
	fflush file
	-- Para cerrar el fichero
	fclose file
	format "fichero creado\n"
)

-- GetPath_Dummies "c:/vjuab2010"

