#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int N = 3;
const int MAX_DEPTH = 3;
typedef vector<vector<char>> Board;

// Imprime el tablero con indentación según la profundidad
void printBoardWithIndent(const Board& board, int depth, int score) {
    string indent(depth * 2, ' ');
    cout << indent << "Evaluando (score = " << score << "):\n";
    for (const auto& row : board) {
        cout << indent;
        for (char cell : row)
            cout << cell << ' ';
        cout << '\n';
    }
    cout << '\n';
}

// Imprime el tablero bonito
void printBoard(const Board& board) {
    cout << "\n  0   1   2\n";
    for (int i = 0; i < N; ++i) {
        cout << i << " ";
        for (int j = 0; j < N; ++j) {
            cout << board[i][j];
            if (j < N - 1) cout << " | ";
        }
        cout << "\n";
        if (i < N - 1) cout << "  ---------\n";
    }
    cout << "\n";
}

bool isWin(const Board& board, char player) {
    for (int i = 0; i < N; ++i)
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
            return true;

    return (board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
           (board[0][2] == player && board[1][1] == player && board[2][0] == player);
}

bool isFull(const Board& board) {
    for (const auto& row : board)
        for (char cell : row)
            if (cell == '.') return false;
    return true;
}

// Algoritmo minimax con poda alfa-beta y profundidad para debug
int minimax(Board& board, bool isMaximizing, int alpha, int beta, int depth = 0) {
    if (isWin(board, 'O')) return +1;
    if (isWin(board, 'X')) return -1;
    if (isFull(board)) return 0;
    if (depth >= MAX_DEPTH) return 0;

    if (isMaximizing) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == '.') {
                    board[i][j] = 'O';
                    int score = minimax(board, false, alpha, beta, depth + 1);
                    printBoardWithIndent(board, depth, score);
                    board[i][j] = '.';
                    best = max(best, score);
                    alpha = max(alpha, best);
                    if (beta <= alpha) return best;
                }
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (board[i][j] == '.') {
                    board[i][j] = 'X';
                    int score = minimax(board, true, alpha, beta, depth + 1);
                    printBoardWithIndent(board, depth, score);
                    board[i][j] = '.';
                    best = min(best, score);
                    beta = min(beta, best);
                    if (beta <= alpha) return best;
                }
            }
        }
        return best;
    }
}

pair<int, int> findBestMove(Board& board) {
    int bestScore = numeric_limits<int>::min();
    pair<int, int> bestMove = {-1, -1};

    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            if (board[i][j] == '.') {
                board[i][j] = 'O';
                int score = minimax(board, false, numeric_limits<int>::min(), numeric_limits<int>::max(), 1);
                board[i][j] = '.';
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = {i, j};
                }
            }

    return bestMove;
}

int main() {
    Board board(N, vector<char>(N, '.'));
    char human = 'X';
    char bot = 'O';
    char currentPlayer = human;

    cout << "==============================\n";
    cout << " Bienvenido a 3 en raya  \n";
    cout << " Tu eres el jugador X         \n";
    cout << "==============================\n";
    printBoard(board);

    while (true) {
        if (currentPlayer == human) {
            int row, col;
            cout << "Tu turno (fila columna): ";
            cin >> row >> col;
            if (cin.fail() || row < 0 || row >= N || col < 0 || col >= N || board[row][col] != '.') {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Movimiento invalido. Intenta de nuevo.\n";
                continue;
            }
            board[row][col] = human;
        } else {
            pair<int, int> move = findBestMove(board);
            board[move.first][move.second] = bot;
            cout << "El bot juega en (" << move.first << ", " << move.second << ")\n";
        }

        printBoard(board);

        if (isWin(board, currentPlayer)) {
            cout << "El jugador " << currentPlayer << " gana!\n";
            break;
        }

        if (isFull(board)) {
            cout << "Empate!\n";
            break;
        }

        currentPlayer = (currentPlayer == human) ? bot : human;
    }

    return 0;
}
