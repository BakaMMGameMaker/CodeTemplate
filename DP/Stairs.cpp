#include <vector>

// 爬楼梯问题

// 1 - 方案数
int climb(int n) {
    if (n <= 2) return n;
    int a = 1, b = 2;
    for (int i = 3; i <= n; i++) {
        int c = a + b;
        a = b;
        b = c;
    }
    return b;
}

// 2 - 爬到最后一个台阶的下一个台阶的最小花费
// 其中离开每个台阶的 cost 由数组给出
int minCost(const std::vector<int> &cost) {
    int a = 0, b = 0; // 初始化为爬到 0 和 1 的最小花费, 初始不用给钱所以都为 0
    for (int i = 2; i <= cost.size(); ++i) {
        int c = std::min(b + cost[i - 1], a + cost[i - 2]);
        a = b;
        b = c;
    }
    return b;
}
