@echo off
chcp 65001 >nul
:: ============================================================================
:: instalar_dependencias.bat
:: Script de instalación de dependencias para la Prueba Diagnóstica
:: Lenguajes y Compiladores — Carlos Brito / Grupo: Prohibido Rendirse
::
:: Instala automáticamente:
::   1. Python 3.x  (via winget)
::   2. MSYS2 + GCC (compilador C, via winget)
::   3. Git          (via winget)
::
:: Requisito previo: Windows 10/11 con winget disponible
:: ============================================================================

echo.
echo =================================================================
echo   INSTALADOR DE DEPENDENCIAS — Prueba Diagnóstica
echo   Lenguajes y Compiladores
echo   Estudiante : Carlos Brito  |  Cedula: 30857129
echo   Grupo      : Prohibido Rendirse
echo =================================================================

:: ---- Verificar que winget está disponible ----
where winget >nul 2>&1
if %errorlevel% neq 0 (
    echo.
    echo [ERROR] winget no encontrado.
    echo         Actualiza Windows o instala manualmente desde:
    echo         https://github.com/microsoft/winget-cli/releases
    pause
    exit /b 1
)

echo.
echo [1/3] Verificando Python 3...
python --version >nul 2>&1
if %errorlevel%==0 (
    echo       Python ya instalado:
    python --version
) else (
    echo       Instalando Python 3.11...
    winget install -e --id Python.Python.3.11 --accept-package-agreements --accept-source-agreements
    if %errorlevel%==0 (
        echo       [OK] Python instalado correctamente.
    ) else (
        echo       [WARN] No se pudo instalar Python automaticamente.
        echo              Descargalo de: https://www.python.org/downloads/
    )
)

echo.
echo [2/3] Verificando GCC (compilador C)...
gcc --version >nul 2>&1
if %errorlevel%==0 (
    echo       GCC ya disponible:
    gcc --version
) else (
    echo       Instalando MSYS2 (incluye GCC/MinGW-w64)...
    winget install -e --id MSYS2.MSYS2 --accept-package-agreements --accept-source-agreements
    if %errorlevel%==0 (
        echo       [OK] MSYS2 instalado en C:\msys64
        echo.
        echo       IMPORTANTE: Para activar GCC ejecuta en MSYS2 terminal:
        echo         pacman -S --noconfirm mingw-w64-x86_64-gcc
        echo.
        echo       Luego agrega al PATH de Windows:
        echo         C:\msys64\mingw64\bin
    ) else (
        echo       [WARN] Instala MinGW manualmente desde: https://winlibs.com
    )
)

echo.
echo [3/3] Verificando Git...
git --version >nul 2>&1
if %errorlevel%==0 (
    echo       Git ya instalado:
    git --version
) else (
    echo       Instalando Git...
    winget install -e --id Git.Git --accept-package-agreements --accept-source-agreements
    if %errorlevel%==0 (
        echo       [OK] Git instalado correctamente.
    ) else (
        echo       [WARN] Descargalo de: https://git-scm.com/download/win
    )
)

echo.
echo =================================================================
echo   Verificacion final de herramientas:
echo =================================================================
python --version 2>&1
gcc    --version 2>&1
git    --version 2>&1
echo =================================================================
echo   Instalacion finalizada. Ahora puedes ejecutar:
echo     compilar_ejecutar.bat
echo =================================================================
pause
