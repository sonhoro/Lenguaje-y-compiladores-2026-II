# Prueba diagnóstica – Lenguajes y Compiladores 2026-II

**Estudiante:** Jesús Rodríguez · UNEG · Prof. Msc. Félix Márquez

**Video de defensa (Drive):** _PENDIENTE: pegar aquí el enlace_

Cada ejercicio está codificado en **Python** y en **C**, y cada programa mide su tiempo y lo escribe en un `.txt`
(`resultado_python.txt`, `resultado_c.txt`; el script los une en `resultado.txt`).

## Cómo ejecutar

Requisitos: Python 3 y gcc (MinGW-w64). Doble clic en [`instalar_y_ejecutar.bat`](instalar_y_ejecutar.bat): verifica/instala
ambos con `winget`, compila los tres `.c` y ejecuta todo. También se puede hacer a mano dentro de cada carpeta:

```
python caballo.py            gcc -O2 -o caballo caballo.c            caballo
python pascal.py [n] [x]     gcc -O2 -o pascal pascal.c              pascal [n] [x]
python palabras_reservadas.py [archivo.c]   gcc -O2 -o palabras_reservadas palabras_reservadas.c   palabras_reservadas [archivo.c]
```

## Ejercicio 1 – Caballo de ajedrez (`ejercicio1/`)

Se parte de la posición `Ak` y se recorre la lista de movimientos; **se detiene en el primero incorrecto**. Un movimiento
`Kcf` es correcto si: empieza por `K` (si no: *"falla letra X no pertenece a un caballo"*), `c` está en a–h, `f` en 1–8, y
el salto desde la casilla actual es de `(1,2)` o `(2,1)` casillas en valor absoluto. Los 9 ejemplos del enunciado se
comprueban automáticamente (`assert` en Python, `strcmp` en C). En C la lista se carga desde `movimientos.txt` con
`malloc` y se parte en tokens con un arreglo que crece con `realloc`.

Notas: la lista del PDF dice tener 500 movimientos pero **trae 224**; se midió con esos 224 (10 repeticiones) y además
n=100 (100 movimientos × 100 repeticiones). Como el PDF no indica la casilla inicial, se usó **a2**, desde la cual la lista es válida.

## Ejercicio 2 – (x+1)ⁿ con el triángulo de Pascal (`ejercicio2/`)

La fila *n* del triángulo son los coeficientes de (x+1)ⁿ. En C se usa **un único arreglo dinámico** (`calloc`) de n+1
elementos que se actualiza en sitio de derecha a izquierda (`c[j] += c[j-1]`), así cada fila nueva no necesita memoria
extra; se libera con `free`. Para n=100 los coeficientes llegan a ~10²⁹ y no caben en 64 bits, por eso C usa `__int128`
(límite n ≤ 120) y Python enteros de precisión ilimitada. Se muestra el polinomio y el cálculo paso a paso de f(x):
cada término `c_k · x^(n-k)` y su suma. En C, si el resultado desborda 128 bits se avisa (`__builtin_*_overflow`) en vez
de dar un valor falso; Python no tiene ese límite. Por defecto n=5, x=2; se pueden pasar como argumentos.

## Ejercicio 3 – Palabras reservadas de C (`ejercicio3/`)

El programa C a analizar (`ejemplo.c`, o el archivo que se pase por argumento) se **carga completo en memoria dinámica**
(`malloc` + `fread`). Un recorrido único ignora comentarios (`//`, `/* */`), cadenas y caracteres, y reconoce
identificadores: si coinciden con una de las 44 palabras reservadas (32 de C89 + `inline`, `restrict` y las `_Bool`,
`_Complex`, `_Atomic`… de C99/C11) se cuenta, se anota la línea y se sustituye por su traducción. La salida es una
tabla `palabra | español | veces | líneas` y el código traducido. En Python se hace con una expresión regular.

## Resultados de una corrida (µs; varían de una ejecución a otra)

| Medición | Python | C |
|---|---|---|
| Ej. 1: 10 × 224 movimientos | ~1130 | ~9 |
| Ej. 1: n=100 (100 mov. × 100 rep.) | ~5250 | ~43 |
| Ej. 2: generar n=100 (por generación) | ~450 | ~2.8 |
| Ej. 3: 100 análisis del archivo | ~6370 | ~1100 |

C resulta de decenas a cientos de veces más rápido porque está compilado a código máquina y no paga el costo del
intérprete ni de los objetos de Python.

## Uso de IA

Se usó IA como apoyo para codificar; el código fue revisado y probado (ejemplos del enunciado, comparación de la salida
de C contra la de Python, casos de desbordamiento y x negativo) y el autor debe poder defender cada función.
