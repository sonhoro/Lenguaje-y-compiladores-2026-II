#!/usr/bin/env bash
# ============================================================================
# instalar_dependencias.sh
# Script de instalación de dependencias — Linux / macOS
# Prueba Diagnóstica — Lenguajes y Compiladores
# Estudiante : Carlos Brito | Cédula: 30857129
# Grupo      : Prohibido Rendirse
# ============================================================================
# Uso:
#   chmod +x instalar_dependencias.sh
#   ./instalar_dependencias.sh
# ============================================================================

set -e

echo ""
echo "================================================================="
echo "  INSTALADOR DE DEPENDENCIAS — Prueba Diagnóstica"
echo "  Lenguajes y Compiladores"
echo "  Estudiante : Carlos Brito  |  Cédula: 30857129"
echo "  Grupo      : Prohibido Rendirse"
echo "================================================================="

# Detectar sistema operativo
OS="$(uname -s)"

install_package() {
    PKG=$1
    echo ""
    echo "  Instalando: $PKG ..."
    if [ "$OS" = "Linux" ]; then
        if command -v apt-get &>/dev/null; then
            sudo apt-get install -y "$PKG"
        elif command -v dnf &>/dev/null; then
            sudo dnf install -y "$PKG"
        elif command -v pacman &>/dev/null; then
            sudo pacman -S --noconfirm "$PKG"
        else
            echo "  [WARN] Instala $PKG manualmente para tu distribución."
        fi
    elif [ "$OS" = "Darwin" ]; then
        if command -v brew &>/dev/null; then
            brew install "$PKG"
        else
            echo "  [WARN] Instala Homebrew primero: https://brew.sh"
        fi
    fi
}

# ---- 1. Python 3 ----
echo ""
echo "[1/3] Verificando Python 3..."
if command -v python3 &>/dev/null; then
    echo "      Python ya instalado: $(python3 --version)"
else
    install_package "python3"
fi

# ---- 2. GCC ----
echo ""
echo "[2/3] Verificando GCC..."
if command -v gcc &>/dev/null; then
    echo "      GCC ya instalado: $(gcc --version | head -1)"
else
    if [ "$OS" = "Linux" ]; then
        install_package "gcc"
        install_package "build-essential"
    elif [ "$OS" = "Darwin" ]; then
        echo "  Instalando Xcode Command Line Tools (incluye clang/gcc)..."
        xcode-select --install 2>/dev/null || true
    fi
fi

# ---- 3. Git ----
echo ""
echo "[3/3] Verificando Git..."
if command -v git &>/dev/null; then
    echo "      Git ya instalado: $(git --version)"
else
    install_package "git"
fi

echo ""
echo "================================================================="
echo "  Verificación final:"
echo "================================================================="
python3 --version 2>/dev/null || python --version 2>/dev/null || echo "  Python: no encontrado"
gcc     --version 2>/dev/null | head -1 || echo "  GCC: no encontrado"
git     --version 2>/dev/null || echo "  Git: no encontrado"
echo "================================================================="
echo "  Instalación finalizada. Ahora compila con:"
echo "    # Ejercicio 1"
echo "    gcc -std=c99 -O2 -o Ejercicio_1/ejercicio1 Ejercicio_1/ejercicio1.c"
echo "    # Ejercicio 2"
echo "    gcc -std=c99 -O2 -lm -o Ejercicio_2/ejercicio2 Ejercicio_2/ejercicio2.c"
echo "    # Ejercicio 3"
echo "    gcc -std=c99 -O2 -o Ejercicio_3/ejercicio3 Ejercicio_3/ejercicio3.c"
echo "================================================================="
