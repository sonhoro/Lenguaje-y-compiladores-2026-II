@echo off
REM Script de instalacion y compilacion - Ejercicio 2
REM Lenguajes y Compiladores 2026-II — Luismer Ortega

echo ============================================================
echo  Ejercicio 2 - Polinomio (x+1)^n - Triangulo de Pascal
echo  Scripts de compilacion e instalacion
echo ============================================================
echo.

REM --- Verificar Python ---
echo [1/4] Verificando Python...
python --version >nul 2>&1
IF ERRORLEVEL 1 (
    echo ERROR: Python no esta instalado o no esta en el PATH.
    echo Descargalo desde https://www.python.org/downloads/
    pause
    exit /b 1
) ELSE (
    echo     OK - Python encontrado:
    python --version
)

REM --- Verificar gcc ---
echo.
echo [2/4] Verificando gcc (MinGW)...
SET GCC=C:\MinGW\bin\gcc.exe
IF NOT EXIST "%GCC%" (
    echo ERROR: gcc no encontrado en C:\MinGW\bin\
    echo Descarga MinGW desde https://www.mingw-w64.org/
    pause
    exit /b 1
) ELSE (
    echo     OK - gcc encontrado:
)
SET "PATH=C:\MinGW\bin;%PATH%"
"%GCC%" --version

REM --- Compilar C ---
echo.
echo [3/4] Compilando pascal.c...
"%GCC%" -o pascal.exe pascal.c -lm
IF ERRORLEVEL 1 (
    echo ERROR: Fallo la compilacion de pascal.c
    pause
    exit /b 1
) ELSE (
    echo     OK - pascal.exe generado.
)

REM --- Ejecutar ambos programas ---
echo.
echo [4/4] Ejecutando los programas...
echo.
echo --- Python ---
python pascal.py

echo.
echo --- C ---
pascal.exe

REM Combinar resultados
echo.
echo Combinando resultados...
IF EXIST resultado.txt (
    IF EXIST resultado_c.txt (
        copy /y resultado.txt resultado_python.txt >nul
        type resultado_c.txt >> resultado.txt
        echo Resultados combinados en resultado.txt
    )
)

echo.
echo Listo.
pause
