@echo off
setlocal enabledelayedexpansion
cd /d "%~dp0"

title UNEG - Lenguajes y Compiladores - Ejercicio I
color 0A

echo ========================================================
echo   UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)
echo        LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA
echo          SCRIPT DE VERIFICACION Y EJECUCION
echo ========================================================
echo.

:: 1. Autodetección y agregado al PATH temporal si no está reconocido
where g++ >nul 2>&1
if %ERRORLEVEL% neq 0 (
    if exist "C:\msys64\ucrt64\bin\g++.exe" (
        set "PATH=C:\msys64\ucrt64\bin;!PATH!"
    ) else if exist "C:\msys64\mingw64\bin\g++.exe" (
        set "PATH=C:\msys64\mingw64\bin;!PATH!"
    )
)

echo [1/3] Verificando compilador de C++ (g++)...
where g++ >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo     [OK] g++ detectado correctamente.
) else (
    echo     [ERROR] No se encontro g++.exe.
    goto FIN
)

echo.
echo [2/3] Verificando interprete de Python...
where python >nul 2>&1
if %ERRORLEVEL% equ 0 (
    echo     [OK] Python detectado correctamente.
) else (
    echo     [ADVERTENCIA] Python no encontrado en el PATH.
)

echo.
echo ========================================================
echo   COMPILANDO Y EJECUTANDO C++ (ajedrez.cpp)
echo ========================================================
g++ ajedrez.cpp -o ajedrez.exe
if %ERRORLEVEL% equ 0 (
    echo [OK] Compilacion exitosa. Ejecutando binario...
    echo.
    ajedrez.exe
) else (
    echo [ERROR] Fallo la compilacion de ajedrez.cpp.
    goto FIN
)

echo.
echo ========================================================
echo   EJECUTANDO PYTHON (ajedrez.py)
echo ========================================================
where python >nul 2>&1
if %ERRORLEVEL% equ 0 (
    python ajedrez.py
) else (
    echo Se omite Python por no estar en el PATH.
)

:FIN
echo.
echo ========================================================
echo   PROCESO COMPLETADO
echo ========================================================
echo Presione cualquier tecla para salir...
pause >nul