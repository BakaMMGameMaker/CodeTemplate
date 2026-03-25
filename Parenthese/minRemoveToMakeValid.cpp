#include <bits/stdc++.h>
using namespace std;

// 删除最少括号使得字符串合法
// - 只要求返回一个合法结果
// - 要求返回所有最少删除方案 - 难度过高，暂不描述

// 只要求返回一个
// 题目 - 字符串包含字母和括号，删除最少括号使得结果合法，返回任意一个结果
// 思路:
// 维护一个计数器，表示当前左括号的余量
// 进行两遍扫描，第一遍从左到右扫描，去除 s 中无法匹配的右括号
// 第二遍从右往左扫描，去除 s 中多出来没人要的左括号
// 删完之后剩下的结果即为最终字符串

string minRemoveToMakeValid(const string &s) {
    string t;
    int balance = 0;
    for (const char c: s) {
        if (c == '(') {
            balance++;
            t += c;
        } else if (c == ')') {
            if (balance > 0) {
                balance--;
                t += c;
            }
            // balance <= 0 时，相当于左括号不够了，删除这个右括号(不加入 t 中)
        } else {
            t += c; // 其他字母
        }
    }
    // 此时 balance 表示还有多少个没有配对的左括号
    // 二次扫描，把 t 末尾中还没有配对上的左括号删除
    string ans;
    for (int i = static_cast<int>(t.size()) - 1; i >= 0; --i) {
        if (t[i] == '(' && balance > 0) {
            balance--;
            continue; // 删掉多余的左括号
        }
        ans += t[i];
    }
    // 由于 ans 由倒序遍历而来，所以要反转一下
    ranges::reverse(ans);
    return ans;
}
