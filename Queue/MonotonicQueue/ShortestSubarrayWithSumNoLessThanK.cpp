#include <bits/stdc++.h>
using namespace std;

// 最短且满足和至少为 K 的子数组
// 思路 s[i] - s[j] >= K => s[j] <= s[i] - K
// 要让长度最短，就要让 j 尽量大
// 遍历到新元素时，可以发现，如果老元素比自己不小，那么必然无法比自己带来更优解，所以要维护单调递增队列
// s[i] - s[q.front()] >= K 时，可以更新答案，弹出队头(不可能带来更优答案)并进一步寻找更优解

int shortestSubarray(const vector<int> &nums, int k) {
    int n = static_cast<int>(nums.size());
    vector<int> a(1), s(n + 1);
    for (int x : nums) a.push_back(x);

    for (int i = 1; i <= n; ++i) s[i] = s[i - 1] + a[i];
    deque<int> dq;
    dq.push_back(0);
    int ans = INT32_MAX;

    for (int i = 1; i <= n; ++i) {
        // 尝试更新答案
        while (!dq.empty() && s[i] - s[dq.front()] >= k) {
            ans = min(ans, i - dq.front());
            dq.pop_front();
        }
        // 维护单调性
        while (!dq.empty() && s[dq.back()] >= s[i]) s.pop_back();
        dq.push_back(i);
    }
    return ans == INT32_MAX ? -1 : ans;
}
