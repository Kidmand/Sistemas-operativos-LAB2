struct stat;

/* Definiciones parciales abiertas a mejoras */
// system calls
int fork(void); // fork(): Crea un nuevo proceso.

int exit(int) __attribute__((noreturn)); // exit(): Termina el proceso actual.

int wait(int *); // wait(): Espera a que un proceso hijo termine.

int pipe(int *); // pipe(): Crea un canal de comunicación entre dos procesos.

int write(int, const void *, int); // write(): Escribe datos en un archivo o dispositivo de E/S.

int read(int, void *, int); // read(): Lee datos de un archivo o dispositivo de E/S.

int close(int); // close(): Cierra un archivo o dispositivo de E/S.

int kill(int); // kill(): Envía una señal a un proceso.

int exec(const char *, char **); // exec(): Ejecuta un nuevo programa.

int open(const char *, int); // open(): Abre un archivo.

int mknod(const char *, short, short); // mknod(): Crea un dispositivo de E/S especial.

int unlink(const char *); // unlink(): Elimina un archivo.

int fstat(int fd, struct stat *); // fstat(): Obtiene información sobre un archivo.

int link(const char *, const char *); // link(): Crea un enlace simbólico a un archivo.

int mkdir(const char *); // mkdir(): Crea un directorio.

int chdir(const char *); // chdir(): Cambia el directorio actual.

int dup(int); // dup(): Crea una copia de un descriptor de archivo.

int getpid(void); // getpid(): Obtiene el ID del proceso actual.

char *sbrk(int); // sbrk(): Ajusta el tamaño del espacio de usuario del proceso actual.

int sleep(int); // sleep(): Suspende la ejecución del proceso actual durante un período de tiempo especificado.

int uptime(void); // uptime(): Obtiene el tiempo de actividad del sistema.

// ulib.c

int stat(const char *, struct stat *); // stat(): Obtiene información sobre un archivo o dispositivo de E/S.

char *strcpy(char *, const char *); // strcpy(): Copia una string en otra string.

void *memmove(void *, const void *, int); // memmove(): Mueve un bloque de memoria de una ubicación a otra.

char *strchr(const char *, char c); // strchr(): Busca una character en una string.

int strcmp(const char *, const char *); // strcmp(): Compara dos strings.

void fprintf(int, const char *, ...); // fprintf(): Imprime un mensaje en un archivo o dispositivo de E/S.

void printf(const char *, ...); // printf(): Imprime un mensaje en la consola.

char *gets(char *, int max); // gets(): Obtiene una string del usuario.

uint strlen(const char *); // strlen(): Obtiene la longitud de una string.

void *memset(void *, int, uint); // memset(): Asigna un valor a un bloque de memoria.

void *malloc(uint); // malloc(): Reserva memoria para una variable.

void free(void *); // free(): Libera memoria que ya no se necesita.

int atoi(const char *); // atoi(): Convierte una string a un número entero.

int memcmp(const void *, const void *, uint); // memcmp(): Compara dos bloques de memoria.

void *memcpy(void *, const void *, uint); // memcpy(): Copia un bloque de memoria de una ubicación a otra.

int sem_open(int id_sem, int value); // sem_open(): Abre un semáforo

int sem_close(int id_sem); // sem_close(): Cierra un semáforo

int sem_up(int id_sem); // sem_up(): Incrementa el valor del semáforo

int sem_down(int id_sem); // sem_down(): Decrementa el valor del semáforo