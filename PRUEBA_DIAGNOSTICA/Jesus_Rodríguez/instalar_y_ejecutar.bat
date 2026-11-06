@echo off
REM Instala (si falta) lo necesario, compila los programas en C y ejecuta los 3 ejercicios.
REM Lenguajes y Compiladores 2026-II - Jesus Rodriguez
setlocal
cd /d "%~dp0"

echo [1/4] Verificando Python...
python --version >nul 2>&1
if errorlevel 1 (
    echo Python no esta instalado. Intentando instalarlo con winget...
    winget install -e --id Python.Python.3.13
    echo Cierre y abra esta ventana de nuevo para que el PATH se actualice, y vuelva a ejecutar el script.
    pause & exit /b 1
)
python --version

echo [2/4] Verificando gcc...
gcc --version >nul 2>&1
if errorlevel 1 (
    echo gcc no esta instalado. Intentando instalar MinGW-w64 con winget...
    winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT
    echo Cierre y abra esta ventana de nuevo para que el PATH se actualice, y vuelva a ejecutar el script.
    pause & exit /b 1
)
gcc --version | findstr /i gcc

echo [3/4] Compilando los programas en C...
gcc -O2 -o ejercicio1\caballo.exe ejercicio1\caballo.c || goto :error
gcc -O2 -o ejercicio2\pascal.exe ejercicio2\pascal.c || goto :error
gcc -O2 -o ejercicio3\palabras_reservadas.exe ejercicio3\palabras_reservadas.c || goto :error

echo [4/4] Ejecutando (genera resultado*.txt en cada carpeta)...
pushd ejercicio1 & python caballo.py >nul & caballo.exe & popd
pushd ejercicio2 & python pascal.py >nul & pascal.exe & popd
pushd ejercicio3 & palabras_reservadas.exe & popd

for %%E in (ejercicio1 ejercicio2) do (
    type %%E\resultado_python.txt > %%E\resultado.txt
    echo.>> %%E\resultado.txt
    type %%E\resultado_c.txt >> %%E\resultado.txt
)
copy /y ejercicio3\resultado_c.txt ejercicio3\resultado.txt >nul
echo Listo. Revise los resultado.txt de cada carpeta.
pause
exit /b 0

:error
echo ERROR al compilar.
pause
exit /b 1
