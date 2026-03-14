#pragma once

#include <bits/stdc++.h>
using namespace std;

// 表达式求值 - 四则运算 + 括号优先级
// 思路 两栈
// 一栈存数字, 一栈存运算符
// 遇到数字, 读完后存 nums
// 遇到左括号, 进 ops
// 遇到右括号, 不断计算直到弹出左括号
// 遇到普通运算符, 根据优先级把栈内优先级更高或者相等的运算做掉, 再把当前运算符入栈
// 最后做完剩余的运算
int calculate(const string &s) {
    stack<int> nums;
    stack<char> ops;
    size_t n = s.size();

    unordered_map<char, int> priority{{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}};

    auto eval = [&] {
        int b = nums.top();
        nums.pop();
        int a = nums.top();
        nums.pop();
        char op = ops.top();
        ops.pop();

        if (op == '+') nums.push(a + b);
        else if (op == '-') nums.push(a - b);
        else if (op == '*') nums.push(a * b);
        else if (op == '/') nums.push(a / b);
    };

    for (size_t i = 0; i < n; ++i) {
        if (s[i] == ' ') continue;

        if (isdigit(s[i])) { // 数字
            int num = 0;
            while (i < n and isdigit(s[i])) {
                num = num * 10 + (s[i] - '0');
                i++;
            }
            i--;
            nums.push(num);
        } else if (s[i] == '(') {
            ops.push(s[i]);
        } else if (s[i] == ')') {
            while (!ops.empty() and ops.top() != '(') {
                eval(); // 求值并放入栈中
            }
            ops.pop(); // 弹出左括号
        } else {
            // 前置正负号时的小技巧, 可以 push 一个 0 进去
            // 比如 -2+3 会被当作 0-2
            // 这样就可以不用对这个数字进行额外的处理
            if ((s[i] == '+' or s[i] == '-') and
                (i == 0 or s[i - 1] == '(' or priority.count(s[i - 1])))
                nums.push(0);

            while (!ops.empty() and priority[ops.top()] >= priority[s[i]]) {
                if (ops.top() == '(') break;
                eval();
            }
            ops.push(s[i]);
        }
    }

    while (!ops.empty()) eval();
    return nums.top();
}

// 中缀转后缀 (RPN)
vector<string> infixToPostfix(const string &s) {
    vector<string> output;
    stack<char> ops;
    size_t n = s.size();

    unordered_map<char, int> priority{{'+', 1}, {'-', 1}, {'*', 2}, {'/', 2}};

    for (size_t i = 0; i < n; ++i) {
        if (s[i] == ' ') continue;

        if (isdigit(s[i])) {
            string num;
            while (i < n and isdigit(s[i])) {
                num += s[i];
                i++;
            }
            i--;
            output.push_back(num);
        } else if (s[i] == '(') {
            ops.push(s[i]);
        } else if (s[i] == ')') {
            while (not ops.empty() and ops.top() != '(') {
                output.emplace_back(1, ops.top());
                ops.pop();
            }
            if (not ops.empty()) ops.pop(); // 弹出左括号
        } else {
            while (!ops.empty() and ops.top() != '(' and priority[ops.top()] >= priority[s[i]]) {
                output.emplace_back(1, ops.top());
                ops.pop();
            }
            ops.push(s[i]);
        }
    }

    while (not ops.empty()) {
        output.emplace_back(1, ops.top());
        ops.pop();
    }
    return output;
}

// 逆波兰表达式求值 (必须掌握)
// 核心思路 - 遇到运算符就从栈里弹两个数字出来进行运算, 放回栈里
// 遇到数字就直接放到栈里
// 最终输出栈顶值即可
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
