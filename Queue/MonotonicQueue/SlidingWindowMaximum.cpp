#include <bits/stdc++.h>
using namespace std;

// 滑动窗口最大值模板

vector<int> SlidingWindowMaximum(const vector<int> &nums, int k) {
    // 维护单调递减队列，队首保持为当前窗口最大值下标
    // 比较大小时 - nums[q.back()] < nums[i]
    // 判断是否滑出窗口 - q.front() <= i - k
    deque<int> q;
    vector<int> ans;
    for (int i = 0; i < nums.size(); ++i) {
        // 踢掉离开窗口的
        while (!q.empty() && q.front() <= i - k) q.pop_front();
        // 踢掉没自己大的
        while (!q.empty() && nums[q.back()] <= nums[i]) q.pop_back();
        // 加入当前元素(下标)
        q.push_back(i);
        // 窗口形成之后，记录当前答案
        if (i >= k - 1) ans.push_back(nums[q.front()]);
    }
    return ans;
}

vector<int> SlidingWindowMinimum(const vector<int> &nums, int k) {
    int n = static_cast<int>(nums.size());
    vector<int> ans;
    deque<int> dq;

    for (int i = 0; i < n; ++i) {
        // 去除离队的
        while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
        // 去除比自己大的
        while (!dq.empty() && nums[dq.back()] >= nums[i]) dq.pop_back();
        // 把自己加进去
        dq.push_back(i);
        // 窗口形成后，添加答案
        if (i >= k - 1) ans.push_back(nums[dq.front()]);
    }
    return ans;
}

// 二维滑动窗口最大值
// 先按行做宽度为 b 的窗口最值，得到中间矩阵
// 再对中间矩阵做高度为 a 的窗口最值，得到结果矩阵
vector<vector<int> > GetRowMax(const vector<vector<int> > &g, int b) {
    int n = static_cast<int>(g.size());
    int m = static_cast<int>(g.back().size());
    vector RowMax(g.size(), vector<int>(m - b + 1));
    for (int i = 0; i < n; ++i) {
        deque<int> q; // 单调队列
        for (int j = 0; j < m; ++j) {
            while (!q.empty() && q.front() <= j - b) q.pop_front();
            while (!q.empty() && g[i][q.back()] <= g[i][j]) q.pop_back();
            q.push_back(j);
            if (j >= b - 1) RowMax[i][j - b + 1] = g[i][q.front()];
        }
    }
    return RowMax;
}

vector<vector<int> > GetColMax(const vector<vector<int> > &g, int a) {
    int n = static_cast<int>(g.size());
    int m = static_cast<int>(g.back().size());
    vector ColMax(n - a + 1, vector<int>(m));
    for (int j = 0; j < m; ++j) {
        deque<int> q;
        for (int i = 0; i < n; ++i) {
            while (!q.empty() && q.front() <= i - a) q.pop_front();
            while (!q.empty() && g[q.back()][j] <= g[i][j]) q.pop_back();
            q.push_back(i);
            if (i >= a - 1) ColMax[i - a + 1][j] = g[q.front()][j];
        }
    }
    return ColMax;
}

// 绝对差不超过限制的最长连续子数组
// 思路 - 同时维护窗口最大值和窗口最小值，如果窗口非法，往右移动左指针
int longestSubarrayWithAbsNoGreaterThanLimit(const vector<int> &nums, int limit) {
    int n = static_cast<int>(nums.size());
    deque<int> MinDeque, MaxDeque;
    int ans = 0;
    int left = 0;
    for (int right = 0; right < n; ++right) {
        while (!MinDeque.empty() && nums[MinDeque.back()] >= nums[right]) MinDeque.pop_back();
        while (!MaxDeque.empty() && nums[MaxDeque.back()] <= nums[right]) MaxDeque.pop_back();
        MinDeque.push_back(right);
        MaxDeque.push_back(right);
        if (int diff = abs(nums[MinDeque.front()] - nums[MaxDeque.front()]); diff > limit) {
            while (!MinDeque.empty() && MinDeque.front() <= left) MinDeque.pop_front();
            while (!MaxDeque.empty() && MaxDeque.front() <= left) MaxDeque.pop_front();
            ++left;
        }
        else { ans = max(ans, right - left + 1); }
    }
    return ans;
}
