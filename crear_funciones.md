¿ Como crear funciones en USER-SPACE y KERNEL-SPACE ?


USER-SPACE
 - 1) Crear un archivo func.c en la carpeta /user
 - 2) Agregarle los includes necesarios
 - 2) Agregar _func en el Makefile en el UPROGS (linea 118)

KERNEL-SPACE 
 - 1) Crear un archivo func.c en la carpeta /kernel
 - 2) 