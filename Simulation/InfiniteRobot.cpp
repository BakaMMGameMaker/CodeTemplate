#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

using ll = long long;

ll encode(ll x, ll y) {
    return (x << 32) | y;
}

bool canreach(unordered_set<ll> &path, int dx, int dy, int x, int y) {
    ll k1 = x / dx;     // 走到 x 至少需要多少完整轮
    ll k2 = y / dy;     // 走到 y 至少需要多少完整轮
    ll k = min(k1, k2); // 取最小者，防止减成负数
    ll rx = x - k * dx;
    ll ry = y - k * dy;
    return path.contains(encode(rx, ry));
}

bool robot(const string &command, vector<vector<int> > &obstacles, int x, int y) {
    unordered_set<ll> path;
    int cx = 0, cy = 0;
    path.insert(encode(0, 0)); // 不要忘记把 0，0 加到路径里

    // 先走一轮，记录前缀路径
    for (char c : command) {
        if (c == 'U') ++cy;
        else ++cx;
        path.insert(encode(cx, cy));
    }

    int dx = cx, dy = cy; // 一轮带来的 dx dy

    if (!canreach(path, dx, dy, x, y)) return false;

    for (auto &ob : obstacles) {
        int ox = ob[0], oy = ob[1];

        if (ox >= 0 and oy >= 0 and ox <= x and oy <= y) {
            // 只考虑需要考虑的障碍物
            if (canreach(path, dx, dy, ox, oy)) return false;
        }
    }
    return true;
}
