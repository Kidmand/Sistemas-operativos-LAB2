// SEMAFORO
#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "memlayout.h"
#include "defs.h"

#define MAX_SEMAPHORES 100 // Número máximo de semáforos, (Ej: con 100 se tienen 101 semaforos).

// Codigos de retorno.
#define ERROR_CODE -1
#define SUCCESS_CODE 0

// Valores para indicar si un semáforo está en uso.
#define IS_OPEN 0
#define NOT_OPEN 1

#define NULL 0

struct semaphore
{
  struct spinlock lock; // Lock del semaforo.
  int value;            // Valor del semaforo (0 , 1 ,2 , ...).
  int status;           // Valor para indicar si el semaforo está en uso (IS_OPEN o NOT_OPEN).
};

struct semaphore semaphore_table[MAX_SEMAPHORES];

/* -------------- Funciones AUXILIARES ----------------*/

/* Obtiene el estado de uso de un semáforo.
 *
 * PRECON:
 *   - 0 <= id_sem <= MAX_SEMAPHORES.
 *
 * RETURN:
 *   - `1` si el semáforo está en uso.
 *   - `0` si el semáforo NO está en uso.
 */
int is_sem_open(int id_sem)
{
  return semaphore_table[id_sem].status == IS_OPEN;
}

/* Convierte un numero positivo a string*/
char *intToString(int numero)
{
  char *cadena;
  // Caso especial para el número 0
  if (numero <= 0)
  {
    cadena = "0";
    return cadena;
  }

  // Inicializa un índice
  int indice = 0;

  // Convierte los dígitos del número en caracteres y los almacena en el arreglo inversamente
  while (numero > 0)
  {
    int digito = numero % 10;
    cadena[indice++] = '0' + digito;
    numero /= 10;
  }

  // Invierte la cadena para que quede en el orden correcto
  int i, j;
  char temp;
  for (i = 0, j = indice - 1; i < j; i++, j--)
  {
    temp = cadena[i];
    cadena[i] = cadena[j];
    cadena[j] = temp;
  }

  // Agrega el carácter nulo al final para indicar el final de la cadena
  cadena[indice] = '\0';

  return cadena;
}

/* ------------- Funciones para el USER ---------------*/

/* Inicializar un semáforo.
 *
 * PRECON:
 *   - 0 <= id_sem <= MAX_SEMAPHORES.
 *   - value  >= 0.
 *
 * PARAMS:
 *   - id_sem: El id del semaforo a crear.
 *   - value:  El valor inicial del semáforo.
 *
 * RETURN:
 *   - `0` en caso de éxito (se inicializo un nuevo semaforo).
 *   - `1` en caso de que el semaforo este en uso.
 *   - `-1` en caso de error.
 */
int sem_open(int id_sem, int value)
{
  if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
  {
    printf("KERNEL-ERROR: Id fuera de rango.\n");
    return ERROR_CODE;
  }
  if (value < 0)
  {
    printf("KERNEL-ERROR: Valor fuera de rango.\n");
    return ERROR_CODE;
  }

  if (is_sem_open(id_sem)) // El caso de que el semaforo este en uso.
    return 1;

  // Se inicializa el lock y su nombre es el id del semaforo.
  initlock(&semaphore_table[id_sem].lock, intToString(id_sem));

  semaphore_table[id_sem].status = IS_OPEN;
  semaphore_table[id_sem].value = value;

  return SUCCESS_CODE;
}

/* Cerrar un semáforo.
 * Libera los recursos asociados al semáforo.
 *
 * PRECON:
 *   - 0 <= id_sem <= MAX_SEMAPHORES.
 *
 * PARAMS:
 *   - id_sem: Id del semáforo a cerrar.
 *
 * RETURN:
 *   - `0` en caso de éxito, otro valor en caso de error.
 *
 * Posibles errores:
 *   - El semáforo no existe o se produjo un error interno.
 */
int sem_close(int id_sem)
{
  if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
  {
    printf("KERNEL-ERROR: Id fuera de rango.\n");
    return ERROR_CODE;
  }

  semaphore_table[id_sem].status = NOT_OPEN;

  return SUCCESS_CODE;
}

/* Incrementa el semáforo `sem`.
 * Indica que se ha terminado de utilizar y lo está liberando.
 *
 * PRECON:
 *   - El semáforo debe estar inicializado.
 *   - 0 <= id_sem <= MAX_SEMAPHORES.
 *
 * PARAMS:
 *   - id_sem: Id del semáforo a incrementar.
 *
 * RETURN:
 *   - `0` en caso de éxito, otro valor en caso de error.
 */
int sem_up(int id_sem)
{
  if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
  {
    printf("KERNEL-ERROR: Id fuera de rango.\n");
    return ERROR_CODE;
  }
  if (!is_sem_open(id_sem))
  {
    printf("KERNEL-ERROR: El semaforo (%d) no esta inicializado. (up)\n", id_sem);
    return ERROR_CODE;
  }

  acquire(&semaphore_table[id_sem].lock);
  semaphore_table[id_sem].value += 1;
  wakeup(&semaphore_table[id_sem]);
  release(&semaphore_table[id_sem].lock);

  return SUCCESS_CODE;
}

/* Decrementa el semáforo `sem`
 * Se utiliza para empezar a usar el semáforo. Bloqueando los procesos cuando su valor es `0`.
 *
 * PRECON:
 *   - El semáforo debe estar inicializado.
 *   - 0 <= id_sem <= MAX_SEMAPHORES.
 *
 * PARAMS:
 *   - id_sem: Id del semáforo a cerrar.
 *
 * RETURN:
 *   - `0` en caso de éxito, otro valor en caso de error.
 */
int sem_down(int id_sem)
{
  if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
  {
    printf("KERNEL-ERROR: Id fuera de rango.\n");
    return ERROR_CODE;
  }
  if (!is_sem_open(id_sem))
  {
    printf("KERNEL-ERROR: El semaforo (%d) no esta inicializado. (down)\n", id_sem);
    return ERROR_CODE;
  }

  acquire(&semaphore_table[id_sem].lock);
  while (semaphore_table[id_sem].value == 0)
    sleep(&semaphore_table[id_sem], &semaphore_table[id_sem].lock);
  semaphore_table[id_sem].value -= 1;
  release(&semaphore_table[id_sem].lock);

  return SUCCESS_CODE;
}

/* ------------- Funciones para el KERNEL ---------------*/

/* Inicializa los semáforos.*/
void init_semaphore()
{
  for (int id = 0; id < MAX_SEMAPHORES; id++)
    semaphore_table[id].status = NOT_OPEN; // Indica que el semáforo está en uso
}