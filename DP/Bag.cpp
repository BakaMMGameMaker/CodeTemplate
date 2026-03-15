#include <bits/stdc++.h>
using namespace std;

// 一维 01 背包可不装满
void bag() {
    int V = 5; // 背包空间
    // 物品体积与物品价值
    vector<pair<int, int> > items{{2, 10}, {4, 5}, {1, 4}};

    /*
     * dp[v] 为使用前 v 格空间时，能装载的最大总价值。由于可以不装满，所以 dp 初始化为 0
     * 遍历每一个物体，在内部对背包进行倒序遍历
     * 倒序遍历的目的是让每个物体只用一次，防止一次遍历中后面的 dp[j] 的更新用到了前面的 dp[i]
     * 倒序遍历中，对每个 dp[v]，如果不放这个物品，那么价值不变，否则为 dp[v - 体积i] + 价值i
     * 两者取最大值
     * 如果是完全背包（每个物品数量无限），只需要把倒序遍历变为正序遍历即可
     * 空间越大自然能装的价值越多，所以直接返回 back 即可
     */
    vector<int> dp(V + 1);
    for (auto [volume, value] : items) {
        for (int v = V; v >= volume; --v) {
            dp[v] = max(dp[v], dp[v - volume] + value);
        }
    }
    cout << dp.back() << endl;
}

// 一维 01 背包必须装满
void fullbag() {
    int V = 5; // 背包空间
    // 物品体积与物品价值
    vector<pair<int, int> > items{{2, 10}, {4, 5}, {1, 4}};

    /*
     * dp[v] 为装满前 v 格空间时的最大总价值。由于必须装满，因此 dp 要初始化为非法价值，如 -1
     * 需要注意的是，dp[0] 需要初始化为 0，因为 0 容量天然被装满，且总价值为 0
     * 遍历每一个物体，在内部对背包进行倒序遍历
     * 倒序遍历中，对每个 dp[v]，如果放这个物品，那么价值不变，否则为 dp[v - 体积i] + 价值i
     * 注意此时需要判断 dp[v - 体积i] 是否为非法价值，如果是，说明此次物品装填是无效的
     */
    vector<int> dp(V + 1, -1);
    dp[0] = 0;
    for (auto [volume, value] : items) {
        for (int v = V; v >= volume; --v) {
            if (dp[v - volume] == -1) continue;
            dp[v] = max(dp[v], dp[v - volume] + value);
        }
    }
    if (dp.back() == -1) cout << 0 << endl;
    else cout << dp.back() << endl;
}

// example - 找零, 本质是完全背包, 每种硬币有无限个
// 总体积就是需要找零的最终数值, 单个物品体积就是其面额
// 这道题不能贪心的原因是, 我们要精确凑到 amount, 而不是 >= amount 的最少数量
// 假设有 1 3 4 要凑 6, 如果贪心, 会找到 4 1 1, 而真正方案是 3 3
int change(vector<int> &coins, int amount) {
    vector dp(amount + 1, INT32_MAX); // 找 [k] 数额的最小硬币数量
    dp[0] = 0;
    for (int i = 1; i <= amount; ++i) {
        for (int coin : coins) {
            // 需要初始化为 int32 max 而非 0 的原因体现在这里
            // i < mincoin 的时候, 所有 dp 都是 0
            // 然而它们是凑不出来, 而不是只需要 0 枚硬币就能凑出来
            // 如果初始化为 0, min 的时候就会得到到错误结果
            if (i >= coin and dp[i - coin] != INT32_MAX) {
                // 假设 coin = 5, 找 i - 5 的最少方案数量为 a
                // 那么如果用这个硬币, 最少硬币数量就变成了 a + 1
                dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
    }
    return dp[amount] == INT32_MAX ? -1 : dp[amount];
}
