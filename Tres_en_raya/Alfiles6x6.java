public class Alfiles6x6 {
    static int N = 6; // Tamaño del tablero
    static char[][] tablero = new char[N][N];

    public static void main(String[] args) {
        // Inicializar el tablero vacío
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                tablero[i][j] = '.';

        // Colocar alfiles usando backtracking
        if (colocarAlfiles(0, 0, 0)) {
            imprimirTablero();
        } else {
            System.out.println("No se encontró una solución.");
        }
    }

    // Función recursiva para colocar alfiles
    public static boolean colocarAlfiles(int fila, int col, int alfilesColocados) {
        if (alfilesColocados == 10) return true; // Máximo número de alfiles encontrados----BUSCANDO ESTA CANTIDAD DE ALFILES

        // Si hemos salido del tablero, terminamos
        if (fila >= N) return false;

        int siguienteFila = (col == N - 1) ? fila + 1 : fila;
        int siguienteColumna = (col == N - 1) ? 0 : col + 1;

        // Intentar colocar un alfil en esta celda
        if (esSeguro(fila, col)) {
            tablero[fila][col] = 'A'; // Colocar alfil
            if (colocarAlfiles(siguienteFila, siguienteColumna, alfilesColocados + 1))
                return true;
            tablero[fila][col] = '.'; // Retroceder (backtracking)
        }

        // Probar siguiente celda sin colocar alfil
        return colocarAlfiles(siguienteFila, siguienteColumna, alfilesColocados);
    }

    // Verifica si un alfil puede colocarse en (fila, col)
    public static boolean esSeguro(int fila, int col) {
        // Revisar diagonales
        for (int i = 1; i < N; i++) {
            if (esValido(fila - i, col - i) && tablero[fila - i][col - i] == 'A') return false;
            if (esValido(fila - i, col + i) && tablero[fila - i][col + i] == 'A') return false;
            if (esValido(fila + i, col - i) && tablero[fila + i][col - i] == 'A') return false;
            if (esValido(fila + i, col + i) && tablero[fila + i][col + i] == 'A') return false;
        }
        return true;
    }

    // Verifica si una posición está dentro del tablero
    public static boolean esValido(int fila, int col) {
        return fila >= 0 && fila < N && col >= 0 && col < N;
    }

    // Imprime el tablero en consola
    public static void imprimirTablero() {
        for (char[] fila : tablero) {
            for (char celda : fila) {
                System.out.print(celda + " ");
            }
            System.out.println();
        }
    }
}
