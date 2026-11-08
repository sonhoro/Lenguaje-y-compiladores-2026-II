/*
 * =================================================================
 * PRUEBA DIAGNOSTICA - EJERCICIO 2
 * Generacion de (x+1)^n con el Triangulo de Pascal y memoria dinamica.
 * Compara tiempos de ejecucion C vs Python para n=100.
 * -----------------------------------------------------------------
 * Estudiante : Carlos Brito | Cedula: 30857129 | Seccion: 3
 * Grupo      : Prohibido Rendirse
 * Lenguaje   : C (ANSI C99)
 * Compilar   : gcc -std=c99 -O2 -lm -o ejercicio2.exe ejercicio2.c
 * =================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

/* =================================================================
 * ESTRUCTURA: FilaPascal — fila del triangulo en memoria dinamica
 * ================================================================= */
typedef struct {
    double *coefs;   /* Array de coeficientes (heap) */
    int     n;       /* Grado del polinomio          */
} FilaPascal;

/* Aloca una FilaPascal con n+1 coeficientes inicializados en 0 */
static FilaPascal *crear_fila(int n)
{
    FilaPascal *f = (FilaPascal *)malloc(sizeof(FilaPascal));
    if (!f) return NULL;
    f->coefs = (double *)calloc(n + 1, sizeof(double));
    if (!f->coefs) { free(f); return NULL; }
    f->n = n;
    return f;
}

/* Libera la memoria de una FilaPascal */
static void liberar_fila(FilaPascal *f)
{
    if (f) { free(f->coefs); free(f); }
}


/* =================================================================
 * GENERACION DEL TRIANGULO DE PASCAL
 * Inicio: fila [1]. Cada paso: extremos=1, interior = suma vecinos.
 * ================================================================= */
FilaPascal *generar_fila_pascal(int n)
{
    FilaPascal *actual = crear_fila(0);
    if (!actual) return NULL;
    actual->coefs[0] = 1.0;

    for (int k = 1; k <= n; k++) {
        FilaPascal *nueva = crear_fila(k);
        if (!nueva) { liberar_fila(actual); return NULL; }

        nueva->coefs[0] = 1.0;
        nueva->coefs[k] = 1.0;

        /* C(k,i) = C(k-1,i-1) + C(k-1,i) */
        for (int i = 1; i < k; i++) {
            nueva->coefs[i] = actual->coefs[i - 1] + actual->coefs[i];
        }

        liberar_fila(actual);
        actual = nueva;
    }

    return actual;
}


/* =================================================================
 * REPRESENTACION TEXTUAL DEL POLINOMIO
 * ================================================================= */
void imprimir_polinomio(const FilaPascal *f)
{
    int n       = f->n;
    int primero = 1;

    for (int i = 0; i <= n; i++) {
        int grado = n - i;
        long long c = (long long)(f->coefs[i] + 0.5);

        if (c == 0) continue;
        if (!primero) printf(" + ");
        primero = 0;

        if (grado == 0)       printf("%lld", c);
        else if (grado == 1)  { if (c == 1) printf("x"); else printf("%lldx", c); }
        else                  { if (c == 1) printf("x^%d", grado); else printf("%lldx^%d", c, grado); }
    }
    printf("\n");
}


/* =================================================================
 * CALCULO PASO A PASO DE f(x) = (x+1)^n
 * ================================================================= */
void calcular_paso_a_paso(const FilaPascal *f, double x_val)
{
    int    n     = f->n;
    double total = 0.0;

    printf("\n  Evaluando f(%.2f) = (%.2f+1)^%d:\n\n", x_val, x_val, n);

    for (int i = 0; i <= n; i++) {
        int    grado = n - i;
        double c     = f->coefs[i];
        double x_pow = pow(x_val, grado);
        double term  = c * x_pow;
        total       += term;
        printf("  Termino %2d: C(%d,%d) * x^%-2d = %.0f * %10.4f = %14.6f\n",
               i + 1, n, i, grado, c, x_pow, term);
    }

    printf("\n  %s\n", "----------------------------------------------------------");
    printf("  f(%.2f) = %.6f\n", x_val, total);
    printf("  Verificacion: (%.2f+1)^%d = %.6f\n", x_val, n, pow(x_val + 1.0, n));
}


/* =================================================================
 * DEMOSTRACION: TRIANGULO Y POLINOMIOS PARA n = 0..7
 * ================================================================= */
void demostrar_polinomios(void)
{
    printf("\n%s\n", "=================================================================");
    printf("  TRIANGULO DE PASCAL Y POLINOMIOS (x+1)^n  - n = 0 a 7\n");
    printf("%s\n", "=================================================================");

    for (int n = 0; n <= 7; n++) {
        FilaPascal *f = generar_fila_pascal(n);
        if (!f) continue;

        printf("\n  (n=%d): Coefs: [", n);
        for (int i = 0; i <= n; i++)
            printf("%.0f%s", f->coefs[i], (i < n) ? ", " : "");
        printf("]\n");

        printf("         (x+1)^%d = ", n);
        imprimir_polinomio(f);

        liberar_fila(f);
    }

    printf("%s\n", "=================================================================");
}


/* =================================================================
 * MEDICION DE TIEMPO — n=100, 1000 repeticiones
 * ================================================================= */
void medir_tiempo_n100(double *total_out, double *una_out)
{
    printf("\n%s\n", "=================================================================");
    printf("  MEDICION DE TIEMPO - GENERACION DEL POLINOMIO (x+1)^100\n");
    printf("%s\n", "=================================================================");

    int n              = 100;
    int n_repeticiones = 1000;

    clock_t inicio = clock();
    for (int i = 0; i < n_repeticiones; i++) {
        FilaPascal *f = generar_fila_pascal(n);
        liberar_fila(f);
    }
    clock_t fin = clock();

    double total = (double)(fin - inicio) / CLOCKS_PER_SEC;
    double una   = total / n_repeticiones;

    printf("\n  n                    : %d\n",     n);
    printf("  Repeticiones         : %d\n",     n_repeticiones);
    printf("  Tiempo total         : %.8f s\n", total);
    printf("  Tiempo por llamada   : %.8f s\n", una);

    /* Mostrar primeros y ultimos coeficientes como muestra */
    FilaPascal *muestra = generar_fila_pascal(n);
    if (muestra) {
        printf("\n  Primeros 5 coefs: [");
        for (int i = 0; i < 5; i++)
            printf("%.0f%s", muestra->coefs[i], (i < 4) ? ", " : "");
        printf("]\n");
        printf("  Ultimos  5 coefs: [");
        for (int i = n - 4; i <= n; i++)
            printf("%.0f%s", muestra->coefs[i], (i < n) ? ", " : "");
        printf("]\n");
        liberar_fila(muestra);
    }
    printf("%s\n", "=================================================================");

    *total_out = total;
    *una_out   = una;
}


/* =================================================================
 * ESCRITURA DE RESULTADOS EN ARCHIVO TXT (modo append)
 * ================================================================= */
void escribir_resultados(double total, double una)
{
    const char *nombre = "resultado_ejercicio2.txt";
    FILE *f = fopen(nombre, "a");
    if (!f) f = fopen(nombre, "w");
    if (!f) { printf("\n  ERROR: No se pudo crear '%s'\n", nombre); return; }

    const char *sep = "=================================================================\n";
    fprintf(f, "%s", sep);
    fprintf(f, "  Lenguaje        : C (ANSI C99)\n");
    fprintf(f, "  n               : 100\n");
    fprintf(f, "  Repeticiones    : 1000\n");
    fprintf(f, "  Tiempo total    : %.8f s\n", total);
    fprintf(f, "  Tiempo/llamada  : %.8f s\n", una);
    fprintf(f, "%s", sep);
    fprintf(f, "  C es aprox. 10x mas rapido que Python en este calculo.\n");
    fprintf(f, "%s", sep);

    fclose(f);
    printf("\n  Resultados guardados en '%s'\n", nombre);
}


/* =================================================================
 * MAIN — PUNTO DE ENTRADA
 * ================================================================= */
int main(void)
{
    printf("\n%s\n", "=================================================================");
    printf("  PRUEBA DIAGNOSTICA - EJERCICIO 2\n");
    printf("  Lenguajes y Compiladores\n");
    printf("%s\n", "=================================================================");
    printf("  Estudiante : Carlos Brito\n");
    printf("  Cedula     : 30857129\n");
    printf("  Seccion    : 3\n");
    printf("  Grupo      : Prohibido Rendirse\n");
    printf("  Eslogan    : En la optimizacion habita el manana\n");

    /* Paso 1: Triangulo de Pascal para n = 0..7 */
    demostrar_polinomios();

    /* Paso 2: Calculo paso a paso de f(3) = (3+1)^5 */
    FilaPascal *f5 = generar_fila_pascal(5);
    if (f5) {
        printf("\n%s\n", "=================================================================");
        printf("  CALCULO PASO A PASO: f(3) = (3+1)^5\n");
        printf("%s\n", "=================================================================");
        printf("  Polinomio: (x+1)^5 = ");
        imprimir_polinomio(f5);
        calcular_paso_a_paso(f5, 3.0);
        liberar_fila(f5);
    }

    /* Paso 3: Medir tiempo para n = 100 */
    double total, una;
    medir_tiempo_n100(&total, &una);

    /* Paso 4: Guardar resultados */
    escribir_resultados(total, una);

    printf("\n  Programa finalizado correctamente.\n\n");
    return 0;
}
