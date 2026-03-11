#include <bits/stdc++.h>
using namespace std;

// 右边第一个更小元素
vector<int> nextSmallerElement(const vector<int> &nums) {
    const int n = static_cast<int>(nums.size());
    vector<int> ans(n, -1);
    stack<int> st; // 让 nums[st] 单调递增

    for (int i = 0; i < n; ++i) {
        while (!st.empty() && nums[st.top()] > nums[i]) {
            ans[st.top()] = i;
            st.pop();
        }
        st.push(i);
    }
    return ans;
}

// example 1 - RemoveKDigits 删除 k 位使得剩下的数字最小
// 核心思路 - 维护单调递增栈，每次遇到更小的数字就把前面更大的踢掉
// 因为数字越大且越往前，就会导致结果越大
string removeKDigits(const string &num, int k) {
    string st;
    for (const char c : num) {
        // 1 - 只有比自己大的数字需要踢掉
        // 2 - 最多只能踢掉 k 个数字
        // 不能跳着踢，因为要维护数字相对顺序，这也是用栈的原因之一
        while (!st.empty() && k > 0 && st.back() > c) {
            st.pop_back();
            k--;
        }
        st.push_back(c);
    }

    // 如果还能踢掉一些数字，由于维护的是单增栈，要踢掉的必然是末尾的数字
    while (k > 0 && !st.empty()) {
        st.pop_back();
        k--;
    }

    int pos = 0;
    while (pos < static_cast<int>(st.size()) && st[pos] == '0') pos++; // 跳过前导 0

    string ans = st.substr(pos);
    return ans.empty() ? "0" : ans;
}
