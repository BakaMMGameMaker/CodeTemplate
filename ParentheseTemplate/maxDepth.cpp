#include <bits/stdc++.h>
using namespace std;

// 计算括号的最大嵌套深度
// 遇到左括号加一，遇到右括号减一

int maxDepth(const string &s) {
    int depth = 0, ans = 0;
    for (const char c : s) {
        if (c == '(') {
            depth++;
            ans = max(ans, depth);
        } else if (c == ')') { depth--; }
    }
    return ans;
}
