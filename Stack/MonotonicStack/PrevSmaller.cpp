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
