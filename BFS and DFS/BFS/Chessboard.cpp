/*
 * 六边形棋盘，0-base 奇数行往右偏移半格宽度，共 6 行 6 列，每个格子中的编号为 col, row
 * n 个攻方棋子，id 从 0 到 n - 1，m个守方棋子，id 从 n 到 n + m - 1
 * 每个棋子有以下属性，初始 col, 初始row, hp, atk, def, spd, rng
 * 分别代表初始坐标，生命值，攻击力，防御值，速度，攻击范围
 * 同一个格子里只能存在一个棋子，被棋子霸占的格子为“已占据”状态
 * a 棋子进攻 b 棋子时，b 棋子受到的伤害为 a.atk * a.atk / (a.atk + b.def)，向下取整且至少为 1
 * 如果 b 棋子死亡，立即离场并更新棋盘，如果 b 棋子还未移动，本帧内不可再移动，同时立即对游戏结束状态进行判定
 * 同一帧内，场上所有的存活棋子都尝试行动，并且速度更高的棋子优先行动，如果多个棋子速度一致，id 小的先行动
 * 一个棋子的行动逻辑如下，先寻找离自己最近的敌方棋子 (无视障碍)，找到后将其标记为此次行动的 target
 * 如果其位于攻击范围内，即 dist<=rng，直接发动进攻
 * 如果不在攻击范围内，从周围 6 个格子选择最‘合适’的格子并移动，只要存在合法邻居就必须移动
 * ‘合适’的判断标准为: 目标格子必须是所有候选格子中离 target 最近的，并且不处于占据状态
 */

#include <algorithm>
#include <climits>
#include <cstring>
#include <queue>
#include <vector>

class Solution {
public:
    // 棋子
    struct Piece {
        int id;
        int side; // 0: 进攻方, 1: 防守方
        int col, row;
        long long hp;
        int atk, def, spd, rng;
        bool alive;
    };

    // 棋盘尺寸
    static constexpr int W = 6;
    static constexpr int H = 6;

    // 是否位于棋盘内部
    bool inBoard(int c, int r) {
        return c >= 0 && c < W && r >= 0 && r < H;
    }

    // 按照优先级获取给定格子的六个邻居
    // 右上、右、右下、左下、左、左上
    std::vector<std::pair<int, int> > getNeighbors(int c, int r) {
        std::vector<std::pair<int, int> > res;
        if (r & 1) {
            // odd row
            res.push_back({c + 1, r - 1}); // 右上
            res.push_back({c + 1, r});     // 右
            res.push_back({c + 1, r + 1}); // 右下
            res.push_back({c, r + 1});     // 左下
            res.push_back({c - 1, r});     // 左
            res.push_back({c, r - 1});     // 左上
        } else {
            // even row
            res.push_back({c, r - 1});     // 右上
            res.push_back({c + 1, r});     // 右
            res.push_back({c, r + 1});     // 右下
            res.push_back({c - 1, r + 1}); // 左下
            res.push_back({c - 1, r});     // 左
            res.push_back({c - 1, r - 1}); // 左上
        }
        return res;
    }

    // 通过 bfs 获取两个格子的距离
    int bfsDist(int sc, int sr, int tc, int tr) {
        if (sc == tc && sr == tr) return 0;

        std::queue<std::pair<int, int> > q;
        int dist[H][W];
        memset(dist, -1, sizeof(dist));

        dist[sr][sc] = 0;
        q.push({sc, sr});

        while (!q.empty()) {
            auto [c, r] = q.front();
            q.pop();

            for (auto &[nc, nr] : getNeighbors(c, r)) {
                if (!inBoard(nc, nr)) continue;
                if (dist[nr][nc] != -1) continue;
                dist[nr][nc] = dist[r][c] + 1;
                if (nc == tc && nr == tr) return dist[nr][nc];
                q.push({nc, nr});
            }
        }
        return INT_MAX;
    }

    // pieces[i] = {col, row, hp, atk, def, spd, rng}
    // 前 n 个是攻方，后 m 个是守方
    // 返回 {winner, frames}
    // winner: 0 守方赢, 1 攻方赢, 2 平局
    std::vector<int> simulateBattle(int n, int m, std::vector<std::vector<int> > &piecesInput) {
        int total = n + m;
        std::vector<Piece> pieces(total);         // 所有棋子
        std::vector board(H, std::vector(W, -1)); // 棋盘状态，-1 代表没有棋子占据

        int aliveAtk = 0, aliveDef = 0; // 存活的攻方棋子数量，存活的守方棋子数量

        for (int i = 0; i < total; i++) {
            pieces[i] = {
                i,               // 棋子 id
                (i < n ? 0 : 1), // 棋子阵营
                piecesInput[i][0],
                piecesInput[i][1],
                piecesInput[i][2],
                piecesInput[i][3],
                piecesInput[i][4],
                piecesInput[i][5],
                piecesInput[i][6],
                true // 是否存活
            };
            board[pieces[i].row][pieces[i].col] = i;
            if (i < n) aliveAtk++;
            else aliveDef++;
        }

        // 判定游戏状态
        auto settle = [&] {
            if (aliveDef == 0) return 1; // 攻方赢
            if (aliveAtk == 0) return 0; // 守方赢
            return -1;                   // 未结束
        };

        // 初始就可能一方为空
        int initState = settle();
        if (initState != -1) return {initState, 0};

        for (int frame = 1; frame <= 100; frame++) {
            std::vector<int> order;
            // 收集所有存活棋子
            for (int i = 0; i < total; i++) {
                if (pieces[i].alive) order.push_back(i);
            }

            // 按照速度降序，id 升序确定行动顺序
            std::ranges::sort(order, [&](int a, int b) {
                if (pieces[a].spd != pieces[b].spd) {
                    return pieces[a].spd > pieces[b].spd;
                }
                return a < b;
            });

            // 让每个棋子行动
            for (int id : order) {
                if (!pieces[id].alive) continue; // 本帧前半段已死

                // 当前行动的棋子
                Piece &cur = pieces[id];

                // 寻找最近敌人，距离相同取 id 最小
                int targetId = -1;
                int bestDist = INT_MAX;

                for (int j = 0; j < total; j++) {
                    if (!pieces[j].alive) continue;           // 已经死了
                    if (pieces[j].side == cur.side) continue; // 友方

                    int d = bfsDist(cur.col, cur.row, pieces[j].col, pieces[j].row);
                    if (d < bestDist || (d == bestDist && j < targetId)) {
                        bestDist = d;
                        targetId = j;
                    }
                }

                if (targetId == -1) {
                    // 没找到敌人，游戏已经结束
                    int state = settle();
                    if (state != -1) return {state, frame};
                    continue;
                }

                // 本次行动的目标敌方单位
                Piece &tar = pieces[targetId];

                // 在攻击范围内，直接攻击
                if (bestDist <= cur.rng) {
                    long long damage = 1LL * cur.atk * cur.atk / (cur.atk + tar.def);
                    damage = std::max(1LL, damage);

                    tar.hp -= damage;
                    if (tar.hp <= 0) { // 如果导致敌方死亡，更新棋盘并立即判定游戏状态
                        tar.alive = false;
                        board[tar.row][tar.col] = -1;
                        if (tar.side == 0) aliveAtk--;
                        else aliveDef--;

                        int state = settle();
                        if (state != -1) return {state, frame};
                    }
                } else {
                    // 尝试移动
                    auto neighbors = getNeighbors(cur.col, cur.row);

                    int moveIdx = -1;
                    int moveDist = INT_MAX;

                    for (int k = 0; k < 6; k++) {
                        auto [nc, nr] = neighbors[k];
                        if (!inBoard(nc, nr)) continue;
                        if (board[nr][nc] != -1) continue; // 已占据

                        int d = bfsDist(nc, nr, tar.col, tar.row);
                        if (d < moveDist) {
                            moveDist = d;
                            moveIdx = k;
                        }
                    }

                    if (moveIdx != -1) {
                        auto [nc, nr] = neighbors[moveIdx];
                        board[cur.row][cur.col] = -1;
                        cur.col = nc;
                        cur.row = nr;
                        board[cur.row][cur.col] = id;
                    }
                }
            }
        }
        return {2, 100};
    }
};
