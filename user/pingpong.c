#include "kernel/types.h"
#include "user/user.h"

/* Crear un semaforo nuevo.
 *
 * Itera hasta encontrar un semaforo disponible.
 *
 * PARAMS:
 *   - value: Número con que inicializar el semaforo.
 *
 * RETURN:
 *   - En caso de devuelde el `Id` del semaforo creado.
 *   - `-1` en caso de error.
 */
int create_sem(int value)
{
    int SEM_ID = 0;

    // Bucle para encontrar un semaforo abierto
    int res_sem = sem_open(SEM_ID, value);
    /* Si el semaforo da error o no esta abierto, prueba con el siguiente */
    while (res_sem == -1 || res_sem == 1)
    {
        SEM_ID++;
        res_sem = sem_open(SEM_ID, value);
    }

    return SEM_ID;
}

/* PingPong
 *
 * Este programa toma un argumento entero N (rally) que determina la cantidad de veces que se imprimirán las
 * palabras "ping" y "pong" por pantalla. El programa crea dos procesos hijos que se alternan para imprimir
 * "ping" y "pong" N veces, asegurándose de que nunca se impriman dos "ping" o dos "pong" seguidos.
 *
 * PARAMS:
 * - N: Número entero positivo que indica la cantidad de veces que se imprimirán "ping" y "pong".
 *
 * Ejemplo de uso:
 *
 * $ pingpong 1
 * ping
 *      pong
 */
int main(int argc, char *argv[])
{
    // Reviso que se paso solo un parametro y que no es negativo.
    if (argc != 2 || argv[1][0] == '-')
    {
        printf("ERROR : No se pasaron correctamente los parametros.\n");
        exit(1);
    }

    int num = atoi(argv[1]); // Se convierte a entero el argumento.
    if (num == 0)            // Si el argumento es 0, terminamos el programa.
        return 0;

    // Incializo los semaforos
    int SEM_PING = create_sem(1);
    int SEM_PONG = create_sem(0);

    // En caso de error terminamos el programa
    if (SEM_PING == -1 || SEM_PONG == -1)
    {
        printf("ERROR: No se logro abrir los semaforos correctamente.\n");
        exit(1);
    }

    int pc_id_1,
        pc_id_2;

    pc_id_1 = fork();

    if (pc_id_1 < 0)
    {
        printf("ERROR: Fallo el fork.\n");
    }
    else if (pc_id_1 == 0) // Si es el primer hijo
    {
        int status_pong = 0, status_ping = 0; // Variables para controlar los errores de los semaforos.

        // La primera vez (i = 0) ejecuta PING porque se inicializo en 1 el semaforo.
        for (int i = 0; (i < num) && status_pong == 0 && status_ping == 0; i++)
        {
            status_ping = sem_down(SEM_PING); // Espero que PONG me active
            printf("ping\n");
            status_pong = sem_up(SEM_PONG); // Le aviso a PONG que ya puede escribir
        }
    }
    else
    {
        pc_id_2 = fork();

        if (pc_id_2 < 0)
        {
            printf("ERROR: Fallo el fork.\n");
        }
        else if (pc_id_2 == 0 && pc_id_1 > 0) // Si es el segundo hijo
        {
            int status_pong = 0, status_ping = 0; // Variables para controlar los errores de los semaforos.

            // La primera vez (i = 0) se esta esperando que PING active el PONG porque el semaforo empezo en 0.
            for (int i = 0; (i < num) && status_pong == 0 && status_ping == 0; i++)
            {
                status_pong = sem_down(SEM_PONG); // Espero que PING me active
                printf("    pong\n");
                status_ping = sem_up(SEM_PING); // Le aviso a PING que ya puede escribir
            }
        }
        else
        {
            wait(&pc_id_1);
            wait(&pc_id_2);

            // Liberamos los semaforos.
            sem_close(SEM_PING);
            sem_close(SEM_PONG);
        }
    }

    exit(0);
}