/*
 * Ejercicio 2 - Polinomio (x+1)^n usando triangulo de Pascal
 * Lenguajes y Compiladores 2026-II
 * Estudiante: Luismer Ortega
 * Universidad Nacional Experimental de Guayana
 *
 * Compilar con:
 *   C:\MinGW\bin\gcc.exe -o pascal pascal.c -lm
 * Ejecutar con:
 *   pascal.exe
 *
 * Implementacion con memoria dinamica (malloc/realloc/free).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define BASE_DECIMAL 1000000000U

typedef struct {
    unsigned int *digitos;
    size_t longitud;
} EnteroGrande;

int inicializar_uno(EnteroGrande *numero) {
    numero->digitos = (unsigned int *)malloc(sizeof(unsigned int));
    if (!numero->digitos) return 0;
    numero->digitos[0] = 1;
    numero->longitud = 1;
    return 1;
}

void liberar_coeficientes(EnteroGrande *coefs, int cantidad) {
    if (!coefs) return;
    for (int i = 0; i < cantidad; i++) free(coefs[i].digitos);
    free(coefs);
}

int sumar_enteros(const EnteroGrande *a, const EnteroGrande *b,
                  EnteroGrande *suma) {
    size_t max_digitos = a->longitud > b->longitud
        ? a->longitud : b->longitud;
    suma->digitos = (unsigned int *)malloc((max_digitos + 1) * sizeof(unsigned int));
    if (!suma->digitos) return 0;

    unsigned long long acarreo = 0;
    for (size_t i = 0; i < max_digitos; i++) {
        unsigned long long valor = acarreo;
        if (i < a->longitud) valor += a->digitos[i];
        if (i < b->longitud) valor += b->digitos[i];
        suma->digitos[i] = (unsigned int)(valor % BASE_DECIMAL);
        acarreo = valor / BASE_DECIMAL;
    }

    suma->longitud = max_digitos;
    if (acarreo) suma->digitos[suma->longitud++] = (unsigned int)acarreo;
    return 1;
}

void imprimir_entero(const EnteroGrande *numero, FILE *f) {
    fprintf(f, "%u", numero->digitos[numero->longitud - 1]);
    for (size_t i = numero->longitud - 1; i > 0; i--) {
        fprintf(f, "%09u", numero->digitos[i - 1]);
    }
}

double entero_a_double(const EnteroGrande *numero) {
    double valor = 0.0;
    for (size_t i = numero->longitud; i > 0; i--) {
        valor = valor * BASE_DECIMAL + numero->digitos[i - 1];
    }
    return valor;
}

/* ------------------------------------------------------------------ */
/* a) Generacion de coeficientes con Triangulo de Pascal              */
/* ------------------------------------------------------------------ */

/*
 * Genera los coeficientes de (x+1)^n usando el triangulo de Pascal.
 * Cada coeficiente usa digitos dinamicos para evitar desbordamiento.
 * El llamador libera el resultado con liberar_coeficientes().
 */
EnteroGrande *generar_pascal(int n) {
    if (n < 0) return NULL;

    EnteroGrande *fila_actual = (EnteroGrande *)calloc(1, sizeof(EnteroGrande));
    if (!fila_actual || !inicializar_uno(&fila_actual[0])) {
        free(fila_actual);
        return NULL;
    }
    int tam_actual = 1;

    for (int i = 0; i < n; i++) {
        int tam_nueva = tam_actual + 1;
        EnteroGrande *nueva_fila =
            (EnteroGrande *)calloc(tam_nueva, sizeof(EnteroGrande));
        if (!nueva_fila) {
            liberar_coeficientes(fila_actual, tam_actual);
            return NULL;
        }

        if (!inicializar_uno(&nueva_fila[0])
                || !inicializar_uno(&nueva_fila[tam_nueva - 1])) {
            liberar_coeficientes(nueva_fila, tam_nueva);
            liberar_coeficientes(fila_actual, tam_actual);
            return NULL;
        }
        for (int j = 1; j < tam_actual; j++) {
            if (!sumar_enteros(&fila_actual[j - 1], &fila_actual[j],
                               &nueva_fila[j])) {
                liberar_coeficientes(nueva_fila, tam_nueva);
                liberar_coeficientes(fila_actual, tam_actual);
                return NULL;
            }
        }

        liberar_coeficientes(fila_actual, tam_actual);
        fila_actual = nueva_fila;
        tam_actual  = tam_nueva;
    }

    return fila_actual;
}

/* ------------------------------------------------------------------ */
/* Mostrar el polinomio en forma expandida                            */
/* ------------------------------------------------------------------ */

void mostrar_polinomio(const EnteroGrande *coefs, int n, FILE *f) {
    if (n == 0) {
        fprintf(f, "1");
        return;
    }
    int primero = 1;
    for (int k = 0; k <= n; k++) {
        const EnteroGrande *c = &coefs[k];
        int grado     = n - k;
        if (!primero) fprintf(f, " + ");
        primero = 0;
        if (grado == 0) {
            imprimir_entero(c, f);
        } else if (grado == 1) {
            if (c->longitud == 1 && c->digitos[0] == 1) fprintf(f, "x");
            else {
                imprimir_entero(c, f);
                fprintf(f, "x");
            }
        } else {
            if (c->longitud == 1 && c->digitos[0] == 1) fprintf(f, "x^%d", grado);
            else {
                imprimir_entero(c, f);
                fprintf(f, "x^%d", grado);
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* b) Evaluacion paso a paso                                          */
/* ------------------------------------------------------------------ */

double evaluar_polinomio(const EnteroGrande *coefs, int n, double x, FILE *f) {
    fprintf(f, "  Evaluando f(%.2f) = (x+1)^%d con x = %.2f:\n", x, n, x);
    double total = 0.0;
    for (int k = 0; k <= n; k++) {
        int grado = n - k;
        double potencia = pow(x, grado);
        double coeficiente = entero_a_double(&coefs[k]);
        double termino  = coeficiente * potencia;
        total += termino;
        fprintf(f, "    Termino %d: ", k + 1);
        imprimir_entero(&coefs[k], f);
        fprintf(f, " * x^%d = %.0f * %.4f = %.4f\n",
                grado, coeficiente, potencia, termino);
    }
    fprintf(f, "  f(%.2f) = %.4f\n", x, total);
    fprintf(f, "  Verificacion: (%.2f+1)^%d = %.4f\n", x, n, pow(x+1, n));
    return total;
}

/* ------------------------------------------------------------------ */
/* Demostracion n=0..7                                                */
/* ------------------------------------------------------------------ */

void demostrar(FILE *f) {
    fprintf(f, "============================================================\n");
    fprintf(f, "TRIANGULO DE PASCAL Y POLINOMIOS (x+1)^n\n");
    fprintf(f, "============================================================\n");
    for (int n = 0; n <= 7; n++) {
        EnteroGrande *coefs = generar_pascal(n);
        if (!coefs) { fprintf(f, "Error de memoria\n"); continue; }
        fprintf(f, "(n=%d): [", n);
        for (int k = 0; k <= n; k++) {
            if (k > 0) fprintf(f, " ");
            imprimir_entero(&coefs[k], f);
        }
        fprintf(f, "]  =>  (x+1)^%d = ", n);
        mostrar_polinomio(coefs, n, f);
        fprintf(f, "\n");
        liberar_coeficientes(coefs, n + 1);
    }
    fprintf(f, "\n");
}

/* ------------------------------------------------------------------ */
/* main                                                               */
/* ------------------------------------------------------------------ */

int main(void) {
    /* 1) Demostracion en consola */
    demostrar(stdout);

    /* 2) Evaluacion paso a paso en consola */
    printf("============================================================\n");
    printf("EVALUACION PASO A PASO f(x) = (x+1)^n\n");
    printf("============================================================\n");

    int ejemplos_n[] = {3, 4, 5};
    double ejemplos_x[] = {2.0, -1.0, 3.0};
    int n_ejemplos = 3;

    for (int i = 0; i < n_ejemplos; i++) {
        int n = ejemplos_n[i];
        double x = ejemplos_x[i];
        EnteroGrande *coefs = generar_pascal(n);
        if (!coefs) continue;
        printf("\n  Polinomio (x+1)^%d = ", n);
        mostrar_polinomio(coefs, n, stdout);
        printf("\n");
        evaluar_polinomio(coefs, n, x, stdout);
        liberar_coeficientes(coefs, n + 1);
    }

    /* 3) Medicion de tiempo para n=100, 100 repeticiones */
    printf("\n============================================================\n");
    printf("MEDICION DE TIEMPO -- Generar coeficientes de (x+1)^100\n");
    printf("============================================================\n");

    EnteroGrande *coefs_100 = NULL;
    clock_t inicio = clock();
    for (int r = 0; r < 100; r++) {
        if (coefs_100) liberar_coeficientes(coefs_100, 101);
        coefs_100 = generar_pascal(100);
        if (!coefs_100) {
            fprintf(stderr, "Error: no se pudo generar Pascal para n=100\n");
            return 1;
        }
    }
    clock_t fin_t = clock();

    double total_seg   = (double)(fin_t - inicio) / CLOCKS_PER_SEC;
    double promedio_seg = total_seg / 100.0;

    printf("n                 : 100\n");
    printf("Repeticiones      : 100\n");
    printf("Tiempo total      : %.6f segundos\n", total_seg);
    printf("Tiempo promedio   : %.6f segundos\n", promedio_seg);
    if (coefs_100) {
        printf("C(100,50)         : ");
        imprimir_entero(&coefs_100[50], stdout);
        printf("\n");
    }

    /* 4) Guardar resultados */
    FILE *f = fopen("resultado_c.txt", "w");
    if (!f) {
        fprintf(stderr, "Error: no se pudo abrir resultado_c.txt\n");
        return 1;
    }

    fprintf(f, "EJERCICIO 2 - POLINOMIO (x+1)^n CON TRIANGULO DE PASCAL\n");
    fprintf(f, "Lenguajes y Compiladores 2026-II -- Luismer Ortega\n");
    fprintf(f, "============================================================\n\n");

    demostrar(f);

    fprintf(f, "EVALUACION PASO A PASO f(x) = (x+1)^n\n");
    fprintf(f, "--------------------------------------------\n");
    for (int i = 0; i < n_ejemplos; i++) {
        int n = ejemplos_n[i];
        double x = ejemplos_x[i];
        EnteroGrande *coefs = generar_pascal(n);
        if (!coefs) continue;
        fprintf(f, "\n  Polinomio (x+1)^%d = ", n);
        mostrar_polinomio(coefs, n, f);
        fprintf(f, "\n");
        evaluar_polinomio(coefs, n, x, f);
        liberar_coeficientes(coefs, n + 1);
    }

    fprintf(f, "\nMEDICION DE TIEMPO (C / MinGW gcc)\n");
    fprintf(f, "--------------------------------------------\n");
    fprintf(f, "Lenguaje        : C (gcc 6.3.0 MinGW)\n");
    fprintf(f, "n               : 100\n");
    fprintf(f, "Repeticiones    : 100\n");
    fprintf(f, "Tiempo total    : %.6f segundos\n", total_seg);
    fprintf(f, "Tiempo promedio : %.6f segundos\n", promedio_seg);
    fprintf(f, "C(100,50)       : ");
    imprimir_entero(&coefs_100[50], f);
    fprintf(f, "\n");

    fclose(f);
    liberar_coeficientes(coefs_100, 101);
    printf("\nResultados guardados en resultado_c.txt\n");
    return 0;
}
