#include  <bits/stdc++.h>
using namespace std;

vector<int> prevGreaterElement(const vector<int> &nums) {
    const int   n = static_cast<int>(nums.size());
    vector<int> ans(n, -1);
    stack<int>  st; // 保持单调递减

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
    const int   n = static_cast<int>(price.size());
    vector<int> ans(n, 0);
    stack<int>  st;

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && price[st.top()] <= price[i]) {
            ans[i] += ans[st.top()];
            st.pop();
        }
        st.push(i);
    }
    return ans;
}
