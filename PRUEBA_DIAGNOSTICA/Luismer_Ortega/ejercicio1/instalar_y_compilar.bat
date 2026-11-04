@echo off
REM Script de instalacion y compilacion - Ejercicio 1
REM Lenguajes y Compiladores 2026-II — Luismer Ortega

echo ============================================================
echo  Ejercicio 1 - Caballo de Ajedrez
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
echo [3/4] Compilando caballo.c...
"%GCC%" -o caballo.exe caballo.c -lm
IF ERRORLEVEL 1 (
    echo ERROR: Fallo la compilacion de caballo.c
    pause
    exit /b 1
) ELSE (
    echo     OK - caballo.exe generado.
)

REM --- Ejecutar ambos programas ---
echo.
echo [4/4] Ejecutando los programas...
echo.
echo --- Python ---
python caballo.py

echo.
echo --- C ---
caballo.exe

REM Combinar ambos resultado.txt en uno solo
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
