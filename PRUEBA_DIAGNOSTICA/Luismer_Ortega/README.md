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
Luismer Ortega/
├── ejercicio1/         ← Validador de movimientos de caballo de ajedrez
│   ├── caballo.py
│   ├── caballo.c
│   └── instalar_y_compilar.bat
├── ejercicio2/         ← Polinomio (x+1)^n con triángulo de Pascal
│   ├── pascal.py
│   ├── pascal.c
│   └── instalar_y_compilar.bat
└── ejercicio3/         ← Palabras reservadas C → Español
    ├── palabras_reservadas.py
    ├── palabras_reservadas.c
    └── instalar_y_compilar.bat
```

---

## ⚙️ Requisitos

| Componente | Versión            |
|------------|--------------------|
| Python     | 3.x (stdlib solo)  |
| GCC / MinGW| 6.3+ en `C:\MinGW` |

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
set PATH=C:\MinGW\bin;%PATH%
C:\MinGW\bin\gcc.exe -o caballo.exe caballo.c -lm
caballo.exe
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
python palabras_reservadas.py [archivo.c]
set PATH=C:\MinGW\bin;%PATH%
C:\MinGW\bin\gcc.exe -o palabras_reservadas.exe palabras_reservadas.c
palabras_reservadas.exe [archivo.c]
```
*(Sin argumento usa un programa C de ejemplo embebido)*

---

## 📊 Resumen de soluciones

### Ejercicio 1
- Convierte posición `a1`→`(0,0)` y verifica que cada salto sea `(±1,±2)` o `(±2,±1)`.
- Se detiene al primer error indicando su número.
- Detecta prefijos inválidos (ej: `Q` en lugar de `K`).
- Mide con temporizador de alta resolución 10 repeticiones de la cadena publicada. Aunque el documento la describe como 500 movimientos, contiene 224; se usa `a2` porque el enunciado no especifica `Ak` para esta cadena y así todos los saltos son legales.

### Ejercicio 2
- Genera coeficientes de `(x+1)^n` con triángulo de Pascal y **memoria dinámica**.
- C usa coeficientes de precisión dinámica para evitar desbordamiento en `n=100`.
- Muestra el polinomio expandido y la evaluación `f(x)` paso a paso.
- Compara tiempo Python vs C para `n=100`, 100 repeticiones.

### Ejercicio 3
- Carga el programa C en **memoria dinámica** (`malloc`/listas Python).
- Tokenizador que respeta strings, chars y comentarios.
- Detecta las 32 palabras reservadas de C y las traduce al español.
- Muestra resumen de ocurrencias y el código traducido.
