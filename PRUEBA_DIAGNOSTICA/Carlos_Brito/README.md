# Prueba Diagnóstica — Carlos Brito

| Campo | Dato |
|---|---|
| **Nombre** | Carlos Brito |
| **Cédula** | 30857129 |
| **Sección** | 3 |
| **Grupo** | Prohibido Rendirse |
| **Eslogan** | *En la optimización habita el mañana* |

---

## 📹 Video de Defensa

> 🎬 **Enlace Google Drive:** https://drive.google.com/file/d/1JqKqR_7gjq5QPfwYpTLD67pw7lXvjqbH/view?usp=drivesdk

### Requisitos del video (según el profesor)
- Duración **máxima 5 minutos**, formato **horizontal** (no shorts/vertical)
- Debes aparecer **visible y audible** durante todo el video
- Comenzar con presentación formal: **nombre, grupo, eslogan y sección**
- Explicar con tus propias palabras **cómo funciona cada ejercicio**
- Demostrar en pantalla la **ejecución del código** y los resultados

---

## Estructura de esta carpeta

```
CARLOS BRITO/
├── README.md                        ← Este archivo (con enlace al video)
├── compilar_ejecutar.bat            ← Compila y corre todo (C + Python)
│
├── Ejercicio_1/
│   ├── ejercicio1.py                ← Validador de movimientos (Python 3)
│   ├── EJECUTAR.bat                 ← Ejecuta el ejercicio 1
│   └── resultado_ejercicio1.txt     ← Tiempos de ejecución generados
│
├── Ejercicio_2/
│   ├── ejercicio2.py                ← Triángulo de Pascal (Python 3)
│   ├── ejercicio2.c                 ← Triángulo de Pascal (C99 + malloc)
│   ├── EJECUTAR.bat                 ← Compila y ejecuta Python + C
│   └── resultado_ejercicio2.txt     ← Tiempos Python vs C generados
│
├── Ejercicio_3/
│   ├── ejercicio3.c                 ← Detector palabras reservadas (C99)
│   ├── programa_ejemplo.c           ← Archivo C analizado por el Ej. 3
│   ├── EJECUTAR.bat                 ← Compila y ejecuta el ejercicio 3
│   └── resultado_ejercicio3.txt     ← Resultados del análisis generados
│
└── scripts/
    ├── instalar_dependencias.bat    ← Instala Python + GCC en Windows
    └── instalar_dependencias.sh     ← Instala Python + GCC en Linux/Mac
```

---

## Descripción de los ejercicios

### Ejercicio 1 — Validador de movimientos del caballo ♞
**Lenguaje:** Python (libre elección del estudiante)

Dado un tablero 8×8 y un caballo en la posición inicial `Ak`, verifica si
cada movimiento de una cadena es válido (movimiento en "L": ±1,±2 ó ±2,±1).
Se detiene al primer movimiento incorrecto. Mide el tiempo de verificar
10 veces una secuencia de 500 movimientos.

**Salidas posibles:**
```
movimiento CORRECTO
INCORRECTO falla en el movimiento N
movimiento INCORRECTO falla letra X no pertenece a un caballo
```

---

### Ejercicio 2 — Polinomio (x+1)^n con Triángulo de Pascal
**Lenguajes:** Python 3 y C (comparativa de tiempos para n=100)

- **a)** Genera los coeficientes de `(x+1)^n` con memoria dinámica (Triángulo de Pascal)
- **b)** Muestra paso a paso el cálculo de `f(x) = (x+1)^n` para un `x` dado
- Mide y compara el tiempo Python vs C para generar n=100 (1000 repeticiones)

---

### Ejercicio 3 — Detector de palabras reservadas en C
**Lenguaje:** C (obligatorio según el profesor)

Carga un archivo `.c` en memoria dinámica (`malloc + fread`), elimina
comentarios y literales, tokeniza el código y detecta las 44 palabras
reservadas del estándar C (C89/C99/C11), mostrando cada una con su
número de línea y traducción al español.

---

## Cómo ejecutar

### Python (sin instalar nada extra)
```bat
rem Ejercicio 1
cd Ejercicio_1
python -X utf8 ejercicio1.py

rem Ejercicio 2
cd Ejercicio_2
python -X utf8 ejercicio2.py
```

### C — Compilar y ejecutar (requiere GCC)
```bat
rem Instalar GCC primero (solo la primera vez)
scripts\instalar_dependencias.bat

rem Luego compilar y ejecutar todo de una vez
compilar_ejecutar.bat
```

### Tiempos obtenidos (GCC 16.2 / Python 3.11)

| Ejercicio | Lenguaje | Tiempo total | Tiempo/llamada |
|---|---|---|---|
| Ej. 1 — 10 iter. × 500 movimientos | Python 3 | 0.01159 s | 0.00116 s |
| Ej. 2 — 1000 iter. × n=100 | Python 3 | 0.844 s | 0.00084 s |
| Ej. 2 — 1000 iter. × n=100 | C (GCC) | 0.049 s | 0.000049 s |
| Ej. 3 — 100 iter. × análisis | C (GCC) | 0.014 s | 0.00014 s |
