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

// 两个字符串的最长公共子串 - 要求连续
// 核心变化 dp[i][j] 从用前 i/j 个字符能达到的最大子序列长度变为
// 以 text1 的 i-1 为结尾和 text2[j-1] 为结尾的最长公共子串的长度
// 转移 - 字符相等 => 左上角 + 1
// 字符不等 => 保持为零, 因为连续性被破坏
int longestCommonSubstring(const string &s1, const string &s2) {
    size_t m = s1.size(), n = s2.size();
    vector dp(m + 1, vector(n + 1, 0));
    int ans = 0;
    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                ans = max(ans, dp[i][j]);
            }
        }
    }
    return ans;
}

// 空间优化 - 可以想象在表格中处理一行新 dp 的时候, 我们只需要用到左上角的那一行数据, 就像这样
// ########x <- 井号位置代表这次更新要用到的数据
// xoooooooo <- 准备更新的行
// 每个 o 依赖左上角的 #, 并在更新过后把正上方的 # 拉下来, 所以为了避免数据丢失, 需要从后往前遍历列
string longestCommonSubstringII(const string &s1, const string &s2) {
    int m = s1.size(), n = s2.size();
    vector dp(n + 1, 0);
    int maxlen = 0;
    int endpos = 0; // 结果在 s1 中结束的位置
    for (int i = 1; i <= m; i++) {
        for (int j = n; j >= 1; j--) {
            if (s1[i - 1] == s2[j - 1]) {
                dp[j] = dp[j - 1] + 1;
                if (dp[j] > maxlen) {
                    maxlen = dp[j];
                    endpos = i - 1;
                }
            } else {
                dp[j] = 0; // 也要更新, 否则会残留旧值
            }
        }
    }
    return s1.substr(endpos - maxlen + 1, maxlen);
}
