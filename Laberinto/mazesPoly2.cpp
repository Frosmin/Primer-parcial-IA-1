#include <bits/stdc++.h>

using namespace std;

#define int long long
#define FIO ios_base::sync_with_stdio(false); cin.tie(NULL);
#define pb push_back
#define all(v) v.begin(),v.end()
#define forn(i,n) for(int i=0;i<n;i++)
#define print(x) cout << x << " "
#define printl(x) cout << x << endl
#define input(x) cin >> x
#define printv(v) for(int i=0;i<v.size();i++) print(v[i]); cout << "\n";
#define vi vector<int>
#define vii vector<pair<int,int>>
#define F first 
#define S second
#define pii pair<int,int>
#define yes cout<<"YES"<<endl
#define no cout<<"NO"<<endl
#define ilz(x) 64-__builtin__clzll(x)-1   //usar -1 para shifts, solo index quitar -1


template<typename T> bool uin(T &a, T b) {return a>b?(a=b,true):false;}
template<typename T> bool uax(T &a, T b) {return a<b?(a=b,true):false;}

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
    {0, 1, 1, 2, 1, 1, 1, 1, 3, 1}
};
vector<vector<int>> maze2 = {
    {1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 3, 1}
};
vector<vector<int>> maze3 = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 0, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 0, 1, 1, 0, 1, 1, 0, 1},
    {2, 0, 0, 0, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 3, 1}
};

vector<vector<int>> maze4 = {
    {1, 2, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1, 0, 3},
    {1, 1, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 1, 0, 0, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 1, 1, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

vector<vi> getMaze(int i){
    if(i==0)return maze1;
    else if(i==1)return maze2;
    else if(i==2)return maze3;
    else return maze4;
}

void printBoard(vector<vi> board){
    for(int i = 0;i<board.size();i++){
        for(int j = 0;j<board[0].size();j++){
            if(board[i][j]==2)cout << 'R'<< " ";
            else if(board[i][j]==3)cout << 'Q' << " ";
            else cout << board[i][j] << " ";
        }
        cout << endl;
    }
}
void bfs(pii pIni, vector<vi> board){
    
    int n = board.size(), m = board[0].size();
    vector<vector<bool>> vis(n,vector<bool>(m,0));
    queue<pii> q;
    q.push({pIni.F,pIni.S});
    vis[pIni.F][pIni.S] = 1;
    int paso = 0;
    for(int i = 0;i<n;i++){
        for(int j = 0;j<m;j++){
            if(board[i][j]==1)vis[i][j]=1;
        }
    }
    while(!q.empty()){
        pii aux = q.front();
        int x = aux.F, y = aux.S;
        q.pop();
        cout << "PASO: " <<  ++paso << endl;
        cout << "POSICION ACTUAL (" <<  x << "," << y <<")" << endl; 
        printBoard(board);
        if(board[x][y]==3){
            cout << "LLEGO" << endl;
            return;
        }
        //escoger ruta
        
        if(x-1>=0){
            if(!vis[x-1][y] and board[x-1][y]!=1)q.push({x-1,y}),board[x-1][y] = board[x-1][y]==3?3:2,vis[x-1][y]=1;
        }
        if(x+1<n){
            if(!vis[x+1][y] and board[x+1][y]!=1)q.push({x+1,y}),board[x+1][y] = board[x+1][y]==3?3:2,vis[x+1][y] = 1;
        }
        if(y-1>=0){
            if(!vis[x][y-1] and board[x][y-1]!=1)q.push({x,y-1}),board[x][y-1] = board[x][y-1]==3?3:2,vis[x][y-1]=1;
        }
        if(y+1<m){
            if(!vis[x][y+1] and board[x][y+1]!=1)q.push({x,y+1}),board[x][y+1] = board[x][y+1]==3?3:2,vis[x][y+1]=1;
        }

        
    }
    cout << "NO LLEGO" << endl;
}

signed main(){
    vector<pair<int,pii>> allLab={{1,{9,3}},{2,{0,1}},{3,{5,0}},{4,{0,1}}};
    for(int i=0;i<allLab.size();i++){
        vector<vi> lab=getMaze(i);
        cout << "TABLERO NUMERO: " << i+1 << endl;
        bfs(allLab[i].S, lab);
        cout << "-------FIN------"<< endl;
    }
}