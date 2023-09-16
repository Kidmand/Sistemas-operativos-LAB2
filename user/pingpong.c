#include "kernel/types.h"
#include "user/user.h"

#define SEM_ID 0

void print_message(char *message)
{
    sem_down(SEM_ID);
    printf("%s\n", message);
    sem_up(SEM_ID);
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

    // int num = atoi(argv[1]); // Se convierte a entero el argumento.

    int res_sem = sem_open(SEM_ID, 1);
    if (res_sem != 0)
    {
        printf("ERROR: Fallo el sem_open.\n");
        return (-1);
    }

    int pc_id_1, pc_id_2;

    pc_id_1 = fork();

    if (pc_id_1 == 0) // Si el proceso de primer hijo
    {
        print_message("ping\n");
    }
    else if (pc_id_1 < 0)
        printf("ERROR: Fallo el fork.\n");

    pc_id_2 = fork();

    if (pc_id_2 == 0 && pc_id_1 > 0) // Si el proceso del segundo hijo
    {
        print_message("pong\n");
    }
    else if (pc_id_2 < 0)
        printf("ERROR: Fallo el fork.\n");

    wait(&pc_id_1);
    wait(&pc_id_2);

    sem_close(SEM_ID);

    exit(0);
}