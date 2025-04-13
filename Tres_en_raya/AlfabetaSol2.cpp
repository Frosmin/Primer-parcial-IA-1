#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

using Tablero = vector<vector<char>>;
using Movimiento = pair<int, int>;

Tablero rotar90(const Tablero& tablero) {
    Tablero rotado(3, vector<char>(3));
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            rotado[j][2 - i] = tablero[i][j];
    return rotado;
}

Tablero reflejar(const Tablero& tablero) {
    Tablero reflejado = tablero;
    for (auto& fila : reflejado)
        reverse(fila.begin(), fila.end());
    return reflejado;
}

vector<Tablero> variantesTablero(const Tablero& tablero) {
    vector<Tablero> variantes;
    Tablero actual = tablero;
    for (int i = 0; i < 4; ++i) {
        variantes.push_back(actual);
        actual = rotar90(actual);
    }
    actual = reflejar(tablero);
    for (int i = 0; i < 4; ++i) {
        variantes.push_back(actual);
        actual = rotar90(actual);
    }
    return variantes;
}

string tableroAString(const Tablero& tablero) {
    string s;
    for (const auto& fila : tablero)
        for (char c : fila)
            s += c;
    return s;
}

vector<Movimiento> obtenerMovimientosSimetricos(const Tablero& tablero) {
    set<string> vistos;
    vector<Movimiento> resultado;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (tablero[i][j] != ' ') continue;
            Tablero temp = tablero;
            temp[i][j] = '*';
            auto variantes = variantesTablero(temp);
            string minVariante = tableroAString(variantes[0]);
            for (const auto& v : variantes)
                minVariante = min(minVariante, tableroAString(v));
            if (vistos.count(minVariante) == 0) {
                vistos.insert(minVariante);
                resultado.emplace_back(i, j);
            }
        }
    }
    return resultado;
}

int contarLineasGanadoras(const Tablero& tablero, char jugador) {
    int total = 0;
    for (int i = 0; i < 3; ++i) {
        if ((tablero[i][0] == jugador || tablero[i][0] == ' ') &&
            (tablero[i][1] == jugador || tablero[i][1] == ' ') &&
            (tablero[i][2] == jugador || tablero[i][2] == ' ')) total++;
        if ((tablero[0][i] == jugador || tablero[0][i] == ' ') &&
            (tablero[1][i] == jugador || tablero[1][i] == ' ') &&
            (tablero[2][i] == jugador || tablero[2][i] == ' ')) total++;
    }
    if ((tablero[0][0] == jugador || tablero[0][0] == ' ') &&
        (tablero[1][1] == jugador || tablero[1][1] == ' ') &&
        (tablero[2][2] == jugador || tablero[2][2] == ' ')) total++;
    if ((tablero[0][2] == jugador || tablero[0][2] == ' ') &&
        (tablero[1][1] == jugador || tablero[1][1] == ' ') &&
        (tablero[2][0] == jugador || tablero[2][0] == ' ')) total++;
    return total;
}

int evaluar(const Tablero& tablero) {
    return contarLineasGanadoras(tablero, 'X') - contarLineasGanadoras(tablero, 'O');
}

Tablero aplicarMovimiento(const Tablero& tablero, Movimiento mov, char ficha) {
    Tablero nuevo = tablero;
    nuevo[mov.first][mov.second] = ficha;
    return nuevo;
}

void imprimirTablero(const Tablero& tablero) {
    cout << "  1   2   3\n";
    for (int i = 0; i < 3; ++i) {
        cout << static_cast<char>('A' + i) << " ";
        for (int j = 0; j < 3; ++j) {
            cout << tablero[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "  --+---+--\n";
    }
    cout << "\n";
}

bool hayGanador(const Tablero& tablero, char jugador) {
    for (int i = 0; i < 3; ++i) {
        if (tablero[i][0] == jugador && tablero[i][1] == jugador && tablero[i][2] == jugador) return true;
        if (tablero[0][i] == jugador && tablero[1][i] == jugador && tablero[2][i] == jugador) return true;
    }
    if (tablero[0][0] == jugador && tablero[1][1] == jugador && tablero[2][2] == jugador) return true;
    if (tablero[0][2] == jugador && tablero[1][1] == jugador && tablero[2][0] == jugador) return true;
    return false;
}

bool esEmpate(const Tablero& tablero) {
    for (const auto& fila : tablero)
        for (char c : fila)
            if (c == ' ') return false;
    return true;
}

Movimiento razonamientoAgente(const Tablero& tableroEI) {
    vector<Movimiento> jugadasMAX = obtenerMovimientosSimetricos(tableroEI);
    vector<int> valoresMAX;

    cout << "--------------{ La máquina está pensando... }--------------\n";
    cout << "Raíz (Estado Inicial - Ei)\n";
    for (const auto& fila : tableroEI) {
        for (char c : fila) cout << "[" << c << "]";
        cout << "\n";
    }

    int mejorValor = numeric_limits<int>::min();
    Movimiento mejorJugada = {-1, -1};

    for (size_t i = 0; i < jugadasMAX.size(); ++i) {
        Tablero tableroMAX = aplicarMovimiento(tableroEI, jugadasMAX[i], 'X');

        vector<Movimiento> jugadasMIN;
        if (i == 0) {
            jugadasMIN = obtenerMovimientosSimetricos(tableroMAX);
        } else {
            vector<Movimiento> todos = obtenerMovimientosSimetricos(tableroMAX);
            if (!todos.empty()) jugadasMIN.push_back(todos[0]);
        }

        vector<Tablero> tablerosMIN;
        vector<int> valoresMIN;

        for (const auto& jugadaMIN : jugadasMIN) {
            Tablero tableroMIN = aplicarMovimiento(tableroMAX, jugadaMIN, 'O');
            int val = evaluar(tableroMIN);
            tablerosMIN.push_back(tableroMIN);
            valoresMIN.push_back(val);
        }

        int valorMin = valoresMIN.empty() ? evaluar(tableroMAX) : *min_element(valoresMIN.begin(), valoresMIN.end());
        valoresMAX.push_back(valorMin);

        string nodo = (i < jugadasMAX.size() - 1 ? "├──" : "└──");
        cout << nodo << " MAX: (" << jugadasMAX[i].first << "," << jugadasMAX[i].second << ") → Valor = " << valorMin << "\n";
        for (const auto& fila : tableroMAX) {
            cout << "│   "; for (char c : fila) cout << "[" << c << "]"; cout << "\n";
        }
        for (size_t j = 0; j < tablerosMIN.size(); ++j) {
            for (const auto& fila : tablerosMIN[j]) {
                cout << "│   │   "; for (char c : fila) cout << "[" << c << "]"; cout << "\n";
            }
            cout << "│   │   → f(v) = " << contarLineasGanadoras(tablerosMIN[j], 'X') << " - " << contarLineasGanadoras(tablerosMIN[j], 'O') << " = " << valoresMIN[j] << "\n";
        }

        if (valorMin > mejorValor) {
            mejorValor = valorMin;
            mejorJugada = jugadasMAX[i];
        }
    }

    cout << "\nEl valor Beta es: " << mejorValor << " en nodo MAX: (" << mejorJugada.first << ", " << mejorJugada.second << ")\n";
    cout << "Mejor jugada sugerida: (" << mejorJugada.first << ", " << mejorJugada.second << ")\n";
    cout << "Fin del razonamiento del agente.\n\n";
    return mejorJugada;
}

void movimientoUsuario(Tablero& tablero) {
    string entrada;
    while (true) {
        cout << "Introduce tu movimiento (ejemplo: A3): ";
        cin >> entrada;
        if (entrada.size() != 2) {
            cout << "Formato inválido.\n"; continue;
        }
        char fila = toupper(entrada[0]);
        char columna = entrada[1];
        if ((fila < 'A' || fila > 'C') || (columna < '1' || columna > '3')) {
            cout << "Coordenada fuera de rango. Usa formato A1, B2, etc.\n";
            continue;
        }
        int i = fila - 'A';
        int j = columna - '1';
        if (tablero[i][j] == ' ') {
            tablero[i][j] = 'O';
            break;
        } else {
            cout << "Movimiento inválido. Intenta nuevamente.\n";
        }
    }
}

int main() {
    Tablero tablero(3, vector<char>(3, ' '));
    cout << "¡Bienvenido al 3 en raya!\n\n";
    imprimirTablero(tablero);

    while (true) {
        Movimiento jugada = razonamientoAgente(tablero);
        tablero[jugada.first][jugada.second] = 'X';
        imprimirTablero(tablero);
        if (hayGanador(tablero, 'X')) {
            cout << "La máquina ha ganado.\n";
            break;
        }
        if (esEmpate(tablero)) {
            cout << "Empate.\n";
            break;
        }

        movimientoUsuario(tablero);
        imprimirTablero(tablero);
        if (hayGanador(tablero, 'O')) {
            cout << "¡Felicidades! Has ganado.\n";
            break;
        }
        if (esEmpate(tablero)) {
            cout << "Empate.\n";
            break;
        }
    }

    return 0;
}
