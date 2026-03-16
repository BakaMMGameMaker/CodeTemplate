#include <vector>
using namespace std;

// 分糖果问题, 每人至少一个, 得分更高的孩子比相邻的拿更多
// 思路 - 两次遍历
// 从左到右扫一遍, 如果分数比左边的高, 糖果数量比左边多 1
// 从右到左扫一遍, 如果分数比右边的高, 糖果数 '至少' 比右边多 1
// 本质是确保从左往右看和从右往左看都能符合题目要求
int candy(vector<int> &ratings) {
    int n = ratings.size();
    vector candies(n, 1);
    for (int i = 1; i < n; ++i) {
        if (ratings[i] > ratings[i - 1]) candies[i] = candies[i - 1] + 1;
    }

    for (int i = n - 2; i >= 0; --i) {
        if (ratings[i] > ratings[i + 1]) candies[i] = max(candies[i], candies[i + 1] + 1);
    }

    int ans = 0;
    for (int x : candies) ans += x;
    return ans;
}

// 如果孩子们站成一个圈, 那么先找一个局部最低点, 给一颗糖果, 然后从这里断开, 继续当作一维数组做
int circle(vector<int> &ratings) {
    int n = ratings.size();
    if (n == 1) return 1;

    int start = 0;
    for (int i = 0; i < n; ++i) {
        int pre = (i - 1 + n) % n;
        int nxt = (i + 1) % n;
        if (ratings[i] <= ratings[pre] and ratings[i] <= ratings[nxt]) {
            start = i;
            break;
        }
    }

    vector candies(n, 1);

    // 两次遍历, 只不过从 start 开始
    for (int k = 1; k < n; ++k) {
        int cur = (start + k) % n;
        int pre = (start + k - 1) % n;
        if (ratings[cur] > ratings[pre]) candies[cur] = candies[pre] + 1;
    }

    for (int k = n - 2; k >= 0; --k) {
        int cur = (start + k) % n;
        int nxt = (start + k + 1) % n;
        if (ratings[cur] > ratings[nxt]) candies[cur] = max(candies[cur], candies[nxt] + 1);
    }

    int ans = 0;
    for (int x : candies) ans += x;
    return ans;
}
