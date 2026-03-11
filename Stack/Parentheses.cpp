#include <bits/stdc++.h>
using namespace std;

// 栈括号题模板

// 一种括号 (): 计数法
// 遇到左括号则++，遇到右括号则--,中途小于 0 则非法，最终等于 0 才合法
bool isValidParentheses(const string &s) {
    int balance = 0;
    for (const char c: s) {
        if (c == '(') balance++;
        else if (c == ')') balance--;
        if (balance < 0) return false;
    }
    return balance == 0;
}

// 多种括号 - 栈
// 遇到左括号，入栈；遇到右括号，检查栈顶是否匹配；不匹配则非法，结束时栈需空
bool isValid(const string &s) {
    stack<char> st;
    unordered_map<char, char> mp = {
        {')', '('},
        {']', '['},
        {'}', '{'}
    };
    for (const char c: s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);
        } else {
            if (st.empty() || st.top() != mp[c]) return false;
            st.pop();
        }
    }
    return st.empty();
}

// 最长有效括号
// 栈放下标，初始化 -1 作为初始基准值，遇到左括号入栈，遇到右括号弹栈
// 如果弹完后栈空，则当前右括号没法匹配，压入当前下标作为新的基准
// 否则更新当前合法长度
int longestValidStack(const string &s) {
    stack<int> st;
    st.push(-1);
    int ans = 0;
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '(') {
            st.push(i);
        } else {
            st.pop();
            if (st.empty()) {
                st.push(i); // 作为新的基准值
            } else {
                ans = max(ans, i - st.top()); // 没空说明构成了合法子串，计算长度
            }
        }
    }
    return ans;
}
