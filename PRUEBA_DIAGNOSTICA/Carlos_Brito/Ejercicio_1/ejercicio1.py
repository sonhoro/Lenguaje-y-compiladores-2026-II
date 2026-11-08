# ==============================================================================
# PRUEBA DIAGNÓSTICA — EJERCICIO 1
# Validación de movimientos del caballo en un tablero de ajedrez
# ==============================================================================
# Asignatura : Lenguajes y Compiladores
# Estudiante : Carlos Brito
# Cédula     : 30857129
# Sección    : 3
# Grupo      : Prohibido Rendirse
# Eslogan    : En la optimización habita el mañana
# Lenguaje   : Python 3
# ==============================================================================
# Descripción:
#   Verifica si una cadena de movimientos es válida para un caballo de ajedrez
#   (movimiento en "L": ±1,±2 ó ±2,±1). Se detiene al primer error.
#   Mide el tiempo de verificar 10 veces una secuencia de ~500 movimientos.
# ==============================================================================

import time
import sys
import io

# Forzar UTF-8 en la salida estándar de Windows (evita UnicodeEncodeError)
if sys.stdout.encoding and sys.stdout.encoding.lower() != 'utf-8':
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')

# ==============================================================================
# SECCIÓN 1: CONFIGURACIÓN DEL TABLERO
# Columnas: a=1, b=2, c=3, d=4, e=5, f=6, g=7, h=8
# Filas   : 1 al 8
# ==============================================================================

COLUMNAS = {
    'a': 1, 'b': 2, 'c': 3, 'd': 4,
    'e': 5, 'f': 6, 'g': 7, 'h': 8
}

# ==============================================================================
# SECCIÓN 2: FUNCIONES DE VALIDACIÓN
# ==============================================================================

def parsear_posicion(pos_str):
    """Convierte 'a1' → (col, fila). Retorna None si está fuera del tablero."""
    if len(pos_str) != 2:
        return None
    col_char = pos_str[0].lower()
    if col_char not in COLUMNAS:
        return None
    try:
        fila = int(pos_str[1])
    except ValueError:
        return None
    if not (1 <= fila <= 8):
        return None
    return (COLUMNAS[col_char], fila)


def es_movimiento_valido(origen, destino):
    """Retorna True si el salto origen→destino es legal para un caballo (en L)."""
    dc = abs(destino[0] - origen[0])
    df = abs(destino[1] - origen[1])
    return (dc == 1 and df == 2) or (dc == 2 and df == 1)


def validar_movimientos(ak, cadena_movimientos):
    """Valida la secuencia de movimientos; se detiene en el primero incorrecto."""
    pos_actual = parsear_posicion(ak)
    if pos_actual is None:
        return f"ERROR: Posición inicial '{ak}' inválida."

    movimientos = cadena_movimientos.strip().split()

    for i, mov in enumerate(movimientos, start=1):
        if not mov or mov[0] != 'K':
            prefijo = mov[0] if mov else '?'
            return (f"movimiento INCORRECTO falla letra {prefijo} "
                    f"no pertenece a un caballo")

        pos_dest = parsear_posicion(mov[1:])
        if pos_dest is None:
            return f"INCORRECTO falla en el movimiento {i}"

        if not es_movimiento_valido(pos_actual, pos_dest):
            return f"INCORRECTO falla en el movimiento {i}"

        pos_actual = pos_dest

    return "movimiento CORRECTO"


# ==============================================================================
# SECCIÓN 3: CASOS DE PRUEBA DEL ENUNCIADO
# ==============================================================================

def ejecutar_casos_prueba():
    """Ejecuta y verifica los 9 casos de prueba del enunciado."""
    print("\n" + "=" * 65)
    print("  CASOS DE PRUEBA — EJERCICIO 1")
    print("=" * 65)

    # Formato: (posicion_inicial, n_movimientos, cadena, resultado_esperado)
    casos = [
        ("a1", 4,  "Kc2 Kd4 Kb3 Kc5",
         "movimiento CORRECTO"),

        ("a1", 4,  "Kc2 Kd4 Kb4 Kc5",
         "INCORRECTO falla en el movimiento 3"),

        ("a2", 5,  "Kc3 Ke4 Kf6 Ke8 Kg7",
         "movimiento CORRECTO"),

        ("a2", 5,  "Kc8 Ke4 Kf6 Ke8 Kg7",
         "INCORRECTO falla en el movimiento 1"),

        ("a8", 2,  "Kb6 Kd7",
         "movimiento CORRECTO"),

        ("a8", 2,  "Kb6 Kb7",
         "INCORRECTO falla en el movimiento 2"),

        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4",
         "movimiento CORRECTO"),

        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4",
         "movimiento INCORRECTO falla letra Q no pertenece a un caballo"),

        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4",
         "INCORRECTO falla en el movimiento 9"),
    ]

    todos_ok = True
    for ak, n, cadena, esperado in casos:
        resultado = validar_movimientos(ak, cadena)
        ok   = (resultado == esperado)
        if not ok:
            todos_ok = False
        marca = "OK  " if ok else "FAIL"

        print(f"\n  [{marca}] Ak={ak}, n={n}")
        print(f"      Cadena   : {cadena}")
        print(f"      Esperado : {esperado}")
        print(f"      Obtenido : {resultado}")

    print("\n" + "-" * 65)
    msg = "TODOS LOS CASOS CORRECTOS [OK]" if todos_ok else "ALGUNOS CASOS FALLARON [FAIL]"
    print(f"  {msg}")
    print("-" * 65)


# ==============================================================================
# SECCIÓN 4: SECUENCIA DE MOVIMIENTOS PARA MEDICIÓN DE TIEMPO
# Secuencia de ~500 movimientos válidos. Posición inicial: 'a2'
# ==============================================================================

MOVIMIENTOS_LARGO = (
    "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 "
    "Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
    "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 "
    "Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
    "Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 "
    "Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 "
    "Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 "
    "Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 "
    "Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 "
    "Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 "
    "Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 "
    "Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 "
    "Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
    "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
    "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
    "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 "
    "Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
    "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 "
    "Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 "
    "Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 "
    "Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 "
    "Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4"
)

POSICION_INICIAL = "a2"   # Desde a2, el primer movimiento Kb4 es válido


# ==============================================================================
# SECCIÓN 5: MEDICIÓN DE TIEMPO (10 iteraciones)
# ==============================================================================

def medir_tiempo():
    """Mide el tiempo de verificar la secuencia larga 10 veces consecutivas."""
    print("\n" + "=" * 65)
    print("  MEDICIÓN DE TIEMPO — 10 ITERACIONES × SECUENCIA DE MOVIMIENTOS")
    print("=" * 65)

    n_iter = 10
    n_movs = len(MOVIMIENTOS_LARGO.split())

    inicio = time.perf_counter()
    for _ in range(n_iter):
        validar_movimientos(POSICION_INICIAL, MOVIMIENTOS_LARGO)
    fin = time.perf_counter()

    total    = fin - inicio
    promedio = total / n_iter

    print(f"\n  Posición inicial      : {POSICION_INICIAL}")
    print(f"  Movimientos en cadena : {n_movs}")
    print(f"  Iteraciones           : {n_iter}")
    print(f"  Tiempo total          : {total:.8f} s")
    print(f"  Tiempo promedio/iter. : {promedio:.8f} s")
    print("=" * 65)

    return total, promedio, n_movs


# ==============================================================================
# SECCIÓN 6: ESCRITURA DE RESULTADOS EN ARCHIVO TXT
# ==============================================================================

def escribir_resultados(total, promedio, n_movs):
    """Guarda el resumen de tiempos en 'resultado_ejercicio1.txt'."""
    nombre_archivo = "resultado_ejercicio1.txt"
    sep = "=" * 65 + "\n"

    with open(nombre_archivo, "w", encoding="utf-8") as f:
        f.write(sep)
        f.write("  PRUEBA DIAGNÓSTICA — EJERCICIO 1\n")
        f.write("  Validación de movimientos del caballo en ajedrez\n")
        f.write(sep)
        f.write("  Estudiante  : Carlos Brito\n")
        f.write("  Cédula      : 30857129\n")
        f.write("  Sección     : 3\n")
        f.write("  Grupo       : Prohibido Rendirse\n")
        f.write("  Eslogan     : En la optimización habita el mañana\n")
        f.write(sep)
        f.write("  Lenguaje    : Python 3\n")
        f.write(f"  Movimientos : {n_movs}\n")
        f.write("  Iteraciones : 10\n")
        f.write(f"  Tiempo total    : {total:.8f} s\n")
        f.write(f"  Tiempo promedio : {promedio:.8f} s\n")
        f.write(sep)

    print(f"\n  Resultados guardados en '{nombre_archivo}'")


# ==============================================================================
# SECCIÓN 7: PUNTO DE ENTRADA PRINCIPAL
# ==============================================================================

if __name__ == "__main__":
    print("\n" + "=" * 65)
    print("  PRUEBA DIAGNÓSTICA — EJERCICIO 1")
    print("  Lenguajes y Compiladores")
    print("=" * 65)
    print("  Estudiante : Carlos Brito")
    print("  Cédula     : 30857129")
    print("  Sección    : 3")
    print("  Grupo      : Prohibido Rendirse")
    print("  Eslogan    : En la optimización habita el mañana")

    # Paso 1: Ejecutar todos los casos de prueba del enunciado
    ejecutar_casos_prueba()

    # Paso 2: Medir el tiempo de verificación de la secuencia larga
    total, promedio, n_movs = medir_tiempo()

    # Paso 3: Escribir resultados en archivo .txt
    escribir_resultados(total, promedio, n_movs)

    print("\n  Programa finalizado correctamente.\n")
