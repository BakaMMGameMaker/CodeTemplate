#include <bits/stdc++.h>
using namespace std;

// 最长合法括号子序列，子序列不一定要连续
// 贪心，尽可能匹配所有括号对即可

int longestValidSubsequence(const string &s) {
    int left = 0, pairs = 0;
    for (const char c : s) {
        if (c == '(') { left++; }
        else if (c == ')' && left > 0) {
            left--;
            pairs++;
        }
    }
    return pairs * 2;
}
