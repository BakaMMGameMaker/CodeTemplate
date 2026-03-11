#include <bits/stdc++.h>
using namespace std;

vector<int> prevSmallerElement(const vector<int> &nums) {
    const int   n = static_cast<int>(nums.size());
    vector<int> ans(n, -1);
    stack<int>  st; // 要弹大，找到第一个比自己小的，所以要单增

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && nums[st.top()] >= nums[i]) st.pop();
        if (!st.empty()) ans[i] = st.top();
        st.push(i);
    }
    return ans;
}

// example 1 - 直方图最大矩形 每根柱子作为高，最大可以扩展多宽
// 核心思路 - 找左边第一个比自己小的位置，第一个比自己大的位置，计算宽度
// 需要注意，左右没有比自己小的元素的时候，要把数组的 begin - 1 和 end 位置当成比自己矮的柱子
vector<int> largestRectangleArea(const vector<int> &heights) {
    const int   n = static_cast<int>(heights.size());
    vector<int> left(n), right(n);
    stack<int>  st1;
    stack<int>  st2;

    // 找右边第一个比自己小的
    // 倒着遍历可以和往左找保持一致的写法
    for (int i = n - 1; i >= 0; --i) {
        while (!st1.empty() && heights[st1.top()] >= heights[i]) { st1.pop(); }
        right[i] = st1.empty() ? n : st1.top();
        st1.push(i);
    }

    // 找左边第一个比自己小的
    for (int i = 0; i < n; ++i) {
        while (!st2.empty() && heights[st2.top()] >= heights[i]) { st2.pop(); }
        left[i] = st2.empty() ? -1 : st2.top();
        st2.push(i);
    }

    vector<int> ans(n);
    for (int i = 0; i < n; ++i) {
        const int wid = right[i] - left[i] - 1;
        ans[i]        = wid * heights[i];
    }
    return ans;
}
