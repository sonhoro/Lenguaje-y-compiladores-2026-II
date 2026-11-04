"""
Ejercicio 2 - Polinomio (x+1)^n usando triangulo de Pascal
Lenguajes y Compiladores 2026-II
Estudiante: Luismer Ortega
Universidad Nacional Experimental de Guayana

Descripcion:
  a) Genera los coeficientes de (x+1)^n usando el triangulo de Pascal
     con memoria dinamica (listas de Python) y muestra el polinomio.
  b) Dado un valor x, calcula f(x) = (x+1)^n paso a paso segun el
     polinomio generado.

Mide el tiempo de generacion para n=100 y compara con C.
"""

import time

# ------------------------------------------------------------------
# a) Generacion de coeficientes con Triangulo de Pascal
# ------------------------------------------------------------------

def generar_pascal(n):
    """
    Genera los coeficientes binomiales de (x+1)^n
    usando el triangulo de Pascal con memoria dinamica (lista de listas).

    Retorna una lista con los n+1 coeficientes C(n,0)..C(n,n).
    """
    if n < 0:
        raise ValueError("n debe ser un entero no negativo")

    # Fila base
    fila_actual = [1]

    for _ in range(n):
        # Crear nueva fila dinamicamente a partir de la anterior
        nueva_fila = [1]
        for j in range(1, len(fila_actual)):
            nueva_fila.append(fila_actual[j-1] + fila_actual[j])
        nueva_fila.append(1)
        fila_actual = nueva_fila

    return fila_actual   # lista con n+1 coeficientes


def mostrar_polinomio(coefs, n):
    """
    Muestra el polinomio (x+1)^n en forma expandida.
    Ejemplo para n=3: x^3 + 3x^2 + 3x + 1
    """
    if n == 0:
        return "1"

    terminos = []
    for k, c in enumerate(coefs):
        grado = n - k
        if c == 0:
            continue
        if grado == 0:
            terminos.append(str(c))
        elif grado == 1:
            if c == 1:
                terminos.append("x")
            else:
                terminos.append(f"{c}x")
        else:
            if c == 1:
                terminos.append(f"x^{grado}")
            else:
                terminos.append(f"{c}x^{grado}")

    return " + ".join(terminos)


# ------------------------------------------------------------------
# b) Evaluacion paso a paso de f(x) = (x+1)^n
# ------------------------------------------------------------------

def evaluar_polinomio(coefs, n, x):
    """
    Evalua f(x) = (x+1)^n paso a paso usando los coeficientes dados.
    Muestra cada termino de la suma y el resultado final.
    """
    print(f"  Evaluando f({x}) = (x+1)^{n} con x = {x}:")
    total = 0
    for k, c in enumerate(coefs):
        grado = n - k
        valor_termino = c * (x ** grado)
        total += valor_termino

        # Mostrar cada paso
        if grado == 0:
            print(f"    Termino {k+1}: {c} * x^0 = {c} * 1 = {valor_termino}")
        elif grado == 1:
            print(f"    Termino {k+1}: {c} * x^1 = {c} * {x} = {valor_termino}")
        else:
            print(f"    Termino {k+1}: {c} * x^{grado} = {c} * {x}^{grado} = {valor_termino}")

    print(f"  f({x}) = {total}")
    print(f"  Verificacion directa: ({x}+1)^{n} = {(x+1)**n}")
    return total


# ------------------------------------------------------------------
# Demostracion con valores de ejemplo
# ------------------------------------------------------------------

def demostrar():
    """Muestra el triangulo de Pascal y polinomios para n=0..7"""
    print("=" * 60)
    print("TRIANGULO DE PASCAL Y POLINOMIOS (x+1)^n")
    print("=" * 60)
    for n in range(8):
        coefs = generar_pascal(n)
        poli  = mostrar_polinomio(coefs, n)
        coef_str = " ".join(str(c) for c in coefs)
        print(f"(n={n}): [{coef_str}]  =>  (x+1)^{n} = {poli}")
    print()


# ------------------------------------------------------------------
# Medicion de tiempo para n=100
# ------------------------------------------------------------------

def medir_tiempo_n100(repeticiones=100):
    """
    Genera los coeficientes de (x+1)^100 'repeticiones' veces
    y mide el tiempo total y promedio.
    """
    inicio = time.perf_counter()
    for _ in range(repeticiones):
        coefs = generar_pascal(100)
    fin = time.perf_counter()
    total   = fin - inicio
    promedio = total / repeticiones
    return coefs, total, promedio


# ------------------------------------------------------------------
# Programa principal
# ------------------------------------------------------------------

if __name__ == "__main__":
    # Triangulo de Pascal y polinomios n=0..7
    demostrar()

    # Evaluacion paso a paso de algunos casos
    print("=" * 60)
    print("EVALUACION PASO A PASO f(x) = (x+1)^n")
    print("=" * 60)
    for n, x in [(3, 2), (4, -1), (5, 3)]:
        coefs = generar_pascal(n)
        print(f"\n  Polinomio (x+1)^{n} = {mostrar_polinomio(coefs, n)}")
        evaluar_polinomio(coefs, n, x)

    # Medicion de tiempo para n=100
    print("\n" + "=" * 60)
    print("MEDICION DE TIEMPO — Generar coeficientes de (x+1)^100")
    print("=" * 60)
    coefs_100, total_seg, promedio_seg = medir_tiempo_n100(100)
    n_coefs = len(coefs_100)
    print(f"n                 : 100")
    print(f"Coeficientes gen. : {n_coefs}")
    print(f"Repeticiones      : 100")
    print(f"Tiempo total      : {total_seg:.6f} segundos")
    print(f"Tiempo promedio   : {promedio_seg:.6f} segundos")
    print(f"Coeficiente C(100,50) = {coefs_100[50]}")

    # Guardar resultados
    with open("resultado.txt", "w", encoding="utf-8") as f:
        f.write("EJERCICIO 2 - POLINOMIO (x+1)^n CON TRIANGULO DE PASCAL\n")
        f.write("Lenguajes y Compiladores 2026-II -- Luismer Ortega\n")
        f.write("=" * 60 + "\n\n")

        f.write("TRIANGULO DE PASCAL Y POLINOMIOS (n=0..7)\n")
        f.write("-" * 40 + "\n")
        for n in range(8):
            coefs = generar_pascal(n)
            poli  = mostrar_polinomio(coefs, n)
            coef_str = " ".join(str(c) for c in coefs)
            f.write(f"(n={n}): [{coef_str}]\n")
            f.write(f"  (x+1)^{n} = {poli}\n\n")

        f.write("\nMEDICION DE TIEMPO (Python)\n")
        f.write("-" * 40 + "\n")
        f.write(f"Lenguaje        : Python 3\n")
        f.write(f"n               : 100\n")
        f.write(f"Repeticiones    : 100\n")
        f.write(f"Tiempo total    : {total_seg:.6f} segundos\n")
        f.write(f"Tiempo promedio : {promedio_seg:.6f} segundos\n")
        f.write(f"C(100,50)       : {coefs_100[50]}\n")

    print("\nResultados guardados en resultado.txt")
