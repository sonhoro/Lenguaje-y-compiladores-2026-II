/*
 * =================================================================
 * PRUEBA DIAGNOSTICA - EJERCICIO 3
 * Carga un archivo C en memoria dinamica, detecta palabras reservadas
 * y muestra su traduccion al espanol.
 * -----------------------------------------------------------------
 * Estudiante : Carlos Brito | Cedula: 30857129 | Seccion: 3
 * Grupo      : Prohibido Rendirse
 * Lenguaje   : C (ANSI C99)
 * Compilar   : gcc -std=c99 -O2 -o ejercicio3.exe ejercicio3.c
 * Uso        : ejercicio3.exe [archivo.c]
 * =================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* --- Tabla de palabras reservadas C → Español --- */

/** Entrada en la tabla de palabras reservadas */
typedef struct {
    const char *palabra_c;    /* Palabra reservada en C    */
    const char *traduccion;   /* Equivalente en español    */
} EntradaReservada;

/*
 * Tabla estática de palabras reservadas.
 * Terminada con una entrada {NULL, NULL} como centinela.
 */
static const EntradaReservada TABLA_RESERVADAS[] = {
    /* ---- C89 / C90 ---- */
    {"auto",           "automatico"},
    {"break",          "romper"},
    {"case",           "caso"},
    {"char",           "caracter"},
    {"const",          "constante"},
    {"continue",       "continuar"},
    {"default",        "predeterminado"},
    {"do",             "hacer"},
    {"double",         "doble"},
    {"else",           "sino"},
    {"enum",           "enumeracion"},
    {"extern",         "externo"},
    {"float",          "flotante"},
    {"for",            "para"},
    {"goto",           "ir_a"},
    {"if",             "si"},
    {"int",            "entero"},
    {"long",           "largo"},
    {"register",       "registro"},
    {"return",         "retornar"},
    {"short",          "corto"},
    {"signed",         "con_signo"},
    {"sizeof",         "tamanio_de"},
    {"static",         "estatico"},
    {"struct",         "estructura"},
    {"switch",         "segun"},
    {"typedef",        "def_tipo"},
    {"union",          "union"},
    {"unsigned",       "sin_signo"},
    {"void",           "vacio"},
    {"volatile",       "volatil"},
    {"while",          "mientras"},
    /* ---- C99 ---- */
    {"inline",         "en_linea"},
    {"restrict",       "restringido"},
    {"_Bool",          "_Booleano"},
    {"_Complex",       "_Complejo"},
    {"_Imaginary",     "_Imaginario"},
    /* ---- C11 ---- */
    {"_Alignas",       "_AlinearComo"},
    {"_Alignof",       "_TamanioAlineacion"},
    {"_Atomic",        "_Atomico"},
    {"_Generic",       "_Generico"},
    {"_Noreturn",      "_SinRetorno"},
    {"_Static_assert", "_AsertionEstatica"},
    {"_Thread_local",  "_LocalHilo"},
    /* Centinela de fin de tabla */
    {NULL,             NULL}
};


/* --- Lista dinamica de ocurrencias (crece con realloc) --- */

typedef struct {
    char  *palabra;      /* Copia dinamica de la palabra */
    char  *traduccion;   /* Referencia a la tabla        */
    int    linea;        /* Linea donde aparece          */
} Ocurrencia;

typedef struct {
    Ocurrencia *datos;   /* Array dinamico (malloc/realloc) */
    int         n;       /* Cantidad de ocurrencias         */
    int         cap;     /* Capacidad actual                */
} ListaOcurrencias;

/* Crea una lista con capacidad inicial dada */
static ListaOcurrencias *crear_lista(int cap_inicial)
{
    ListaOcurrencias *lista =
        (ListaOcurrencias *)malloc(sizeof(ListaOcurrencias));
    if (!lista) return NULL;

    lista->datos = (Ocurrencia *)malloc(cap_inicial * sizeof(Ocurrencia));
    if (!lista->datos) { free(lista); return NULL; }

    lista->n   = 0;
    lista->cap = cap_inicial;
    return lista;
}

/* Agrega una ocurrencia; duplica capacidad con realloc si es necesario */
static int agregar_ocurrencia(ListaOcurrencias *lista,
                               const char       *palabra,
                               const char       *traduccion,
                               int               linea)
{
    /* Redimensionar si es necesario */
    if (lista->n >= lista->cap) {
        int nueva_cap = lista->cap * 2;
        Ocurrencia *nuevo = (Ocurrencia *)realloc(
            lista->datos, nueva_cap * sizeof(Ocurrencia));
        if (!nuevo) return 0;
        lista->datos = nuevo;
        lista->cap   = nueva_cap;
    }

    /* Alocar copia de la palabra */
    lista->datos[lista->n].palabra = (char *)malloc(strlen(palabra) + 1);
    if (!lista->datos[lista->n].palabra) return 0;
    strcpy(lista->datos[lista->n].palabra, palabra);

    lista->datos[lista->n].traduccion = (char *)traduccion; /* ref. a tabla */
    lista->datos[lista->n].linea      = linea;
    lista->n++;
    return 1;
}

/** Libera toda la memoria de una lista de ocurrencias */
static void liberar_lista(ListaOcurrencias *lista)
{
    if (!lista) return;
    for (int i = 0; i < lista->n; i++) {
        free(lista->datos[i].palabra);
    }
    free(lista->datos);
    free(lista);
}


/* --- Carga un archivo en memoria dinamica (fseek+ftell+malloc+fread) --- */
char *cargar_archivo_en_memoria(const char *ruta, long *tam_out)
{
    FILE *f = fopen(ruta, "r");
    if (!f) {
        fprintf(stderr, "  ERROR: No se pudo abrir '%s'\n", ruta);
        return NULL;
    }

    /* Determinar tamaño del archivo */
    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fseek(f, 0, SEEK_SET);

    /* Alocar buffer dinámico en el heap */
    char *buffer = (char *)malloc(tam + 1);
    if (!buffer) {
        fprintf(stderr, "  ERROR: Memoria insuficiente para cargar el archivo.\n");
        fclose(f);
        return NULL;
    }

    /* Volcar el contenido del archivo al buffer */
    long leidos = (long)fread(buffer, 1, tam, f);
    buffer[leidos] = '\0';   /* Terminar buffer con null */

    fclose(f);
    if (tam_out) *tam_out = leidos;
    return buffer;
}


/* --- Busqueda lineal en la tabla de palabras reservadas --- */
static const char *buscar_reservada(const char *palabra)
{
    for (int i = 0; TABLA_RESERVADAS[i].palabra_c != NULL; i++) {
        if (strcmp(TABLA_RESERVADAS[i].palabra_c, palabra) == 0)
            return TABLA_RESERVADAS[i].traduccion;
    }
    return NULL;
}


/* --- Elimina comentarios y literales del buffer (in-place, preserva \n) --- */
void eliminar_comentarios(char *buf, long tam)
{
    long i = 0;
    while (i < tam) {

        /* ---- Comentario de línea: // ---- */
        if (buf[i] == '/' && i + 1 < tam && buf[i+1] == '/') {
            buf[i] = buf[i+1] = ' ';
            i += 2;
            while (i < tam && buf[i] != '\n') {
                buf[i] = ' ';
                i++;
            }
        }

        /* ---- Comentario de bloque: /* ... * / ---- */
        else if (buf[i] == '/' && i + 1 < tam && buf[i+1] == '*') {
            buf[i] = buf[i+1] = ' ';
            i += 2;
            while (i < tam &&
                   !(buf[i] == '*' && i + 1 < tam && buf[i+1] == '/'))
            {
                if (buf[i] != '\n') buf[i] = ' ';
                i++;
            }
            if (i < tam) {               /* Consumir el cierre: asterisco-barra */
                buf[i] = buf[i+1] = ' ';
                i += 2;
            }
        }

        /* ---- Literal de cadena: "..." ---- */
        else if (buf[i] == '"') {
            buf[i] = ' '; i++;
            while (i < tam && buf[i] != '"') {
                if (buf[i] == '\\' && i + 1 < tam) { buf[i] = ' '; i++; }
                if (buf[i] != '\n') buf[i] = ' ';
                i++;
            }
            if (i < tam) { buf[i] = ' '; i++; }
        }

        /* ---- Literal de carácter: '...' ---- */
        else if (buf[i] == '\'') {
            buf[i] = ' '; i++;
            while (i < tam && buf[i] != '\'') {
                if (buf[i] == '\\' && i + 1 < tam) { buf[i] = ' '; i++; }
                buf[i] = ' '; i++;
            }
            if (i < tam) { buf[i] = ' '; i++; }
        }

        else {
            i++;
        }
    }
}


/* --- Tokeniza el buffer y detecta palabras reservadas --- */
ListaOcurrencias *analizar_buffer(const char *buf, long tam)
{
    ListaOcurrencias *lista = crear_lista(64);
    if (!lista) return NULL;

    char token[256];     /* Buffer temporal para acumular el token actual */
    int  tok_len = 0;
    int  linea   = 1;

    for (long i = 0; i <= tam; i++) {
        /* Carácter actual (o '\0' como centinela al final) */
        char c = (i < tam) ? buf[i] : '\0';

        if (c == '\n') linea++;

        /* Acumular caracteres de un identificador C */
        if (c == '_' || isalpha((unsigned char)c) ||
            (tok_len > 0 && isdigit((unsigned char)c)))
        {
            if (tok_len < (int)(sizeof(token) - 1))
                token[tok_len++] = c;
        }
        else {
            /* Fin de token: procesar si existe */
            if (tok_len > 0) {
                token[tok_len] = '\0';
                tok_len = 0;

                /* Buscar si es palabra reservada */
                const char *trad = buscar_reservada(token);
                if (trad) {
                    agregar_ocurrencia(lista, token, trad, linea);
                }
            }
        }
    }

    return lista;
}


/* --- Muestra en pantalla las ocurrencias detectadas --- */

void mostrar_resultados(const char *ruta, const ListaOcurrencias *lista)
{
    printf("\n%s\n", "=================================================================");
    printf("  ANALISIS DE PALABRAS RESERVADAS EN C\n");
    printf("  Archivo: %s\n", ruta);
    printf("%s\n", "=================================================================");

    if (lista->n == 0) {
        printf("\n  No se encontraron palabras reservadas de C.\n");
        return;
    }

    printf("\n  Total de ocurrencias : %d\n\n", lista->n);
    printf("  %-20s %-25s %s\n", "Palabra C", "Traduccion", "Linea");
    printf("  %-20s %-25s %s\n",
           "--------------------", "-------------------------", "------");

    for (int i = 0; i < lista->n; i++) {
        printf("  %-20s %-25s %d\n",
               lista->datos[i].palabra,
               lista->datos[i].traduccion,
               lista->datos[i].linea);
    }
    printf("%s\n", "=================================================================");
}


/* --- Escribe los resultados en resultado_ejercicio3.txt --- */

void escribir_resultados(const char           *ruta,
                         const ListaOcurrencias *lista,
                         double                 tiempo_total,
                         double                 tiempo_promedio,
                         long                   tam_buf)
{
    const char *nombre = "resultado_ejercicio3.txt";
    FILE *f = fopen(nombre, "w");
    if (!f) {
        printf("\n  ERROR: No se pudo crear '%s'\n", nombre);
        return;
    }

    const char *sep = "=================================================================\n";
    fprintf(f, "%s", sep);
    fprintf(f, "  PRUEBA DIAGNOSTICA — EJERCICIO 3\n");
    fprintf(f, "  Deteccion y traduccion de palabras reservadas en C\n");
    fprintf(f, "%s", sep);
    fprintf(f, "  Estudiante  : Carlos Brito\n");
    fprintf(f, "  Cedula      : 30857129\n");
    fprintf(f, "  Seccion     : 3\n");
    fprintf(f, "  Grupo       : Prohibido Rendirse\n");
    fprintf(f, "  Eslogan     : En la optimización habita el mañana\n");
    fprintf(f, "%s", sep);
    fprintf(f, "  Lenguaje         : C (ANSI C99)\n");
    fprintf(f, "  Archivo analizado: %s\n",    ruta);
    fprintf(f, "  Tamano en memoria: %ld bytes\n", tam_buf);
    fprintf(f, "  Iteraciones      : 100\n");
    fprintf(f, "  Tiempo total     : %.8f s\n", tiempo_total);
    fprintf(f, "  Tiempo promedio  : %.8f s\n", tiempo_promedio);
    fprintf(f, "%s", sep);
    fprintf(f, "\n  PALABRAS RESERVADAS ENCONTRADAS (%d ocurrencias):\n\n",
            lista->n);
    fprintf(f, "  %-20s %-25s %s\n", "Palabra C", "Traduccion", "Linea");
    fprintf(f, "  %-20s %-25s %s\n",
            "--------------------", "-------------------------", "------");

    for (int i = 0; i < lista->n; i++) {
        fprintf(f, "  %-20s %-25s %d\n",
                lista->datos[i].palabra,
                lista->datos[i].traduccion,
                lista->datos[i].linea);
    }

    fprintf(f, "\n%s", sep);
    fclose(f);
    printf("\n  Resultados guardados en '%s'\n", nombre);
}


/* --- Mide tiempo del analisis completo en 100 iteraciones --- */

void medir_tiempo(const char *ruta, double *total_out, double *promedio_out)
{
    int n_iter = 100;

    clock_t inicio = clock();
    for (int i = 0; i < n_iter; i++) {
        long  tam;
        char *buf = cargar_archivo_en_memoria(ruta, &tam);
        if (buf) {
            eliminar_comentarios(buf, tam);
            ListaOcurrencias *lista = analizar_buffer(buf, tam);
            liberar_lista(lista);
            free(buf);
        }
    }
    clock_t fin = clock();

    *total_out   = (double)(fin - inicio) / CLOCKS_PER_SEC;
    *promedio_out = *total_out / n_iter;
}


/* --- Punto de entrada --- */

int main(int argc, char *argv[])
{
    printf("\n%s\n", "=================================================================");
    printf("  PRUEBA DIAGNOSTICA — EJERCICIO 3\n");
    printf("  Lenguajes y Compiladores\n");
    printf("%s\n", "=================================================================");
    printf("  Estudiante : Carlos Brito\n");
    printf("  Cedula     : 30857129\n");
    printf("  Seccion    : 3\n");
    printf("  Grupo      : Prohibido Rendirse\n");
    printf("  Eslogan    : En la optimización habita el mañana\n");

    /* Ruta del archivo a analizar (argumento CLI o por defecto) */
    const char *ruta = (argc > 1) ? argv[1] : "programa_ejemplo.c";

    /* Paso 1: Cargar archivo en buffer dinámico (malloc + fread) */
    printf("\n  Cargando '%s' en memoria...\n", ruta);
    long  tam;
    char *buffer = cargar_archivo_en_memoria(ruta, &tam);
    if (!buffer) return 1;
    printf("  Archivo cargado: %ld bytes en memoria.\n", tam);

    /* Paso 2: Crear copia del buffer para preprocesamiento
     *         (preservar el original por si se necesita mostrar) */
    char *buf_limpio = (char *)malloc(tam + 1);
    if (!buf_limpio) { free(buffer); return 1; }
    memcpy(buf_limpio, buffer, tam + 1);

    /* Paso 3: Eliminar comentarios y literales del buffer (in-place) */
    eliminar_comentarios(buf_limpio, tam);

    /* Paso 4: Tokenizar y detectar palabras reservadas */
    ListaOcurrencias *lista = analizar_buffer(buf_limpio, tam);
    if (!lista) { free(buffer); free(buf_limpio); return 1; }

    /* Paso 5: Mostrar resultados en pantalla */
    mostrar_resultados(ruta, lista);

    /* Paso 6: Medir tiempo (100 iteraciones del análisis completo) */
    double tiempo_total, tiempo_promedio;
    medir_tiempo(ruta, &tiempo_total, &tiempo_promedio);
    printf("\n  Medicion de tiempo (100 iteraciones):\n");
    printf("    Tiempo total   : %.8f s\n", tiempo_total);
    printf("    Tiempo promedio: %.8f s\n", tiempo_promedio);

    /* Paso 7: Guardar resultados en archivo .txt */
    escribir_resultados(ruta, lista, tiempo_total, tiempo_promedio, tam);

    /* Liberar toda la memoria alojada dinámicamente */
    liberar_lista(lista);
    free(buf_limpio);
    free(buffer);

    printf("\n  Programa finalizado correctamente.\n\n");
    return 0;
}
