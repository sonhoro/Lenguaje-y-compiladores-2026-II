"""Ejercicio 3 - Palabras reservadas de C y su traduccion al espanol (Python).

Uso (desde esta carpeta): python palabras_reservadas.py [archivo.c]   (por defecto ejemplo.c)
Carga el archivo en memoria, ignora comentarios y cadenas, cuenta las palabras reservadas
(C89 + C99/C11), las traduce y mide 100 analisis. Escribe resultado_python.txt.
"""
import re
import sys
from time import perf_counter

RESERVADAS = {
    "auto": "automático", "break": "romper", "case": "caso", "char": "carácter",
    "const": "constante", "continue": "continuar", "default": "por_defecto", "do": "hacer",
    "double": "doble", "else": "sino", "enum": "enumeración", "extern": "externo",
    "float": "flotante", "for": "para", "goto": "ir_a", "if": "si", "int": "entero",
    "long": "largo", "register": "registro", "return": "retornar", "short": "corto",
    "signed": "con_signo", "sizeof": "tamaño_de", "static": "estático", "struct": "estructura",
    "switch": "según", "typedef": "definir_tipo", "union": "unión", "unsigned": "sin_signo",
    "void": "vacío", "volatile": "volátil", "while": "mientras",
    # C99 / C11
    "inline": "en_línea", "restrict": "restringido", "_Bool": "_Booleano",
    "_Complex": "_Complejo", "_Imaginary": "_Imaginario", "_Alignas": "_AlinearComo",
    "_Alignof": "_AlineaciónDe", "_Atomic": "_Atómico", "_Generic": "_Genérico",
    "_Noreturn": "_SinRetorno", "_Static_assert": "_Aserción_Estática",
    "_Thread_local": "_Local_de_Hilo",
}

# comentario // | comentario /* */ | cadena | caracter | identificador
TOKEN = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|[A-Za-z_]\w*', re.S)


def analizar(src):
    """Devuelve (codigo traducido, {palabra: [lineas]})."""
    lineas = {}

    def sustituir(m):
        t = m.group()
        if t in RESERVADAS:
            lineas.setdefault(t, []).append(src.count("\n", 0, m.start()) + 1)
            return RESERVADAS[t]
        return t

    return TOKEN.sub(sustituir, src), lineas


def main():
    archivo = sys.argv[1] if len(sys.argv) > 1 else "ejemplo.c"
    with open(archivo, encoding="utf-8") as f:
        src = f.read()  # el programa C queda cargado en memoria

    traducido, lineas = analizar(src)
    out = ["EJERCICIO 3 - Palabras reservadas de C (Python)", "", f"Archivo: {archivo}", "",
           "palabra | español | veces | líneas"]
    for p in RESERVADAS:  # mismo orden que la tabla de C
        if p in lineas:
            out.append(f"{p} | {RESERVADAS[p]} | {len(lineas[p])} | {','.join(map(str, lineas[p]))}")
    out += ["", f"Total de palabras reservadas distintas: {len(lineas)}",
            f"Total de apariciones: {sum(map(len, lineas.values()))}",
            "", "Código traducido:", traducido]

    t0 = perf_counter()
    for _ in range(100):
        analizar(src)
    dt = (perf_counter() - t0) * 1e6
    out += ["", f"Tiempo n=100 (100 análisis del archivo): {dt:.2f} us"]

    texto = "\n".join(out) + "\n"
    with open("resultado_python.txt", "w", encoding="utf-8") as f:
        f.write(texto)
    sys.stdout.reconfigure(encoding="utf-8")
    print(texto)


if __name__ == "__main__":
    main()
