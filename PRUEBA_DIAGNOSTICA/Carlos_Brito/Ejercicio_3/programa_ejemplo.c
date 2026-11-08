/*
 * programa_ejemplo.c
 * Calculadora de promedios de estudiantes.
 * Archivo de entrada para el analisis del Ejercicio 3.
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_NOTAS 5

/* Estructura para un estudiante */
typedef struct {
    int    id;
    float  notas[MAX_NOTAS];
    float  promedio;
} Estudiante;

/* Calcula el promedio de las notas */
float calcular_promedio(float *notas, int n)
{
    float suma = 0.0f;
    for (int i = 0; i < n; i++) {
        suma += notas[i];
    }
    return suma / n;
}

/* Muestra si el estudiante aprobó o reprobó */
void mostrar_resultado(Estudiante *est)
{
    if (est == NULL) return;

    printf("Estudiante %d | Promedio: %.2f | ", est->id, est->promedio);

    if (est->promedio >= 60.0f) {
        printf("APROBADO\n");
    } else {
        printf("REPROBADO\n");
    }
}

int main(void)
{
    int n = 3;

    /* Reservar memoria dinamica para los estudiantes */
    Estudiante *lista = (Estudiante *)malloc(n * sizeof(Estudiante));
    if (lista == NULL) {
        fprintf(stderr, "Error: memoria insuficiente\n");
        return 1;
    }

    /* Datos de prueba */
    float datos[3][MAX_NOTAS] = {
        {70, 80, 90, 85, 75},
        {40, 50, 45, 55, 48},
        {60, 65, 70, 68, 72}
    };

    /* Inicializar y calcular */
    for (int i = 0; i < n; i++) {
        lista[i].id = i + 1;
        for (int j = 0; j < MAX_NOTAS; j++) {
            lista[i].notas[j] = datos[i][j];
        }
        lista[i].promedio = calcular_promedio(lista[i].notas, MAX_NOTAS);
    }

    /* Mostrar resultados */
    printf("=== Resultados de Estudiantes ===\n");
    for (int i = 0; i < n; i++) {
        mostrar_resultado(&lista[i]);
    }

    /* Liberar memoria */
    free(lista);
    return 0;
}
