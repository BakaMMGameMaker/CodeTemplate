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

// 二分解法
// 思路 d[k] 记录长度为 k+1 的最长上升子序列中末尾元素的最小值
// 理由是越小的值，越容易接新的数字
// 严格上升中，使用 lower bound，原因是此时只能接在严格比自己小的数字后面
// 所以要更新的位置就是第一个 >= 自己的位置
// 如果 lb 和 ub 找到 end，那么 push back
// 最终 d 的长度就是结果，毕竟它记录了每种长度的序列的末尾元素最小值，注意 d 本身不是最长序列
size_t BinaryLIS(const vector<int> &a) {
    vector<int> d;
    for (int x: a) {
        if (auto it = ranges::lower_bound(d, x); it == d.end()) d.push_back(x);
        else *it = x;
    }
    return d.size();
}

// LNDS 中，只需要把 lb 换成 rb 即可，因为可以接在相等的元素后面
size_t BinaryLNDS(const vector<int> &a) {
    vector<int> d;
    for (int x: a) {
        if (auto it = ranges::upper_bound(d, x); it == d.end()) d.push_back(x);
        else *it = x;
    }
    return d.size();
}

// 如果需要返回具体的序列
vector<int> GetLIS(const vector<int> &a) {
    const size_t n = a.size();
    vector<int> d; // d[k] = 长度为 k+1 的序列末尾元素的最小值
    vector<int> pos; // pos[k] = 长度为 k+1 的序列对应的末尾最小值节点下标
    vector<int> pre(n, -1); // 前驱节点的下标

    for (int i = 0; i < n; ++i) {
        int x = a[i];
        // 除了最后一行，其实逻辑基本一致，只不过从拿 it 变成了拿差值
        // 更新 d 的时候顺带更新一下 pos
        // 如果是 LNDS，只需要把 lb 改成 rb
        const size_t p = ranges::lower_bound(d, x) - d.begin();

        if (p == d.size()) {
            // 相当于 it == end()
            d.push_back(x);
            pos.push_back(i);
        } else {
            // 相当于 it != end()
            d[p] = x;
            pos[p] = i; // 长度为 p+1 的序列的末尾元素的下标为 i
        }
        if (p > 0) pre[i] = pos[p - 1]; // 下标为 i 的元素的前驱节点是长度为 p 的序列的末尾元素
    }
    vector<int> res;
    int cur = pos.back(); // 获取最长上升子序列的末尾节点的下标
    while (cur != -1) {
        res.push_back(a[cur]);
        cur = pre[cur];
    }
    ranges::reverse(res);
    return res;
}
