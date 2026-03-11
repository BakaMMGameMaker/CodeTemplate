#include <bits/stdc++.h>
using namespace std;

// 一维 01 背包可不装满模板

void BagNotFull() {
    constexpr int V = 5; // 背包空间
    // 物品体积与物品价值
    vector<pair<int, int> > items{{2, 10}, {4, 5}, {1, 4}};

    /*
     * 解法说明：
     * dp[v] 为使用前 v 格空间时，能装载的最大总价值。由于可以不装满，所以 dp 初始化为 0
     * 遍历每一个物体，在内部对背包进行倒序遍历
     * 倒序遍历的目的是让每个物体只用一次，防止一次遍历中后面的 dp[j] 的更新用到了前面的 dp[i]
     * 倒序遍历中，对每个 dp[v]，如果放这个物品，那么价值不变，否则为 dp[v - 体积i] + 价值i
     * 如果是完全背包（每个物品数量无限），只需要把倒序遍历变为正序遍历即可
     * 两者取最大值即可
     * 空间越大自然能装的价值越多，所以直接返回 back 即可
     */
    vector<int> dp(V + 1);
    for (auto [volume, value]: items) {
        for (int v = V; v >= volume; --v) {
            dp[v] = max(dp[v], dp[v - volume] + value);
        }
    }
    cout << dp.back() << endl;
}
