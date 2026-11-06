#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Tabla de simbolos para palabras reservadas de C a Español
typedef struct {
    const char* c_word;
    const char* es_word;
} EntradaDiccionario;

EntradaDiccionario tabla_reservadas[] = {
    {"int", "entero"},
    {"float", "flotante"},
    {"double", "doble"},
    {"char", "caracter"},
    {"void", "vacio"},
    {"if", "si"},
    {"else", "sino"},
    {"while", "mientras"},
    {"for", "para"},
    {"do", "hacer"},
    {"return", "retornar"},
    {"struct", "estructura"},
    {"switch", "segun"},
    {"case", "caso"},
    {"break", "romper"},
    {"continue", "continuar"},
    {"default", "defecto"},
    {"sizeof", "tamano_de"},
    {"static", "estatico"},
    {"const", "constante"},
    {NULL, NULL}
};

// Busca si una palabra es reservada y retorna su traduccion
const char* buscar_traduccion(const char* palabra) {
    for (int i = 0; tabla_reservadas[i].c_word != NULL; i++) {
        if (strcmp(palabra, tabla_reservadas[i].c_word) == 0) {
            return tabla_reservadas[i].es_word;
        }
    }
    return NULL;
}

// Carga un archivo en un buffer reservado con MEMORIA DINAMICA (malloc)
char* cargar_en_memoria_dinamica(const char* ruta, long* tamano_archivo) {
    FILE* f = fopen(ruta, "rb");
    if (!f) {
        printf("[ERROR] No se pudo abrir el archivo de entrada: '%s'\n", ruta);
        return NULL;
    }

    fseek(f, 0, SEEK_END);
    long tam = ftell(f);
    fseek(f, 0, SEEK_SET);

    char* buffer = (char*)malloc((tam + 1) * sizeof(char));
    if (!buffer) {
        printf("[ERROR] Fallo al reservar memoria dinamica (%ld bytes).\n", tam);
        fclose(f);
        return NULL;
    }

    size_t leidos = fread(buffer, sizeof(char), tam, f);
    buffer[leidos] = '\0';
    fclose(f);

    *tamano_archivo = (long)leidos;
    return buffer;
}

int main(void) {
    printf("========================================================\n");
    printf("  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)   \n");
    printf("       LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA    \n");
    printf("     EJERCICIO III: TRADUCTOR DE PALABRAS RESERVADAS    \n");
    printf("                     (LENGUAJE: C)                      \n");
    printf("========================================================\n\n");

    const char* origen = "codigo_prueba.c";
    const char* destino = "codigo_traducido_c.c";

    long tam_origen = 0;
    // 1. Cargar archivo dinamicamente en memoria
    char* fuente = cargar_en_memoria_dinamica(origen, &tam_origen);
    if (!fuente) return 1;

    printf("[OK] Archivo '%s' cargado dinamicamente (%ld bytes).\n", origen, tam_origen);

    // Buffer de salida en memoria dinamica (con capacidad para palabras mas largas)
    long capacidad_salida = tam_origen * 2 + 1024;
    char* salida = (char*)malloc(capacidad_salida * sizeof(char));
    if (!salida) {
        free(fuente);
        return 1;
    }

    size_t i = 0;
    size_t pos_out = 0;
    int palabras_traducidas = 0;

    // 2. Analizador Lexico (Scanner / Tokenizer)
    while (fuente[i] != '\0') {
        // Si detecta una letra o guion bajo, inicia un identificador/palabra
        if (isalpha((unsigned char)fuente[i]) || fuente[i] == '_') {
            char token[128];
            int len = 0;

            while (isalnum((unsigned char)fuente[i]) || fuente[i] == '_') {
                if (len < 127) token[len++] = fuente[i];
                i++;
            }
            token[len] = '\0';

            // Comprobar si es palabra reservada
            const char* traduccion = buscar_traduccion(token);
            if (traduccion != NULL) {
                size_t len_tr = strlen(traduccion);
                memcpy(&salida[pos_out], traduccion, len_tr);
                pos_out += len_tr;
                palabras_traducidas++;
            } else {
                memcpy(&salida[pos_out], token, len);
                pos_out += len;
            }
        } else {
            // Copiar operadores, llaves, espacios, saltos de linea tal cual
            salida[pos_out++] = fuente[i++];
        }
    }
    salida[pos_out] = '\0';

    // 3. Escribir el codigo traducido en archivo
    FILE* f_out = fopen(destino, "w");
    if (f_out) {
        fputs(salida, f_out);
        fclose(f_out);
        printf("[OK] Se identificaron y tradujeron %d palabras reservadas.\n", palabras_traducidas);
        printf("[OK] Archivo traducido guardado en '%s'.\n", destino);
    }

    printf("\n--- VISTA PREVIA DEL CODIGO TRADUCIDO EN C ---\n\n");
    printf("%s\n", salida);

    // 4. Liberar la memoria dinamica utilizada
    free(fuente);
    free(salida);

    return 0;
}