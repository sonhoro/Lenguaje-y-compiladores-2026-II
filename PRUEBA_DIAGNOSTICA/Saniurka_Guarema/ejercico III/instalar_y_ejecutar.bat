@echo off
cd /d "%~dp0"

title UNEG - Lenguajes y Compiladores - Ejercicio III
color 0A

echo ========================================================
echo   UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)
echo        LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA
echo       EJERCICIO III: TRADUCTOR DE PALABRAS RESERVADAS
echo                        (LENGUAJE: C)
echo ========================================================
echo.

where gcc >nul 2>nul
if %ERRORLEVEL% neq 0 (
    if exist "C:\msys64\ucrt64\bin\gcc.exe" (
        set "PATH=C:\msys64\ucrt64\bin;%PATH%"
    ) else if exist "C:\msys64\mingw64\bin\gcc.exe" (
        set "PATH=C:\msys64\mingw64\bin;%PATH%"
    )
)

echo [1/1] Verificando compilador de C (gcc)...
where gcc >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo     [OK] gcc detectado correctamente.
) else (
    echo     [ERROR] No se encontro gcc.exe en el sistema.
    goto SALIDA
)

echo.
echo ========================================================
echo   COMPILANDO Y EJECUTANDO TRADUCTOR EN C (traductor.c)
echo ========================================================
if exist "traductor.exe" del /f /q "traductor.exe"
gcc traductor.c -o traductor.exe
if %ERRORLEVEL% equ 0 (
    echo [OK] Compilacion exitosa. Ejecutando binario:
    echo.
    traductor.exe
) else (
    echo [ERROR] Fallo la compilacion de traductor.c.
    goto SALIDA
)

echo.
echo ========================================================
echo   VERIFICACION DE SALIDA
echo ========================================================
if exist "codigo_traducido_c.c" (
    echo [OK] Archivo 'codigo_traducido_c.c' generado exitosamente.
)

:SALIDA
echo.
echo ========================================================
echo   PROCESO COMPLETADO EXITOSAMENTE
echo ========================================================
echo Presione cualquier tecla para cerrar esta ventana...
pause >nul