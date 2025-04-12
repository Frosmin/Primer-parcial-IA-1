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
    
    // Obtener valor de una celda
    int obtenerCelda(int fila, int columna) const {
        return celdas[fila][columna];
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

// Función para evaluar un movimiento usando estrategia greedy
int evaluarMovimiento(Tablero& tablero, int fila, int columna) {
    int jugadorActual = tablero.obtenerTurno();
    
    // Probar el movimiento
    Tablero nuevoTablero = tablero;
    if (!nuevoTablero.hacerMovimiento(fila, columna)) {
        return -1000; // Movimiento inválido
    }
    
    // Verificar si gano con este movimiento
    if (nuevoTablero.obtenerGanador() == jugadorActual) {
        return 100; // Movimiento ganador (máxima prioridad)
    }
    
    // Verificar si bloqueo una victoria del oponente
    int oponente = (jugadorActual == 1) ? 2 : 1;
    
    // Contar cuántas fichas propias y del oponente hay en filas, columnas y diagonales
    int puntaje = 0;
    
    // Valorar posiciones estratégicas (centro y esquinas)
    if (fila == 1 && columna == 1) puntaje += 5; // Centro
    if ((fila == 0 && columna == 0) || 
        (fila == 0 && columna == 2) || 
        (fila == 2 && columna == 0) || 
        (fila == 2 && columna == 2)) puntaje += 3; // Esquinas
    
    // Contar fichas en la fila
    int fichasPropias = 0;
    int fichasOponente = 0;
    for (int j = 0; j < 3; j++) {
        int valor = (j == columna) ? jugadorActual : tablero.obtenerCelda(fila, j);
        if (valor == jugadorActual) fichasPropias++;
        else if (valor == oponente) fichasOponente++;
    }
    
    // Potencial de victoria en fila
    if (fichasPropias == 2 && fichasOponente == 0) puntaje += 10;
    // Potencial de bloqueo en fila
    if (fichasOponente == 2 && fichasPropias == 0) puntaje += 20;
    
    // Contar fichas en la columna
    fichasPropias = 0;
    fichasOponente = 0;
    for (int i = 0; i < 3; i++) {
        int valor = (i == fila) ? jugadorActual : tablero.obtenerCelda(i, columna);
        if (valor == jugadorActual) fichasPropias++;
        else if (valor == oponente) fichasOponente++;
    }
    
    // Potencial de victoria en columna
    if (fichasPropias == 2 && fichasOponente == 0) puntaje += 10;
    // Potencial de bloqueo en columna
    if (fichasOponente == 2 && fichasPropias == 0) puntaje += 20;
    
    // Contar fichas en diagonal principal (si aplica)
    if (fila == columna) {
        fichasPropias = 0;
        fichasOponente = 0;
        for (int i = 0; i < 3; i++) {
            int valor = (i == fila) ? jugadorActual : tablero.obtenerCelda(i, i);
            if (valor == jugadorActual) fichasPropias++;
            else if (valor == oponente) fichasOponente++;
        }
        
        // Potencial de victoria en diagonal
        if (fichasPropias == 2 && fichasOponente == 0) puntaje += 10;
        // Potencial de bloqueo en diagonal
        if (fichasOponente == 2 && fichasPropias == 0) puntaje += 20;
    }
    
    // Contar fichas en diagonal secundaria (si aplica)
    if (fila + columna == 2) {
        fichasPropias = 0;
        fichasOponente = 0;
        for (int i = 0; i < 3; i++) {
            int valor = (i == fila) ? jugadorActual : tablero.obtenerCelda(i, 2-i);
            if (valor == jugadorActual) fichasPropias++;
            else if (valor == oponente) fichasOponente++;
        }
        
        // Potencial de victoria en diagonal
        if (fichasPropias == 2 && fichasOponente == 0) puntaje += 10;
        // Potencial de bloqueo en diagonal
        if (fichasOponente == 2 && fichasPropias == 0) puntaje += 20;
    }
    
    return puntaje;
}

// Encontrar el mejor movimiento usando estrategia greedy
pair<int, int> encontrarMejorMovimientoGreedy(Tablero& tablero) {
    auto movimientos = tablero.obtenerMovimientosPosibles();
    pair<int, int> mejorMovimiento = make_pair(-1, -1);
    int mejorPuntaje = -1000;
    
    cout << "\nEvaluando movimientos con estrategia greedy:" << endl;
    
    for (auto movimiento : movimientos) {
        int puntaje = evaluarMovimiento(tablero, movimiento.first, movimiento.second);
        
        cout << "Movimiento [" << movimiento.first << ", " << movimiento.second 
             << "] tiene un puntaje de: " << puntaje << endl;
        
        if (puntaje > mejorPuntaje) {
            mejorPuntaje = puntaje;
            mejorMovimiento = movimiento;
        }
    }
    
    return mejorMovimiento;
}

int main() {
    Tablero tablero;
    char jugarPrimero;
    
    cout << "Bienvenido al juego del Tres en Raya con algoritmo Greedy" << endl;
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
            cout << "Turno de la IA (usando estrategia greedy)..." << endl;
            auto mejorMovimiento = encontrarMejorMovimientoGreedy(tablero);
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