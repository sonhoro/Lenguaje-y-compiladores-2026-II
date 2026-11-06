@echo off
cd /d "%~dp0"

title UNEG - Lenguajes y Compiladores - Ejercicio II
color 0A

echo ========================================================
echo   UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)
echo        LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA
echo       EJERCICIO II: SCRIPT DE VERIFICACION Y EJECUCION
echo ========================================================
echo.

:: 1. Agregar rutas de GCC si no estan en el PATH
where gcc >nul 2>nul
if %ERRORLEVEL% neq 0 (
    if exist "C:\msys64\ucrt64\bin\gcc.exe" (
        set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    ) else if exist "C:\msys64\mingw64\bin\gcc.exe" (
        set "PATH=C:\msys64\mingw64\bin;%PATH%"
    )
)

:: 2. Buscar Python
set "PY_CMD="
where python >nul 2>nul
if %ERRORLEVEL% equ 0 (
    set "PY_CMD=python"
) else (
    where py >nul 2>nul
    if %ERRORLEVEL% equ 0 (
        set "PY_CMD=py"
    ) else (
        for /d %%D in ("%LOCALAPPDATA%\Programs\Python\Python*") do (
            if exist "%%D\python.exe" (
                set "PATH=%%D;%%D\Scripts;%PATH%"
                set "PY_CMD=%%D\python.exe"
            )
        )
    )
)

echo [1/2] Verificando compilador GCC...
where gcc >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo     [OK] gcc detectado correctamente.
) else (
    echo     [ERROR] No se encontro gcc.exe en el sistema.
    goto SALIDA
)

echo.
echo [2/2] Verificando interprete Python...
if defined PY_CMD (
    echo     [OK] Python detectado correctamente.
) else (
    echo     [ERROR] No se encontro python.exe en el sistema.
    goto SALIDA
)

echo.
echo ========================================================
echo   COMPILANDO Y EJECUTANDO C (pascal.c)
echo ========================================================
if exist "pascal.exe" del /f /q "pascal.exe"
gcc pascal.c -o pascal.exe
if %ERRORLEVEL% equ 0 (
    echo [OK] Compilacion exitosa. Ejecutando binario:
    echo.
    pascal.exe
) else (
    echo [ERROR] Fallo la compilacion de pascal.c.
    goto SALIDA
)

echo.
echo ========================================================
echo   EJECUTANDO PYTHON (pascal.py)
echo ========================================================
%PY_CMD% pascal.py

echo.
echo ========================================================
echo   ARCHIVOS GENERADOS (.TXT)
echo ========================================================
if exist "tiempos_n100_c.txt" echo [OK] Archivo 'tiempos_n100_c.txt' presente.
if exist "tiempos_n100_py.txt" echo [OK] Archivo 'tiempos_n100_py.txt' presente.

:SALIDA
echo.
echo ========================================================
echo   PROCESO TERMINADO
echo ========================================================
echo Presione cualquier tecla para cerrar esta ventana...
pause >nul