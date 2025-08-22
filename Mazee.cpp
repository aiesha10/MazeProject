#include <iostream>  
#include <cstdlib>  
#include <ctime>  

const int WIDTH = 21;
const int HEIGHT = 21;
const int NODES = WIDTH * HEIGHT;
const int MAX_DEG = 4;

char maze[HEIGHT][WIDTH];
char solvedMaze[HEIGHT][WIDTH];

// Adjacency list representation without vectors  
int adj[NODES][MAX_DEG];
int deg[NODES];

inline int toId(int x, int y) {
    return y * WIDTH + x;
}

inline void toCoord(int id, int& x, int& y) {
    x = id % WIDTH;
    y = id / WIDTH;
}

class Graph {
public:
    Graph() {
        for (int i = 0; i < NODES; ++i) {
            deg[i] = 0;
        }
    }

    void addEdge(int u, int v) {
        if (deg[u] < MAX_DEG) {
            adj[u][deg[u]++] = v;
        }
        if (deg[v] < MAX_DEG) {
            adj[v][deg[v]++] = u;
        }
    }

    bool dfs(int start, int goal, int parent[]) {
        bool visited[NODES] = { false };
        int stackArr[NODES];
        int top = 0;
        // initialize parent  
        for (int i = 0; i < NODES; ++i) {
            parent[i] = -1;
        }
        // push start  
        stackArr[top++] = start;
        visited[start] = true;

        while (top > 0) {
            int u = stackArr[--top];
            if (u == goal) {
                return true;
            }
            for (int i = 0; i < deg[u]; ++i) {
                int v = adj[u][i];
                if (!visited[v]) {
                    visited[v] = true;
                    parent[v] = u;
                    stackArr[top++] = v;
                }
            }
        }
        return false;
    }
};

Graph mazeGraph;

void initMaze() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            maze[y][x] = '#';
        }
    }
}

void printMaze() {
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (solvedMaze[y][x] == '*') {
                std::cout << "* ";
            }
            else if (maze[y][x] == '#') {
                std::cout << "||";
            }
            else {
                std::cout << "  ";
            }
        }
        std::cout << "\n";
    }
}

bool isValid(int x, int y) {
    return x > 0 && x < WIDTH - 1 && y > 0 && y < HEIGHT - 1;
}

void shuffleDirs(int dirs[], int n) {
    for (int i = 0; i < n; ++i) {
        int r = rand() % n;
        int tmp = dirs[i];
        dirs[i] = dirs[r];
        dirs[r] = tmp;
    }
}

void carve(int x, int y) {
    maze[y][x] = ' ';
    int dirs[4] = { 0, 1, 2, 3 };
    shuffleDirs(dirs, 4);

    for (int i = 0; i < 4; ++i) {
        int dx = 0, dy = 0;
        if (dirs[i] == 0) dy = -1;
        else if (dirs[i] == 1) dy = 1;
        else if (dirs[i] == 2) dx = -1;
        else dx = 1;

        int nx = x + dx * 2;
        int ny = y + dy * 2;
        if (isValid(nx, ny) && maze[ny][nx] == '#') {
            // Carve wall and path  
            maze[y + dy][x + dx] = ' ';
            // Add an edge between cell nodes  
            int u = toId(x, y);
            int v = toId(nx, ny);
            mazeGraph.addEdge(u, v);
            // Recurse  
            carve(nx, ny);
        }
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    initMaze();
    carve(1, 1);

    // Initialize solvedMaze  
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            solvedMaze[y][x] = maze[y][x];
        }
    }

    int parent[NODES];
    int start = toId(1, 1);
    int goal = toId(WIDTH - 2, HEIGHT - 2);

    if (mazeGraph.dfs(start, goal, parent)) {
        // Reconstruct path  
        for (int v = goal; v != -1; v = parent[v]) {
            int px, py;
            toCoord(v, px, py);
            solvedMaze[py][px] = '*';
        }
        std::cout << "Solved Maze:" << std::endl;
        printMaze();
    }
    else {
        std::cout << "No solution found!" << std::endl;
    }

    return 0;
}
