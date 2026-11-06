@echo off
setlocal
cd /d "%~dp0"
REM Script de instalacion y compilacion - Ejercicio 3
REM Lenguajes y Compiladores 2026-II — Luismer Ortega

echo ============================================================
echo  Ejercicio 3 - Palabras Reservadas C a Espanol
echo  Scripts de compilacion e instalacion
echo ============================================================
echo.

REM --- Verificar gcc ---
echo.
echo [1/3] Verificando gcc (MinGW)...
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
echo [2/3] Compilando palabras_reservadas.c...
"%GCC%" -o palabras_reservadas.exe palabras_reservadas.c
IF ERRORLEVEL 1 (
    echo ERROR: Fallo la compilacion de palabras_reservadas.c
    pause
    exit /b 1
) ELSE (
    echo     OK - palabras_reservadas.exe generado.
)

REM --- Ejecutar C ---
echo.
echo [3/3] Ejecutando la solucion C...
palabras_reservadas.exe %*
IF ERRORLEVEL 1 (
    echo ERROR: Fallo el analisis o la escritura de resultados.
    pause
    exit /b 1
)

echo.
copy /y resultado_c.txt resultado.txt >nul
IF ERRORLEVEL 1 (
    echo ERROR: No se pudo copiar el resultado C a resultado.txt.
    exit /b 1
)

echo.
echo USO AVANZADO:
echo   instalar_y_compilar.bat "C:\ruta\mi_programa.c"
echo.
echo Listo.
pause
exit /b 0
