#include <bits/stdc++.h>
using namespace std;

// 求每一个括号的匹配位置
// 栈存左括号下标，遇到右括号时弹出并记录

vector<int> matchParentheses(const string &s) {
    int n = static_cast<int>(s.size());
    vector<int> match(n, -1);
    stack<int> st;

    for (int i = 0; i < n; ++i) {
        if (s[i] == '(') {
            st.push(i);
        } else if (s[i] == ')') {
            const int j = st.top();
            st.pop();
            match[i] = j;
            match[j] = i;
        }
    }
    return match;
}
