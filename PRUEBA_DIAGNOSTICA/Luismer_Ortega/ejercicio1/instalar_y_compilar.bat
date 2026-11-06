@echo off
setlocal
cd /d "%~dp0"
REM Script de instalacion y compilacion - Ejercicio 1
REM Lenguajes y Compiladores 2026-II — Luismer Ortega

echo ============================================================
echo  Ejercicio 1 - Caballo de Ajedrez
echo  Scripts de compilacion e instalacion
echo ============================================================
echo.

REM --- Verificar Python ---
echo [1/2] Verificando Python...
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

echo.
echo [2/2] Ejecutando la solucion Python...
python caballo.py
IF ERRORLEVEL 1 (
    echo ERROR: Fallo la ejecucion o las pruebas del caballo.
    pause
    exit /b 1
)
copy /y resultado.txt resultado_python.txt >nul
IF ERRORLEVEL 1 (
    echo ERROR: No se pudo guardar resultado_python.txt.
    exit /b 1
)

echo.
echo Listo.
pause
exit /b 0
