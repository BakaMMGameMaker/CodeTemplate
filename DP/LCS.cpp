#include <algorithm>
#include <string>
#include <vector>
using namespace std;

// 两个给定字符串的最长公共子序列 - 子序列并不一定连续

// 二维 dp
// dp[i][j] 表示使用 text1 的前 i 个字符和 text2 的前 j 个字符的最长公共子序列长度
// 如果 text1[i-1] == text2[j-1], 那么 dp[i][j] = dp[i-1][j-1] + 1
// 否则 dp[i][j] = max(dp[i-1][j], dp[i][j-1])
string lcs(const string &s1, const string &s2) {
    size_t m = s1.size(), n = s2.size();
    vector dp(m + 1, vector(n + 1, 0));

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
    if (dp[m][n] == 0) return "-1";

    // 构造 LCS
    string ans;
    size_t i = m, j = n;
    while (i > 0 and j > 0) {
        if (s1[i - 1] == s2[j - 1]) {
            // 从 dp[i][j] 回到 dp[i-1][j-1]
            ans.push_back(s1[i - 1]); // 因为的确是相同字符, 所以加入到答案中
            i--;
            j--;
            // 后面两种情况都不会新增答案, 一直往左或者往上走, 直到再次符合第一个 if
        } else if (dp[i - 1][j] >= dp[i][j - 1]) {
            i--;
        } else {
            j--;
        }
    }
    ranges::reverse(ans);
    return ans;
}
