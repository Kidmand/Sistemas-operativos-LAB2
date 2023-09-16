// SEMAFORO
#include "types.h"
#include "param.h"
#include "riscv.h"
#include "memlayout.h"
#include "defs.h"

#define MAX_SEMAPHORES 100 // Número máximo de semáforos, (Ej: con 100 se tienen 101 semaforos).
#define NOT_IN_USE -1      // Valor para indicar que un semáforo no está en uso.
#define IN_USE = 0         // Valor para indicar que un semáforo está en uso.

#define ERROR_CODE -1
#define SUCCESS_CODE 0

struct semaphore
{
    int value;  // Valor del semaforo (0 , 1 ,2 , ...)
    int in_use; // Bool que indica si esl semaforo está en uso
};

struct semaphore semaphore_table[MAX_SEMAPHORES];

/* -------------- Funciones AUXILIARES ----------------*/

/* Obtiene el estado de uso de un semáforo.
 *
 * PRECON:
 *   - 0 <= id_sem <= MAX_SEMAPHORES.
 */
int is_sem_in_use(int id_sem)
{
    return semaphore_table[id_sem].in_use;
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
 *   - `0` en caso de éxito, otro valor en caso de error.
 */
int sem_open(int id_sem, int value)
{
    if (!(0 <= id_sem && id_sem >= MAX_SEMAPHORES))
        return ERROR_CODE;
    if (!(value >= 0))
        return ERROR_CODE;

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
    if (!(0 <= id_sem && id_sem >= MAX_SEMAPHORES))
        return ERROR_CODE;

    return SUCCESS_CODE;
}

/* Incrementa el semáforo `sem`.
 * Indica que se ha terminado de utilizar y lo está liberando.
 *
 * PRECON:
 *   - El semáforo debe estar en uso (in_use == IN_USE).
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
    if (!(0 <= id_sem && id_sem >= MAX_SEMAPHORES))
        return ERROR_CODE;
    if (is_sem_in_use(id_sem) == NOT_IN_USE)
        return ERROR_CODE;

    return SUCCESS_CODE;
}

/* Decrementa el semáforo `sem`
 * Se utiliza para empezar a usar el semáforo. Bloqueando los procesos cuando su valor es `0`.
 *
 * PRECON:
 *   - El semáforo debe estar en uso (in_use == IN_USE).
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
    if (!(0 <= id_sem && id_sem >= MAX_SEMAPHORES))
        return ERROR_CODE;
    if (is_sem_in_use(id_sem) == NOT_IN_USE)
        return ERROR_CODE;

    return SUCCESS_CODE;
}

/* --------------- Funcion para el KERNEL --------------*/

/* Inicializa los semáforos.
 */
void init_semaphore()
{
    for (int i = 0; i < MAX_SEMAPHORES; i++)
    {
        semaphore_table[i].value = 0;           // Establece el valor inicial del semáforo
        semaphore_table[i].in_use = NOT_IN_USE; // Indica que el semáforo está en uso
    }
}