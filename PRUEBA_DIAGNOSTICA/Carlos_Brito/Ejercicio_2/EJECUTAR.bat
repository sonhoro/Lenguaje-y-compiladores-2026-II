@echo off
chcp 65001 >nul
cd /d "%~dp0"

echo.
echo =================================================================
echo  EJERCICIO 2 - Python + C
echo  Triangulo de Pascal y Polinomios (x+1)^n
echo =================================================================

:: -------------------------------------------------------------------
:: VERIFICAR: Python instalado?
:: -------------------------------------------------------------------
set "PYTHON_OK=0"
python --version >nul 2>&1
if %errorlevel%==0 (
    set "PYTHON_OK=1"
    echo  [OK] Python detectado.
    goto :check_gcc
)

echo  [NO] Python NO encontrado.
echo.
set /p RESP_PY="      Deseas ejecutar el instalador ahora? (S/N): "
if /i "%RESP_PY%"=="S" (
    echo.
    echo  Ejecutando instalador...
    call "%~dp0..\scripts\instalar_dependencias.bat"
    python --version >nul 2>&1
    if %errorlevel%==0 (
        set "PYTHON_OK=1"
        echo  [OK] Python instalado correctamente.
    ) else (
        echo  [AVISO] Python aun no detectado. Reinicia este bat tras instalar.
    )
) else (
    echo  [OMITIDO] Se continuara sin Python.
)

:check_gcc
:: -------------------------------------------------------------------
:: VERIFICAR: GCC instalado?
:: -------------------------------------------------------------------
set "GCC_OK=0"
set "GCC=gcc"
gcc --version >nul 2>&1
if %errorlevel%==0 (
    set "GCC_OK=1"
    echo  [OK] GCC detectado.
    goto :check_files
)

echo  [NO] GCC NO encontrado.
echo.
set /p RESP_GCC="      Deseas ejecutar el instalador ahora? (S/N): "
if /i "%RESP_GCC%"=="S" (
    echo.
    echo  Ejecutando instalador...
    call "%~dp0..\scripts\instalar_dependencias.bat"
    gcc --version >nul 2>&1
    if %errorlevel%==0 (
        set "GCC_OK=1"
        echo  [OK] GCC instalado correctamente.
    ) else (
        echo  [AVISO] GCC aun no detectado. Reinicia este bat tras instalar.
    )
) else (
    echo  [OMITIDO] Se continuara sin GCC (solo Python).
)

:check_files
:: -------------------------------------------------------------------
:: VERIFICAR: Archivos fuente existen?
:: -------------------------------------------------------------------
if "%PYTHON_OK%"=="1" (
    if not exist "%~dp0ejercicio2.py" (
        echo  [ERROR] No se encuentra ejercicio2.py en esta carpeta.
        goto :error
    )
    echo  [OK] ejercicio2.py encontrado.
)
if "%GCC_OK%"=="1" (
    if not exist "%~dp0ejercicio2.c" (
        echo  [ERROR] No se encuentra ejercicio2.c en esta carpeta.
        goto :error
    )
    echo  [OK] ejercicio2.c encontrado.
)

if "%PYTHON_OK%"=="0" if "%GCC_OK%"=="0" (
    echo  [ERROR] Sin Python ni GCC no hay nada que ejecutar.
    goto :error
)

echo.

:: -------------------------------------------------------------------
:: EJECUTAR Python
:: -------------------------------------------------------------------
if "%PYTHON_OK%"=="1" (
    echo =================================================================
    echo  Ejecutando Python...
    echo =================================================================
    echo.
    python -X utf8 ejercicio2.py
)

:: -------------------------------------------------------------------
:: COMPILAR Y EJECUTAR C
:: -------------------------------------------------------------------
if "%GCC_OK%"=="1" (
    echo.
    echo =================================================================
    echo  Compilando y ejecutando C...
    echo =================================================================
    gcc -std=c99 -O2 -lm -o "%~dp0ejercicio2.exe" "%~dp0ejercicio2.c"
    if %errorlevel%==0 (
        echo  [OK] Compilado. Ejecutando...
        echo.
        ejercicio2.exe
        del "%~dp0ejercicio2.exe" >nul 2>&1
    ) else (
        echo  [ERROR] No se pudo compilar ejercicio2.c
    )
)

echo.
echo =================================================================
echo  Ejercicio 2 finalizado.
echo =================================================================
pause
exit /b 0

:error
echo.
pause
exit /b 1
