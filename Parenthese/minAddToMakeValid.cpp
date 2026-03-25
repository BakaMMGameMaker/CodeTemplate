#include <bits/stdc++.h>
using namespace std;

// 添加最少括号使得字符串合法
// needRight: 当前需要多少个右括号
// 遇到左括号，++；遇到右括号，若大于 0，匹配一个，否则说明缺少一个左括号

int minAddToMakeValid(const string &s) {
    int needRight = 0, addLeft = 0;
    for (const char c: s) {
        if (c == '(') {
            needRight++;
        } else {
            if (needRight > 0)needRight--;
            else addLeft++; // 注意不能接着 needRight--，因为必须立即添加一个左括号在右括号左手边
        }
    }
    return addLeft + needRight;
}
