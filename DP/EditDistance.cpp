#include <string>
#include <vector>
using namespace std;

// 编辑距离
// 核心思路 dp[i][j] = word1 前 i 个字符转换为 word2 前 j 个字符的最少操作数
// w1[i-1]==w2[j-1] => dp[i][j]=dp[i-1][j-1] (不用操作)
// w1[i-1]!=w2[j-1] {
// 目标把前 i 个字符改造得和对面前 j 个字符一样
// 插入 w2[j-1] => dp[i][j]=dp[i][j-1]+1, 先把前 i 个字符变成对面前 j-1 个字符, 再插入一个 w2[j-1]
// 删除 w1[i-1] => dp[i][j]=dp[i-1][j]+1, 先把前 i-1 个字符变成对面前 j 个字符, 再删除末尾的字符
// 替换 w1[i-1] => dp[i][j]=dp[i-1][j-1]+1, 先把前 i-1 个字符变成对面前 j-1 个字符, 再把末尾字符换成和对面一样的

// 核心记忆点 - 除了字符相等而无需操作的情况, 否则左上角三个状态都要纳入考量范围, 分别对应三种操作
// 相等则等于左上角, 否则等于左上角三个 + 1 次操作
// }

int editdistance(const string &word1, const string &word2) {
    int m = word1.size(), n = word2.size();
    vector dp(m + 1, vector(n + 1, 0));
    // 处理第一行和第一列
    for (int i = 0; i <= m; ++i) dp[i][0] = i; // 删掉所有字符
    for (int j = 0; j <= n; ++j) dp[0][j] = j; // 插入 j 个字符
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (word1[i - 1] == word2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = ranges::min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]}) + 1;
        }
    }
    return dp[m][n];
}

// 空间优化, prev 记录左上角, dp[j] 等于上面, dp[j-1] 等于左边, 所以要顺序遍历
int onspace(const string &word1, const string &word2) {
    int m = word1.size(), n = word2.size();
    vector dp(n + 1, 0);
    // 处理第一行和第一列
    for (int j = 0; j <= n; ++j) dp[j] = j; // 插入 j 个字符
    for (int i = 1; i <= m; ++i) {
        // 先保存左上角的值, 再更新开头元素
        int prev = dp[0];
        dp[0] = i;
        for (int j = 1; j <= n; ++j) {
            int temp = dp[j];
            if (word1[i - 1] == word2[j - 1]) dp[j] = prev;
            else dp[j] = ranges::min({dp[j], dp[j - 1], prev}) + 1;
            prev = temp;
        }
    }
    return dp[n];
}
