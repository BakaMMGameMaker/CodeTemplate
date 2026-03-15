#include <string>
#include <vector>
using namespace std;

// 翻译数字的总方法数, 1->a, 2->b, ..., 25->z

int translate(int num) {
    string s = to_string(num);
    int n = s.size();
    vector dp(n + 1, 0); //前 k 个字符总共有多少种翻译的方法
    dp[0] = 1;
    dp[1] = 1;
    for (int i = 2; i <= n; ++i) {
        // 注意, 我们在计算 dp[i] 的时候, 是在对 s[i-1] 进行考察
        // 假设 s[i-1] 不与 s[i-2] 组合, 那么 dp[i] = d[i-1]
        // 否则还能额外获得 dp[i-2] 种组合方法
        dp[i] = dp[i - 1];
        int val = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');
        if (val >= 10 and val <= 25) dp[i] += dp[i - 2];
    }
    return dp[n];
}

// 空间优化, 只有 dp[i-1] 与 dp[i-2] 参与计算
int o1(int num) {
    string s = to_string(num);
    int n = s.size();

    int a = 1, b = 1; // 使用前 0 和前 1 个字符, 都只有一种翻译方式
    for (int i = 2; i <= n; ++i) {
        int c = b;
        int val = (s[i - 2] - '0') * 10 + (s[i - 1] - '0');
        if (val >= 10 and val <= 25) c += a;
        a = b;
        b = c;
    }
    return b;
}

// 递归解法 - 仅说明, 不推荐
int recursive(int num) {
    auto dfs = [&](auto &&self, const string &s, int i) {
        if (i >= s.size() - 1) return 1;
        int res = self(self, s, i + 1); // 这个字符自成一家
        // 考虑和后面的字符结合
        int val = (s[i] - '0') * 10 + (s[i + 1] - '0');
        if (val >= 10 and val <= 25) res += self(self, s, i + 2);
        return res;
    };

    return dfs(dfs, to_string(num), 0);
}
