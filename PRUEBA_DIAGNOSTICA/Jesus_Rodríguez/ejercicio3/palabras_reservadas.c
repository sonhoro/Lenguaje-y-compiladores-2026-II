/* Ejercicio 3 - Palabras reservadas de C y su traduccion al espanol (C).
 * Compilar: gcc -O2 -o palabras_reservadas palabras_reservadas.c
 * Ejecutar (en esta carpeta): palabras_reservadas [archivo.c]   (por defecto ejemplo.c)
 * Carga el archivo en memoria dinamica, ignora comentarios y cadenas, cuenta y traduce las
 * palabras reservadas (C89 + C99/C11) y mide 100 analisis. Escribe resultado_c.txt (UTF-8). */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h>
#endif

typedef struct { const char *c, *es; } Par;
static const Par TABLA[] = {
    {"auto", "automático"}, {"break", "romper"}, {"case", "caso"}, {"char", "carácter"},
    {"const", "constante"}, {"continue", "continuar"}, {"default", "por_defecto"}, {"do", "hacer"},
    {"double", "doble"}, {"else", "sino"}, {"enum", "enumeración"}, {"extern", "externo"},
    {"float", "flotante"}, {"for", "para"}, {"goto", "ir_a"}, {"if", "si"}, {"int", "entero"},
    {"long", "largo"}, {"register", "registro"}, {"return", "retornar"}, {"short", "corto"},
    {"signed", "con_signo"}, {"sizeof", "tamaño_de"}, {"static", "estático"},
    {"struct", "estructura"}, {"switch", "según"}, {"typedef", "definir_tipo"},
    {"union", "unión"}, {"unsigned", "sin_signo"}, {"void", "vacío"}, {"volatile", "volátil"},
    {"while", "mientras"},
    /* C99 / C11 */
    {"inline", "en_línea"}, {"restrict", "restringido"}, {"_Bool", "_Booleano"},
    {"_Complex", "_Complejo"}, {"_Imaginary", "_Imaginario"}, {"_Alignas", "_AlinearComo"},
    {"_Alignof", "_AlineaciónDe"}, {"_Atomic", "_Atómico"}, {"_Generic", "_Genérico"},
    {"_Noreturn", "_SinRetorno"}, {"_Static_assert", "_Aserción_Estática"},
    {"_Thread_local", "_Local_de_Hilo"},
};
#define NP ((int)(sizeof TABLA / sizeof *TABLA))

typedef struct { int n, cap, *lin; } Apar; /* lineas donde aparece cada palabra (arreglo dinamico) */

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

static int buscar(const char *q, size_t len) { /* ponytail: busqueda lineal, 44 entradas */
    for (int k = 0; k < NP; k++)
        if (strlen(TABLA[k].c) == len && memcmp(TABLA[k].c, q, len) == 0) return k;
    return -1;
}

static void anotar(Apar *a, int linea) {
    if (a->n == a->cap) {
        a->cap = a->cap ? a->cap * 2 : 4;
        a->lin = realloc(a->lin, a->cap * sizeof *a->lin);
    }
    a->lin[a->n++] = linea;
}

/* Recorre el codigo; si trad != NULL escribe alli el codigo traducido. */
static void analizar(const char *s, Apar *ap, FILE *trad) {
    int linea = 1;
    const char *p = s;
    while (*p) {
        const char *q = p;
        if (p[0] == '/' && p[1] == '/') { /* comentario de linea */
            while (*p && *p != '\n') p++;
            if (trad) fwrite(q, 1, p - q, trad);
        } else if (p[0] == '/' && p[1] == '*') { /* comentario de bloque */
            p += 2;
            while (*p && !(p[0] == '*' && p[1] == '/')) { if (*p == '\n') linea++; p++; }
            if (*p) p += 2;
            if (trad) fwrite(q, 1, p - q, trad);
        } else if (*p == '"' || *p == '\'') { /* cadena o caracter */
            char d = *p++;
            while (*p && *p != d && *p != '\n') {
                if (*p == '\\' && p[1]) { if (p[1] == '\n') linea++; p++; }
                p++;
            }
            if (*p == d) p++;
            if (trad) fwrite(q, 1, p - q, trad);
        } else if (isalpha((unsigned char)*p) || *p == '_') { /* identificador */
            while (isalnum((unsigned char)*p) || *p == '_') p++;
            int k = buscar(q, p - q);
            if (k >= 0) {
                if (ap) anotar(&ap[k], linea);
                if (trad) fputs(TABLA[k].es, trad);
            } else if (trad) fwrite(q, 1, p - q, trad);
        } else {
            if (*p == '\n') linea++;
            if (trad) fputc(*p, trad);
            p++;
        }
    }
}

int main(int argc, char **argv) {
    const char *archivo = argc > 1 ? argv[1] : "ejemplo.c";
    FILE *f = fopen(archivo, "rb");
    if (!f) { perror(archivo); return 1; }
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    rewind(f);
    char *src = malloc(tam + 1); /* el programa C queda cargado en memoria dinamica */
    src[fread(src, 1, tam, f)] = '\0';
    fclose(f);

    Apar *ap = calloc(NP, sizeof *ap);
    FILE *out = fopen("resultado_c.txt", "w");
    if (!out) { perror("resultado_c.txt"); return 1; }
    fprintf(out, "EJERCICIO 3 - Palabras reservadas de C (C)\n\nArchivo: %s\n\n"
                 "palabra | español | veces | líneas\n", archivo);

    FILE *nul = fopen("nul_tmp.txt", "w+"); /* destino temporal del codigo traducido */
    analizar(src, ap, nul);
    int distintas = 0, total = 0;
    for (int k = 0; k < NP; k++) {
        if (!ap[k].n) continue;
        distintas++;
        total += ap[k].n;
        fprintf(out, "%s | %s | %d | ", TABLA[k].c, TABLA[k].es, ap[k].n);
        for (int i = 0; i < ap[k].n; i++) fprintf(out, i ? ",%d" : "%d", ap[k].lin[i]);
        fputc('\n', out);
    }
    fprintf(out, "\nTotal de palabras reservadas distintas: %d\nTotal de apariciones: %d\n\n"
                 "Código traducido:\n", distintas, total);
    rewind(nul);
    for (int ch; (ch = fgetc(nul)) != EOF;) fputc(ch, out);
    fclose(nul);
    remove("nul_tmp.txt");

    double t0 = ahora();
    for (int r = 0; r < 100; r++) analizar(src, NULL, NULL);
    fprintf(out, "\nTiempo n=100 (100 análisis del archivo): %.2f us\n", (ahora() - t0) * 1e6);

    for (int k = 0; k < NP; k++) free(ap[k].lin);
    free(ap);
    free(src);
    fclose(out);
    printf("OK: resultado_c.txt escrito\n");
    return 0;
}
