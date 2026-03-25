#include <vector>
#include <unordered_set>
using namespace std;

// 坐标编码
long long encode(int x, int y) {
    return (static_cast<long long>(x) << 32) ^ static_cast<unsigned int>(y);
}

int robotsim(vector<int> &commands, vector<vector<int> > &obstacles) {
    unordered_set<long long> st;
    for (auto &ob : obstacles) st.insert(encode(ob[0], ob[1]));

    vector<int> dx = {0, 1, 0, -1};
    vector<int> dy = {1, 0, -1, 0};
    int dir = 0; // 0 北 1 东 2 南 3 西
    int x = 0, y = 0;
    int ans = 0;
    for (int cmd : commands) {
        if (cmd == -2) {
            dir = (dir + 3) % 4;
        } else if (cmd == -1) {
            dir = (dir + 1) % 4;
        } else {
            for (int i = 0; i < cmd; ++i) {
                int nx = x + dx[dir];
                int ny = y + dy[dir];

                if (st.contains(encode(nx, ny))) break;

                x = nx;
                y = ny;
                ans = max(ans, x * x + y * y);
            }
        }
    }
    return ans;
}
