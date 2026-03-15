#include <vector>
using namespace std;

// 矩阵中的最长递增路径

int longestIncreasingPath(const vector<vector<int> > &matrix) {
    int m = static_cast<int>(matrix.size()), n = static_cast<int>(matrix.back().size());
    vector memory(m, vector(n, 0)); // 每个格子的答案

    int dirs[5] = {-1, 0, 1, 0, -1};

    auto dfs = [&](auto &&self, int i, int j) {
        if (memory[i][j] != 0) return memory[i][j]; // 已经 d 过
        int best = 1;                               // 假设哪也去不了, 那么最优长度就是 1
        for (int k = 0; k < 4; ++k) {
            int ni = i + dirs[k], nj = j + dirs[k + 1];
            if (ni < 0 or ni >= m or nj < 0 or nj >= n) continue;
            if (matrix[ni][nj] <= matrix[i][j]) continue;
            best = max(best, 1 + self(self, ni, nj)); // 注意不要忘记 1+
        }
        memory[i][j] = best;
        return best;
    };

    int ans = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            ans = max(ans, dfs(dfs, i, j));
        }
    }
    return ans;
}
