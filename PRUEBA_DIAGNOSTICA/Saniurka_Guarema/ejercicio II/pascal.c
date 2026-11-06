#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#define MAX_DIGITOS_N100 40

// Estructura eficiente para números de precisión arbitraria (BigInt)
typedef struct {
    uint8_t digitos[MAX_DIGITOS_N100]; // Dígitos almacenados en orden inverso (unidad en índice 0)
    int len;                           // Cantidad de dígitos
} BigInt;

// Inicializa un BigInt desde un entero simple
void init_bigint_u(BigInt* b, unsigned int val) {
    b->len = 0;
    if (val == 0) {
        b->digitos[0] = 0;
        b->len = 1;
        return;
    }
    while (val > 0) {
        b->digitos[b->len++] = val % 10;
        val /= 10;
    }
}

// Suma dos BigInt en memoria directa: res = a + b (sin conversiones de strings)
void sumar_bigint(const BigInt* a, const BigInt* b, BigInt* res) {
    int max_len = (a->len > b->len) ? a->len : b->len;
    int acarreo = 0;
    res->len = 0;

    for (int i = 0; i < max_len || acarreo; i++) {
        int suma = acarreo;
        if (i < a->len) suma += a->digitos[i];
        if (i < b->len) suma += b->digitos[i];
        acarreo = suma / 10;
        res->digitos[res->len++] = (uint8_t)(suma % 10);
    }
}

// Convierte BigInt a cadena de texto para imprimir o guardar
void bigint_a_string(const BigInt* b, char* buffer) {
    for (int i = 0; i < b->len; i++) {
        buffer[i] = (char)(b->digitos[b->len - 1 - i] + '0');
    }
    buffer[b->len] = '\0';
}

// Convierte BigInt a double (para evaluación de f(x))
double bigint_a_double(const BigInt* b) {
    double valor = 0.0;
    for (int i = b->len - 1; i >= 0; i--) {
        valor = valor * 10.0 + b->digitos[i];
    }
    return valor;
}

// Genera la fila n del Triángulo de Pascal utilizando MEMORIA DINÁMICA (malloc / free)
BigInt* generar_fila_pascal(int n) {
    if (n < 0) return NULL;

    // Fila base de tamaño 1 reservada en el heap
    BigInt* fila_actual = (BigInt*)malloc(1 * sizeof(BigInt));
    if (!fila_actual) return NULL;
    init_bigint_u(&fila_actual[0], 1);

    for (int k = 1; k <= n; k++) {
        // Reservar dinámicamente la memoria para la nueva fila de tamaño (k + 1)
        BigInt* siguiente_fila = (BigInt*)malloc((k + 1) * sizeof(BigInt));
        if (!siguiente_fila) {
            free(fila_actual);
            return NULL;
        }

        // Extremos del triángulo siempre son 1
        init_bigint_u(&siguiente_fila[0], 1);
        init_bigint_u(&siguiente_fila[k], 1);

        // Sumar elementos adyacentes de la fila anterior: C(k, j) = C(k-1, j-1) + C(k-1, j)
        for (int j = 1; j < k; j++) {
            sumar_bigint(&fila_actual[j - 1], &fila_actual[j], &siguiente_fila[j]);
        }

        // Liberar la memoria dinámica de la fila previa
        free(fila_actual);
        fila_actual = siguiente_fila;
    }

    return fila_actual;
}

// Formatea e imprime el polinomio algebraico (x+1)^n
void mostrar_polinomio(int n, const BigInt* coefs) {
    char buffer[MAX_DIGITOS_N100 + 1];
    printf("(x+1)^%d = ", n);
    for (int i = 0; i <= n; i++) {
        int exp = n - i;
        bigint_a_string(&coefs[i], buffer);

        if (i > 0) printf(" + ");

        if (exp == 0) {
            printf("%s", buffer);
        } else if (exp == 1) {
            if (strcmp(buffer, "1") != 0) printf("%sx", buffer);
            else printf("x");
        } else {
            if (strcmp(buffer, "1") != 0) printf("%sx^%d", buffer, exp);
            else printf("x^%d", exp);
        }
    }
    printf("\n");
}

// Evalúa y muestra el cálculo paso a paso para un valor dado de x
void evaluar_paso_a_paso(int n, const BigInt* coefs, double x) {
    char buffer[MAX_DIGITOS_N100 + 1];
    printf("\n--- EVALUACION PASO A PASO PARA x = %.2f ---\n", x);
    printf("f(x) = (x + 1)^%d con x = %.2f\n\n", n, x);

    double suma_total = 0.0;
    for (int i = 0; i <= n; i++) {
        int exp = n - i;
        bigint_a_string(&coefs[i], buffer);
        double coef_num = bigint_a_double(&coefs[i]);
        double potencia = pow(x, exp);
        double termino = coef_num * potencia;
        suma_total += termino;

        printf("Termino %d (grado %d): %s * (%.2f)^%d = %s * %.2f = %.2f  --> Suma parcial = %.2f\n",
               i + 1, exp, buffer, x, exp, buffer, potencia, termino, suma_total);
    }

    printf("\nResultado final f(%.2f) = %.2f\n", x, suma_total);
    printf("Verificacion directa (x+1)^%d = (%.2f)^%d = %.2f\n", n, x + 1.0, n, pow(x + 1.0, n));
}

// Medición de alta precisión en milisegundos
double obtener_tiempo_ms(void) {
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1000000.0;
}

int main(void) {
    FILE* reporte = fopen("tiempos_n100_c.txt", "w");

    printf("========================================================\n");
    printf("  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)   \n");
    printf("       LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA    \n");
    printf("       EJERCICIO II: TRIANGULO DE PASCAL Y POLINOMIOS   \n");
    printf("                     (LENGUAJE: C)                      \n");
    printf("========================================================\n\n");

    // 1. Polinomios base como en la guía oficial (n = 0 a 4)
    printf("--- GENERACION DE POLINOMIOS (MEMORIA DINAMICA) ---\n");
    for (int n = 0; n <= 4; n++) {
        BigInt* coefs = generar_fila_pascal(n);
        if (coefs) {
            mostrar_polinomio(n, coefs);
            free(coefs); // Liberar memoria
        }
    }

    // 2. Cálculo paso a paso para n = 3 y x = 2.0
    int n_demo = 3;
    double x_demo = 2.0;
    BigInt* coefs_demo = generar_fila_pascal(n_demo);
    if (coefs_demo) {
        evaluar_paso_a_paso(n_demo, coefs_demo, x_demo);
        free(coefs_demo);
    }

    // 3. Medición de tiempo de alta resolución para n = 100
    printf("\n--- BENCHMARK REQUERIDO: TIEMPO PARA n = 100 ---\n");
    int n_objetivo = 100;

    double t_inicio = obtener_tiempo_ms();
    BigInt* coefs100 = generar_fila_pascal(n_objetivo);
    double t_fin = obtener_tiempo_ms();

    double duracion_ms = t_fin - t_inicio;

    char buffer_inicial[MAX_DIGITOS_N100 + 1];
    char buffer_central[MAX_DIGITOS_N100 + 1];
    char buffer_final[MAX_DIGITOS_N100 + 1];

    bigint_a_string(&coefs100[0], buffer_inicial);
    bigint_a_string(&coefs100[50], buffer_central);
    bigint_a_string(&coefs100[100], buffer_final);

    printf("Tiempo de generacion para n=100 en C: %.5f ms.\n", duracion_ms);
    printf("Cantidad de coeficientes generados:   %d\n", n_objetivo + 1);
    printf("Coeficiente inicial c[0]:             %s\n", buffer_inicial);
    printf("Coeficiente central c[50]:            %s\n", buffer_central);
    printf("Coeficiente final c[100]:             %s\n", buffer_final);

    if (reporte) {
        fprintf(reporte, "UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA\n");
        fprintf(reporte, "LENGUAJES Y COMPILADORES - EJERCICIO II (C)\n");
        fprintf(reporte, "========================================================\n");
        fprintf(reporte, "Tiempo de generacion para n=100 en C: %.5f ms\n", duracion_ms);
        fprintf(reporte, "Cantidad de terminos: %d\n", n_objetivo + 1);
        fprintf(reporte, "Coeficiente central (c[50]): %s\n", buffer_central);
        fclose(reporte);
        printf("\n[OK] Resultados guardados exitosamente en 'tiempos_n100_c.txt'.\n");
    }

    // Liberar la memoria dinámica de n=100
    free(coefs100);

    return 0;
}