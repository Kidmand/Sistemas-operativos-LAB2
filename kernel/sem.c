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
 *   - `-1` en caso de error.
 */
int is_sem_open(int id_sem)
{
  /* Manejo de errores */
  if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
    return ERROR_CODE; // Id fuera de rango.

  return semaphore_table[id_sem].status == IS_OPEN;
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
  /* Manejo de errores */
  if ((id_sem < 0 || id_sem >= MAX_SEMAPHORES) || value < 0)
    return ERROR_CODE; // Id fuera de rango -o- Valor fuera de rango

  /* Se inicializa el semaforo */
  initlock(&semaphore_table[id_sem].lock, "0" + id_sem); // Se inicializa el lock y su nombre es el id del semaforo.

  acquire(&semaphore_table[id_sem].lock); // Se abre la zona critica.

  /* Manejo de estados */
  if (is_sem_open(id_sem)) // El caso de que el semaforo este en uso.
  {
    release(&semaphore_table[id_sem].lock);
    return 1; // Codigo de error para avisar que el semaforo esta abierto.
  }
  else
  {
    // Se establecen las variables necesarias
    semaphore_table[id_sem].status = IS_OPEN;
    semaphore_table[id_sem].value = value;

    release(&semaphore_table[id_sem].lock);
    return SUCCESS_CODE;
  }
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
  /* Manejo de errores */
  if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
    return ERROR_CODE; // Id fuera de rango.

  acquire(&semaphore_table[id_sem].lock); // Se abre la zona critica.

  semaphore_table[id_sem].status = NOT_OPEN;

  release(&semaphore_table[id_sem].lock);

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
  /* Manejo de errores */
  if ((id_sem < 0 || id_sem >= MAX_SEMAPHORES) || !is_sem_open(id_sem))
    return ERROR_CODE; // Id fuera de rango -o- El semaforo no esta inicializado

  acquire(&semaphore_table[id_sem].lock); // Se "prende" el lock.
  // --------SECCIÓN CRÍTICA --------
  semaphore_table[id_sem].value += 1;
  // --------------------------------
  wakeup(&semaphore_table[id_sem]);       // Se "despierta" el procesos que estaba durmiendo.
  release(&semaphore_table[id_sem].lock); // Se "apaga" el lock.

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
  /* Manejo de errores */
  if ((id_sem < 0 || id_sem >= MAX_SEMAPHORES) || !is_sem_open(id_sem))
    return ERROR_CODE; // Id fuera de rango -o- El semaforo no esta inicializado

  acquire(&semaphore_table[id_sem].lock);    // Se "prende" el lock.
  while (semaphore_table[id_sem].value == 0) // Se "dureme" el procesos simepre que el `valor` sea 0.
    sleep(&semaphore_table[id_sem], &semaphore_table[id_sem].lock);
  // --------SECCIÓN CRÍTICA --------
  semaphore_table[id_sem].value -= 1;
  // --------------------------------
  release(&semaphore_table[id_sem].lock); // Se "apaga" el lock.

  return SUCCESS_CODE;
}

/* ------------- Funciones solo para el KERNEL ---------------*/

/* Inicializa los semáforos (KERNEL).
 *
 * ¿Como funciona ?
 * Establece todos los semaforos en `NOT_OPEN`.
 */
void init_semaphore()
{
  for (int id = 0; id < MAX_SEMAPHORES; id++) // Se recorren todo los semaforos
    semaphore_table[id].status = NOT_OPEN;    // Establece que el semáforo está en uso
}