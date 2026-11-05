"""Ejercicio 2 - Coeficientes de (x+1)^n con el triangulo de Pascal (Python).

Uso (desde esta carpeta): python pascal.py [n] [x]     (por defecto n=5, x=2)
Muestra el polinomio y el calculo paso a paso de f(x) para n y x dados, y mide
el tiempo de generar n=100. Escribe resultado_python.txt.
La lista de Python es memoria dinamica (crece con append); los enteros son de precision ilimitada.
"""
import sys
from math import comb
from time import perf_counter

REPS = 1000  # una generacion de n=100 dura microsegundos: se promedia sobre REPS


def pascal(n):
    """Fila n del triangulo de Pascal, construida fila a fila."""
    fila = [1]
    for _ in range(n):
        fila = [1] + [fila[i] + fila[i + 1] for i in range(len(fila) - 1)] + [1]
    return fila


def polinomio(c):
    n = len(c) - 1
    terminos = []
    for k, ck in enumerate(c):
        p = n - k
        coef = "" if ck == 1 and p > 0 else str(ck)
        var = "" if p == 0 else "x" if p == 1 else f"x^{p}"
        terminos.append(coef + var)
    return " + ".join(terminos)


def pasos(c, x):
    n = len(c) - 1
    lineas = [f"f({x}) = ({x}+1)^{n}"]
    total = 0
    for k, ck in enumerate(c):
        p = n - k
        t = ck * x ** p
        total += t
        lineas.append(f"  termino {k}: {ck} * {x}^{p} = {t}")
    lineas.append(f"  Suma = {total}")
    assert total == (x + 1) ** n
    return lineas


def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 5
    x = int(sys.argv[2]) if len(sys.argv) > 2 else 2
    c = pascal(n)
    out = ["EJERCICIO 2 - Pascal / (x+1)^n (Python)", "", f"Coeficientes n={n}: {c}",
           f"(x+1)^{n} = {polinomio(c)}", ""] + pasos(c, x)

    t0 = perf_counter()
    for _ in range(REPS):
        c100 = pascal(100)
    dt = (perf_counter() - t0) * 1e6
    assert c100[50] == comb(100, 50) and sum(c100) == 2 ** 100
    out += ["", "Generacion n=100:", f"  C(100,50) = {c100[50]}",
            f"  Tiempo total {REPS} generaciones: {dt:.2f} us",
            f"  Tiempo por generacion: {dt / REPS:.2f} us"]

    texto = "\n".join(out) + "\n"
    with open("resultado_python.txt", "w", encoding="utf-8") as f:
        f.write(texto)
    print(texto)


if __name__ == "__main__":
    main()
