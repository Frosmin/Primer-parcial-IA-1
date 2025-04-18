// %%writefile raton_laberinto.cpp
#include <iostream>
#include <vector>
#include <queue>
#include <stack>

using namespace std;

#define pii pair<int,int>
#define S second
#define F first
const int N = 10, M = 10;

// Laberinto 6x6: 0 = camino, 1 = muro
int laberinto[N][M] = {
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

vector<vector<int>> maze1 = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 1, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 1, 0, 1, 1, 0, 1, 0, 0, 1},
    {1, 1, 0, 0, 0, 0, 1, 0, 1, 1},
    {1, 0, 0, 1, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 1, 0, 1},
    {0, 1, 1, 0, 1, 1, 1, 1, 0, 1}
};

vector<vector<int>> maze2 = {
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 1}
};

vector<vector<int>> maze3 = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {0, 0, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 0, 1}
};

vector<vector<int>> maze4 = {
    {1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

struct Nodo {
    int x, y;
    vector<pair<int, int>> camino;
};

bool es_valido(int x, int y, vector<vector<bool>>& visitado, vector<vector<int>> &board) {
    return x >= 0 && x < N && y >= 0 && y < M &&
           board[x][y] == 0 && !visitado[x][y];
}

void imprimir_laberinto(const vector<pair<int, int>>& camino, int queso_x, int queso_y, vector<vector<int>> &board) {
    char mapa[N][M];
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            mapa[i][j] = (board[i][j] == 1) ? '#' : '.';

    for (auto [x, y] : camino)
        mapa[x][y] = '*';

    mapa[camino.front().first][camino.front().second] = 'R'; // Ratón fijo al inicio
    mapa[queso_x][queso_y] = 'Q'; // Queso fijo

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j)
            cout << mapa[i][j] << ' ';
        cout << '\n';
    }
}

void imprimir_estado(string metodo, const Nodo& actual, const vector<pair<int, int>>& frontera, int queso_x, int queso_y, vector<vector<int>> &board) {
    cout << "\n[" << metodo << "] Visitando: (" << actual.x << "," << actual.y << ")\n";
    cout << "[" << metodo << "] Camino actual: ";
    for (auto [x, y] : actual.camino)
        cout << "(" << x << "," << y << ") ";
    cout << "\n[" << metodo << "] Frontera: ";
    for (auto [x, y] : frontera)
        cout << "(" << x << "," << y << ") ";
    cout << "\n[" << metodo << "] Estado del laberinto:\n";
    imprimir_laberinto(actual.camino, queso_x, queso_y, board);
    cout << "------------------------\n";
}

void bfs(int inicio_x, int inicio_y, int queso_x, int queso_y, vector<vector<int>> &board) {
    cout << "\n=========== BFS ===========\n";
    queue<Nodo> cola;
    vector<vector<bool>> visitado(N, vector<bool>(M, false));

    cola.push({inicio_x, inicio_y, {{inicio_x, inicio_y}}});
    visitado[inicio_x][inicio_y] = true;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!cola.empty()) {
        Nodo actual = cola.front(); cola.pop();

        vector<pair<int, int>> frontera;
        queue<Nodo> temp = cola;
        while (!temp.empty()) {
            frontera.push_back({temp.front().x, temp.front().y});
            temp.pop();
        }

        imprimir_estado("BFS", actual, frontera, queso_x, queso_y, board);

        if (actual.x == queso_x && actual.y == queso_y) {
            cout << "\n*********************************************\n";
            cout << "*                                           *\n";
            cout << "*       [BFS]  CAMINO ENCONTRADO!           *\n";
            cout << "*                                           *\n";
            cout << "*********************************************\n";
            return;
        }

        for (int i = 0; i < 4; i++) {
            int nx = actual.x + dx[i];
            int ny = actual.y + dy[i];
            if (es_valido(nx, ny, visitado, board)) {
                visitado[nx][ny] = true;
                auto nuevo_camino = actual.camino;
                nuevo_camino.push_back({nx, ny});
                cola.push({nx, ny, nuevo_camino});
            }
        }
    }

    cout << "\n*********************************************\n";
    cout << "*                                           *\n";
    cout << "*       [BFS] No se encontro camino.        *\n";
    cout << "*                                           *\n";
    cout << "*********************************************\n";
}

void dfs(int inicio_x, int inicio_y, int queso_x, int queso_y, vector<vector<int>> &board) {
    cout << "\n=========== DFS ===========\n";
    stack<Nodo> pila;
    vector<vector<bool>> visitado(N, vector<bool>(M, false));

    pila.push({inicio_x, inicio_y, {{inicio_x, inicio_y}}});
    visitado[inicio_x][inicio_y] = true;

    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};

    while (!pila.empty()) {
        Nodo actual = pila.top(); pila.pop();

        vector<pair<int, int>> frontera;
        stack<Nodo> temp = pila;
        while (!temp.empty()) {
            frontera.push_back({temp.top().x, temp.top().y});
            temp.pop();
        }

        imprimir_estado("DFS", actual, frontera, queso_x, queso_y, board);

        if (actual.x == queso_x && actual.y == queso_y) {
            cout << "\n*********************************************\n";
            cout << "*                                           *\n";
            cout << "*       [DFS]  Camino encontrado!           *\n";
            cout << "*                                           *\n";
            cout << "*********************************************\n";
            return;
        }

        for (int i = 3; i >= 0; i--) {
            int nx = actual.x + dx[i];
            int ny = actual.y + dy[i];
            if (es_valido(nx, ny, visitado, board)) {
                visitado[nx][ny] = true;
                auto nuevo_camino = actual.camino;
                nuevo_camino.push_back({nx, ny});
                pila.push({nx, ny, nuevo_camino});
            }
        }
    }

    cout << "\n*********************************************\n";
    cout << "*                                           *\n";
    cout << "*       [DFS]  No se encontro camino.       *\n";
    cout << "*                                           *\n";
    cout << "*********************************************\n";
}

vector<vector<int>> getMaze(int i){
    if(i==0)return maze1;
    else if(i==1)return maze2;
    else if(i==2)return maze3;
    else return maze4;
}

int main() {
    vector<pair<int, pair<pii, pii>>> allLab = {
        {1, {{9, 3}, {9, 8}}},
        {2, {{0, 1}, {9, 8}}},
        {3, {{5, 0}, {9, 8}}},
        {4, {{0, 1}, {2, 9}}}
    };

    while (true) {
        cout << "#############################################\n";
        cout << "Seleccione el laberinto que desea resolver:\n";
        cout << "1. Laberinto 1\n";
        cout << "2. Laberinto 2\n";
        cout << "3. Laberinto 3\n";
        cout << "4. Laberinto 4\n";
        cout << "5. Salir\n";
        cout << "#############################################\n";
        cout << "Ingrese su opcion: ";

        int opcion;
        cin >> opcion;

        if (opcion == 5) {
            cout << "Saliendo del programa...\n";
            break;
        }

        if (opcion < 1 || opcion > 4) {
            cout << "Opcion invalida. Intente de nuevo.\n";
            continue;
        }

        cout << "#############################################\n";
        cout << "Este es el laberinto numero: " << opcion << endl;
        cout << "#############################################\n";

        vector<vector<int>> lab = getMaze(opcion - 1);
        int inicio_x = allLab[opcion - 1].S.F.F, inicio_y = allLab[opcion - 1].S.F.S;
        int queso_x = allLab[opcion - 1].S.S.F, queso_y = allLab[opcion - 1].S.S.S;

        cout << "Seleccione el algoritmo que desea usar:\n";
        cout << "1. BFS (Breadth-First Search)\n";
        cout << "2. DFS (Depth-First Search)\n";
        cout << "Ingrese su opcion: ";

        int algoritmo;
        cin >> algoritmo;

        if (algoritmo == 1) {
            bfs(inicio_x, inicio_y, queso_x, queso_y, lab);
        } else if (algoritmo == 2) {
            dfs(inicio_x, inicio_y, queso_x, queso_y, lab);
        } else {
            cout << "Opcion invalida. Regresando al menu principal.\n";
            continue;
        }

        cout << "----------------------------------------------\n";
    }

    return 0;
}
