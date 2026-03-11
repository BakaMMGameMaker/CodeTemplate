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
