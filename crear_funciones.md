# ¿ Como crear y usar funciones en USER-SPACE y KERNEL-SPACE ?

## USER-SPACE (ejemplo)
1. Crear un archivo `func.c` en la carpeta **/user**
2. Editar el `Makefile` agregando `$U/_suma\` en `UPROGS`
3. Agregarle los includes necesarios
   ``` c
   #include "kernel/stat.h"   // Agrega una estructura para el manejo de archivos, directorios, etc.
   #include "user/user.h"     // Agrega las llamadas al sistema (syscalls del kernel).
   #include "kernel/types.h"  // Agrega diferentes tipos de datos (si usas user.h, debes agregarla sí o sí).
   ```
4. Listo ya esta implementada


## KERNEL-SPACE (ejemplo)
1. Crear un archivo `func.c` en la carpeta **/kernel** e implementa todas tus funciones (func1, func2, ...)
2. Agregar `$K/func.o \` a **MAKEFLE** en `OBJS` (linea 4)
3. Agregar todas las **funciones implementadas en kernel/func.c** a `user/usys.pl`    
   ``` c 
      entry("func1");     
   ```
4. Agregar todas las **funciones implementadas en kernel/func.c** a `user/user.h`      
   ``` c 
      int func1(int);    
   ```
5. Agregar todas las **especificaciones de las funciones implementadas en kerner/func.c**  a `kernel/defs.h`
    ``` c 
    int  func1(char*, char**); 
    ```
6. Agregar todas las **funciones implementadas en kerner/func.c** a `kernel/syscall.h`   
   ``` c  
      #define SYS_func1  22 
   ```       
7. Agregar todas las **funciones implementadas en kerner/func.c** a `kernel/syscall.c` en estos dos lugares del codigo.   
   ``` c
      extern uint64 sys_func1(void);    //(linea 82) 
      [SYS_func1]   sys_func1,          //(linea 107)
   ```
8. Una vez agregado lo anterior al kernel, decidir donde implementar lo ulitmo, en **(sysfile.c)** o **(sysproc.c)** aca se define como se pasan los argumentos del modo usuario al modo kernel, porque se deben copiar. Ejemplo:
   ``` c
   uint64 sys_func1(void)
   {
      int a , b;     
     argint(0, &a);
     argint(1, &b);    
     return func1(a, b); 
     // Llama a tu función sum implementada en fun   pasandole dos argumentos
   }
   ```
9. Listo ya esta implementada


## Ejemplo que usa USER-SPACE & KERNEL-SPACE
Vamos a hacer un simple programa de consola que toma dos numeros y retorna la suma por pantalla. Segui todos los pasos para la implementacion.

1. Crear el archivo en `/user/suma.c`
2. Editar el `Makefile` agregando `$U/_suma\` en `UPROGS`
3. Pegar este codigo en `suma.c`
   ``` c
   #include "kernel/types.h"
   #include "user/user.h"

   int main(int argc, char *argv[])
   {
    if (argc <= 2)
    {
        printf("El funcion toma dos argumentos.\n");
        exit(1);
    }
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    printf("%d + %d = %d \n", a, b, sum(a, b));
    exit(0);
   }
   ```
4. Crear el archivo `kernel/sum.c`
5. Editar el `Makefile` agregando `$K/sum.o \` en `OBJS`
6. Pegar este codigo en `sum.c`
   ``` c
   int sum(int a, int b)
   {
      return a + b;
   }
   ```
7. Se agrega en `user/usys.pl` 
   ``` pl
   entry("sum");
   ```
8. Se agrega en `user/user.h` 
   ``` c
   int sum(int, int); // sum(): Suma dos numeros
   ```
9. Agregar en `kernel/defs.h`
    ``` c
    //  sum.c
    int sum(int, int);
    ```
10. Agregar en `kernel/syscall.h`
    ``` c
    #define SYS_sum 22
    ```
11. Agregar en `kernel/syscall.h`
    ``` c
    extern uint64 sys_sum(void);
    ```
    ``` c
    [SYS_sum] sys_sum
    ```
12. Agregar en `kernel/sysfile.h`
    ``` c
    uint64
    sys_sum(void)
    {
    int a, b;

    argint(0, &a);
    argint(1, &b);

    return sum(a, b); // Llama a tu función sum implementada en sum.c
    }
    ```
13. **Listo terminamos toda la implementacion**. Ahora ejecuta `make qemu` y una vez tengas la terminal andando proba ejecutar `suma 1 2` esto deberia retornar `1 + 2 = 3`. 