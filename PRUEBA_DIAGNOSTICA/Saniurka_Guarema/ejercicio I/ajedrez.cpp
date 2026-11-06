#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <chrono>
#include <fstream>
#include <iomanip>

using namespace std;

// Representacion de una casilla en el tablero (0..7, 0..7)
struct Coordenada {
    int col; // 0 = 'a', ..., 7 = 'h'
    int fil; // 0 = '1', ..., 7 = '8'
};

// Convierte texto tipo "a1" a Coordenada matematica
bool parsearCoordenada(const string& coord, Coordenada& pos) {
    if (coord.length() != 2) return false;
    char c = coord[0];
    char f = coord[1];
    if (c < 'a' || c > 'h' || f < '1' || f > '8') return false;
    pos.col = c - 'a';
    pos.fil = f - '1';
    return true;
}

// Comprueba la regla del caballo (salto en 'L')
inline bool esSaltoValidoCaballo(const Coordenada& origen, const Coordenada& destino) {
    int dx = abs(origen.col - destino.col);
    int dy = abs(origen.fil - destino.fil);
    return (dx == 1 && dy == 2) || (dx == 2 && dy == 1);
}

// Analizador lexico y semantico directo (O(1) memoria auxiliar)
string verificarMovimientos(const string& Ak, int n, const string& cadena) {
    Coordenada posActual;
    if (!parsearCoordenada(Ak, posActual)) {
        return "Posicion inicial invalida.";
    }

    size_t i = 0;
    int movContador = 0;

    while (i < cadena.length() && movContador < n) {
        // Ignorar espacios en blanco iniciales
        while (i < cadena.length() && isspace(static_cast<unsigned char>(cadena[i]))) {
            i++;
        }
        if (i >= cadena.length()) break;

        // Extraer token del movimiento
        size_t inicioToken = i;
        while (i < cadena.length() && !isspace(static_cast<unsigned char>(cadena[i]))) {
            i++;
        }
        string token = cadena.substr(inicioToken, i - inicioToken);
        movContador++;

        // 1. Regla lexica: La pieza debe ser un caballo ('K')
        if (token.empty() || token[0] != 'K') {
            char letraErronea = token.empty() ? '?' : token[0];
            return "movimiento INCORRECTO falla letra " + string(1, letraErronea) + " no pertenece a un caballo.";
        }

        // 2. Regla sintactica: Formato de longitud y coordenadas
        if (token.length() < 3) {
            return "INCORRECTO falla en el movimiento " + to_string(movContador) + " (longitud invalida).";
        }

        string coordDestino = token.substr(1, 2);
        Coordenada posDestino;
        if (!parsearCoordenada(coordDestino, posDestino)) {
            return "INCORRECTO falla en el movimiento " + to_string(movContador) + " (coordenada fuera de tablero).";
        }

        // 3. Regla semantica del ajedrez: Salto en 'L'
        if (!esSaltoValidoCaballo(posActual, posDestino)) {
            return "INCORRECTO falla en el movimiento " + to_string(movContador) + ".";
        }

        posActual = posDestino;
    }

    return "movimiento CORRECTO.";
}

int main() {
    ofstream reporte("resultados_de_cpp.txt");

    cout << "========================================================\n";
    cout << "  UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA (UNEG)   \n";
    cout << "       LENGUAJES Y COMPILADORES - PRUEBA DIAGNOSTICA    \n";
    cout << "       EJERCICIO I: VALIDACION DE CABALLO EN AJEDREZ    \n";
    cout << "                     (LENGUAJE: C++)                    \n";
    cout << "========================================================\n\n";

    if (reporte.is_open()) {
        reporte << "UNIVERSIDAD NACIONAL EXPERIMENTAL DE GUAYANA\n";
        reporte << "LENGUAJES Y COMPILADORES - EJERCICIO I (C++)\n";
        reporte << "========================================================\n\n";
    }

    // 1. Casos de prueba oficiales del documento
    struct CasoPrueba {
        string ak;
        int n;
        string cadena;
    } casos[] = {
        {"a1", 4, "Kc2 Kd4 Kb3 Kc5"},
        {"a1", 4, "Kc2 Kd4 Kb4 Kc5"},
        {"a2", 5, "Kc3 Ke4 Kf6 Ke8 Kg7"},
        {"a2", 5, "Kc8 Ke4 Kf6 Ke8 Kg7"},
        {"a8", 2, "Kb6 Kd7"},
        {"a8", 2, "Kb6 Kb7"},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Kb4"},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kc6 Qb4"},
        {"a1", 10, "Kc2 Kd4 Kf3 Ke5 Kg6 Kh8 Kf7 Kd8 Kh6 Kb4"}
    };

    cout << "--- CASOS DE PRUEBA OFICIALES (PAGINA 1) ---\n";
    for (int i = 0; i < 9; i++) {
        string resultado = verificarMovimientos(casos[i].ak, casos[i].n, casos[i].cadena);
        cout << "Caso " << (i + 1) << " [Ak=" << casos[i].ak << ", n=" << casos[i].n << "]: " 
             << resultado << "\n";
        if (reporte.is_open()) {
            reporte << "Caso " << (i + 1) << " [Ak=" << casos[i].ak << ", n=" << casos[i].n << "]: " 
                    << resultado << "\n";
        }
    }

    // 2. Secuencia de 500 movimientos 100% VALIDOS (desde a2)
    string movs500 = 
        "Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 "
        "Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 "
        "Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 "
        "Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 "
        "Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 "
        "Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 "
        "Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
        "Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 "
        "Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 "
        "Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 "
        "Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 "
        "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 "
        "Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 "
        "Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 "
        "Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 "
        "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 "
        "Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
        "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 "
        "Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 "
        "Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 "
        "Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 ";

    cout << "\n--- MEDICION DE TIEMPO (10 VECES LOS 500 MOVIMIENTOS VALIDOS) ---\n";
    cout << "Posicion inicial Ak = a2, n = 500 movimientos.\n";

    string res500 = verificarMovimientos("a2", 500, movs500);
    cout << "Resultado de verificacion (500 movimientos): " << res500 << "\n";

    auto inicio = chrono::high_resolution_clock::now();
    for (int rep = 0; rep < 10; rep++) {
        verificarMovimientos("a2", 500, movs500);
    }
    auto fin = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracionMs = fin - inicio;

    cout << fixed << setprecision(5);
    cout << "Tiempo total de ejecucion (10 x 500 movs): " << duracionMs.count() << " ms.\n";
    cout << "Tiempo promedio por verificacion:          " << (duracionMs.count() / 10.0) << " ms.\n";

    // 3. Prueba con salto fallido inyectado en el movimiento 250
    cout << "\n--- PRUEBA ADICIONAL: MOVIMIENTO FALLIDO EN SECUENCIA LARGA ---\n";
    stringstream ss(movs500);
    string token;
    vector<string> vecMovs;
    while (ss >> token) {
        vecMovs.push_back(token);
    }

    if (vecMovs.size() >= 250) {
        vecMovs[249] = "Kh1";
    }

    string movsConFallo = "";
    for (const string& m : vecMovs) {
        movsConFallo += m + " ";
    }

    string resFallo = verificarMovimientos("a2", 500, movsConFallo);
    cout << "Resultado al inyectar fallo en el movimiento 250: " << resFallo << "\n";

    if (reporte.is_open()) {
        reporte << "\n--- MEDICION DE TIEMPO (10x500 movimientos desde a2) ---\n";
        reporte << "Resultado 500 validos: " << res500 << "\n";
        reporte << "Tiempo total:    " << duracionMs.count() << " ms.\n";
        reporte << "Tiempo promedio: " << (duracionMs.count() / 10.0) << " ms.\n";
        reporte << "Prueba con fallo inyectado: " << resFallo << "\n";
        reporte.close();
        cout << "\n[OK] Resultados guardados exitosamente en 'resultados_de_cpp.txt'.\n";
    }

    // 4. MODO INTERACTIVO DE PRUEBA MANUAL
    cout << "\n========================================================\n";
    cout << "  ¿Desea probar una secuencia personalizada manualmente? (s/n): ";
    char opcion;
    if (cin >> opcion && (opcion == 's' || opcion == 'S')) {
        string akManual;
        int nManual;
        cout << "Ingrese posicion inicial (ej. a1): ";
        cin >> akManual;
        cout << "Ingrese cantidad de movimientos n: ";
        cin >> nManual;
        cout << "Ingrese la secuencia de movimientos separada por espacios (ej. Kc2 Kd4):\n> ";
        cin.ignore();
        string cadenaManual;
        getline(cin, cadenaManual);

        string resManual = verificarMovimientos(akManual, nManual, cadenaManual);
        cout << "\n--> RESULTADO: " << resManual << "\n";
    }

    return 0;
}