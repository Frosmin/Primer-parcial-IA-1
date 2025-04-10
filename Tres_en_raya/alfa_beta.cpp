#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

// Representación del tablero: 0 = vacío, 1 = X (MAX), 2 = O (MIN)
class Tablero {
private:
    vector<vector<int>> celdas;
    int turno; // 1 para X, 2 para O

public:
    Tablero() {
        // Inicializar tablero vacío
        celdas = vector<vector<int>>(3, vector<int>(3, 0));
        turno = 1; // Inicia X
    }
    
    // Constructor de copia
    Tablero(const Tablero& otro) {
        celdas = otro.celdas;
        turno = otro.turno;
    }
    
    bool hacerMovimiento(int fila, int columna) {
        if (fila < 0 || fila > 2 || columna < 0 || columna > 2 || celdas[fila][columna] != 0) {
            return false; // Movimiento inválido
        }
        
        celdas[fila][columna] = turno;
        turno = (turno == 1) ? 2 : 1; // Cambiar turno
        return true;
    }
    
    bool estaLleno() const {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (celdas[i][j] == 0) {
                    return false;
                }
            }
        }
        return true;
    }
    
    int obtenerGanador() const {
        // Revisar filas
        for (int i = 0; i < 3; i++) {
            if (celdas[i][0] != 0 && celdas[i][0] == celdas[i][1] && celdas[i][1] == celdas[i][2]) {
                return celdas[i][0];
            }
        }
        
        // Revisar columnas
        for (int j = 0; j < 3; j++) {
            if (celdas[0][j] != 0 && celdas[0][j] == celdas[1][j] && celdas[1][j] == celdas[2][j]) {
                return celdas[0][j];
            }
        }
        
        // Revisar diagonales
        if (celdas[0][0] != 0 && celdas[0][0] == celdas[1][1] && celdas[1][1] == celdas[2][2]) {
            return celdas[0][0];
        }
        
        if (celdas[0][2] != 0 && celdas[0][2] == celdas[1][1] && celdas[1][1] == celdas[2][0]) {
            return celdas[0][2];
        }
        
        return 0; // No hay ganador
    }
    
    bool juegoTerminado() const {
        return obtenerGanador() != 0 || estaLleno();
    }
    
    int obtenerTurno() const {
        return turno;
    }
    
    // Obtener movimientos posibles (celdas vacías)
    vector<pair<int, int>> obtenerMovimientosPosibles() const {
        vector<pair<int, int>> movimientos;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (celdas[i][j] == 0) {
                    movimientos.push_back(make_pair(i, j));
                }
            }
        }
        return movimientos;
    }
    
    void mostrar() const {
        cout << "-------------" << endl;
        for (int i = 0; i < 3; i++) {
            cout << "| ";
            for (int j = 0; j < 3; j++) {
                char simbolo = ' ';
                if (celdas[i][j] == 1) simbolo = 'X';
                if (celdas[i][j] == 2) simbolo = 'O';
                cout << simbolo << " | ";
            }
            cout << endl << "-------------" << endl;
        }
        cout << endl;
    }
};

// Algoritmo Minimax con poda Alfa-Beta
int alfaBeta(Tablero& tablero, int profundidad, int alfa, int beta, bool esMaximizador, int& nodosPodados) {
    int ganador = tablero.obtenerGanador();
    
    // Condiciones de término
    if (ganador == 1) return 10 - profundidad; // X gana (preferir victorias rápidas)
    if (ganador == 2) return -10 + profundidad; // O gana
    if (tablero.estaLleno()) return 0; // Empate
    
    // Mostrar el tablero actual en la búsqueda
    cout << "Evaluando tablero (profundidad " << profundidad << ", " 
         << (esMaximizador ? "MAX" : "MIN") << "):" << endl;
    tablero.mostrar();
    
    if (esMaximizador) {
        int mejorVal = numeric_limits<int>::min();
        auto movimientos = tablero.obtenerMovimientosPosibles();
        
        for (auto movimiento : movimientos) {
            Tablero nuevoTablero = tablero; // Crear copia
            nuevoTablero.hacerMovimiento(movimiento.first, movimiento.second);
            
            int val = alfaBeta(nuevoTablero, profundidad + 1, alfa, beta, false, nodosPodados);
            mejorVal = max(mejorVal, val);
            alfa = max(alfa, mejorVal);
            
            if (beta <= alfa) {
                cout << "¡PODA ALFA! en profundidad " << profundidad << " (alfa=" << alfa << ", beta=" << beta << ")" << endl;
                nodosPodados++;
                break;
            }
        }
        return mejorVal;
    } else {
        int mejorVal = numeric_limits<int>::max();
        auto movimientos = tablero.obtenerMovimientosPosibles();
        
        for (auto movimiento : movimientos) {
            Tablero nuevoTablero = tablero; // Crear copia
            nuevoTablero.hacerMovimiento(movimiento.first, movimiento.second);
            
            int val = alfaBeta(nuevoTablero, profundidad + 1, alfa, beta, true, nodosPodados);
            mejorVal = min(mejorVal, val);
            beta = min(beta, mejorVal);
            
            if (beta <= alfa) {
                cout << "¡PODA BETA! en profundidad " << profundidad << " (alfa=" << alfa << ", beta=" << beta << ")" << endl;
                nodosPodados++;
                break;
            }
        }
        return mejorVal;
    }
}

// Encontrar el mejor movimiento para el jugador actual (IA)
pair<int, int> encontrarMejorMovimiento(Tablero& tablero) {
    int mejorVal = (tablero.obtenerTurno() == 1) ? 
                   numeric_limits<int>::min() : 
                   numeric_limits<int>::max();
    
    pair<int, int> mejorMovimiento = make_pair(-1, -1);
    bool esMaximizador = (tablero.obtenerTurno() == 1); // X es maximizador
    
    int nodosPodados = 0;
    auto movimientos = tablero.obtenerMovimientosPosibles();
    
    for (auto movimiento : movimientos) {
        Tablero nuevoTablero = tablero; // Crear copia
        nuevoTablero.hacerMovimiento(movimiento.first, movimiento.second);
        
        cout << "\n\nEvaluando movimiento: [" << movimiento.first << ", " << movimiento.second << "]" << endl;
        
        int valorMovimiento = alfaBeta(nuevoTablero, 0, 
                             numeric_limits<int>::min(), 
                             numeric_limits<int>::max(), 
                             !esMaximizador, nodosPodados);
        
        cout << "Valor del movimiento [" << movimiento.first << ", " << movimiento.second << "]: " 
             << valorMovimiento << endl;
        
        if ((esMaximizador && valorMovimiento > mejorVal) || 
            (!esMaximizador && valorMovimiento < mejorVal)) {
            mejorVal = valorMovimiento;
            mejorMovimiento = movimiento;
        }
    }
    
    cout << "Total de nodos podados: " << nodosPodados << endl;
    return mejorMovimiento;
}

int main() {
    Tablero tablero;
    char jugarPrimero;
    
    cout << "Bienvenido al juego del Tres en Raya con algoritmo Alfa-Beta" << endl;
    cout << "¿Quieres jugar primero? (s/n): ";
    cin >> jugarPrimero;
    
    bool turnoHumano = (jugarPrimero == 's' || jugarPrimero == 'S');
    char simboloHumano = turnoHumano ? 'X' : 'O';
    
    cout << "Juegas con " << simboloHumano << endl;
    
    while (!tablero.juegoTerminado()) {
        tablero.mostrar();
        
        if (turnoHumano) {
            int fila, columna;
            bool movimientoValido = false;
            
            while (!movimientoValido) {
                cout << "Tu turno. Ingresa fila (0-2) y columna (0-2): ";
                cin >> fila >> columna;
                
                movimientoValido = tablero.hacerMovimiento(fila, columna);
                if (!movimientoValido) {
                    cout << "Movimiento inválido. Intenta de nuevo." << endl;
                }
            }
        } else {
            cout << "Turno de la IA..." << endl;
            auto mejorMovimiento = encontrarMejorMovimiento(tablero);
            tablero.hacerMovimiento(mejorMovimiento.first, mejorMovimiento.second);
            cout << "La IA ha elegido la posición: [" << mejorMovimiento.first << ", " 
                 << mejorMovimiento.second << "]" << endl;
        }
        
        turnoHumano = !turnoHumano;
    }
    
    // Mostrar resultado final
    tablero.mostrar();
    int ganador = tablero.obtenerGanador();
    
    if (ganador == 0) {
        cout << "¡Empate!" << endl;
    } else {
        char simboloGanador = (ganador == 1) ? 'X' : 'O';
        cout << "¡" << simboloGanador << " ha ganado!" << endl;
    }
    
    return 0;
}