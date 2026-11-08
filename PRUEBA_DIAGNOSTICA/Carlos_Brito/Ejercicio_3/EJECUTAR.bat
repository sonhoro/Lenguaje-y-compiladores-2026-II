@echo off
chcp 65001 >nul
cd /d "%~dp0"

echo.
echo =================================================================
echo  EJERCICIO 3 - C
echo  Detector de palabras reservadas y traduccion al espanol
echo =================================================================

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

echo  [NO] GCC NO encontrado. Este ejercicio requiere C obligatoriamente.
echo.
set /p RESP="      Deseas ejecutar el instalador ahora? (S/N): "
if /i "%RESP%"=="S" (
    echo.
    echo  Ejecutando instalador...
    call "%~dp0..\scripts\instalar_dependencias.bat"
    gcc --version >nul 2>&1
    if %errorlevel%==0 (
        set "GCC_OK=1"
        echo  [OK] GCC instalado correctamente.
        goto :check_files
    ) else (
        echo  [AVISO] GCC aun no detectado. Reinicia este bat tras instalar.
        goto :error
    )
) else (
    echo  [CANCELADO] Sin GCC no es posible ejecutar el Ejercicio 3.
    goto :error
)

:check_files
:: -------------------------------------------------------------------
:: VERIFICAR: Archivos fuente y de entrada existen?
:: -------------------------------------------------------------------
if not exist "%~dp0ejercicio3.c" (
    echo  [ERROR] No se encuentra ejercicio3.c en esta carpeta.
    goto :error
)
echo  [OK] ejercicio3.c encontrado.

if not exist "%~dp0programa_ejemplo.c" (
    echo  [ERROR] No se encuentra programa_ejemplo.c en esta carpeta.
    goto :error
)
echo  [OK] programa_ejemplo.c encontrado.

:: -------------------------------------------------------------------
:: COMPILAR Y EJECUTAR
:: -------------------------------------------------------------------
echo.
echo =================================================================
echo  Compilando...
echo =================================================================
gcc -std=c99 -O2 -o "%~dp0ejercicio3.exe" "%~dp0ejercicio3.c"

if %errorlevel%==0 (
    echo  [OK] Compilado. Ejecutando...
    echo.
    echo =================================================================
    echo.
    ejercicio3.exe programa_ejemplo.c
    del "%~dp0ejercicio3.exe" >nul 2>&1
) else (
    echo  [ERROR] No se pudo compilar ejercicio3.c
)

echo.
echo =================================================================
echo  Ejercicio 3 finalizado.
echo =================================================================
pause
exit /b 0

:error
echo.
pause
exit /b 1
