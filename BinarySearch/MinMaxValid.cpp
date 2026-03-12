#include <bits/stdc++.h>
using namespace std;

// 答案二分 - 在所有可行结果中，越小越好或者越大越好
// 越小越好 - mid 满足需求，则 r = mid, mid 不满足则 l = mid + 1
// 越大越好 - mid 满足需求，则记录当前 ans 为 mid，l = mid + 1，否则 r = mid - 1

// 吃香蕉，可以自由调控每小时吃 k 根，速度越小越好
// 每次只能挑一堆香蕉来吃
// 必须在 h 小时内吃完全部香蕉
class Banana {
    bool check(const vector<int> &piles, int h, int k) {
        int hours = 0;
        for (int x : piles) {
            hours += (x + k - 1) / k; // 吃完这一堆的耗时
        }
        return hours <= h;
    }

    int minEatSpd(const vector<int> &piles, int h) {
        int l = 1, r = *ranges::max_element(piles); // r 更大也没用，因为顶多一小时吃一堆
        while (l < r) {
            int mid = l + ((r - l) >> 1);
            if (check(piles, h, mid)) r = mid; // mid 速度合法，r = mid，往左寻找更优解
            else l = mid + 1;
        }
        return l;
    }
};

// example - x 的平方根
// 在 [0...x] 内寻找最大的满足 mid * mid <= x 的 target
int BinarySqrt(int x) {
    int l = 0, r = x;
    int ans = 0;
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        if (mid * mid <= x) { // 相当于 check = true
            ans = mid;        // 保留结果
            l = mid + 1;
        } else { r = mid - 1; }
    }
    return ans;
}
