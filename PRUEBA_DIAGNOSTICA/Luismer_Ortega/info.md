# Estado de implementación — Prueba Diagnóstica Lenguajes y Compiladores 2026-II

## Descripción

La prueba requiere tres ejercicios, con soluciones en Python y C. Este archivo registra lo implementado y lo pendiente según el enunciado local `prueba diagnostica.md`.

---

## Estructura de carpetas propuesta

```
PRUEBA_DIAGNOSTICA/Luismer_Ortega/
├── README.md                  ← enlace pendiente al video de defensa
├── info.md                    ← estado y verificación
├── prueba diagnostica.md      ← enunciado
├── ejercicio1/
│   ├── caballo.py / caballo.c
│   ├── instalar_y_compilar.bat
│   └── resultado*.txt         ← resultados separados y combinados
├── ejercicio2/
│   ├── pascal.py / pascal.c
│   ├── instalar_y_compilar.bat
│   └── resultado*.txt
└── ejercicio3/
    ├── palabras_reservadas.py / palabras_reservadas.c
    ├── instalar_y_compilar.bat
    └── resultado*.txt
```

---

## Ejercicio 1 — Validador de movimientos de caballo de ajedrez

### Descripción del problema

- Tablero 8×8: filas `[1..8]`, columnas `[a..h]`.
- Entrada: posición inicial `Ak` y cadena `C` de `n` movimientos en notación `K<col><fila>`.
- Validar si cada movimiento desde la posición actual es un movimiento de caballo legal.
- Detenerse al primer movimiento incorrecto.
- Medir el tiempo de verificar **10 veces** la cadena de movimientos del enunciado. El archivo proporcionado dice "500 movimientos", pero la cadena completa de sus páginas 1 y 2 tiene **224 tokens**.

### Lógica principal

Un caballo en `(col, fila)` puede saltar a posiciones que difieran exactamente en `(±1, ±2)` o `(±2, ±1)` en el tablero.

### Casos de prueba a verificar

| Ak   | n  | C (resumen)                | Salida esperada                                   |
|------|----|----------------------------|---------------------------------------------------|
| a1   | 4  | Kc2 Kd4 Kb3 Kc5            | movimiento CORRECTO                               |
| a1   | 4  | Kc2 Kd4 Kb4 Kc5            | INCORRECTO falla en el movimiento 3               |
| a2   | 5  | Kc3 Ke4 Kf6 Ke8 Kg7        | movimiento CORRECTO                               |
| a2   | 5  | Kc8 Ke4 Kf6 Ke8 Kg7        | INCORRECTO falla en el movimiento 1               |
| a8   | 2  | Kb6 Kd7                   | movimiento CORRECTO                               |
| a8   | 2  | Kb6 Kb7                   | INCORRECTO falla en el movimiento 2               |
| a1   | 10 | Kc2…Kb4                   | movimiento CORRECTO                               |
| a1   | 10 | Kc2…Qb4                   | INCORRECTO falla letra Q no pertenece a un caballo|
| a1   | 10 | Kc2…Kh6…Kb4               | INCORRECTO falla en el movimiento 9               |

### Cadena de movimientos del ejercicio 1

El enunciado la describe como una cadena de 500 movimientos, pero el texto proporcionado contiene 224 tokens (46 en la primera página y 178 en la segunda). Ambos programas verifican esa cadena exacta 10 veces; no se duplican ni inventan movimientos para completar 500. Como el enunciado no especifica `Ak` para esta medición, se usa `a2`: con ese inicio la secuencia completa es legal; `a1` la rechaza en el primer salto.

---

## Ejercicio 2 — Polinomio (x+1)^n con triángulo de Pascal

### Descripción del problema

- Dado `n` entero no negativo:
  - **a)** Generar los coeficientes de `(x+1)^n` usando el triángulo de Pascal y mostrar el polinomio.
  - **b)** Dado un valor `x`, calcular `f(x) = (x+1)^n` paso a paso según el polinomio generado.
- Usar **memoria dinámica** (Python: listas dinámicas; C: `malloc`/`free`).
- Medir tiempo para `n=100` en Python vs C.

### Lógica

```
Pascal[0] = [1]
Pascal[n][i] = Pascal[n-1][i-1] + Pascal[n-1][i]
```

Resultado: coeficientes `C(n,k)` para `k=0..n`.

---

## Ejercicio 3 — Detector y traductor de palabras reservadas de C

### Descripción del problema

- Leer un programa C cargado **dinámicamente** en memoria.
- Verificar si existen palabras reservadas de C.
- Traducirlas a su **equivalente en español**.

### Palabras reservadas de C → Español (tabla completa)

| C keyword    | Español          |
|--------------|------------------|
| auto         | automático       |
| break        | romper           |
| case         | caso             |
| char         | carácter         |
| const        | constante        |
| continue     | continuar        |
| default      | predeterminado   |
| do           | hacer            |
| double       | doble            |
| else         | sino             |
| enum         | enumeración      |
| extern       | externo          |
| float        | flotante         |
| for          | para             |
| goto         | ir_a             |
| if           | si               |
| int          | entero           |
| long         | largo            |
| register     | registro         |
| return       | retornar         |
| short        | corto            |
| signed       | con_signo        |
| sizeof       | tamaño_de        |
| static       | estático         |
| struct       | estructura       |
| switch       | interruptor      |
| typedef      | tipo_def         |
| union        | unión            |
| unsigned     | sin_signo        |
| void         | vacío            |
| volatile     | volátil          |
| while        | mientras         |

### Lógica

- Cargar el archivo C en memoria dinámica.
- Tokenizar palabra por palabra.
- Comparar cada token con la lista de palabras reservadas.
- Reemplazar y mostrar el código traducido.

---

## Mediciones y archivos de resultados

- **Ejercicio 1:** mide 10 validaciones de la cadena completa del enunciado (224 movimientos).
- **Ejercicio 2:** mide 100 generaciones de la fila `n=100` en Python y C.
- **Ejercicio 3:** el enunciado no solicita medición de tiempo; guarda el análisis y la traducción en el archivo de resultados.
- Los scripts ejecutan Python y C y combinan las salidas en `resultado.txt`; cada programa C también conserva su salida en `resultado_c.txt`.

El formato esperado para los tiempos es:

```
Lenguaje: Python
Tiempo total (100 iter): X.XXXXXX segundos
Tiempo promedio: X.XXXXXX segundos

Lenguaje: C
Tiempo total (100 iter): X.XXXXXX segundos
Tiempo promedio: X.XXXXXX segundos
```

---

## Estado y pendientes

- Los seis programas (tres ejercicios en Python y C) están implementados. Python usa solo la biblioteca estándar; C se compila con MinGW GCC 6.3.0 en `C:\MinGW`.
- Los scripts agregan `C:\MinGW\bin` al `PATH` del proceso para que GCC encuentre sus DLL de ejecución.
- Ejercicio 1 usa los 224 movimientos impresos en el enunciado y `a2` como posición inicial de la medición, porque el enunciado no especifica `Ak` para esa cadena y desde `a2` todos los movimientos son legales.
- Ejercicio 2 usa enteros de precisión dinámica en C para obtener coeficientes exactos hasta `n=100`.
- Ejercicio 3 mantiene sin cambios comentarios, literales y partes de identificadores durante la traducción; las tablas Python y C traducen `union` igual.
- **Pendiente:** grabar y enlazar el video de defensa. Debe durar máximo 5 minutos, ser horizontal, mostrar y escuchar claramente al estudiante, incluir nombre del grupo, eslogan, nombre y apellido, sección, explicación y demostración práctica.

---

## Verificación realizada / restante

- [x] Ejecutar y verificar los casos de prueba del Ejercicio 1 y los 224 movimientos desde `a2`.
- [x] Verificar los coeficientes de Pascal para `n=0,1,2,3,4,5,7,100`.
- [x] Verificar que el traductor respeta comentarios, literales y límites de identificadores.
- [x] Compilar y ejecutar los programas C con MinGW GCC 6.3.0.
- [x] Generar resultados Python y C para los tres ejercicios.
- [x] Probar los tres scripts `.bat` de principio a fin, incluyendo la compilación, ejecución y combinación de resultados.
- [ ] Grabar y enlazar el video de defensa.
