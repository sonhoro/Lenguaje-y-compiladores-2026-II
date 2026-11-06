# Luismer Ortega — Prueba Diagnóstica
## Lenguajes y Compiladores 2026-II
### Universidad Nacional Experimental de Guayana

---

## 🎥 Video de Defensa

> **[Enlace al video en Google Drive — pendiente de grabación]**
>
> Requisitos: máximo 5 minutos, formato horizontal, estudiante visible y audible. Presentar nombre del grupo, eslogan, nombre y apellido, y sección; explicar y demostrar el funcionamiento de los ejercicios.

---

## 📁 Estructura del repositorio

```
Luismer_Ortega/
├── ejercicio1/         ← Validador de movimientos de caballo de ajedrez
│   ├── caballo.py
│   ├── test_caballo.py
│   ├── caballo.c        ← referencia opcional de la versión anterior
│   └── instalar_y_compilar.bat
├── ejercicio2/         ← Polinomio (x+1)^n con triángulo de Pascal
│   ├── pascal.py
│   ├── pascal.c
│   └── instalar_y_compilar.bat
└── ejercicio3/         ← Palabras reservadas C → Español
    ├── palabras_reservadas.py  ← referencia opcional
    ├── palabras_reservadas.c
    └── instalar_y_compilar.bat
```

---

## ⚙️ Requisitos

| Componente | Versión            |
|------------|--------------------|
| Python     | 3.x (stdlib solo)  |
| GCC / MinGW| 6.3+ en `C:\MinGW` |

Según la aclaración del profesor: ejercicio 1 en un lenguaje libre (se eligió Python), ejercicio 2 en Python y C, ejercicio 3 en C. El ejercicio 1 no requiere GCC y el ejercicio 3 no requiere Python.

La frase inicial que exigía dos lenguajes, `n=100` y archivos TXT para todos los ejercicios fue retirada. Se conservan los TXT como evidencia opcional de ejecución. El grado `n=100` del ejercicio 2 y las 10 verificaciones del ejercicio 1 siguen siendo requisitos específicos.

---

## 🚀 Cómo ejecutar cada ejercicio

### Ejercicio 1 — Caballo de Ajedrez
```bat
cd ejercicio1
instalar_y_compilar.bat
```
O manualmente:
```bat
python caballo.py
python -m unittest -v test_caballo
```

### Ejercicio 2 — Polinomio Pascal
```bat
cd ejercicio2
instalar_y_compilar.bat
```
O manualmente:
```bat
python pascal.py
set PATH=C:\MinGW\bin;%PATH%
C:\MinGW\bin\gcc.exe -o pascal.exe pascal.c -lm
pascal.exe
```

### Ejercicio 3 — Palabras Reservadas
```bat
cd ejercicio3
instalar_y_compilar.bat
```
O manualmente:
```bat
set PATH=C:\MinGW\bin;%PATH%
C:\MinGW\bin\gcc.exe -o palabras_reservadas.exe palabras_reservadas.c
palabras_reservadas.exe [archivo.c]
```
*(Sin argumento usa un programa C de ejemplo embebido)*

El script del ejercicio 3 también acepta la ruta de un archivo: `instalar_y_compilar.bat "C:\ruta\programa.c"`.

Los scripts guardan la evidencia principal en `resultado.txt`: solo Python en el ejercicio 1, Python y C en el 2, y solo C en el 3. Los archivos opcionales `ejercicio1/resultado_c.txt` y `ejercicio3/resultado_python.txt` son evidencia histórica y no se regeneran en la entrega actual.

---

## 📊 Resumen de soluciones

### Ejercicio 1
- Convierte posición `a1`→`(0,0)` y verifica que cada salto sea `(±1,±2)` o `(±2,±1)`.
- Se detiene al primer error indicando su número.
- Detecta prefijos inválidos (ej: `Q` en lugar de `K`).
- Genera 500 saltos legales desde `a2` con semilla 2026 y mide su validación 10 veces con `perf_counter`. La generación queda fuera de la medición.
- Es una cadena propia, opción autorizada por el profesor, no una transcripción de sus 500 movimientos corregidos. La versión corregida del enunciado aún no está disponible localmente.
- Verifica los nueve ejemplos originales y cadenas de 500 movimientos con fallos introducidos en las posiciones 1, 250 y 500; se detiene en el primer error.
- Conserva la cadena válida completa en el archivo de resultados. La versión C opcional sigue siendo una referencia de la prueba anterior de 224 movimientos, no la solución principal.

### Ejercicio 2
- Genera coeficientes de `(x+1)^n` con triángulo de Pascal y **memoria dinámica**.
- C usa coeficientes de precisión dinámica para evitar desbordamiento en `n=100`.
- Muestra el polinomio expandido y la evaluación `f(x)` paso a paso.
- Compara tiempo Python vs C para `n=100`, 100 repeticiones.
- `n=100` es el grado del polinomio; las 100 repeticiones son una decisión de medición para obtener tiempos más estables, no una obligación general.

### Ejercicio 3
- Solución principal en C: carga el programa en **memoria dinámica** (`malloc`).
- Tokenizador que respeta strings, chars y comentarios.
- Detecta las 32 palabras reservadas de C y las traduce al español.
- Muestra resumen de ocurrencias y el código traducido.
