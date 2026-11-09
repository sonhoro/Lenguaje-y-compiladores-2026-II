# Universidad Nacional Experimental de Guayana (UNEG)
## Vicerrectorado Académico • Coordinación de Ingeniería en Informática
### Asignatura: Lenguajes y Compiladores — Período: 2026-II
**Docente:** Msc. Félix Márquez  

---

## 📋 Información del Estudiante y Grupo

- **Estudiante:** Saniurka Guarema
- **Cédula de Identidad:** 27.644.990
- **Sección:** 3
- **Grupo:** Prohibido rendirse
- **Eslogan:** *"En la optimización habita el mañana"*

---
## 📹 Enlace a la Defensa Audiovisual (5%)
🔗 **[Ver Video de Defensa en Google Drive](https://drive.google.com/file/d/1cLoZxsG_kGlBFsKw7_3udG3PUIsSaPc1/view?usp=sharing)**
## 📊 Matriz Resumen de Ejercicios y Lenguajes Utilizados

| Ejercicio | ¿En qué consiste? | Lenguaje(s) Usado(s) | Archivo Principal | Justificación Técnica |
| :--- | :--- | :---: | :--- | :--- |
| **Ejercicio I** | **Validador de Saltos de Caballo en Ajedrez:** Analiza secuencias de movimientos en un tablero de $8 \times 8$. Comprueba la regla léxica `'K'` y la regla semántica del salto en "L". Evalúa 500 saltos válidos desde `a2` y reporta fallos con detención inmediata. | **C++** | `ejercicio I/ajedrez.cpp` | Alto rendimiento para procesar tokens en $O(1)$ de memoria auxiliar y medir tiempos de ejecución en submilisegundos ($< 2\text{ ms}$). |
| **Ejercicio II** | **Triángulo de Pascal y Polinomio $(x+1)^n$:** Genera coeficientes con memoria dinámica en el Heap, muestra el polinomio expandido y evalúa $f(x)$ paso a paso para $x=2$. Compara el tiempo para $n=100$ (BigInt de 30 dígitos). | **C puro vs. Python** | `ejercicio II/pascal.c`<br>`ejercicio II/pascal.py` | C permite el uso estricto de punteros con `malloc`/`free`. Python sirve como lenguaje de contraste de rendimiento con BigInt nativo. |
| **Ejercicio III** | **Traductor Léxico de C a Español:** Simula el Scanner de un compilador. Carga dinámicamente un código fuente en memoria RAM y traduce palabras reservadas (`int` $\rightarrow$ `entero`, `while` $\rightarrow$ `mientras`, etc.) sin alterar variables. | **C puro** | `ejercicio III/traductor.c` | Uso de `fseek`/`ftell` para dimensionar el buffer y `malloc` para alojar el código en memoria dinámica según exige el planteamiento. |

---

## 📂 Estructura del Repositorio

```text
Saniurka_Guarema/
├── ejercicio I/
│   ├── ajedrez.cpp                 # Código en C++ (Analizador de movimientos de caballo)
│   ├── instalar_y_ejecutar.bat     # Script de compilación y ejecución automática
│   └── resultados_de_cpp.txt       # Reporte de salidas y benchmarks generados
├── ejercicio II/
│   ├── pascal.c                    # Código en C con memoria dinámica (malloc/free)
│   ├── pascal.py                   # Código en Python para comparativa de rendimiento
│   ├── tiempos_n100_c.txt          # Reporte de tiempos y coeficientes de C (n=100)
│   ├── tiempos_n100_py.txt         # Reporte de tiempos y coeficientes de Python (n=100)
│   └── instalar_y_ejecutar.bat     # Script de ejecución para C y Python
├── ejercicio III/
│   ├── codigo_prueba.c             # Archivo fuente de entrada con código en C
│   ├── traductor.c                 # Analizador léxico en C con carga dinámica de memoria
│   ├── codigo_traducido_c.c        # Archivo resultante con palabras reservadas traducidas
│   └── instalar_y_ejecutar.bat     # Script de compilación y ejecución en Windows
├── enlace_video.txt                # Enlace al video de defensa en Google Drive (5%)
└── README.md                       # Documentación técnica completa del proyecto