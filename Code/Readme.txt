Los archivos "Test.vcproj.Killos.Marco Torres.user" son los encargados en visual studio
de guardar el directorio de trabajo de los proyectos.
En esta solucion on hay ningun archivo con extension .user en el SVN, asi que se tienen que
configurar manualmente cada vez que se vaya a correr el proyecto o solucion.
Esta configuracion se hace dando click derecho en el proyecto a correr, luego propiedades y
Depurador (Debuging), por ultimo en el directorio de trabajo se debe configurar con
"../../Run/{NombreDelProyecto}". Por ejemplo, para el proyecto Test se debe configurar con el path:
"../../Run/Test".
Tambien es importante construir los proyectos por separado debido a que algunas dependencias podrian
verse afectadas la primera ves que se corre el proyecto.