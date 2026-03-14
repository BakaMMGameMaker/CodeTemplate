#include <bits/stdc++.h>
using namespace std;

// 逆波兰表达式求值

int evalRPN(const vector<string> &tokens) {
    stack<int> st;
    for (const string &s : tokens) {
        if (s == "+" || s == "-" || s == "*" || s == "/") { // operation
            int b = st.top();
            st.pop();
            int a = st.top();
            st.pop();
            if (s == "+") st.push(a + b);
            else if (s == "-") st.push(a - b);
            else if (s == "*") st.push(a * b);
            else st.push(a / b);
        } else {
            st.push(stoi(s));
        }
    }
    return st.top();
}
