#include <bits/stdc++.h>
using namespace std;

// 长度至少为 k 的最大子数组和 (长度不超过 m 的版本见单调队列的同名文件)
// i - j >= k => j <= i - k
// 相当于在 [0, i-k] 范围找 s 的最小值
// 不需要单调队列，因为左边界永远为 0, 只需要维护一个最小值即可
int NoShorterThanK(const vector<int> &nums, int k) {
    int n = static_cast<int>(nums.size());
    vector<int> presum(n + 1);
    for (int i = 1; i <= n; ++i) presum[i] = presum[i - 1] + nums[i - 1];

    int ans = INT32_MIN;
    int minPrefix = 0; // s[0]
    for (int i = k; i <= n; ++i) {
        minPrefix = min(minPrefix, presum[i - k]);
        ans = max(ans, presum[i] - minPrefix);
    }
    return ans;
}
