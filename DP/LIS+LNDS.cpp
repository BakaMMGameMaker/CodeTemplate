#include <bits/stdc++.h>
using namespace std;

// 最长不下降子序列与最长上升子序列模板
// 包含朴素 DP 和进阶二分解法

// 朴素 DP 严格上升子序列
// 思路 dp[i] 记录以下标为 i 的元素结尾时的最大结果
// 每个元素都往前遍历，遇到比自己小的元素，接在后面并更新最大值
int LIS(const vector<int> &a) {
    const size_t n = a.size();
    vector<int> dp(n, 1);
    int ans = 1;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < i; ++j) {
            // 严格上升，不允许相等
            if (a[j] < a[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        ans = max(ans, dp[i]);
    }
    return ans;
}

int LNDS(const vector<int> &a) {
    const size_t n = a.size();
    vector<int> dp(n, 1);
    int ans = 1;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < i; ++j) {
            if (a[j] <= a[i]) {
                dp[i] = max(dp[i], dp[j] + 1);
            }
        }
        ans = max(ans, dp[i]);
    }
    return ans;
}
