"""
Ejercicio 1 - Validador de movimientos de caballo de ajedrez
Lenguajes y Compiladores 2026-II
Estudiante: Luismer Ortega
Universidad Nacional Experimental de Guayana

Descripcion:
  Dado un tablero 8x8 con columnas [a..h] y filas [1..8],
  verifica si una cadena de movimientos de caballo es valida.
  El programa se detiene al primer movimiento incorrecto.

Notacion: K<col><fila> (ej: Kc2)
"""

import random
import time

# ------------------------------------------------------------------
# Funciones principales
# ------------------------------------------------------------------

def pos_a_coords(pos):
    """
    Convierte una posicion tipo 'a1' a coordenadas (col, fila)
    donde col y fila son enteros de 0..7.
    Retorna None si la posicion es invalida.
    """
    if len(pos) != 2:
        return None
    col_char = pos[0]
    fila_char = pos[1]
    if col_char not in 'abcdefgh':
        return None
    if fila_char not in '12345678':
        return None
    col = ord(col_char) - ord('a')   # 0..7
    fila = int(fila_char) - 1        # 0..7
    return (col, fila)


def es_movimiento_caballo(origen, destino):
    """
    Verifica si el salto de 'origen' a 'destino' es un movimiento
    legal de caballo. Ambos son tuplas (col, fila) con valores 0..7.
    Un caballo se mueve en L: diferencia de (+-1,+-2) o (+-2,+-1).
    """
    dc = abs(destino[0] - origen[0])
    df = abs(destino[1] - origen[1])
    return (dc == 1 and df == 2) or (dc == 2 and df == 1)


def parsear_movimiento(token):
    """
    Analiza un token de movimiento.
    Formato valido: K<col><fila> donde col in [a-h] y fila in [1-8].
    Retorna:
      - 'destino' (tupla coords) si es valido
      - 'letra_invalida' si la letra inicial no es 'K'
      - 'formato_invalido' si el formato es incorrecto
      - 'fuera_tablero' si las coordenadas estan fuera del tablero
    """
    if len(token) != 3:
        return 'formato_invalido', None
    prefijo = token[0]
    if prefijo != 'K':
        return 'letra_invalida', prefijo
    coords = pos_a_coords(token[1:])
    if coords is None:
        return 'fuera_tablero', None
    return 'ok', coords


def validar_movimientos(posicion_inicial, movimientos):
    """
    Valida la secuencia de movimientos del caballo.
    - posicion_inicial: string tipo 'a1'
    - movimientos: lista de tokens tipo ['Kc2', 'Kd4', ...]

    Retorna una tupla (correcto, mensaje) donde:
      - correcto: True/False
      - mensaje: descripcion del resultado
    """
    coords_actual = pos_a_coords(posicion_inicial)
    if coords_actual is None:
        return False, f"Posicion inicial '{posicion_inicial}' invalida."

    for i, token in enumerate(movimientos, start=1):
        estado, info = parsear_movimiento(token)

        if estado == 'letra_invalida':
            return False, (f"movimiento INCORRECTO falla letra {info} "
                           f"no pertenece a un caballo")

        if estado == 'formato_invalido' or estado == 'fuera_tablero':
            return False, f"INCORRECTO falla en el movimiento {i}"

        # estado == 'ok', info es la tupla de coordenadas destino
        destino = info
        if not es_movimiento_caballo(coords_actual, destino):
            return False, f"INCORRECTO falla en el movimiento {i}"

        coords_actual = destino

    return True, "movimiento CORRECTO"


# ------------------------------------------------------------------
# Casos de prueba del enunciado
# ------------------------------------------------------------------

CASOS_PRUEBA = [
    ("a1", "Kc2 Kd4 Kb3 Kc5",                              "movimiento CORRECTO"),
    ("a1", "Kc2 Kd4 Kb4 Kc5",                              "INCORRECTO falla en el movimiento 3"),
    ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7",                          "movimiento CORRECTO"),
    ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7",                          "INCORRECTO falla en el movimiento 1"),
    ("a8", "Kb6 Kd7",                                       "movimiento CORRECTO"),
    ("a8", "Kb6 Kb7",                                       "INCORRECTO falla en el movimiento 2"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4",    "movimiento CORRECTO"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4",    "movimiento INCORRECTO falla letra Q no pertenece a un caballo"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4",    "INCORRECTO falla en el movimiento 9"),
]

# ------------------------------------------------------------------
# Generacion propia de 500 movimientos, autorizada por el profesor
# ------------------------------------------------------------------

POSICION_INICIAL_500 = "a2"


def generar_movimientos(posicion_inicial, cantidad, semilla=2026):
    """Genera saltos legales reproducibles; permite volver a visitar casillas."""
    actual = pos_a_coords(posicion_inicial)
    if actual is None:
        raise ValueError("La posicion inicial debe estar dentro del tablero")
    if cantidad < 0:
        raise ValueError("La cantidad de movimientos no puede ser negativa")

    saltos = ((1, 2), (2, 1), (-1, 2), (-2, 1),
              (1, -2), (2, -1), (-1, -2), (-2, -1))
    aleatorio = random.Random(semilla)
    movimientos = []
    for _ in range(cantidad):
        destinos = [(actual[0] + dc, actual[1] + df)
                    for dc, df in saltos
                    if 0 <= actual[0] + dc < 8 and 0 <= actual[1] + df < 8]
        actual = aleatorio.choice(destinos)
        movimientos.append(f"K{chr(ord('a') + actual[0])}{actual[1] + 1}")
    return movimientos


MOVIMIENTOS_500 = generar_movimientos(POSICION_INICIAL_500, 500)

for numero in (1, 250, 500):
    cadena_fallida = MOVIMIENTOS_500.copy()
    anterior = POSICION_INICIAL_500 if numero == 1 else MOVIMIENTOS_500[numero - 2][1:]
    cadena_fallida[numero - 1] = "K" + anterior
    CASOS_PRUEBA.append((POSICION_INICIAL_500, " ".join(cadena_fallida),
                         f"INCORRECTO falla en el movimiento {numero}"))


def ejecutar_pruebas():
    """Ejecuta todos los casos de prueba del enunciado."""
    print("=" * 60)
    print("CASOS DE PRUEBA DEL ENUNCIADO")
    print("=" * 60)
    todos_ok = True
    for ak, cadena, esperado in CASOS_PRUEBA:
        movimientos = cadena.split()
        correcto, mensaje = validar_movimientos(ak, movimientos)
        # Normalizar comparacion
        paso = mensaje.lower().strip() == esperado.lower().strip()
        estado = "✓ OK" if paso else "✗ FALLA"
        if not paso:
            todos_ok = False
        print(f"[{estado}] Ak={ak} n={len(movimientos)}")
        print(f"       Cadena : {cadena}")
        print(f"       Obtenido : {mensaje}")
        print(f"       Esperado : {esperado}")
        print()
    return todos_ok


def medir_tiempo_500(repeticiones=10):
    """Mide solo la validacion de los 500 movimientos, no su generacion."""
    if repeticiones <= 0:
        raise ValueError("Las repeticiones deben ser positivas")
    if len(MOVIMIENTOS_500) != 500:
        raise ValueError("La cadena de medicion debe contener exactamente 500 movimientos")
    if not validar_movimientos(POSICION_INICIAL_500, MOVIMIENTOS_500)[0]:
        raise ValueError("La cadena de medicion contiene un movimiento incorrecto")
    inicio = time.perf_counter()
    for _ in range(repeticiones):
        validar_movimientos(POSICION_INICIAL_500, MOVIMIENTOS_500)
    fin = time.perf_counter()
    total = fin - inicio
    promedio = total / repeticiones
    return total, promedio


# ------------------------------------------------------------------
# Programa principal
# ------------------------------------------------------------------

if __name__ == "__main__":
    # 1) Ejecutar casos de prueba
    todos_correctos = ejecutar_pruebas()
    if not todos_correctos:
        raise RuntimeError("Fallaron los casos de prueba del caballo")

    # 2) Medir tiempo con los 500 movimientos generados x 10 repeticiones
    print("=" * 60)
    print("MEDICION DE TIEMPO — 500 movimientos x 10 repeticiones")
    print("=" * 60)
    total_seg, promedio_seg = medir_tiempo_500(10)
    print(f"Movimientos en cadena : {len(MOVIMIENTOS_500)}")
    print(f"Repeticiones          : 10")
    print(f"Tiempo total          : {total_seg:.6f} segundos")
    print(f"Tiempo promedio       : {promedio_seg:.6f} segundos")
    print()

    # 3) Guardar resultados en archivo
    with open("resultado.txt", "w", encoding="utf-8") as f:
        f.write("EJERCICIO 1 - VALIDADOR DE MOVIMIENTOS DE CABALLO\n")
        f.write("Lenguajes y Compiladores 2026-II — Luismer Ortega\n")
        f.write("=" * 60 + "\n\n")

        f.write("CASOS DE PRUEBA\n")
        f.write("-" * 40 + "\n")
        for ak, cadena, esperado in CASOS_PRUEBA:
            movimientos = cadena.split()
            correcto, mensaje = validar_movimientos(ak, movimientos)
            f.write(f"Ak={ak}, n={len(movimientos)}\n")
            f.write(f"  Cadena   : {cadena}\n")
            f.write(f"  Resultado: {mensaje}\n\n")

        f.write("\nMEDICION DE TIEMPO (Python)\n")
        f.write("-" * 40 + "\n")
        f.write(f"Lenguaje        : Python 3\n")
        f.write(f"Posicion inicial: {POSICION_INICIAL_500}\n")
        f.write("Origen          : generacion propia autorizada; semilla 2026\n")
        f.write(f"Movimientos     : {len(MOVIMIENTOS_500)}\n")
        f.write(f"Repeticiones    : 10\n")
        f.write(f"Tiempo total    : {total_seg:.6f} segundos\n")
        f.write(f"Tiempo promedio : {promedio_seg:.6f} segundos\n")
        f.write("\nCADENA VALIDA UTILIZADA EN LA MEDICION\n")
        f.write(" ".join(MOVIMIENTOS_500) + "\n")

    print("Resultados guardados en resultado.txt")
