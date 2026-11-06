import os
import time

def generar_fila_pascal(n: int) -> list:
    """
    Genera la fila n del Triangulo de Pascal usando comprension de listas y zip.
    En Python los enteros poseen precision arbitraria nativa (BigInt automatico).
    """
    if n < 0:
        return []

    fila = [1]
    for _ in range(n):
        # Cada fila nueva suma pares adyacentes de la fila anterior
        fila = [1] + [a + b for a, b in zip(fila, fila[1:])] + [1]

    return fila

def mostrar_polinomio(n: int, coefs: list) -> str:
    """Retorna la expresion algebraica formateada de (x+1)^n."""
    terminos = []
    for i in range(n + 1):
        exp = n - i
        c = coefs[i]

        if exp == 0:
            terminos.append(str(c))
        elif exp == 1:
            terminos.append(f"{c}x" if c != 1 else "x")
        else:
            terminos.append(f"{c}x^{exp}" if c != 1 else f"x^{exp}")

    return f"(x+1)^{n} = " + " + ".join(terminos)

def evaluar_paso_a_paso(n: int, coefs: list, x: float):
    """Muestra detalladamente cada termino y la acumulacion progresiva de f(x)."""
    print(f"\n--- EVALUACION PASO A PASO PARA x = {x:.2f} ---")
    print(f"f(x) = (x + 1)^{n} con x = {x:.2f}\n")

    suma_total = 0.0
    for i in range(n + 1):
        exp = n - i
        c = coefs[i]
        potencia = x ** exp
        termino = c * potencia
        suma_total += termino

        print(f"Termino {i + 1} (grado {exp}): {c} * ({x:.2f})^{exp} = "
              f"{c} * {potencia:.2f} = {termino:.2f}  --> Suma parcial = {suma_total:.2f}")

    print(f"\nResultado final f({x:.2f}) = {suma_total:.2f}")
    print(f"Verificacion directa (x+1)^{n} = ({x + 1.0:.2f})^{n} = {(x + 1.0) ** n:.2f}")

def main():
    print("========================================================")
    print("  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)   ")
    print("       LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA    ")
    print("       EJERCICIO II: TRIANGULO DE PASCAL Y POLINOMIOS   ")
    print("                    (LENGUAJE: PYTHON)                  ")
    print("========================================================\n")

    directorio_actual = os.path.dirname(os.path.abspath(__file__))
    ruta_txt = os.path.join(directorio_actual, "tiempos_n100_py.txt")

    # 1. Demostracion de polinomios base (n = 0 a 4) como en la guia
    print("--- GENERACION DE POLINOMIOS ---")
    for n in range(5):
        coefs = generar_fila_pascal(n)
        print(mostrar_polinomio(n, coefs))

    # 2. Calculo paso a paso para n = 3 y x = 2.0
    n_demo = 3
    x_demo = 2.0
    coefs_demo = generar_fila_pascal(n_demo)
    evaluar_paso_a_paso(n_demo, coefs_demo, x_demo)

    # 3. Benchmark requerido: tiempo para n = 100
    print("\n--- BENCHMARK REQUERIDO: TIEMPO PARA n = 100 ---")
    n_objetivo = 100

    t_inicio = time.perf_counter()
    coefs_100 = generar_fila_pascal(n_objetivo)
    t_fin = time.perf_counter()

    duracion_ms = (t_fin - t_inicio) * 1000.0

    print(f"Tiempo de generacion para n=100 en Python: {duracion_ms:.5f} ms.")
    print(f"Cantidad de coeficientes generados:       {len(coefs_100)}")
    print(f"Coeficiente inicial c[0]:                  {coefs_100[0]}")
    print(f"Coeficiente central c[50]:                 {coefs_100[50]}")
    print(f"Coeficiente final c[100]:                  {coefs_100[100]}")

    # Guardar reporte en archivo de texto
    with open(ruta_txt, "w", encoding="utf-8") as archivo:
        archivo.write("UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA\n")
        archivo.write("LENGUAJES Y COMPILADORES - EJERCICIO II (PYTHON)\n")
        archivo.write("========================================================\n")
        archivo.write(f"Tiempo de generacion para n=100 en Python: {duracion_ms:.5f} ms\n")
        archivo.write(f"Cantidad de terminos: {len(coefs_100)}\n")
        archivo.write(f"Coeficiente central (c[50]): {coefs_100[50]}\n")

    print(f"\n[OK] Resultados guardados exitosamente en '{os.path.basename(ruta_txt)}'.")

if __name__ == "__main__":
    main()