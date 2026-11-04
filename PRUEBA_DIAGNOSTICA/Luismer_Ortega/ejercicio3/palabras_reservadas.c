/*
 * Ejercicio 3 - Detector y traductor de palabras reservadas de C
 * Lenguajes y Compiladores 2026-II
 * Estudiante: Luismer Ortega
 * Universidad Nacional Experimental de Guayana
 *
 * Compilar con:
 *   C:\MinGW\bin\gcc.exe -o palabras_reservadas palabras_reservadas.c
 * Ejecutar con:
 *   palabras_reservadas.exe [archivo.c]
 *   (sin argumento usa el programa de ejemplo embebido)
 *
 * El programa carga el codigo C en memoria dinamica (malloc),
 * detecta palabras reservadas y las traduce al espanol.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------ */
/* Tabla de palabras reservadas C -> Espanol                          */
/* ------------------------------------------------------------------ */

typedef struct {
    const char *keyword;
    const char *espanol;
} EntradaKeyword;

static EntradaKeyword TABLA[] = {
    {"auto",     "automatico"},
    {"break",    "romper"},
    {"case",     "caso"},
    {"char",     "caracter"},
    {"const",    "constante"},
    {"continue", "continuar"},
    {"default",  "predeterminado"},
    {"do",       "hacer"},
    {"double",   "doble"},
    {"else",     "sino"},
    {"enum",     "enumeracion"},
    {"extern",   "externo"},
    {"float",    "flotante"},
    {"for",      "para"},
    {"goto",     "ir_a"},
    {"if",       "si"},
    {"int",      "entero"},
    {"long",     "largo"},
    {"register", "registro"},
    {"return",   "retornar"},
    {"short",    "corto"},
    {"signed",   "con_signo"},
    {"sizeof",   "tamano_de"},
    {"static",   "estatico"},
    {"struct",   "estructura"},
    {"switch",   "interruptor"},
    {"typedef",  "tipo_def"},
    {"union",    "union"},
    {"unsigned", "sin_signo"},
    {"void",     "vacio"},
    {"volatile", "volatil"},
    {"while",    "mientras"},
    {NULL, NULL}
};

#define N_KEYWORDS 32

/* ------------------------------------------------------------------ */
/* Buscar keyword en la tabla                                         */
/* ------------------------------------------------------------------ */

const char *buscar_traduccion(const char *palabra) {
    for (int i = 0; TABLA[i].keyword != NULL; i++) {
        if (strcmp(palabra, TABLA[i].keyword) == 0) {
            return TABLA[i].espanol;
        }
    }
    return NULL;
}

/* ------------------------------------------------------------------ */
/* Cargar archivo en memoria dinamica                                 */
/* ------------------------------------------------------------------ */

char *cargar_archivo(const char *ruta, long *tam_out) {
    FILE *f = fopen(ruta, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* Alojar memoria dinamicamente */
    char *buf = (char *)malloc(tam + 1);
    if (!buf) { fclose(f); return NULL; }

    fread(buf, 1, tam, f);
    buf[tam] = '\0';
    fclose(f);

    if (tam_out) *tam_out = tam;
    return buf;
}

/* ------------------------------------------------------------------ */
/* Verificar si un char es separador de token                        */
/* ------------------------------------------------------------------ */

int es_separador(char c) {
    return !isalnum((unsigned char)c) && c != '_';
}

/* ------------------------------------------------------------------ */
/* Analizar y mostrar palabras reservadas                             */
/* ------------------------------------------------------------------ */

typedef struct {
    char   palabra[64];
    char   traduccion[64];
    int    posicion;
} Ocurrencia;

/*
 * Analiza el codigo C y llena el array de ocurrencias.
 * Retorna la cantidad de ocurrencias encontradas.
 * El array 'ocurrencias' debe tener capacidad suficiente (malloc externo).
 */
int analizar_codigo(const char *codigo, Ocurrencia *ocurrencias, int max_ocs) {
    int n_ocs = 0;
    int len   = (int)strlen(codigo);
    int i     = 0;

    while (i < len) {
        /* Saltar comentario bloque */
        if (i + 1 < len && codigo[i] == '/' && codigo[i+1] == '*') {
            i += 2;
            while (i + 1 < len && !(codigo[i] == '*' && codigo[i+1] == '/')) i++;
            i += 2;
            continue;
        }
        /* Saltar comentario linea */
        if (i + 1 < len && codigo[i] == '/' && codigo[i+1] == '/') {
            while (i < len && codigo[i] != '\n') i++;
            continue;
        }
        /* Saltar string literal */
        if (codigo[i] == '"') {
            i++;
            while (i < len && codigo[i] != '"') {
                if (codigo[i] == '\\') i++;
                i++;
            }
            i++;
            continue;
        }
        /* Saltar char literal */
        if (codigo[i] == '\'') {
            i++;
            while (i < len && codigo[i] != '\'') {
                if (codigo[i] == '\\') i++;
                i++;
            }
            i++;
            continue;
        }
        /* Identificador */
        if (isalpha((unsigned char)codigo[i]) || codigo[i] == '_') {
            int inicio = i;
            while (i < len && (isalnum((unsigned char)codigo[i]) || codigo[i] == '_'))
                i++;
            int largo = i - inicio;
            if (largo < 63) {
                char palabra[64];
                strncpy(palabra, codigo + inicio, largo);
                palabra[largo] = '\0';
                const char *trad = buscar_traduccion(palabra);
                if (trad && n_ocs < max_ocs) {
                    strncpy(ocurrencias[n_ocs].palabra,     palabra, 63);
                    strncpy(ocurrencias[n_ocs].traduccion,  trad,    63);
                    ocurrencias[n_ocs].posicion = inicio;
                    n_ocs++;
                }
            }
            continue;
        }
        i++;
    }
    return n_ocs;
}

/* ------------------------------------------------------------------ */
/* Traducir el codigo: reemplazar keywords por su equivalente espanol */
/* ------------------------------------------------------------------ */

/*
 * Retorna un nuevo buffer con el codigo traducido.
 * El llamador debe liberar con free().
 */
char *traducir_codigo(const char *codigo) {
    int len = (int)strlen(codigo);
    /* Peor caso: cada keyword se expande a maximo 15 chars ("predeterminado") */
    int max_out = len * 3 + 1;
    char *out = (char *)malloc(max_out);
    if (!out) return NULL;

    int i   = 0;
    int pos = 0;

    while (i < len) {
        /* Pasar comentarios y literales sin modificar */
        if (i + 1 < len && codigo[i] == '/' && codigo[i+1] == '*') {
            out[pos++] = codigo[i++];
            out[pos++] = codigo[i++];
            while (i + 1 < len && !(codigo[i] == '*' && codigo[i+1] == '/')) {
                out[pos++] = codigo[i++];
            }
            if (i + 1 < len) { out[pos++] = codigo[i++]; out[pos++] = codigo[i++]; }
            continue;
        }
        if (i + 1 < len && codigo[i] == '/' && codigo[i+1] == '/') {
            while (i < len && codigo[i] != '\n') out[pos++] = codigo[i++];
            continue;
        }
        if (codigo[i] == '"') {
            out[pos++] = codigo[i++];
            while (i < len && codigo[i] != '"') {
                if (codigo[i] == '\\') out[pos++] = codigo[i++];
                out[pos++] = codigo[i++];
            }
            if (i < len) out[pos++] = codigo[i++];
            continue;
        }
        if (codigo[i] == '\'') {
            out[pos++] = codigo[i++];
            while (i < len && codigo[i] != '\'') {
                if (codigo[i] == '\\') out[pos++] = codigo[i++];
                out[pos++] = codigo[i++];
            }
            if (i < len) out[pos++] = codigo[i++];
            continue;
        }

        /* Identificador: buscar si es keyword */
        if (isalpha((unsigned char)codigo[i]) || codigo[i] == '_') {
            int inicio = i;
            while (i < len && (isalnum((unsigned char)codigo[i]) || codigo[i] == '_'))
                i++;
            int largo = i - inicio;
            char palabra[64] = {0};
            if (largo < 63) {
                strncpy(palabra, codigo + inicio, largo);
                const char *trad = buscar_traduccion(palabra);
                if (trad) {
                    int tlen = (int)strlen(trad);
                    memcpy(out + pos, trad, tlen);
                    pos += tlen;
                    continue;
                }
            }
            /* No es keyword: copiar tal cual */
            memcpy(out + pos, codigo + inicio, largo);
            pos += largo;
            continue;
        }

        out[pos++] = codigo[i++];
    }
    out[pos] = '\0';
    return out;
}

/* ------------------------------------------------------------------ */
/* Programa de ejemplo embebido                                       */
/* ------------------------------------------------------------------ */

static const char *EJEMPLO_C =
    "#include <stdio.h>\n"
    "#include <stdlib.h>\n"
    "\n"
    "/* Ejemplo de programa C con palabras reservadas */\n"
    "\n"
    "typedef struct Nodo {\n"
    "    int valor;\n"
    "    struct Nodo *siguiente;\n"
    "} Nodo;\n"
    "\n"
    "int factorial(int n) {\n"
    "    if (n <= 0) return 1;\n"
    "    return n * factorial(n - 1);\n"
    "}\n"
    "\n"
    "int main(void) {\n"
    "    int i;\n"
    "    float suma = 0.0;\n"
    "    char letra = 'A';\n"
    "    const int MAX = 10;\n"
    "    unsigned int contador = 0;\n"
    "\n"
    "    for (i = 0; i < MAX; i++) {\n"
    "        if (i % 2 == 0) {\n"
    "            suma += (float)i;\n"
    "            contador++;\n"
    "        } else {\n"
    "            continue;\n"
    "        }\n"
    "    }\n"
    "\n"
    "    while (contador > 0) {\n"
    "        switch (contador % 3) {\n"
    "            case 0: break;\n"
    "            case 1: suma -= 1.0; break;\n"
    "            default: suma += 0.5;\n"
    "        }\n"
    "        contador--;\n"
    "    }\n"
    "\n"
    "    int *arr = (int *)malloc(MAX * sizeof(int));\n"
    "    if (arr == NULL) return 1;\n"
    "\n"
    "    for (i = 0; i < MAX; i++) {\n"
    "        arr[i] = factorial(i);\n"
    "    }\n"
    "\n"
    "    free(arr);\n"
    "    return 0;\n"
    "}\n";

/* ------------------------------------------------------------------ */
/* main                                                               */
/* ------------------------------------------------------------------ */

int main(int argc, char *argv[]) {
    char *codigo = NULL;
    long tam     = 0;
    int  liberar = 0;

    if (argc > 1) {
        /* Cargar archivo en memoria dinamica */
        codigo = cargar_archivo(argv[1], &tam);
        if (!codigo) {
            fprintf(stderr, "Error: no se pudo leer '%s'\n", argv[1]);
            return 1;
        }
        liberar = 1;
        printf("Archivo cargado: %s (%ld bytes)\n\n", argv[1], tam);
    } else {
        /* Usar ejemplo embebido */
        tam    = (long)strlen(EJEMPLO_C);
        codigo = (char *)malloc(tam + 1);
        if (!codigo) return 1;
        memcpy(codigo, EJEMPLO_C, tam + 1);
        liberar = 1;
        printf("Usando programa de ejemplo embebido (%ld bytes)\n\n", tam);
    }

    /* Analizar */
    int max_ocs = 4096;
    Ocurrencia *ocs = (Ocurrencia *)malloc(max_ocs * sizeof(Ocurrencia));
    if (!ocs) { free(codigo); return 1; }

    int n_ocs = analizar_codigo(codigo, ocs, max_ocs);

    /* Mostrar codigo original */
    printf("============================================================\n");
    printf("CODIGO FUENTE ORIGINAL\n");
    printf("============================================================\n");
    printf("%s\n", codigo);

    /* Mostrar palabras encontradas */
    printf("============================================================\n");
    printf("PALABRAS RESERVADAS ENCONTRADAS\n");
    printf("============================================================\n");

    if (n_ocs == 0) {
        printf("No se encontraron palabras reservadas de C.\n\n");
    } else {
        /* Contar unicas */
        int unicas[N_KEYWORDS] = {0};
        for (int i = 0; i < n_ocs; i++) {
            for (int j = 0; TABLA[j].keyword != NULL; j++) {
                if (strcmp(ocs[i].palabra, TABLA[j].keyword) == 0) {
                    unicas[j]++;
                    break;
                }
            }
        }
        printf("Total de ocurrencias: %d\n\n", n_ocs);
        printf("%-15s %-20s %s\n", "Keyword C", "Traduccion", "Ocurrencias");
        printf("%-15s %-20s %s\n", "--------------", "--------------------", "-----------");
        for (int j = 0; TABLA[j].keyword != NULL; j++) {
            if (unicas[j] > 0) {
                printf("%-15s %-20s %d\n",
                       TABLA[j].keyword, TABLA[j].espanol, unicas[j]);
            }
        }
        printf("\n");
    }

    /* Traducir y mostrar */
    char *traducido = traducir_codigo(codigo);
    if (traducido) {
        printf("============================================================\n");
        printf("CODIGO CON PALABRAS RESERVADAS TRADUCIDAS AL ESPANOL\n");
        printf("============================================================\n");
        printf("%s\n", traducido);
    }

    /* Guardar en archivo */
    FILE *f = fopen("resultado_c.txt", "w");
    if (f) {
        fprintf(f, "EJERCICIO 3 - PALABRAS RESERVADAS C -> ESPANOL\n");
        fprintf(f, "Lenguajes y Compiladores 2026-II -- Luismer Ortega\n");
        fprintf(f, "============================================================\n\n");

        fprintf(f, "TABLA COMPLETA DE PALABRAS RESERVADAS\n");
        fprintf(f, "%-15s %-20s\n", "Keyword C", "Traduccion");
        fprintf(f, "%-15s %-20s\n", "--------------", "--------------------");
        for (int j = 0; TABLA[j].keyword != NULL; j++) {
            fprintf(f, "%-15s %-20s\n", TABLA[j].keyword, TABLA[j].espanol);
        }

        fprintf(f, "\nCODIGO FUENTE ORIGINAL\n");
        fprintf(f, "--------------------------------------------\n");
        fprintf(f, "%s\n", codigo);

        if (n_ocs > 0) {
            fprintf(f, "\nPALABRAS ENCONTRADAS (%d ocurrencias)\n", n_ocs);
            fprintf(f, "--------------------------------------------\n");
            int unicas[N_KEYWORDS] = {0};
            for (int i = 0; i < n_ocs; i++) {
                for (int j = 0; TABLA[j].keyword != NULL; j++) {
                    if (strcmp(ocs[i].palabra, TABLA[j].keyword) == 0) {
                        unicas[j]++;
                        break;
                    }
                }
            }
            fprintf(f, "%-15s %-20s %s\n", "Keyword C", "Traduccion", "Ocurrencias");
            for (int j = 0; TABLA[j].keyword != NULL; j++) {
                if (unicas[j] > 0)
                    fprintf(f, "%-15s %-20s %d\n",
                            TABLA[j].keyword, TABLA[j].espanol, unicas[j]);
            }
        }

        if (traducido) {
            fprintf(f, "\nCODIGO TRADUCIDO AL ESPANOL\n");
            fprintf(f, "--------------------------------------------\n");
            fprintf(f, "%s\n", traducido);
        }

        fclose(f);
        printf("Resultados guardados en resultado_c.txt\n");
    }

    free(ocs);
    if (traducido) free(traducido);
    if (liberar)   free(codigo);

    return 0;
}
