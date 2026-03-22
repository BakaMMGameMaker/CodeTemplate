#include <bits/stdc++.h>
using namespace std;

// 数组差分模板

void DifferenceArray() {
    // n 个数字，m 次操作
    // 每次操作代表从 [l, r] 每个数字加上 k
    // diff[i] += k, diff[r+1] -= k，最后对差分数组求前缀和，就可以还原每个位置被加了多少
    // 原因：我们只需要清楚被操作的区间的 begin 位置和 end 位置
    // 对于 begin 位置，被操作过后，其比原本左侧元素的差值多了 k
    // 对于 end 位置，由于其左侧的元素多了 k，所以差分少了 k
    int n, m;
    cin >> n >> m;

    vector<long long> a(n + 1), diff(n + 2, 0);
    for (int i = 1; i <= n; ++i) cin >> a[i];
    // 把原数组转换为差分数组
    diff[1] = a[1];
    for (int i = 2; i <= n; ++i) diff[i] = a[i] - a[i - 1];

    // 处理 m 次区间修改
    while (m--) {
        int l, r;
        long long k;
        cin >> l >> r >> k;
        diff[l] += k;
        if (r + 1 <= n) diff[r + 1] -= k;
    }

    // 前缀和还原最终数组
    for (int i = 1; i <= n; ++i) {
        diff[i] += diff[i - 1];
        cout << diff[i] << ' ';
    }
}
