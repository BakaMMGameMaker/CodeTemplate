#include  <bits/stdc++.h>
using namespace std;

vector<int> prevGreaterElement(const vector<int> &nums) {
    const int n = static_cast<int>(nums.size());
    vector<int> ans(n, -1);
    stack<int> st; // 保持单调递减

    // 和找右边的区别就是找右边会在 while 里更新旧元素的结果
    // 而找左边的是在循环外立即更新当前元素的结果
    for (int i = 0; i < n; ++i) {
        // 注意是小于等于，因为要把等于自己的都弹出去，找到第一个比自己大的
        while (!st.empty() && nums[st.top()] <= nums[i]) { st.pop(); }
        if (!st.empty()) ans[i] = st.top();
        st.push(i);
    }
    return ans;
}

// example 1 - stock span - 今天的股票价格往左边连续多少天都小于等于今天
vector<int> StockSpan(const vector<int> &price) {
    const int n = static_cast<int>(price.size());
    vector<int> ans(n, 0);
    stack<int> st;

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && price[st.top()] <= price[i]) {
            ans[i] += ans[st.top()];
            st.pop();
        }
        st.push(i);
    }
    return ans;
}

// example 2 - 接雨水
// 思路 - 对每一个柱子，尝试将其作为右边界
// 不断弹栈，以弹出的柱子作为槽底，看看能接多少水
int trap(const vector<int> &height) {
    const int n = static_cast<int>(height.size());
    int ans = 0;
    stack<int> st;

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && height[i] > height[st.top()]) { // 尝试让比自己矮的柱子成为槽底
            const int mid = st.top();
            st.pop();

            if (st.empty()) break; // 没有 >= 自己的左边界了，水只会流走

            const int left = st.top();  // mid 左手边第一个不矮于自己的柱子
            const int w = i - left - 1; // 槽宽
            const int h = min(height[left], height[i]) - height[mid];
            ans += w * h;
            // 这里算的其实不是某个位置上能放多少水，而是宽度为 w 的槽位整体水位能上升 h 格
            // 不用担心漏算，因为维护的是非递增栈，所以每时每刻都在找能不能‘骑’在刚在高高在上
            // 的右边界的头上放水。底下的水在之前就已经被算过了
        }
        st.push(i);
    }
    return ans;
}
