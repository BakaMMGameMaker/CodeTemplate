#include <bits/stdc++.h>
using namespace std;

// 双指针模板
// 找出所有两两元素不同的最长子数组

/*
 * 思路 右指针不断向右扩展
 * 如果出现重复，移动左指针直到窗口重新合法
 * 得到合法窗口后，长度为 r-l+1
 * 维护当前最大长度
 * 如果当前窗口更长，清空答案并记录当前区间
 * 如果当前窗口等于最大长度，直接加入答案
 */
void solution() {
    int n;
    cin >> n;

    vector<int> a(n + 1);
    for (int i = 1; i <= n; ++i) cin >> a[i];

    vector<int> cnt(n + 1, 0); // 统计元素出现次数 根据元素范围灵活选择容器
    int l = 1;
    int maxLen = 0;
    vector<pair<int, int> > ans;

    for (int r = 1; r <= n; ++r) {
        cnt[a[r]]++;
        while (cnt[a[r]] > 1) {
            // 右侧元素的加入导致 a[r] 发生重复
            cnt[a[l]]--;
            l++;
        }
        if (const int len = r - l + 1; len > maxLen) {
            maxLen = len;
            ans.clear();
            ans.emplace_back(l, r);
        } else if (len == maxLen) {
            ans.emplace_back(l, r);
        }
    }

    cout << ans.size() << '\n';
    for (auto [ll, rr]: ans) {
        cout << ll << ' ' << rr << '\n';
    }
}
