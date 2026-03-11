#include <bits/stdc++.h>
using namespace std;

// 右边第一个更大元素
vector<int> nextGreaterElement(const vector<int> &nums) {
    const int   n = static_cast<int>(nums.size());
    vector<int> ans(n, -1);
    stack<int>  st; // 保存下标，保持 nums[st] 单调递减

    for (int i = 0; i < n; ++i) {
        // 对于栈顶所有比自己小的元素，其右侧第一个更大元素就是当前元素
        while (!st.empty() && nums[i] > nums[st.top()]) {
            ans[st.top()] = i;
            st.pop();
        }
        st.push(i); // 加入当前元素等待审判
    }
    return ans;
}

// example 1 - 每一天往后等几天会变得更热
vector<int> dailyTemperatures(const vector<int> &temperatures) {
    const int   n = static_cast<int>(temperatures.size());
    vector<int> ans(n, -1);
    stack<int>  st;

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && temperatures[st.top()] < temperatures[i]) {
            ans[st.top()] = i - st.top();
            st.pop();
        }
        st.push(i);
    }
    return ans;
}

// example 2 - 循环数组的 NextGreater
// 把数组遍历两遍即可
vector<int> nextGreaterElementII(const vector<int> &nums) {
    const int   n = static_cast<int>(nums.size());
    vector<int> ans(n, -1);
    stack<int>  st;

    for (int i = 0; i < 2 * n; ++i) {
        const int x = nums[i % n];
        while (!st.empty() && nums[st.top()] < x) {
            ans[st.top()] = x; // 要下标就 i%n
            st.pop();
        }
        if (i < n) st.push(i); // 每个元素只需要进一次栈
    }
    return ans;
}
