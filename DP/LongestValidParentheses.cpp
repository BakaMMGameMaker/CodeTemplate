#include <bits/stdc++.h>
using namespace std;

// DP 括号题模板

// dp[i] 表示以 i 为结尾的最长的有效括号的长度
// 如果 s[i] == ')'
// - 如果 s[i-1] == '('，那么 dp[i] = dp[i-2] + 2
// - 如果 s[i-1] 仍为右括号，设前面有效段长度为 dp[i-1]，找能和当前右括号匹配的小括号的位置 j=i-dp[i-1]-1
// -- 如果这个位置为左括号，那么 dp[i] = dp[i-1] + 2 + dp[j-1]，j 位置已经算在 2 里面了
int longestValidDP(const string &s) {
    const int n = static_cast<int>(s.size());
    vector<int> dp(n);
    int ans = 0;
    for (int i = 1; i < n; ++i) {
        if (s[i] == '(') continue;
        if (s[i - 1] == '(') dp[i] = (i >= 2 ? dp[i - 2] : 0) + 2;
        else {
            const int j = i - dp[i - 1] - 1;
            if (j < 0 || s[j] == ')') continue;
            dp[i] = dp[i - 1] + 2 + (j >= 1 ? dp[j - 1] : 0);
        }
        ans = max(dp[i], ans);
    }
    return ans;
}
