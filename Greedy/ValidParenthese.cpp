#include <bits/stdc++.h>
using namespace std;

// 含 * 号的有效括号，其中 * 可以表示左括号，右括号或者空串
// 维护一个左括号余量范围 [low, high]
// 遇到左括号，那么二者++；遇到右括号，二者--；遇到星号，low--而high++
// 其中 low 不小于 0，high 小于 0 则说明右括号太多，无论怎么选都无法构成有效括号
// 最后只要 low 为 0 就合法

bool checkValid(const string &s) {
    int low = 0, high = 0;
    for (const char c: s) {
        if (c == '(') {
            low++;
            high++;
        } else if (c == ')') {
            low--;
            high--;
        } else {
            low--;
            high++;
        }
        if (high < 0)return false; // 中途检测到右括号过多
        low = max(low, 0); // 防止星号过多导致 low 小于 0
    }
    return low == 0;
}
