#include <bits/stdc++.h>
using namespace std;

// 回溯法生成所有有效括号
// 给定 n，生成所有 n 对括号的合法组合
// 思路 任意时刻左括号数量小于 n 的时候，可以放左括号
// 右括号数量严格小于左括号数量的时候，可以放右括号

vector<string> ans;

void dfs(const string &path, const int left, const int right, const int n) {
    if (static_cast<int>(path.size()) == 2 * n) {
        // 已经填充完毕
        ans.push_back(path);
        return;
    }
    if (left < n) dfs(path + '(', left + 1, right, n);
    if (right < left) dfs(path + ')', left, right + 1, n);
}

vector<string> generateParenthesis(int n) {
    dfs("", 0, 0, n);
    return ans;
}
