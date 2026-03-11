#include <bits/stdc++.h>
using namespace std;

// 长度不超过 m 的最大子数组和
// 1 - 计算数组前缀和，得到 s[i] = a[1] + a[2] + ... + a[i]
// 区间 [j+1, i] 的和为 s[i] - s[j]
// 要求长度不超过 m，则 i - j <= m => j >= i - m
// 当 i 固定时，要最大化 s[i] - s[j]，就要最小化 s[j]，相当于在合法范围内寻找最小的前缀和
// 于是问题转化为求取窗口 [i-m, i-1] 中前缀和的最小值 窗口大小为 m
int NoLongerThanM(const vector<int> &nums, int m) {
    int n = static_cast<int>(nums.size());
    vector<int> a(1);
    for (int x : nums) a.push_back(x);

    // 构造前缀和
    vector<int> s(n + 1);
    for (int i = 1; i <= n; ++i) s[i] = s[i - 1] + a[i];

    // 维护单调队列
    deque<int> dq;
    dq.push_back(0); // i 从 1 开始遍历，所以先推一个 0 进去
    int ans = INT32_MIN;

    for (int i = 1; i <= n; ++i) {
        // 踢出离开窗口的
        while (!dq.empty() && dq.front() < i - m) dq.pop_front();
        // 先别着急踢，不然可能没答案了
        // 不用等窗口长度变为 m
        ans = max(ans, s[i] - s[dq.front()]);
        // 踢出比当前前缀和大的
        while (!dq.empty() && s[dq.back()] > s[i]) dq.pop_back();
        dq.push_back(i);
    }
    return ans;
}
