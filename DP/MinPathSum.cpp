#include <vector>
using namespace std;

// 从左上角到右下角的最小路径和

int minpathsum(vector<vector<int> > &grid) {
    int m = grid.size(), n = grid.back().size();
    vector dp(m, vector(n, 0)); // dp[i][j] 走到每个格子时的最小 sum
    dp[0][0] = grid[0][0];
    for (int j = 1; j < n; ++j) dp[0][j] = dp[0][j - 1] + grid[0][j]; // 只能从左边来
    for (int i = 1; i < m; ++i) dp[i][0] = dp[i - 1][0] + grid[i][0]; // 只能从上面来
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j < n; ++j) {
            dp[i][j] = min(dp[i - 1][j], dp[i][j - 1]) + grid[i][j];
        }
    }
    return dp[m - 1][n - 1];
}

int on(vector<vector<int> > &grid) {
    int m = grid.size(), n = grid.back().size();
    vector dp(n, 0);

    dp[0] = grid[0][0];
    for (int j = 1; j < n; ++j) dp[j] = dp[j - 1] + grid[0][j];
    for (int i = 1; i < m; ++i) {
        dp[0] += grid[i][0];
        for (int j = 1; j < n; ++j) {
            dp[j] = min(dp[j], dp[j - 1]) + grid[i][j];
        }
    }
    return dp[n - 1];
}
