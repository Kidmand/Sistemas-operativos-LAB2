// SEMAFORO
#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "riscv.h"
#include "memlayout.h"
#include "defs.h"

#define MAX_SEMAPHORES 100 // Número máximo de semáforos, (Ej: con 100 se tienen 101 semaforos).

#define ERROR_CODE -1
#define SUCCESS_CODE 0

#define NULL 0

struct semaphore
{
    struct spinlock lock; // Lock del semaforo.
    int value;            // Valor del semaforo (0 , 1 ,2 , ...).
};

struct semaphore *semaphore_table[MAX_SEMAPHORES];

/* -------------- Funciones AUXILIARES ----------------*/

/* Obtiene un semáforo. */
struct semaphore *get_sem(int id_sem)
{
    return semaphore_table[id_sem];
}

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
    return get_sem(id_sem) != NULL;
}

/* ------------- Funciones para el USER ---------------*/

/* Inicializar un semáforo.
 *
 * PRECON:
 *   - 0 <= id_sem <= MAX_SEMAPHORES.
 *   - value  >= 0.
 *   - El id del semáforo NO debe estar en uso.
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
    if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
    {
        printf("Id fuera de rango.\n");
        return ERROR_CODE;
    }
    if (value < 0)
    {
        printf("Valor fuera de rango.\n");
        return ERROR_CODE;
    }
    if (is_sem_open(id_sem))
    {
        printf("El semaforo ya esta inicialzado.\n");
        return ERROR_CODE;
    }

    struct semaphore *s = get_sem(id_sem);

    if ((s = (struct semaphore *)kalloc()) == 0)
    {
        printf("Fallo al alojar la memoria del semaforo.\n");
        return ERROR_CODE;
    }
    initlock(&s->lock, "nombre_arbitrario");

    s->value = value;

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
        printf("Id fuera de rango.\n");
        return ERROR_CODE;
    }

    // struct semaphore *s = get_sem(id_sem);

    // kfree((char *)s);

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
    if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
    {
        printf("Id fuera de rango.\n");
        return ERROR_CODE;
    }
    if (!is_sem_open(id_sem))
    {
        printf("El semaforo no esta inicializado.\n");
        return ERROR_CODE;
    }

    struct semaphore *s = get_sem(id_sem);

    acquire(&s->lock);
    s->value += 1;
    wakeup(&s);
    release(&s->lock);

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
    if (id_sem < 0 || id_sem >= MAX_SEMAPHORES)
    {
        printf("Id fuera de rango.\n");
        return ERROR_CODE;
    }
    if (!is_sem_open(id_sem))
    {
        printf("El semaforo no esta inicializado.\n");
        return ERROR_CODE;
    }

    struct semaphore *s = get_sem(id_sem);

    acquire(&s->lock);
    while (s->value == 0)
        sleep(&s, &s->lock);
    s->value -= 1;
    release(&s->lock);

    return SUCCESS_CODE;
}
