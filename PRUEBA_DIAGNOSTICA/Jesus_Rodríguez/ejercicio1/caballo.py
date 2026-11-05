"""Ejercicio 1 - Verificar movimientos de un caballo de ajedrez (Python).

Uso (desde esta carpeta): python caballo.py
Escribe resultado_python.txt. Los ejemplos del enunciado se comprueban con assert.
"""
from time import perf_counter

# (posicion inicial Ak, cadena C, salida esperada segun el enunciado)
EJEMPLOS = [
    ("a1", "Kc2 Kd4 Kb3 Kc5", "movimiento CORRECTO"),
    ("a1", "Kc2 Kd4 Kb4 Kc5", "INCORRECTO falla en el movimiento 3"),
    ("a2", "Kc3 Ke4 Kf6 Ke8 Kg7", "movimiento CORRECTO"),
    ("a2", "Kc8 Ke4 Kf6 Ke8 Kg7", "INCORRECTO falla en el movimiento 1"),
    ("a8", "Kb6 Kd7", "movimiento CORRECTO"),
    ("a8", "Kb6 Kb7", "INCORRECTO falla en el movimiento 2"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4", "movimiento CORRECTO"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4",
     "INCORRECTO falla letra Q no pertenece a un caballo"),
    ("a1", "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4", "INCORRECTO falla en el movimiento 9"),
]


def verificar(ak, movs):
    """Recorre los movimientos y se detiene en el primer incorrecto."""
    col, fil = ord(ak[0]) - 97, int(ak[1]) - 1
    for i, m in enumerate(movs, 1):
        if m[0] != "K":
            return f"INCORRECTO falla letra {m[0]} no pertenece a un caballo"
        if len(m) != 3 or m[1] not in "abcdefgh" or m[2] not in "12345678":
            return f"INCORRECTO falla en el movimiento {i}"
        c, f = ord(m[1]) - 97, int(m[2]) - 1
        # un salto de caballo es (1,2) o (2,1) en valor absoluto
        if sorted((abs(c - col), abs(f - fil))) != [1, 2]:
            return f"INCORRECTO falla en el movimiento {i}"
        col, fil = c, f
    return "movimiento CORRECTO"


def medir(ak, movs, reps):
    t0 = perf_counter()
    for _ in range(reps):
        r = verificar(ak, movs)
    return (perf_counter() - t0) * 1e6, r  # microsegundos


def main():
    out = ["EJERCICIO 1 - Caballo de ajedrez (Python)", "", "Ejemplos del enunciado:"]
    for ak, c, esperado in EJEMPLOS:
        r = verificar(ak, c.split())
        assert r == esperado, (ak, c, r)
        out.append(f"  Ak={ak} n={len(c.split())} C={c} -> {r}")

    with open("movimientos.txt") as f:
        movs = f.read().split()
    out += ["", f"Lista de prueba: {len(movs)} movimientos, posicion inicial a2",
            f"  Resultado: {verificar('a2', movs)}"]
    t, _ = medir("a2", movs, 10)
    out.append(f"  Tiempo 10 repeticiones x {len(movs)} movimientos: {t:.2f} us")
    t, _ = medir("a2", movs[:100], 100)
    out.append(f"  Tiempo n=100 (100 movimientos x 100 repeticiones): {t:.2f} us")

    texto = "\n".join(out) + "\n"
    with open("resultado_python.txt", "w", encoding="utf-8") as f:
        f.write(texto)
    print(texto)


if __name__ == "__main__":
    main()
