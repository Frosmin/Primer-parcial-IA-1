#include <bits/stdc++.h>
using namespace std;

const int N = 3;
typedef vector<vector<char>> Board;

void printBoard(const Board& board) {
    cout << "\n";
    for (const auto& row : board) {
        for (char cell : row)
            cout << cell << ' ';
        cout << endl;
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

int minimax(Board& board, bool isMaximizing) {
    if (isWin(board, 'O')) return +1;
    if (isWin(board, 'X')) return -1;
    if (isFull(board)) return 0;

    if (isMaximizing) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (board[i][j] == '.') {
                    board[i][j] = 'O';
                    int score = minimax(board, false);
                    board[i][j] = '.';
                    best = max(best, score);
                }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                if (board[i][j] == '.') {
                    board[i][j] = 'X';
                    int score = minimax(board, true);
                    board[i][j] = '.';
                    best = min(best, score);
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
                int score = minimax(board, false);
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
    char currentPlayer = 'X';

    cout << "Bienvenido a 3 en raya contra el bot!\n";
    printBoard(board);

    while (true) {
        if (currentPlayer == human) {
            int row, col;
            cout << "Tu turno (fila columna): ";
            cin >> row >> col;
            if (row < 0 || row >= N || col < 0 || col >= N || board[row][col] != '.') {
                cout << "Movimiento invalido. Intenta otra vez.\n";
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

        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    return 0;
}
