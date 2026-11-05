import os
import re
import time

# Conjunto inmutable con los 8 desplazamientos posibles de un caballo en el plano
SALTOS_VALIDOS_CABALLO = {
    (1, 2), (1, -2), (-1, 2), (-1, -2),
    (2, 1), (2, -1), (-2, 1), (-2, -1)
}

# Expresión regular precompilada para extraer tokens ignorando espacios (Lexer)
PATRON_TOKEN = re.compile(r'\S+')

def parsear_coordenada(coord: str):
    """Convierte 'a1'-'h8' a indices (0..7, 0..7)."""
    if len(coord) != 2:
        return None
    c, f = coord[0], coord[1]
    if not ('a' <= c <= 'h' and '1' <= f <= '8'):
        return None
    return ord(c) - 97, ord(f) - 49  # 97 = 'a', 49 = '1'

def verificar_movimientos(ak: str, n: int, cadena: str) -> str:
    """
    Analizador lexico y semantico de la secuencia de movimientos.
    Usa escaneo perezoso (Lazy evaluation) para O(1) de memoria adicional.
    """
    pos_actual = parsear_coordenada(ak)
    if pos_actual is None:
        return "Posicion inicial invalida."

    mov_contador = 0

    # Iterador perezoso de tokens: no fragmenta la cadena completa en memoria
    for match in PATRON_TOKEN.finditer(cadena):
        if mov_contador >= n:
            break

        token = match.group(0)
        mov_contador += 1

        # 1. Regla lexica: La pieza debe ser 'K'
        if not token.startswith('K'):
            return f"movimiento INCORRECTO falla letra {token[0]} no pertenece a un caballo."

        # 2. Regla sintactica: Casilla destino
        if len(token) < 3:
            return f"INCORRECTO falla en el movimiento {mov_contador} (longitud invalida)."

        pos_destino = parsear_coordenada(token[1:3])
        if pos_destino is None:
            return f"INCORRECTO falla en el movimiento {mov_contador} (fuera del tablero)."

        # 3. Regla semantica del ajedrez: Salto en 'L' mediante tabla hash O(1)
        delta = (pos_destino[0] - pos_actual[0], pos_destino[1] - pos_actual[1])
        if delta not in SALTOS_VALIDOS_CABALLO:
            return f"INCORRECTO falla en el movimiento {mov_contador}."

        pos_actual = pos_destino

    return "movimiento CORRECTO."

def main():
    print("========================================================")
    print("  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)   ")
    print("       LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA    ")
    print("       EJERCICIO I: VALIDACION DE CABALLO EN AJEDREZ    ")
    print("                    (LENGUAJE: PYTHON)                  ")
    print("========================================================\n")

    casos_prueba = [
        ("a1", 4, "Kc2 Kd4 Kb3 Kc5"),
        ("a1", 4, "Kc2 Kd4 Kb4 Kc5"),
        ("a2", 5, "Kc3 Ke4 Kf6 Ke8 Kg7"),
        ("a2", 5, "Kc8 Ke4 Kf6 Ke8 Kg7"),
        ("a8", 2, "Kb6 Kd7"),
        ("a8", 2, "Kb6 Kb7"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"),
        ("a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"),
    ]

    directorio_actual = os.path.dirname(os.path.abspath(__file__))
    ruta_txt = os.path.join(directorio_actual, "resultados_de_python.txt")

    lineas_reporte = [
        "UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA\n",
        "LENGUAJES Y COMPILADORES - EJERCICIO I (PYTHON)\n",
        "========================================================\n\n"
    ]

    print("--- CASOS DE PRUEBA OFICIALES ---")
    for idx, (ak, n, cadena) in enumerate(casos_prueba, start=1):
        resultado = verificar_movimientos(ak, n, cadena)
        print(f"Caso {idx}: {resultado}")
        lineas_reporte.append(f"Caso {idx} [Ak={ak}, n={n}]: {resultado}\n")

    # Cadena oficial de 500 movimientos
    movs500 = (
        "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 "
        "Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
        "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 "
        "Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
        "Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 "
        "Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 "
        "Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 "
        "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 "
        "Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8"
    )

    print("\n--- MEDICION DE TIEMPO (BENCHMARK) ---")
    print("Ejecutando 10 repeticiones de la secuencia de 500 movimientos...")

    inicio = time.perf_counter()
    for _ in range(10):
        verificar_movimientos("a2", 500, movs500)
    fin = time.perf_counter()

    duracion_ms = (fin - inicio) * 1000.0
    promedio_ms = duracion_ms / 10.0

    print(f"Tiempo total de ejecucion (10 x 500 movs): {duracion_ms:.4f} ms.")
    print(f"Tiempo promedio por verificacion:          {promedio_ms:.4f} ms.")

    lineas_reporte.append("\n--- MEDICION DE TIEMPO (10x500 movimientos) ---\n")
    lineas_reporte.append(f"Tiempo total:    {duracion_ms:.4f} ms.\n")
    lineas_reporte.append(f"Tiempo promedio: {promedio_ms:.4f} ms.\n")

    with open(ruta_txt, "w", encoding="utf-8") as archivo:
        archivo.writelines(lineas_reporte)

    print(f"\n[OK] Resultados guardados exitosamente en '{os.path.basename(ruta_txt)}'.")

if __name__ == "__main__":
    main()