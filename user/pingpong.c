#include "kernel/types.h"
#include "user/user.h"

void print_message(char *message, int cant, int SEM_ID)
{
    for (int i = 0; i < cant; i++)
    {
        sem_down(SEM_ID);
        printf("%s", message);
        sem_up(SEM_ID);
    }
}

/**
 * PingPong
 *
 * Este programa toma un argumento entero N (rally) que determina la cantidad de veces que se imprimirán las
 * palabras "ping" y "pong" por pantalla. El programa crea dos procesos hijos que se alternan para imprimir
 * "ping" y "pong" N veces, asegurándose de que nunca se impriman dos "ping" o dos "pong" seguidos.
 *
 * Parámetros:
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

    /*  Estaria bueno mejorar la implemntacion para que detecte
        si se pasa un parametro que no es un numero.               */

    int num = atoi(argv[1]); // Se convierte a entero el argumento.
    int SEM_ID = 0;

    // Bucle para encontrar un semaforo abierto
    int res_sem = sem_open(SEM_ID, 1);

    /* Si el semaforo da error o no esta abierto, prueba con el siguiente */
    while (res_sem == -1 || res_sem == 1)
    {
        SEM_ID++;
        res_sem = sem_open(SEM_ID, 1);
    }

    //En caso de error terminamos el programa
    if (res_sem == -1)
    {
        printf("ERROR: No se pudo abrir el semaforo.\n");
        exit(1);
    }

    int pc_id_1,
        pc_id_2;

    pc_id_1 = fork();

    if (pc_id_1 < 0)
    {
        printf("ERROR: Fallo el fork.\n");
    }
    else if (pc_id_1 == 0) // Si el proceso de primer hijo
    {
        print_message("ping\n", num, SEM_ID);
    }
    else
    {
        pc_id_2 = fork();

        if (pc_id_2 < 0)
        {
            printf("ERROR: Fallo el fork.\n");
        }
        else if (pc_id_2 == 0 && pc_id_1 > 0) // Si el proceso del segundo hijo
        {
            print_message("    pong\n", num, SEM_ID);
        }
        else
        {
            wait(&pc_id_1);
            wait(&pc_id_2);

            sem_close(SEM_ID);
        }
    }

    exit(0);
}