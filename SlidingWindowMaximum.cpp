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
