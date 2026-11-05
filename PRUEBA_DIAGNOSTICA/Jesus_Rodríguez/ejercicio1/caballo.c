/* Ejercicio 1 - Verificar movimientos de un caballo de ajedrez (C).
 * Compilar: gcc -O2 -o caballo caballo.c      Ejecutar (en esta carpeta): caballo
 * Escribe resultado_c.txt. Los ejemplos del enunciado se comprueban con strcmp. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

static double ahora(void) { /* segundos, reloj de alta resolucion */
#ifdef _WIN32
    LARGE_INTEGER f, c;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&c);
    return (double)c.QuadPart / (double)f.QuadPart;
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (double)t.tv_sec + (double)t.tv_nsec / 1e9;
#endif
}

/* Escribe el veredicto en out; devuelve 1 si todos los movimientos son correctos. */
static int verificar(const char *ak, char **movs, int n, char *out, size_t cap) {
    int col = ak[0] - 'a', fil = ak[1] - '1';
    for (int i = 0; i < n; i++) {
        const char *m = movs[i];
        if (m[0] != 'K') {
            snprintf(out, cap, "INCORRECTO falla letra %c no pertenece a un caballo", m[0]);
            return 0;
        }
        if (strlen(m) != 3 || m[1] < 'a' || m[1] > 'h' || m[2] < '1' || m[2] > '8') {
            snprintf(out, cap, "INCORRECTO falla en el movimiento %d", i + 1);
            return 0;
        }
        int c = m[1] - 'a', f = m[2] - '1';
        int dc = abs(c - col), df = abs(f - fil);
        /* un salto de caballo es (1,2) o (2,1) */
        if (!((dc == 1 && df == 2) || (dc == 2 && df == 1))) {
            snprintf(out, cap, "INCORRECTO falla en el movimiento %d", i + 1);
            return 0;
        }
        col = c;
        fil = f;
    }
    snprintf(out, cap, "movimiento CORRECTO");
    return 1;
}

/* Parte s (se modifica) en tokens; el arreglo se agranda con realloc. */
static char **tokenizar(char *s, int *n) {
    char **v = NULL;
    int cap = 0;
    *n = 0;
    for (char *t = strtok(s, " \t\r\n"); t; t = strtok(NULL, " \t\r\n")) {
        if (*n == cap) {
            cap = cap ? cap * 2 : 16;
            v = realloc(v, cap * sizeof *v);
        }
        v[(*n)++] = t;
    }
    return v;
}

static double medir(const char *ak, char **movs, int n, int reps) {
    char r[128];
    double t0 = ahora();
    for (int i = 0; i < reps; i++) verificar(ak, movs, n, r, sizeof r);
    return (ahora() - t0) * 1e6; /* microsegundos */
}

typedef struct { const char *ak, *cadena, *esperado; } Ejemplo;
static const Ejemplo EJEMPLOS[] = {
    {"a1", "Kc2 Kd4 Kb3 Kc5", "movimiento CORRECTO"},
    {"a1", "Kc2 Kd4 Kb4 Kc5", "INCORRECTO falla en el movimiento 3"},
    {"a2", "Kc3 Ke4 Kf6 Ke8 Kg7", "movimiento CORRECTO"},
    {"a2", "Kc8 Ke4 Kf6 Ke8 Kg7", "INCORRECTO falla en el movimiento 1"},
    {"a8", "Kb6 Kd7", "movimiento CORRECTO"},
    {"a8", "Kb6 Kb7", "INCORRECTO falla en el movimiento 2"},
    {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4", "movimiento CORRECTO"},
    {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4",
     "INCORRECTO falla letra Q no pertenece a un caballo"},
    {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4", "INCORRECTO falla en el movimiento 9"},
};

int main(void) {
    FILE *out = fopen("resultado_c.txt", "w");
    if (!out) { perror("resultado_c.txt"); return 1; }
    fprintf(out, "EJERCICIO 1 - Caballo de ajedrez (C)\n\nEjemplos del enunciado:\n");

    for (size_t i = 0; i < sizeof EJEMPLOS / sizeof *EJEMPLOS; i++) {
        char *copia = strdup(EJEMPLOS[i].cadena), r[128];
        int n;
        char **v = tokenizar(copia, &n);
        verificar(EJEMPLOS[i].ak, v, n, r, sizeof r);
        if (strcmp(r, EJEMPLOS[i].esperado) != 0) {
            fprintf(stderr, "FALLO ejemplo %zu: %s\n", i + 1, r);
            return 1;
        }
        fprintf(out, "  Ak=%s n=%d C=%s -> %s\n", EJEMPLOS[i].ak, n, EJEMPLOS[i].cadena, r);
        free(v);
        free(copia);
    }

    /* cargar movimientos.txt en memoria dinamica */
    FILE *f = fopen("movimientos.txt", "rb");
    if (!f) { perror("movimientos.txt"); return 1; }
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);
    char *buf = malloc(tam + 1);
    buf[fread(buf, 1, tam, f)] = '\0';
    fclose(f);
    int n;
    char **movs = tokenizar(buf, &n);

    char r[128];
    verificar("a2", movs, n, r, sizeof r);
    fprintf(out, "\nLista de prueba: %d movimientos, posicion inicial a2\n  Resultado: %s\n", n, r);
    fprintf(out, "  Tiempo 10 repeticiones x %d movimientos: %.2f us\n", n, medir("a2", movs, n, 10));
    fprintf(out, "  Tiempo n=100 (100 movimientos x 100 repeticiones): %.2f us\n",
            medir("a2", movs, 100, 100));

    fclose(out);
    free(movs);
    free(buf);
    printf("OK: resultado_c.txt escrito\n");
    return 0;
}
