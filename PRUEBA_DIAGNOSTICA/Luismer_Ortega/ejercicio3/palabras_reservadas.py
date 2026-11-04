"""
Ejercicio 3 - Detector y traductor de palabras reservadas de C
Lenguajes y Compiladores 2026-II
Estudiante: Luismer Ortega
Universidad Nacional Experimental de Guayana

Descripcion:
  - Lee un programa escrito en C (cargado dinamicamente en memoria).
  - Verifica si existen palabras reservadas del lenguaje C.
  - Traduce cada palabra reservada a su equivalente en espanol.
  - Muestra las palabras encontradas y el codigo con las traducciones.
"""

import sys

# ------------------------------------------------------------------
# Tabla de palabras reservadas C -> Espanol
# ------------------------------------------------------------------

PALABRAS_RESERVADAS = {
    "auto":     "automatico",
    "break":    "romper",
    "case":     "caso",
    "char":     "caracter",
    "const":    "constante",
    "continue": "continuar",
    "default":  "predeterminado",
    "do":       "hacer",
    "double":   "doble",
    "else":     "sino",
    "enum":     "enumeracion",
    "extern":   "externo",
    "float":    "flotante",
    "for":      "para",
    "goto":     "ir_a",
    "if":       "si",
    "int":      "entero",
    "long":     "largo",
    "register": "registro",
    "return":   "retornar",
    "short":    "corto",
    "signed":   "con_signo",
    "sizeof":   "tamano_de",
    "static":   "estatico",
    "struct":   "estructura",
    "switch":   "interruptor",
    "typedef":  "tipo_def",
    "union":    "union",
    "unsigned": "sin_signo",
    "void":     "vacio",
    "volatile": "volatil",
    "while":    "mientras",
}

# ------------------------------------------------------------------
# Cargar programa C en memoria dinamica
# ------------------------------------------------------------------

def cargar_programa(ruta_archivo):
    """
    Carga el contenido completo de un archivo C en memoria
    (lista de caracteres — simula memoria dinamica en Python).
    Retorna la cadena de texto completa.
    """
    with open(ruta_archivo, "r", encoding="utf-8", errors="replace") as f:
        contenido = f.read()
    # Simulamos memoria dinamica: lo almacenamos como lista de chars
    memoria = list(contenido)
    return memoria


def memoria_a_texto(memoria):
    """Convierte la lista de chars de vuelta a string."""
    return "".join(memoria)

# ------------------------------------------------------------------
# Tokenizador respetando strings y comentarios
# ------------------------------------------------------------------

def tokenizar_codigo_c(codigo):
    """
    Tokeniza el codigo C extrayendo palabras identifiers/keywords
    respetando literales de cadena, caracteres y comentarios.
    Retorna lista de (token, posicion_inicio).
    """
    tokens = []
    i = 0
    n = len(codigo)

    while i < n:
        # Saltar comentario de bloque /* ... */
        if i + 1 < n and codigo[i] == '/' and codigo[i+1] == '*':
            i += 2
            while i + 1 < n and not (codigo[i] == '*' and codigo[i+1] == '/'):
                i += 1
            i += 2
            continue

        # Saltar comentario de linea // ...
        if i + 1 < n and codigo[i] == '/' and codigo[i+1] == '/':
            while i < n and codigo[i] != '\n':
                i += 1
            continue

        # Saltar literales de cadena "..."
        if codigo[i] == '"':
            i += 1
            while i < n and codigo[i] != '"':
                if codigo[i] == '\\':
                    i += 1
                i += 1
            i += 1
            continue

        # Saltar literales de caracter '...'
        if codigo[i] == "'":
            i += 1
            while i < n and codigo[i] != "'":
                if codigo[i] == '\\':
                    i += 1
                i += 1
            i += 1
            continue

        # Identificador / posible keyword
        if codigo[i].isalpha() or codigo[i] == '_':
            inicio = i
            while i < n and (codigo[i].isalnum() or codigo[i] == '_'):
                i += 1
            palabra = codigo[inicio:i]
            tokens.append((palabra, inicio))
            continue

        i += 1

    return tokens


# ------------------------------------------------------------------
# Analisis y traduccion
# ------------------------------------------------------------------

def analizar_y_traducir(codigo):
    """
    Analiza el codigo C, detecta palabras reservadas y las traduce.
    Retorna:
      - encontradas: lista de (keyword, posicion, traduccion)
      - codigo_traducido: codigo con keywords reemplazados
    """
    tokens = tokenizar_codigo_c(codigo)
    encontradas = []

    for palabra, pos in tokens:
        if palabra in PALABRAS_RESERVADAS:
            encontradas.append((palabra, pos, PALABRAS_RESERVADAS[palabra]))

    partes = []
    posicion_anterior = 0
    for palabra, posicion in tokens:
        traduccion = PALABRAS_RESERVADAS.get(palabra)
        if traduccion is not None:
            partes.append(codigo[posicion_anterior:posicion])
            partes.append(traduccion)
            posicion_anterior = posicion + len(palabra)
    partes.append(codigo[posicion_anterior:])
    codigo_traducido = "".join(partes)

    return encontradas, codigo_traducido


# ------------------------------------------------------------------
# Programa de ejemplo para demostrar (si no se da archivo externo)
# ------------------------------------------------------------------

PROGRAMA_C_EJEMPLO = """\
#include <stdio.h>
#include <stdlib.h>

/* Ejemplo de programa C con palabras reservadas */

typedef struct Nodo {
    int valor;
    struct Nodo *siguiente;
} Nodo;

int factorial(int n) {
    if (n <= 0) return 1;
    return n * factorial(n - 1);
}

int main(void) {
    int i;
    float suma = 0.0;
    char letra = 'A';
    const int MAX = 10;
    unsigned int contador = 0;

    for (i = 0; i < MAX; i++) {
        if (i % 2 == 0) {
            suma += (float)i;
            contador++;
        } else {
            continue;
        }
    }

    while (contador > 0) {
        switch (contador % 3) {
            case 0:
                break;
            case 1:
                suma -= 1.0;
                break;
            default:
                suma += 0.5;
        }
        contador--;
    }

    int *arr = (int *)malloc(MAX * sizeof(int));
    if (arr == NULL) return 1;

    for (i = 0; i < MAX; i++) {
        arr[i] = factorial(i);
    }

    free(arr);
    return 0;
}
"""


def ejecutar_analisis(codigo, nombre_fuente="<programa>"):
    """Ejecuta el analisis completo e imprime resultados."""
    print("=" * 60)
    print(f"ANALISIS DE PALABRAS RESERVADAS: {nombre_fuente}")
    print("=" * 60)
    print("\n--- Codigo fuente original ---")
    print(codigo)
    print()

    encontradas, codigo_traducido = analizar_y_traducir(codigo)

    if not encontradas:
        print("No se encontraron palabras reservadas de C.")
    else:
        # Resumen sin duplicados
        unicas = {}
        for kw, pos, trad in encontradas:
            if kw not in unicas:
                unicas[kw] = trad

        print(f"Palabras reservadas encontradas ({len(encontradas)} ocurrencias, "
              f"{len(unicas)} unicas):\n")
        print(f"  {'Keyword C':<15} {'Traduccion al espanol'}")
        print(f"  {'-'*14} {'-'*25}")
        for kw, trad in sorted(unicas.items()):
            print(f"  {kw:<15} {trad}")

    print("\n--- Codigo con palabras reservadas traducidas al espanol ---")
    print(codigo_traducido)

    return encontradas, codigo_traducido


# ------------------------------------------------------------------
# Programa principal
# ------------------------------------------------------------------

if __name__ == "__main__":
    # Si se pasa un archivo como argumento, cargarlo dinamicamente
    if len(sys.argv) > 1:
        ruta = sys.argv[1]
        try:
            memoria = cargar_programa(ruta)
            codigo  = memoria_a_texto(memoria)
            nombre  = ruta
        except FileNotFoundError:
            print(f"Error: archivo '{ruta}' no encontrado.")
            sys.exit(1)
    else:
        # Usar el programa de ejemplo embebido
        codigo = PROGRAMA_C_EJEMPLO
        nombre = "programa_ejemplo.c"

    encontradas, traducido = ejecutar_analisis(codigo, nombre)

    # Guardar resultados
    with open("resultado.txt", "w", encoding="utf-8") as f:
        f.write("EJERCICIO 3 - PALABRAS RESERVADAS C -> ESPANOL\n")
        f.write("Lenguajes y Compiladores 2026-II -- Luismer Ortega\n")
        f.write("=" * 60 + "\n\n")

        f.write("TABLA DE PALABRAS RESERVADAS (C -> Espanol)\n")
        f.write("-" * 40 + "\n")
        f.write(f"{'Keyword C':<15} {'Traduccion'}\n")
        f.write(f"{'-'*14} {'-'*20}\n")
        for kw, esp in sorted(PALABRAS_RESERVADAS.items()):
            f.write(f"{kw:<15} {esp}\n")

        f.write("\n\nANALISIS DEL ARCHIVO: " + nombre + "\n")
        f.write("-" * 40 + "\n")

        if encontradas:
            unicas = {}
            for kw, pos, trad in encontradas:
                if kw not in unicas:
                    unicas[kw] = trad
            f.write(f"Ocurrencias totales : {len(encontradas)}\n")
            f.write(f"Keywords unicos     : {len(unicas)}\n\n")
            f.write(f"{'Keyword C':<15} {'Traduccion'}\n")
            f.write(f"{'-'*14} {'-'*20}\n")
            for kw, trad in sorted(unicas.items()):
                f.write(f"{kw:<15} {trad}\n")
        else:
            f.write("No se encontraron palabras reservadas.\n")

        f.write("\n\nCODIGO FUENTE ORIGINAL\n")
        f.write("-" * 40 + "\n")
        f.write(codigo)

        f.write("\n\nCODIGO TRADUCIDO AL ESPANOL\n")
        f.write("-" * 40 + "\n")
        f.write(traducido)

    print("\nResultados guardados en resultado.txt")
