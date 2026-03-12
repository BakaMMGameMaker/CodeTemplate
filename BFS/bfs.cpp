#include <bits/stdc++.h>
using namespace std;

// 基础 bfs
// [[1, [1 2 3 4]], [2, [3 5 6 7]], ... ]
int bfs(int n, const vector<vector<int> > &graph, int start, int target) {
    queue<int> q;
    vector<int> dist(n, -1);

    q.push(start);
    dist[start] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == target) return dist[u];

        for (int v : graph[u]) {
            if (dist[v] != -1) continue;
            dist[v] = dist[u] + 1; // 入队立即标记
            q.push(v);
        }
    }
    return -1;
}

// 网格 bfs
int shortestPath(const vector<vector<int> > &grid) {
    int m = static_cast<int>(grid.size()), n = static_cast<int>(grid.back().size());
    queue<pair<int, int> > q;
    vector dist(m, vector(n, -1));
    array<pair<int, int>, 4> dirs({{-1, 0}, {1, 0}, {0, -1}, {0, 1}});

    q.emplace(0, 0); // 起点为 0 0
    dist[0][0] = 0;
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
            if (grid[nx][ny] == 1) continue;  // 墙壁
            if (dist[nx][ny] != -1) continue; // 访问过
            dist[nx][ny] = dist[x][y] + 1;
            q.emplace(nx, ny);
        }
    }
    return dist[m - 1][n - 1];
}

// 多源 bfs
vector<vector<int> > updateMatrix(const vector<vector<int> > &mat) {
    int m = static_cast<int>(mat.size()), n = static_cast<int>(mat.back().size());
    queue<pair<int, int> > q;
    vector dist(m, vector(n, -1));
    array<pair<int, int>, 4> dirs({{-1, 0}, {1, 0}, {0, -1}, {0, 1}});

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (mat[i][j] == 0) { // Source
                q.emplace(i, j);
                dist[i][j] = 0;
            }
        }
    }

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (auto [dx, dy] : dirs) {
            int nx = x + dx, ny = y + dy;
            if (nx < 0 || nx >= m || ny < 0 || ny >= m) continue;
            if (dist[nx][ny] != -1) continue;

            dist[nx][ny] = dist[x][y] + 1;
            q.emplace(nx, ny);
        }
    }
    return dist;
}
