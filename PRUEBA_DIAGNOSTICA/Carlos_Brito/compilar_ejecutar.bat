@echo off
chcp 65001 >nul
:: ============================================================================
:: compilar_ejecutar.bat - Prueba Diagnostica
:: Lenguajes y Compiladores - Carlos Brito / Grupo: Prohibido Rendirse
:: ============================================================================

echo.
echo =================================================================
echo   PRUEBA DIAGNOSTICA - Lenguajes y Compiladores
echo   Estudiante : Carlos Brito  - Cedula: 30857129  - Seccion: 3
echo   Grupo      : Prohibido Rendirse
echo   Eslogan    : En la optimizacion habita el manana
echo =================================================================
echo.
echo  Verificando dependencias...
echo  -----------------------------------------------------------------

:: ================================================================
:: VERIFICAR PYTHON
:: ================================================================
set PYTHON_OK=0
python --version >nul 2>&1
if errorlevel 1 goto :noPython
set PYTHON_OK=1
echo  [OK] Python detectado.
goto :checkGCC

:noPython
echo  [NO] Python NO encontrado.
echo.
set /p RESP_PY="      Deseas ejecutar el instalador ahora? (S/N): "
if /i not "%RESP_PY%"=="S" goto :checkGCC
echo.
echo  Ejecutando instalador...
call "%~dp0scripts\instalar_dependencias.bat"
python --version >nul 2>&1
if errorlevel 1 goto :pyNoInstalado
set PYTHON_OK=1
echo  [OK] Python instalado correctamente.
goto :checkGCC
:pyNoInstalado
echo  [AVISO] Python aun no detectado. Reinicia el bat tras instalar.

:: ================================================================
:: VERIFICAR GCC
:: ================================================================
:checkGCC
set GCC_OK=0
gcc --version >nul 2>&1
if errorlevel 1 goto :noGCC
set GCC_OK=1
echo  [OK] GCC detectado.
goto :run

:noGCC
echo  [NO] GCC NO encontrado.
echo.
set /p RESP_GCC="      Deseas ejecutar el instalador ahora? (S/N): "
if /i not "%RESP_GCC%"=="S" goto :run
echo.
echo  Ejecutando instalador...
call "%~dp0scripts\instalar_dependencias.bat"
gcc --version >nul 2>&1
if errorlevel 1 goto :gccNoInstalado
set GCC_OK=1
echo  [OK] GCC instalado correctamente.
goto :run
:gccNoInstalado
echo  [AVISO] GCC aun no detectado. Reinicia el bat tras instalar.

:: ================================================================
:: EJECUTAR EJERCICIOS
:: ================================================================
:run
if %PYTHON_OK%==0 if %GCC_OK%==0 (
    echo.
    echo  [ERROR] Sin Python ni GCC no se puede ejecutar nada.
    goto :fin_error
)
echo  -----------------------------------------------------------------
echo.

:: --- EJERCICIO 1 - Python ---
if %PYTHON_OK%==0 goto :skip_ej1
if not exist "%~dp0Ejercicio_1\ejercicio1.py" goto :skip_ej1
echo =================================================================
echo  EJERCICIO 1 - Python
echo =================================================================
cd /d "%~dp0Ejercicio_1"
python -X utf8 ejercicio1.py
cd /d "%~dp0"
:skip_ej1

:: --- EJERCICIO 2 - Python ---
if %PYTHON_OK%==0 goto :skip_ej2py
if not exist "%~dp0Ejercicio_2\ejercicio2.py" goto :skip_ej2py
echo.
echo =================================================================
echo  EJERCICIO 2 - Python
echo =================================================================
cd /d "%~dp0Ejercicio_2"
python -X utf8 ejercicio2.py
cd /d "%~dp0"
:skip_ej2py

:: --- EJERCICIO 2 - C ---
if %GCC_OK%==0 goto :skip_ej2c
if not exist "%~dp0Ejercicio_2\ejercicio2.c" goto :skip_ej2c
echo.
echo =================================================================
echo  EJERCICIO 2 - C (compilando...)
echo =================================================================
gcc -std=c99 -O2 -lm -o "%~dp0Ejercicio_2\ejercicio2.exe" "%~dp0Ejercicio_2\ejercicio2.c"
if errorlevel 1 goto :errorEj2C
echo  [OK] Compilado. Ejecutando...
cd /d "%~dp0Ejercicio_2"
ejercicio2.exe
cd /d "%~dp0"
del "%~dp0Ejercicio_2\ejercicio2.exe" >nul 2>&1
goto :skip_ej2c
:errorEj2C
echo  [ERROR] Fallo la compilacion del Ejercicio 2 en C.
:skip_ej2c

:: --- EJERCICIO 3 - C ---
if %GCC_OK%==0 goto :skip_ej3
if not exist "%~dp0Ejercicio_3\ejercicio3.c" goto :skip_ej3
echo.
echo =================================================================
echo  EJERCICIO 3 - C (compilando...)
echo =================================================================
gcc -std=c99 -O2 -o "%~dp0Ejercicio_3\ejercicio3.exe" "%~dp0Ejercicio_3\ejercicio3.c"
if errorlevel 1 goto :errorEj3
echo  [OK] Compilado. Ejecutando...
cd /d "%~dp0Ejercicio_3"
ejercicio3.exe programa_ejemplo.c
cd /d "%~dp0"
del "%~dp0Ejercicio_3\ejercicio3.exe" >nul 2>&1
goto :skip_ej3
:errorEj3
echo  [ERROR] Fallo la compilacion del Ejercicio 3 en C.
:skip_ej3

echo.
echo =================================================================
echo  Todos los programas finalizaron.
echo =================================================================
pause
exit /b 0

:fin_error
echo.
pause
exit /b 1
