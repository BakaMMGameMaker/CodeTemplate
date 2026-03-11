#include <bits/stdc++.h>
using namespace std;

// 右边第一个更小元素
vector<int> nextSmallerElement(const vector<int> &nums) {
    const int   n = static_cast<int>(nums.size());
    vector<int> ans(n, -1);
    stack<int>  st; // 让 nums[st] 单调递增

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && nums[st.top()] > nums[i]) {
            ans[st.top()] = i;
            st.pop();
        }
        st.push(i);
    }
    return ans;
}
