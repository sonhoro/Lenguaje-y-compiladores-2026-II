@echo off
cd /d "%~dp0"

title UNEG - Lenguajes y Compiladores - Ejercicio I
color 0A

echo ========================================================
echo   UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)
echo        LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA
echo       EJERCICIO I: VALIDACION DE CABALLO EN AJEDREZ
echo                        (LENGUAJE: C++)
echo ========================================================
echo.

:: 1. Autodeteccion del compilador g++ si no esta en el PATH
where g++ >nul 2>nul
if %ERRORLEVEL% neq 0 (
    if exist "C:\msys64\ucrt64\bin\g++.exe" (
        set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    ) else if exist "C:\msys64\mingw64\bin\g++.exe" (
        set "PATH=C:\msys64\mingw64\bin;%PATH%"
    )
)

echo [1/1] Verificando compilador de C++ (g++)...
where g++ >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo     [OK] g++ detectado correctamente.
) else (
    echo     [ERROR] No se encontro g++.exe en el sistema.
    goto SALIDA
)

echo.
echo ========================================================
echo   COMPILANDO Y EJECUTANDO C++ (ajedrez.cpp)
echo ========================================================
if exist "ajedrez.exe" del /f /q "ajedrez.exe"
g++ ajedrez.cpp -o ajedrez.exe
if %ERRORLEVEL% equ 0 (
    echo [OK] Compilacion exitosa. Ejecutando binario:
    echo.
    ajedrez.exe
) else (
    echo [ERROR] Fallo la compilacion de ajedrez.cpp.
    goto SALIDA
)

echo.
echo ========================================================
echo   VERIFICACION DE ARCHIVO GENERADO
echo ========================================================
if exist "resultados_de_cpp.txt" (
    echo [OK] Archivo 'resultados_de_cpp.txt' actualizado exitosamente.
)

:SALIDA
echo.
echo ========================================================
echo   PROCESO TERMINADO EXITOSAMENTE
echo ========================================================
echo Presione cualquier tecla para cerrar esta ventana...
pause >nul