@echo off
chcp 65001 >nul
cd /d "%~dp0"

echo.
echo =================================================================
echo  EJERCICIO 1 - Python
echo  Validador de movimientos del Caballo de Ajedrez
echo =================================================================

:: -------------------------------------------------------------------
:: VERIFICAR: Python instalado?
:: -------------------------------------------------------------------
set "PYTHON_OK=0"
python --version >nul 2>&1
if %errorlevel%==0 (
    set "PYTHON_OK=1"
    echo  [OK] Python detectado.
) else (
    echo  [NO] Python NO encontrado.
    echo.
    set /p RESP="      Deseas ejecutar el instalador ahora? (S/N): "
    if /i "%RESP%"=="S" (
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
        echo  [CANCELADO] Instala Python manualmente desde: https://www.python.org/downloads/
    )
)

if "%PYTHON_OK%"=="0" (
    echo.
    pause
    exit /b 1
)

:: -------------------------------------------------------------------
:: VERIFICAR: Archivo fuente existe?
:: -------------------------------------------------------------------
if not exist "%~dp0ejercicio1.py" (
    echo.
    echo  [ERROR] No se encuentra ejercicio1.py en esta carpeta.
    echo.
    pause
    exit /b 1
)
echo  [OK] ejercicio1.py encontrado.

:: -------------------------------------------------------------------
:: TODO OK - Ejecutar
:: -------------------------------------------------------------------
echo.
echo  Ejecutando...
echo =================================================================
echo.
python -X utf8 ejercicio1.py

echo.
echo =================================================================
echo  Ejercicio 1 finalizado.
echo =================================================================
pause
