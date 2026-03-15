#include <vector>
using namespace std;

// 左上角到右下角, 只能往右或者往下, 路径总数

// 解法 1 : dp
// dp[i][j] = 走到 i j 的不同路径数量
int uniquepaths(int m, int n) {
    vector dp(m, vector(n, 1));
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j < n; ++j) {
            dp[i][j] = dp[i - 1][j] + dp[i][j - 1];
        }
    }
    return dp[m - 1][n - 1];
}

// 由于 dp[i][j] 只依赖于上方和左边的值, 左边的值又可以通过实时更新得到
// 所以只需保存一行数据即可
int lessspace(int m, int n) {
    vector dp(n, 1);
    for (int i = 1; i < m; ++i) {
        for (int j = 1; j < n; ++j) {
            dp[j] += dp[j - 1];
            // 本质上是两步合成一步
            // 1 - dp[i][j] = dp[i-1][j]
            // 2 - dp[i][j] += dp[i][j-1]
        }
    }
    return dp[n - 1];
}

// 组合数学
// 由于整体来讲, 要往下走 m-1 步, 往右走 n-1 步
// 所以就是总步数里, 挑选其中 m-1 步往下走
// 所以就是 c(m-n+2, m-1/n-1), 取 m-1/n-1 中的较小者可以减少循环次数
int usemath(int m, int n) {
    int down = m - 1;
    int right = n - 1;
    int k = min(down, right); // b
    int ans = 1;
    int total = m + n - 2; // a
    // 求 c(a,b) = a(a-1)...(a-b+1)/b!
    for (int i = 1; i <= k; ++i) ans = ans * (total - k + i) / i;
    return ans;
}
