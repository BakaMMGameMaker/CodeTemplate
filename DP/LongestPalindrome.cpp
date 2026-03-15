#include <string>
#include <vector>
using namespace std;

// 最长回文子串

// on2 中心扩展法
// 重点是处理奇偶的技巧
string longestpalindrome(const string &s) {
    int n = s.size();
    if (n < 2) return s;
    int start = 0, maxlen = 1;

    auto expand = [&](int left, int right) {
        while (left > 0 and right < n and s[left] == s[right]) {
            if (right - left + 1 > maxlen) {
                maxlen = right - left + 1;
                start = left;
            }
            --left;
            ++right;
        }
    };

    for (int i = 0; i < n; ++i) {
        expand(i, i);     //奇数长度
        expand(i, i + 1); //偶数长度, i 与 i + 1 位置字符不同时直接返回
    }
    return s.substr(start, maxlen);
}

// 区间 dp, 也是 on2, 而且空间也 on2 了
string dpmethod(const string &s) {
    int n = s.size();
    if (n < 2) return s;
    // dp - dp[i][j] 表示 s[i...j] 是否为回文串
    // s[i] == s[j] 时, 如果长度 <=3, 那么就是回文串, 否则看 dp[i+1][j-1]
    vector dp(n, vector<char>(n));
    int start = 0, maxlen = 1;

    for (int i = 0; i < n; ++i) dp[i][i] = 1; //单独一个字符本身就是一个回文串

    for (int len = 2; len <= n; ++len) {        // 枚举不同长度的回文串
        for (int i = 0; i + len - 1 < n; ++i) { // 枚举回文串的起点
            int j = i + len - 1;                // 回文串的终点
            if (s[i] != s[j]) dp[i][j] = 0;
            else if (len <= 3) dp[i][j] = 1;
            else dp[i][j] = dp[i + 1][j - 1];

            if (dp[i][j] and len > maxlen) {
                maxlen = len;
                start = i;
            }
        }
    }
    return s.substr(start, maxlen);
}
