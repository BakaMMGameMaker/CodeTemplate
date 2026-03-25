#include <bits/stdc++.h>
using namespace std;

// 最长有效括号 - 左右扫描法
// 还有其它两种解法见 Stack/ 与 DP/
int longestValidParentheses(const string &s) {
    int left = 0, right = 0, ans = 0;
    for (const char c: s) {
        if (c == '(')left++;
        else right++;
        if (left == right)ans = max(ans, 2 * right);
        else if (right > left)left = right = 0;
    }
    // 只进行一边扫描会漏答案，如 left 永远大于 right 的时候
    left = right = 0;
    for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
        if (s[i] == '(')left++;
        else right++;
        if (left == right)ans = max(ans, 2 * left);
        else if (left > right)left = right = 0;
    }
    return ans;
}
