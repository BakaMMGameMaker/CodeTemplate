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

// example 2 - RemoveDuplicateLetters - 每个字母只保留一个，使得结果字典序最小
// 和删除数字类似，要让字典序更小的字母尽量靠前
// 这个比较难理解，看看就行
string removeDuplicateLetters(const string &s) {
    vector<int> cnt(26, 0), vis(26, 0);
    for (const char c : s) cnt[c - 'a']++; // 统计每个字符出现次数

    string st;
    for (const char c : s) {
        cnt[c - 'a']--;
        if (vis[c - 'a']) continue; // 防止结果中有重复字符

        // 1 - 要让小字符更靠前，所以栈中比自己大的要出去
        // 2 - 如果栈中比自己大的字符是孤儿，那么就不能再踢走了
        while (!st.empty() && st.back() > c && cnt[st.back() - 'a'] > 0) {
            vis[st.back() - 'a'] = 0;
            st.pop_back();
        }
        st.push_back(c);
        vis[c - 'a'] = 1;
    }
    return st;
}

// 商品折扣后的最终价格 - 每个商品的最终价格为原价格减去右边第一个不贵于他的价格
vector<int> finalprices(vector<int> &prices) {
    stack<int> stk;
    vector res(prices);

    for (int i = 0; i < prices.size(); ++i) {
        while (not stk.empty() and prices[stk.top()] >= prices[i]) {
            res[stk.top()] -= prices[i];
            stk.pop();
        }
        stk.push(i);
    }
    return res;
}
