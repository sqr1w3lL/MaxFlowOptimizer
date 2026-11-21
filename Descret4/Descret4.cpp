#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <Windows.h>
#include <limits>

using namespace std;

const int N = 105;
int capacity[N][N];
int flow[N][N];
vector<int> adj[N];

void addEdge(int u, int v, int c) {
    capacity[u][v] = c;
    adj[u].push_back(v);
    adj[v].push_back(u); // для BFS
}

// ====== BFS для знаходження шляху збільшення потоку ======
int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({ s, numeric_limits<int>::max() });

    while (!q.empty()) {
        int cur = q.front().first;
        int f = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next] - flow[cur][next] > 0) {
                parent[next] = cur;
                int new_flow = min(f, capacity[cur][next] - flow[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({ next, new_flow });
            }
        }
    }
    return 0;
}

// ====== Знаходження максимального потоку (Edmonds-Karp) ======
int edmondsKarp(int s, int t) {
    int max_flow = 0;
    vector<int> parent(N);
    int new_flow;

    while ((new_flow = bfs(s, t, parent))) {
        max_flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            flow[prev][cur] += new_flow;
            flow[cur][prev] -= new_flow;
            cur = prev;
        }
    }

    return max_flow;
}

// ====== Побудова повного потоку (жадібно зверху вниз) ======
void buildFullFlow(int s, int t) {
    // Жадібно пробуємо усі можливі шляхи (в порядку номерів)
    bool updated;
    do {
        updated = false;
        vector<int> parent(N, -1);
        parent[s] = -2;
        queue<int> q;
        q.push(s);

        while (!q.empty()) {
            int cur = q.front(); q.pop();
            for (int next : adj[cur]) {
                if (parent[next] == -1 && capacity[cur][next] - flow[cur][next] > 0) {
                    parent[next] = cur;
                    q.push(next);
                }
            }
        }

        if (parent[t] != -1) {
            int cur = t;
            int path_flow = numeric_limits<int>::max();

            // знайти мін. залишкову ємність на шляху
            while (cur != s) {
                int prev = parent[cur];
                path_flow = min(path_flow, capacity[prev][cur] - flow[prev][cur]);
                cur = prev;
            }

            // пропустити потік
            cur = t;
            while (cur != s) {
                int prev = parent[cur];
                flow[prev][cur] += path_flow;
                flow[cur][prev] -= path_flow;
                cur = prev;
            }

            updated = true;
        }

    } while (updated);
}

int main() {

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    int source = 1, sink = 13;

    // Додаємо ребра згідно твоїх даних
    addEdge(1, 2, 2);
    addEdge(1, 3, 2);
    addEdge(1, 4, 7);
    addEdge(1, 5, 3);
    addEdge(2, 6, 3);
    addEdge(2, 7, 1);
    addEdge(3, 10, 3);
    addEdge(3, 8, 1);
    addEdge(4, 8, 4);
    addEdge(4, 9, 3);
    addEdge(5, 8, 1);
    addEdge(5, 9, 1);
    addEdge(6, 10, 4);
    addEdge(6, 11, 2);
    addEdge(7, 11, 1);
    addEdge(8, 11, 4);
    addEdge(8, 12, 3);
    addEdge(9, 12, 3);
    addEdge(10, 13, 3);
    addEdge(11, 13, 6);
    addEdge(12, 13, 4);

    // === 1. Побудова повного потоку ===
    buildFullFlow(source, sink);
    cout << "✅ Побудовано повний потік.\n";

    int fullFlow = 0;
    for (int i = 0; i < N; ++i)
        fullFlow += max(0, flow[i][sink]); // вхідні в sink
    cout << "🔵 Повний потік: " << fullFlow << "\n";

    // === 2. Оптимізація до максимального потоку ===
    int optimizedFlow = edmondsKarp(source, sink);
    cout << "🟢 Максимальний потік після оптимізації: " << optimizedFlow << "\n";

    return 0;
}
