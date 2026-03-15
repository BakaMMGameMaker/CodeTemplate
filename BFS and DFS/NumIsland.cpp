#include <vector>
#include <array>
#include <queue>
#include <stack>
#include <utility>
using namespace std;

// 岛屿数量

static array<pair<int, int>, 4> dirs({
    {1, 0}, {-1, 0}, {0, 1}, {0, -1}
});

// DFS 递归
int NumIslandsRecursive(vector<vector<char> > &grid) {
    int m = static_cast<int>(grid.size()), n = static_cast<int>(grid.back().size());
    int ans = 0;

    auto dfs = [&](auto &&self, int i, int j) {
        if (i < 0 or i >= m or j < 0 or j >= n or grid[i][j] != '1') return;
        grid[i][j] = 0;
        self(self, i + 1, j);
        self(self, i - 1, j);
        self(self, i, j + 1);
        self(self, i, j - 1);
    };

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] == '1') {
                ans++;
                dfs(dfs, i, j);
            }
        }
    }
    return ans;
}

// 非递归 DFS
int NumIslandsIterative(vector<vector<char> > &grid) {
    int m = static_cast<int>(grid.size()), n = static_cast<int>(grid.back().size());
    int ans = 0;

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] != '1') continue;
            ans++;
            stack<pair<int, int> > st;
            st.emplace(i, j);
            grid[i][j] = 0;
            while (!st.empty()) {
                auto [x, y] = st.top();
                st.pop();

                for (auto [dx, dy] : dirs) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx < 0 or nx >= m or ny < 0 or ny >= n) continue;
                    grid[nx][ny] = '0'; // 入栈立马标记
                    st.emplace(nx, ny);
                }
            }
        }
    }
    return ans;
}

int NumIslandsBFS(vector<vector<char> > &grid) {
    int m = static_cast<int>(grid.size()), n = static_cast<int>(grid.back().size());
    int ans = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (grid[i][j] != '1') continue;
            ans++;
            queue<pair<int, int> > q;
            q.emplace(i, j);
            grid[i][j] = '0';

            while (!q.empty()) {
                auto [x, y] = q.front();
                q.pop();
                for (auto [dx, dy] : dirs) {
                    int nx = x + dx, ny = y + dy;
                    if (nx < 0 or nx >= m or ny < 0 or ny >= n) continue;
                    grid[nx][ny] = '0';
                    q.emplace(nx, ny);
                }
            }
        }
    }
    return ans;
}
