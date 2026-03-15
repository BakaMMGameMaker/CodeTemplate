#include <bits/stdc++.h>
using namespace std;

// 基础 bfs
// [[1, [1 2 3 4]], [2, [3 5 6 7]], ... ]
int bfs(int n, const vector<vector<int> > &graph, int start, int target) {
    queue<int> q;
    vector dist(n, -1);

    q.push(start);
    dist[start] = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == target) return dist[u];

        for (int v : graph[u]) {
            if (dist[v] != -1) continue; // 访问过了
            dist[v] = dist[u] + 1;       // 入队立即标记
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

// 状态压缩 bfs
// [[1, [1 2 3 4]], [2, [3 5 6 7]], ... ]
// leetcode 847
// 变体 - leetcode 864
int shortestPathLength(const vector<vector<int> > &graph) {
    // mask: 当前这条路径访问过哪一些点
    // 第 i 位表示节点 i 是否访问过
    int n = static_cast<int>(graph.size());
    int full = (1 << n) - 1;  // 所有节点都访问过的状态
    queue<pair<int, int> > q; // {node, mask}
    // dist 不再代表访问到 i j 网格时的最短步，而是代表访问到节点 u
    // 且已访问的集合为 mask 的时候的最短步
    vector dist(n, vector(1 << n, -1));

    // 把任意一个点都作为 bfs 的起点
    for (int i = 0; i < n; ++i) {
        int mask = 1 << i;
        q.emplace(i, mask);
        dist[i][mask] = 0;
    }

    while (!q.empty()) {
        auto [u, mask] = q.front();
        q.pop();

        if (mask == full) return dist[u][mask]; // 如果当前状态已经访问完了所有节点，直接返回最短步数

        for (int v : graph[u]) {
            int nextMask = mask | 1 << v; // 保留原本访问过的节点，标记新的节点已访问
            if (dist[v][nextMask] != -1) continue;
            dist[v][nextMask] = dist[u][mask] + 1;
            q.emplace(v, nextMask);
        }
    }
    return -1;
}

// 双向 bfs
// 题意 找到从 begin 到 end 的最短路径，begin 不一定在 wordList 中
int ladderLength(const string &beginWord, const string &endWord, const vector<string> &wordList) {
    unordered_set dict(wordList.begin(), wordList.end());
    if (!dict.contains(endWord)) return 0;

    // 双向 bfs
    // q1 q2 表示从起点/终点扩展的一整层
    // 而且需要高频判断某个状态是否位于对侧层中，所以不能用 queue
    unordered_set q1{beginWord}, q2{endWord}, visited{beginWord, endWord};
    int step = 1;

    while (!q1.empty() && !q2.empty()) {
        // 优先扩展元素更少的一边可以减少分支数量
        if (q1.size() > q2.size()) swap(q1, q2);

        unordered_set<string> next;
        for (string word : q1) {
            for (int i = 0; i < word.size(); ++i) {
                char old = word[i];
                for (char c = 'a'; c <= 'z'; c++) {
                    word[i] = c;
                    if (q2.contains(word)) return step + 1; // 对面也有这个单词位于队列中了
                    if (dict.contains(word) && !visited.contains(word)) {
                        visited.emplace(word);
                        next.emplace(word);
                    }
                }
                word[i] = old;
            }
        }
        q1 = std::move(next);
        step++;
    }
    return 0;
}
