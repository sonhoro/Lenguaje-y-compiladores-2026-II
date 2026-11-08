# ==============================================================================
# PRUEBA DIAGNÓSTICA — EJERCICIO 2
# Generación del polinomio (x+1)^n usando el Triángulo de Pascal
# con memoria dinámica (listas de Python = arrays dinámicos)
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
#   a) Genera los coeficientes de (x+1)^n con el Triángulo de Pascal
#      usando listas (memoria dinámica asignada en tiempo de ejecución).
#   b) Muestra paso a paso el cálculo de f(x) = (x+1)^n para un x dado.
#   c) Mide el tiempo de generar la fila n=100 (1000 repeticiones).
# ==============================================================================

import time
import sys
import io

# Forzar UTF-8 en la salida estándar de Windows (evita UnicodeEncodeError)
if sys.stdout.encoding and sys.stdout.encoding.lower() != 'utf-8':
    sys.stdout = io.TextIOWrapper(sys.stdout.buffer, encoding='utf-8', errors='replace')

# ==============================================================================
# SECCIÓN 1: GENERACIÓN DEL TRIÁNGULO DE PASCAL CON MEMORIA DINÁMICA
# En Python, una lista es dinámica: su tamaño se determina en tiempo de
# ejecución y se gestiona automáticamente en el heap. Cada fila se aloca
# de forma independiente sin tamaño predefinido en compilación.
# ==============================================================================

def generar_fila_pascal(n):
    """Genera la fila n del triángulo de Pascal (coeficientes de (x+1)^n)."""
    fila = [1]
    for k in range(1, n + 1):
        nueva_fila = [1] * (k + 1)
        for i in range(1, k):
            nueva_fila[i] = fila[i - 1] + fila[i]
        fila = nueva_fila   # La fila anterior es liberada por el GC
    return fila


def generar_triangulo_pascal(n_max):
    """Genera todas las filas del triángulo de Pascal de 0 a n_max."""
    triangulo = []
    fila = [1]
    for k in range(n_max + 1):
        triangulo.append(list(fila))
        siguiente = [1] * (len(fila) + 1)
        for i in range(1, len(fila)):
            siguiente[i] = fila[i - 1] + fila[i]
        fila = siguiente
    return triangulo


# ==============================================================================
# SECCIÓN 2: REPRESENTACIÓN TEXTUAL DEL POLINOMIO
# ==============================================================================

def coefs_a_polinomio(coefs, var='x'):
    """Convierte la lista de coeficientes a cadena textual del polinomio."""
    n = len(coefs) - 1
    if n == 0:
        return str(coefs[0])

    terminos = []
    for i, c in enumerate(coefs):
        grado = n - i
        if c == 0:
            continue
        if grado == 0:
            terminos.append(str(c))
        elif grado == 1:
            terminos.append(var if c == 1 else f"{c}{var}")
        else:
            terminos.append(f"{var}^{grado}" if c == 1
                            else f"{c}{var}^{grado}")

    return " + ".join(terminos) if terminos else "0"


# ==============================================================================
# SECCIÓN 3: CÁLCULO PASO A PASO DE f(x) = (x+1)^n
# ==============================================================================

def calcular_paso_a_paso(coefs, x_val):
    """Evalúa f(x_val) mostrando la contribución de cada término."""
    n     = len(coefs) - 1
    pasos = []
    total = 0

    for i, c in enumerate(coefs):
        grado         = n - i
        x_pow         = x_val ** grado
        valor_termino = c * x_pow
        total        += valor_termino
        paso = (f"    Término {i+1:2d}: "
                f"C({n},{i}) · x^{grado:<2d} = "
                f"{c:>6} · ({x_val})^{grado} = "
                f"{c:>6} · {x_pow:<12} = {valor_termino}")
        pasos.append(paso)

    return pasos, total


# ==============================================================================
# SECCIÓN 4: DEMOSTRACIÓN — TRIÁNGULO Y POLINOMIOS PARA n = 0 .. 7
# ==============================================================================

def demostrar_polinomios():
    """Muestra el triángulo de Pascal y los polinomios para n = 0 a 7."""
    print("\n" + "=" * 65)
    print("  TRIÁNGULO DE PASCAL Y POLINOMIOS (x+1)^n  — n = 0 a 7")
    print("=" * 65)

    triangulo = generar_triangulo_pascal(7)
    for n, fila in enumerate(triangulo):
        coefs_str = "  ".join(str(c).rjust(3) for c in fila)
        poly_str  = coefs_a_polinomio(fila)
        print(f"\n  (n={n}):  Coeficientes → [{coefs_str} ]")
        print(f"          (x+1)^{n} = {poly_str}")

    print("=" * 65)


# ==============================================================================
# SECCIÓN 5: DEMOSTRACIÓN — CÁLCULO PASO A PASO PARA UN EJEMPLO
# ==============================================================================

def demostrar_calculo(n, x_val):
    """Muestra paso a paso el cálculo de f(x_val) = (x_val+1)^n."""
    print("\n" + "=" * 65)
    print(f"  CÁLCULO PASO A PASO: f({x_val}) = ({x_val}+1)^{n}")
    print("=" * 65)

    coefs    = generar_fila_pascal(n)
    poly_str = coefs_a_polinomio(coefs)
    print(f"\n  Polinomio  : (x+1)^{n} = {poly_str}")
    print(f"  Evaluando en x = {x_val}:\n")

    pasos, resultado = calcular_paso_a_paso(coefs, x_val)
    for paso in pasos:
        print(paso)

    print(f"\n  {'─' * 55}")
    print(f"  f({x_val}) = {resultado}")
    print(f"  Verificación directa : ({x_val}+1)^{n} = {(x_val + 1) ** n}")
    print("=" * 65)


# ==============================================================================
# SECCIÓN 6: MEDICIÓN DE TIEMPO PARA n = 100
# ==============================================================================

def medir_tiempo_n100():
    """Mide el tiempo de generar la fila n=100, repetido 1000 veces."""
    print("\n" + "=" * 65)
    print("  MEDICIÓN DE TIEMPO — GENERACIÓN DEL POLINOMIO (x+1)^100")
    print("=" * 65)

    n              = 100
    n_repeticiones = 1000

    inicio = time.perf_counter()
    for _ in range(n_repeticiones):
        coefs = generar_fila_pascal(n)
    fin = time.perf_counter()

    tiempo_total = fin - inicio
    tiempo_una   = tiempo_total / n_repeticiones

    print(f"\n  n                    : {n}")
    print(f"  Repeticiones         : {n_repeticiones}")
    print(f"  Tiempo total         : {tiempo_total:.8f} s")
    print(f"  Tiempo por llamada   : {tiempo_una:.8f} s")
    print(f"\n  Primeros 5 coefs : {coefs[:5]}")
    print(f"  Últimos  5 coefs : {coefs[-5:]}")
    print(f"  Total coeficientes   : {len(coefs)}")
    print("=" * 65)

    return tiempo_total, tiempo_una, coefs


# ==============================================================================
# SECCIÓN 7: ESCRITURA DE RESULTADOS EN ARCHIVO TXT
# ==============================================================================

def escribir_resultados(tiempo_total, tiempo_una):
    """Guarda los tiempos de ejecución en 'resultado_ejercicio2.txt' (modo w)."""
    nombre = "resultado_ejercicio2.txt"
    sep    = "=" * 65 + "\n"

    # Modo "w": crea o reemplaza el archivo; el programa C luego agrega su sección
    with open(nombre, "w", encoding="utf-8") as f:
        f.write(sep)
        f.write("  PRUEBA DIAGNOSTICA - EJERCICIO 2\n")
        f.write("  Generacion de (x+1)^n con el Triangulo de Pascal\n")
        f.write(sep)
        f.write("  Estudiante  : Carlos Brito\n")
        f.write("  Cedula      : 30857129\n")
        f.write("  Seccion     : 3\n")
        f.write("  Grupo       : Prohibido Rendirse\n")
        f.write("  Eslogan     : En la optimizacion habita el manana\n")
        f.write(sep)
        f.write("  --- TIEMPOS DE EJECUCION ---\n")
        f.write(sep)
        f.write("  Lenguaje        : Python 3\n")
        f.write("  n               : 100\n")
        f.write("  Repeticiones    : 1000\n")
        f.write(f"  Tiempo total    : {tiempo_total:.8f} s\n")
        f.write(f"  Tiempo/llamada  : {tiempo_una:.8f} s\n")
        f.write(sep)
        f.write(sep)

    print(f"\n  Resultados guardados en '{nombre}'")


# ==============================================================================
# SECCIÓN 8: PUNTO DE ENTRADA PRINCIPAL
# ==============================================================================

if __name__ == "__main__":
    print("\n" + "=" * 65)
    print("  PRUEBA DIAGNÓSTICA — EJERCICIO 2")
    print("  Lenguajes y Compiladores")
    print("=" * 65)
    print("  Estudiante : Carlos Brito")
    print("  Cédula     : 30857129")
    print("  Sección    : 3")
    print("  Grupo      : Prohibido Rendirse")
    print("  Eslogan    : En la optimización habita el mañana")

    # Paso 1: Mostrar triángulo de Pascal y polinomios para n = 0 a 7
    demostrar_polinomios()

    # Paso 2: Cálculo paso a paso de f(3) = (3+1)^5 como ejemplo
    demostrar_calculo(n=5, x_val=3)

    # Paso 3: Medir tiempo de generación para n = 100
    tiempo_total, tiempo_una, _ = medir_tiempo_n100()

    # Paso 4: Guardar resultados en archivo .txt
    escribir_resultados(tiempo_total, tiempo_una)

    print("\n  Programa finalizado correctamente.\n")
