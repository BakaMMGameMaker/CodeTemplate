#include <string>
#include<vector>
using namespace std;

// 正则表达式匹配 (高难)
bool match(const string &s, const string &p) {
    int m = s.size(), n = p.size();
    vector dp(m + 1, vector<char>(n + 1, 0)); // dp[i][j]=s 前 i 个字符可否匹配 p 前 j 个字符

    dp[0][0] = 1; // 不要忘记

    // 要点 1 - 第一行的处理
    // 想要 s 的前 0 个字符能够匹配 pattern, pattern 只能是 x*x*x*... 的格式, 并且所有 'x' 出现 0 次
    for (int j = 2; j <= n; ++j) if (p[j - 1] == '*') dp[0][j] = dp[0][j - 2];

    auto matchtwochar = [&](char a, char b) { return b == '.' or a == b; };

    for (int i = 1; i <= m; ++i) {
        for (int j = 1; j <= n; ++j) {
            if (p[j - 1] != '*') {
                // p 的第 j 个字符是字母或者 '.'
                // 那么想要 s 的前 i 个字符能和 p 的前 j 个字符匹配
                // 1 - 前 i - 1 个字符能和前 j - 1 个字符匹配
                // 2 - 第 i 个字符能匹配上第 j 个字符
                dp[i][j] = dp[i - 1][j - 1] and matchtwochar(s[i - 1], p[j - 1]);
            } else {
                // '*' 作用在 p[j - 2]=c 上
                // 如果 c 出现次数为 0, 那么 dp[i][j] = dp[i][j-2]
                dp[i][j] = dp[i][j - 2];
                // 如果想要让 c 出现至少一次, 那么 c 至少要能和 s[i-1] 能匹配上
                // 匹配上之后, dp[i][j] 是否为真只取决于 dp[i-1][j]
                // 只要后者为真, 前者自然为真, 相当于给 * 喂了一个 s 末尾的 c
                // 总结起来就是, s 有能力喂一个字符给 * 号, 于是能否匹配就取决于前面是否匹配了
                if (matchtwochar(s[i - 1], p[j - 2])) dp[i][j] = dp[i][j] || dp[i - 1][j];
            }
        }
    }
    return dp[m][n];
}
