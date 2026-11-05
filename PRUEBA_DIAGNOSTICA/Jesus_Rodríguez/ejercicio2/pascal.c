/* Ejercicio 2 - Coeficientes de (x+1)^n con el triangulo de Pascal (C).
 * Compilar: gcc -O2 -o pascal pascal.c      Ejecutar: pascal [n] [x]   (por defecto n=5, x=2)
 * Muestra el polinomio y el calculo paso a paso de f(x), y mide el tiempo de generar n=100.
 * Escribe resultado_c.txt. Usa memoria dinamica (calloc/free) y __int128 (gcc/clang). */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef __int128 i128;
#define REPS 1000 /* una generacion de n=100 dura microsegundos: se promedia sobre REPS */
#define NMAX 120  /* ponytail: C(n,n/2) debe caber en 128 bits; para n mayor usar Python */

static double ahora(void) {
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

static void imprimir(FILE *f, i128 v) {
    char b[48];
    int i = 47, neg = v < 0;
    b[i] = '\0';
    if (neg) v = -v;
    do { b[--i] = '0' + (int)(v % 10); v /= 10; } while (v);
    if (neg) b[--i] = '-';
    fputs(b + i, f);
}

/* Fila n del triangulo: un solo arreglo dinamico actualizado en sitio de derecha a izquierda. */
static i128 *generar_pascal(int n) { /* no llamarla "pascal": es una macro de windows.h */
    i128 *c = calloc(n + 1, sizeof *c);
    c[0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = i; j > 0; j--) c[j] += c[j - 1];
    return c;
}

static void polinomio(FILE *f, const i128 *c, int n) {
    for (int k = 0; k <= n; k++) {
        int p = n - k;
        if (k) fputs(" + ", f);
        if (!(c[k] == 1 && p > 0)) imprimir(f, c[k]);
        if (p == 1) fputs("x", f);
        else if (p > 1) fprintf(f, "x^%d", p);
    }
}

static void pasos(FILE *f, const i128 *c, int n, long long x) {
    i128 *pw = malloc((n + 1) * sizeof *pw), total = 0;
    pw[0] = 1;
    int ok = 1;
    for (int p = 1; p <= n && ok; p++) ok = !__builtin_mul_overflow(pw[p - 1], (i128)x, &pw[p]);
    fprintf(f, "f(%lld) = (%lld+1)^%d\n", x, x, n);
    for (int k = 0; k <= n && ok; k++) {
        i128 t;
        ok = !__builtin_mul_overflow(c[k], pw[n - k], &t) && !__builtin_add_overflow(total, t, &total);
        if (!ok) break;
        fprintf(f, "  termino %d: ", k);
        imprimir(f, c[k]);
        fprintf(f, " * %lld^%d = ", x, n - k);
        imprimir(f, t);
        fputc('\n', f);
    }
    if (ok) { fputs("  Suma = ", f); imprimir(f, total); fputc('\n', f); }
    else fputs("  (desbordamiento de 128 bits: use pascal.py para este x y n)\n", f);
    free(pw);
}

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 5;
    long long x = argc > 2 ? atoll(argv[2]) : 2;
    if (n < 0 || n > NMAX) { fprintf(stderr, "n debe estar entre 0 y %d\n", NMAX); return 1; }

    FILE *out = fopen("resultado_c.txt", "w");
    if (!out) { perror("resultado_c.txt"); return 1; }
    fputs("EJERCICIO 2 - Pascal / (x+1)^n (C)\n\n", out);
    i128 *c = generar_pascal(n);
    fprintf(out, "Coeficientes n=%d:", n);
    for (int k = 0; k <= n; k++) { fputc(' ', out); imprimir(out, c[k]); }
    fprintf(out, "\n(x+1)^%d = ", n);
    polinomio(out, c, n);
    fputs("\n\n", out);
    pasos(out, c, n, x);
    free(c);

    i128 *c100 = NULL;
    double t0 = ahora();
    for (int r = 0; r < REPS; r++) { free(c100); c100 = generar_pascal(100); }
    double dt = (ahora() - t0) * 1e6;
    fputs("\nGeneracion n=100:\n  C(100,50) = ", out);
    imprimir(out, c100[50]);
    fprintf(out, "\n  Tiempo total %d generaciones: %.2f us\n  Tiempo por generacion: %.2f us\n",
            REPS, dt, dt / REPS);
    free(c100);
    fclose(out);
    printf("OK: resultado_c.txt escrito\n");
    return 0;
}
