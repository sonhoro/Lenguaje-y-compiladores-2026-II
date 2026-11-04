/*
 * Ejercicio 1 - Validador de movimientos de caballo de ajedrez
 * Lenguajes y Compiladores 2026-II
 * Estudiante: Luismer Ortega
 * Universidad Nacional Experimental de Guayana
 *
 * Compilar con:
 *   C:\MinGW\bin\gcc.exe -o caballo caballo.c -lm
 * Ejecutar con:
 *   caballo.exe
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <windows.h>

/* ------------------------------------------------------------------ */
/* Definiciones                                                        */
/* ------------------------------------------------------------------ */

#define MAX_MOVIMIENTOS 1000
#define MAX_TOKEN       10

typedef struct {
    int col;   /* 0=a .. 7=h */
    int fila;  /* 0=1 .. 7=8 */
} Coord;

/* ------------------------------------------------------------------ */
/* Funciones de utilidad                                               */
/* ------------------------------------------------------------------ */

/* Convierte "a1" -> Coord{0,0}. Retorna 1 si valido, 0 si no. */
int pos_a_coords(const char *pos, Coord *c) {
    if (strlen(pos) != 2) return 0;
    if (pos[0] < 'a' || pos[0] > 'h') return 0;
    if (pos[1] < '1' || pos[1] > '8') return 0;
    c->col  = pos[0] - 'a';
    c->fila = pos[1] - '1';
    return 1;
}

/* Verifica si el salto origen->destino es legal para un caballo */
int es_movimiento_caballo(Coord origen, Coord destino) {
    int dc = abs(destino.col  - origen.col);
    int df = abs(destino.fila - origen.fila);
    return (dc == 1 && df == 2) || (dc == 2 && df == 1);
}

/* ------------------------------------------------------------------ */
/* Resultado de validacion                                             */
/* ------------------------------------------------------------------ */
#define RES_OK                0
#define RES_LETRA_INVALIDA    1
#define RES_FORMATO_INVALIDO  2
#define RES_FUERA_TABLERO     3
#define RES_MOVIMIENTO_MALO   4

typedef struct {
    int tipo;          /* uno de los RES_* */
    int numero;        /* numero de movimiento que fallo (1-based) */
    char letra;        /* letra invalida (si tipo==RES_LETRA_INVALIDA) */
} Resultado;

/*
 * Valida la secuencia de movimientos.
 * pos_inicial: "a1"
 * movimientos: array de tokens como "Kc2"
 * n: cantidad de tokens
 * res: estructura de resultado
 */
void validar_movimientos(const char *pos_inicial,
                         char movimientos[][MAX_TOKEN],
                         int n,
                         Resultado *res)
{
    Coord actual;
    if (!pos_a_coords(pos_inicial, &actual)) {
        res->tipo = RES_FORMATO_INVALIDO;
        res->numero = 0;
        return;
    }

    for (int i = 0; i < n; i++) {
        const char *tok = movimientos[i];
        int num = i + 1;

        /* Verificar longitud */
        if (strlen(tok) != 3) {
            res->tipo   = RES_FORMATO_INVALIDO;
            res->numero = num;
            return;
        }

        /* Verificar prefijo K */
        if (tok[0] != 'K') {
            res->tipo   = RES_LETRA_INVALIDA;
            res->numero = num;
            res->letra  = tok[0];
            return;
        }

        /* Verificar coordenadas */
        char dest_str[3];
        dest_str[0] = tok[1];
        dest_str[1] = tok[2];
        dest_str[2] = '\0';

        Coord destino;
        if (!pos_a_coords(dest_str, &destino)) {
            res->tipo   = RES_FUERA_TABLERO;
            res->numero = num;
            return;
        }

        /* Verificar que sea movimiento de caballo */
        if (!es_movimiento_caballo(actual, destino)) {
            res->tipo   = RES_MOVIMIENTO_MALO;
            res->numero = num;
            return;
        }

        actual = destino;
    }

    res->tipo = RES_OK;
}

/* Imprime/escribe el mensaje de resultado */
void imprimir_resultado(Resultado *res, FILE *f) {
    switch (res->tipo) {
        case RES_OK:
            fprintf(f, "movimiento CORRECTO");
            break;
        case RES_LETRA_INVALIDA:
            fprintf(f, "movimiento INCORRECTO falla letra %c no pertenece a un caballo",
                    res->letra);
            break;
        case RES_FORMATO_INVALIDO:
        case RES_FUERA_TABLERO:
        case RES_MOVIMIENTO_MALO:
            fprintf(f, "INCORRECTO falla en el movimiento %d", res->numero);
            break;
    }
}

/* ------------------------------------------------------------------ */
/* Casos de prueba del enunciado                                       */
/* ------------------------------------------------------------------ */

typedef struct {
    const char *ak;
    const char *cadena;
    const char *esperado;
} CasoPrueba;

static CasoPrueba CASOS[] = {
    {"a1", "Kc2 Kd4 Kb3 Kc5",                              "movimiento CORRECTO"},
    {"a1", "Kc2 Kd4 Kb4 Kc5",                              "INCORRECTO falla en el movimiento 3"},
    {"a2", "Kc3 Ke4 Kf6 Ke8 Kg7",                          "movimiento CORRECTO"},
    {"a2", "Kc8 Ke4 Kf6 Ke8 Kg7",                          "INCORRECTO falla en el movimiento 1"},
    {"a8", "Kb6 Kd7",                                       "movimiento CORRECTO"},
    {"a8", "Kb6 Kb7",                                       "INCORRECTO falla en el movimiento 2"},
    {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4",    "movimiento CORRECTO"},
    {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4",
            "movimiento INCORRECTO falla letra Q no pertenece a un caballo"},
    {"a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4",    "INCORRECTO falla en el movimiento 9"},
};

/* Tokeniza una cadena de movimientos en un array */
int tokenizar(const char *cadena, char tokens[][MAX_TOKEN]) {
    int n = 0;
    char buf[4096];
    strncpy(buf, cadena, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';
    char *tok = strtok(buf, " \t\n\r");
    while (tok != NULL && n < MAX_MOVIMIENTOS) {
        strncpy(tokens[n], tok, MAX_TOKEN-1);
        tokens[n][MAX_TOKEN-1] = '\0';
        n++;
        tok = strtok(NULL, " \t\n\r");
    }
    return n;
}

/* ------------------------------------------------------------------ */
/* Cadena de movimientos publicada en el enunciado (224 tokens)        */
/* ------------------------------------------------------------------ */

static const char *MOVS_500 =
    "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 "
    "Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 "
    "Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 "
    "Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 "
    "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 "
    "Ka1 Kc2 Kb4 Kd5 Kb6 Ka8";

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */

int main(void) {
    int n_casos = sizeof(CASOS) / sizeof(CASOS[0]);
    char tokens[MAX_MOVIMIENTOS][MAX_TOKEN];
    Resultado res;

    /* 1) Casos de prueba */
    printf("============================================================\n");
    printf("CASOS DE PRUEBA DEL ENUNCIADO\n");
    printf("============================================================\n");

    for (int i = 0; i < n_casos; i++) {
        int n = tokenizar(CASOS[i].cadena, tokens);
        validar_movimientos(CASOS[i].ak, tokens, n, &res);

        printf("Ak=%s, n=%d\n", CASOS[i].ak, n);
        printf("  Cadena   : %s\n", CASOS[i].cadena);
        printf("  Resultado: ");
        imprimir_resultado(&res, stdout);
        printf("\n\n");
    }

    /* 2) Medir tiempo con la cadena publicada x 10 repeticiones */
    char tokens500[MAX_MOVIMIENTOS][MAX_TOKEN];
    int n500 = tokenizar(MOVS_500, tokens500);
    validar_movimientos("a2", tokens500, n500, &res);
    if (res.tipo != RES_OK) {
        fprintf(stderr, "La cadena del enunciado no es valida desde a2\n");
        return 1;
    }

    printf("============================================================\n");
    printf("MEDICION DE TIEMPO — %d movimientos x 10 repeticiones\n", n500);
    printf("============================================================\n");

    LARGE_INTEGER frecuencia, inicio, fin;
    if (!QueryPerformanceFrequency(&frecuencia)) {
        fprintf(stderr, "Error: no se pudo consultar el temporizador de alta resolucion\n");
        return 1;
    }
    QueryPerformanceCounter(&inicio);
    for (int r = 0; r < 10; r++) {
        validar_movimientos("a2", tokens500, n500, &res);
    }
    QueryPerformanceCounter(&fin);

    double total_seg = (double)(fin.QuadPart - inicio.QuadPart)
        / (double)frecuencia.QuadPart;
    double promedio_seg = total_seg / 10.0;

    printf("Movimientos en cadena : %d\n", n500);
    printf("Repeticiones          : 10\n");
    printf("Tiempo total          : %.6f segundos\n", total_seg);
    printf("Tiempo promedio       : %.6f segundos\n\n", promedio_seg);

    /* 3) Guardar en resultado.txt */
    FILE *f = fopen("resultado_c.txt", "w");
    if (f == NULL) {
        fprintf(stderr, "Error: no se pudo abrir resultado_c.txt\n");
        return 1;
    }

    fprintf(f, "EJERCICIO 1 - VALIDADOR DE MOVIMIENTOS DE CABALLO\n");
    fprintf(f, "Lenguajes y Compiladores 2026-II -- Luismer Ortega\n");
    fprintf(f, "============================================================\n\n");

    fprintf(f, "CASOS DE PRUEBA\n");
    fprintf(f, "--------------------------------------------\n");
    for (int i = 0; i < n_casos; i++) {
        int n = tokenizar(CASOS[i].cadena, tokens);
        validar_movimientos(CASOS[i].ak, tokens, n, &res);
        fprintf(f, "Ak=%s, n=%d\n", CASOS[i].ak, n);
        fprintf(f, "  Cadena   : %s\n", CASOS[i].cadena);
        fprintf(f, "  Resultado: ");
        imprimir_resultado(&res, f);
        fprintf(f, "\n\n");
    }

    fprintf(f, "\nMEDICION DE TIEMPO (C / MinGW gcc)\n");
    fprintf(f, "--------------------------------------------\n");
    fprintf(f, "Lenguaje        : C (gcc 6.3.0 MinGW)\n");
    fprintf(f, "Posicion inicial: a2\n");
    fprintf(f, "Movimientos     : %d\n", n500);
    fprintf(f, "Repeticiones    : 10\n");
    fprintf(f, "Tiempo total    : %.6f segundos\n", total_seg);
    fprintf(f, "Tiempo promedio : %.6f segundos\n", promedio_seg);

    fclose(f);
    printf("Resultados guardados en resultado_c.txt\n");

    return 0;
}
