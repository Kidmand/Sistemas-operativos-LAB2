#include "kernel/types.h"
#include "user/user.h"

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

    exit(0);
}