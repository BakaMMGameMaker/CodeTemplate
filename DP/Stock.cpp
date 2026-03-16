#include <cstdint>
#include <vector>
using namespace std;

// 买卖股票最好时机 1 2 3

// 1 - 只能进行一次交易
// 思路 - 维护当前最小值, 实时更新最大收益
int maxprofit(vector<int> &prices) {
    int minprice = INT32_MAX;
    int ans = 0;
    for (int price : prices) {
        minprice = min(minprice, price);
        ans = max(ans, price - minprice);
    }
    return ans;
}

// 2 - 可以进行多次交易, 但是同一时刻只能持有一只股票
// 由于可以进行无限次交易, 所以我们见好就收, 发现有上涨立即从中获益
int maxprofit2(vector<int> &prices) {
    int ans = 0;
    for (int i = 1; i < prices.size(); ++i) {
        if (prices[i] > prices[i - 1]) ans += prices[i] - prices[i - 1];
    }
    return ans;
}

// 含有冷冻期的无限次交易, 卖出后硬控一天
// 三个状态 - hold 手里持有股票的最大收益, sold - 今天卖出的最大收益, rest - 今天啥也不干也没股票的最大收益
int maxprofitwithcool(vector<int> &prices) {
    int hold = -prices[0];
    int sold = 0, rest = 0;
    for (int i = 1; i < prices.size(); ++i) {
        int ph = hold, ps = sold, pr = rest;
        hold = max(ph, pr - prices[i]); // 不在高点买入
        sold = ph + prices[i];          // 若今天卖出, 手里最多会有多少钱
        rest = max(pr, ps);             // 今天不卖出手里也没股票, 那么金币最大值就是 max(昨天开摆, 昨天卖股票)
    }
    return max(sold, rest); // 今天卖掉股票或者今天无事发生
}

// 含有手续费的无限次交易, 卖出时扣除 fee
int maxprofitwithfee(vector<int> &prices, int fee) {
    int hold = -prices[0]; // 当天结束后持股的最大收益
    int cash = 0;          // 当前结束后不持股的最大收益
    for (int i = 1; i < prices.size(); ++i) {
        int ph = hold;
        int pc = cash;
        hold = max(ph, pc - prices[i]);
        cash = max(pc, ph + prices[i] - fee);
    }
    return cash;
}

// 3 - 只允许进行两次交易, 同时只能持有一只股票
// 定义四个状态
// buy1 - 第一次买入后手里的最大收益
// sell1 - 第一次卖出后手里的最大收益
// buy2, sell2
int maxprofit3(vector<int> &prices) {
    // 初始化的解释
    // 买完之后亏是正常的
    // 卖了之后要是别赚钱那就别卖了
    int buy1 = INT32_MIN, sell1 = 0;
    int buy2 = INT32_MIN, sell2 = 0;

    for (int price : prices) {
        // buy1 更新 : 是否要在今天进行第一次买入? 买入后能否达到更优状态?
        buy1 = max(buy1, -price);

        // sell1 更新 : 是否要在今天进行第一次卖出? 卖出后能否达到更优状态?
        sell1 = max(sell1, buy1 + price);
        buy2 = max(buy2, sell1 - price);
        sell2 = max(sell2, buy2 + price);
    }
    return sell2;
}

// 最多进行 k 次交易
// 如果 k 大于 n / 2, 相当于 maxprofit2 可以进行无限次交易
// 否则依旧维护 buy 和 sell 的最优状态, 只不过变成数组
int maxprofit4(int k, vector<int> &prices) {
    int n = prices.size();
    if (n == 0) return 0;
    if (k >= n / 2) {
        int ans = 0;
        for (int i = 1; i < n; ++i) {
            if (prices[i] > prices[i - 1]) ans += prices[i] - prices[i - 1];
        }
        return ans;
    }

    vector buy(k + 1,INT32_MIN), sell(k + 1, 0);

    for (int price : prices) {
        for (int i = 1; i <= k; ++i) {
            buy[i] = max(buy[i], sell[i - 1] - price); // 上一次卖完后再花 price 元买股票
            sell[i] = max(sell[i], buy[i] + price);    // 这次买入后又获得 price 元
        }
    }
    return sell[k];
}
