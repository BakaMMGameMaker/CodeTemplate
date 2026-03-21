/*
 * 有 m 行 n 列的网格，起点为 bx, by，终点 ex, ey
 * 玩家进行 q 次放置障碍物操作，每次都在 xi, yi 位置放一个障碍物
 * 如果放完之后还有完全没有障碍物的路径，输出 ok
 * 否则输出 blocked，并按顺序输出最优路径上的所有障碍物的坐标
 * 最优路径的判断标准：
 * 1 - 障碍物数量最少
 * 2 - 如果有多条路径障碍物数量一致，选择路径最短的
 * 3 - 如果仍有多条合法路径，选择字典序最小的
 * 不在起点上放障碍物
 */

#include <string>
#include <vector>
#include <cstdint>
#include <queue>
using namespace std;

class Solution {
public:
    struct QueryAnswer {
        bool ok; // 有无障碍通路
        vector<pair<int, int> > blocks;
    };

    vector<QueryAnswer> processQueries(
        int m, int n,
        int bx, int by,
        int ex, int ey,
        vector<vector<int> > &queries) {
        constexpr int INF = INT32_MAX;
        const int BASE = 1LL * m * n + 1;

        auto id = [&](int x, int y) { return x * n + y; };

        auto inside = [&](int x, int y) { return x >= 0 and x < m and y >= 0 and y < n; };

        vector obs(m, vector(n, 0)); // obstacle
        vector<QueryAnswer> ans;

        int S = id(bx, by);
        int T = id(ex, ey);

        int dx[4] = {-1, 0, 0, 1};
        int dy[4] = {0, -1, 1, 0};

        // 从起点到所有点的最小代价
        auto dijkstraFromStart = [&](vector<long long> &dist) {
            int N = m * n;
            dist.assign(N, INF);

            // 小根堆 {cost, gridid}
            // 优先节点而非普通队列：dijkstra 优先拓展当前代价最小的节点
            priority_queue<pair<long long, int>, vector<pair<long long, int> >, greater<pair<long long, int> > > pq;
            dist[S] = 0;
            pq.push({0, S});

            while (!pq.empty()) {
                auto [d, u] = pq.top();
                pq.pop();
                if (d != dist[u]) continue; // 过时节点

                int x = u / n, y = u % n;
                for (int k = 0; k < 4; ++k) {
                    int nx = x + dx[k], ny = y + dy[k];
                    if (!inside(nx, ny)) continue;
                    int v = id(nx, ny);
                    long long w = 1LL * obs[nx][ny] * BASE + 1; // 进入 v 的代价
                    // 和普通 BFS，设置初始距离为 -1，距离不是 -1 就是访问过不一样
                    // 这里可能后期才会找到更优解，因为最优解的首要要求是障碍物最少
                    // 所以要不断尝试更新格子代价，即便格子之前被访问过
                    // 一旦格子被更新，就意味着有障碍物更少的路被找到了
                    // 那么原本在队列里的旧节点就没用了
                    if (dist[v] > dist[u] + w) {
                        dist[v] = dist[u] + w;
                        pq.push({dist[v], v});
                    }
                }
            }
        };

        // 从任一点到终点的最小代价
        auto dijkstraToTarget = [&](vector<long long> &dist) {
            // dist[u] = 从 u 到 T 的最小代价
            // 反图转移：若 u -> v 正向可走，则在反图里 v -> u 的代价是进入 v 的代价
            int N = m * n;
            dist.assign(N, INF);
            priority_queue<pair<long long, int>, vector<pair<long long, int> >, greater<pair<long long, int> > > pq;
            dist[T] = 0;
            pq.push({0, T});

            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();
                if (d != dist[v]) continue;

                int x = v / n, y = v % n;
                for (int k = 0; k < 4; ++k) {
                    int px = x + dx[k], py = y + dy[k];
                    if (!inside(px, py)) continue;
                    int u = id(px, py);

                    // 正向 u -> v 的代价 = obstacle[v] * BASE + 1
                    long long w = 1LL * obs[x][y] * BASE + 1;
                    if (dist[u] > dist[v] + w) {
                        dist[u] = dist[v] + w;
                        pq.push({dist[u], u});
                    }
                }
            }
        };

        // 还原最优路径 - 在所有总代价为 distS[T] 的路径中选择字典序最小的一条
        // 依赖 1 - distS，从起点到 u 的最小总代价
        // 依赖 2 - distT，从 u 走到终点的最小总代价
        auto rebuildBestPath =
                [&](const vector<long long> &distS, const vector<long long> &distT) {
            // 流程 - 恢复 fullPath
            // 筛选出障碍物坐标
            vector<pair<int, int> > fullPath;
            vector<pair<int, int> > blockCoords;

            int cur = S;
            fullPath.push_back({bx, by});

            while (cur != T) {
                int x = cur / n, y = cur % n;

                pair bestPos = {INT_MAX, INT_MAX};
                int bestId = -1;

                for (int k = 0; k < 4; ++k) {
                    int nx = x + dx[k], ny = y + dy[k];
                    if (!inside(nx, ny)) continue;

                    int v = id(nx, ny);
                    long long w = 1LL * obs[nx][ny] * BASE + 1;

                    // 注意不是 distS[v] + distT[v] == distS[v]
                    // 因为上式子只说明 v 出现在了某条最优路径中
                    // 但不一定是通过 cur 到达的
                    if (distS[cur] + w + distT[v] == distS[T]) {
                        if (make_pair(nx, ny) < bestPos) {
                            bestPos = {nx, ny};
                            bestId = v;
                        }
                    }
                }
                cur = bestId;
                fullPath.push_back(bestPos);
            }

            for (auto &p : fullPath) {
                if (obs[p.first][p.second]) {
                    blockCoords.push_back(p);
                }
            }
            return blockCoords;
        };

        for (auto &q : queries) {
            int x = q[0], y = q[1];
            obs[x][y] = 1;

            vector<long long> distS;
            dijkstraFromStart(distS);

            long long best = distS[T];
            long long minObstacles = best / BASE; // 因为 best = obs*BASE + len，且 len < BASE

            if (minObstacles == 0) {
                ans.push_back({true, {}});
            } else {
                vector<long long> distT;
                dijkstraToTarget(distT);
                vector<pair<int, int> > blocks =
                        rebuildBestPath(distS, distT);
                ans.push_back({false, blocks});
            }
        }
        return ans;
    }
};
