#include <bits/stdc++.h>
using namespace std;

// 答案二分 - 在所有可行结果中，越小越好或者越大越好
// 越小越好 - mid 满足需求，则 r = mid, mid 不满足则 l = mid + 1
// 越大越好 - mid 满足需求，则记录当前 ans 为 mid，l = mid + 1，否则 r = mid - 1

// 吃香蕉，可以自由调控每小时吃 k 根，速度越小越好
// 每次只能挑一堆香蕉来吃
// 必须在 h 小时内吃完全部香蕉
class Banana {
    static bool check(const vector<int> &piles, int h, int k) {
        int hours = 0;
        for (int x : piles) {
            hours += (x + k - 1) / k; // 吃完这一堆的耗时
        }
        return hours <= h;
    }

public:
    static int minEatSpd(const vector<int> &piles, int h) {
        int l = 1, r = *ranges::max_element(piles); // r 更大也没用，因为顶多一小时吃一堆
        while (l < r) {
            int mid = l + ((r - l) >> 1);
            if (check(piles, h, mid)) r = mid; // mid 速度合法，r = mid，往左寻找更优解
            else l = mid + 1;
        }
        return l;
    }
};

// example - leetcode 1011 在 D 天内运送包裹的能力
// days 天内按照给定顺序运送包裹，求船只最低运载能力
class DDays {
    static bool validCapacity(const vector<int> &weights, int days, int capacity) {
        // 求当前 capacity 运载所有货物所需要的天数，如果无法在 days 天内运完，返回 false
        int dayNeed = 1;
        int curTotal = 0;
        for (int w : weights) {
            if (curTotal + w > capacity) {
                dayNeed++;
                if (dayNeed > days) return false;
                curTotal = w;
            } else { curTotal += w; }
        }
        return true;
    }

public:
    static int shipWithinDays(const vector<int> &weights, int days) {
        int maxWeight = *ranges::max_element(weights);
        int l = maxWeight, r = accumulate(weights.begin(), weights.end(), 0);
        while (l < r) {
            int mid = l + ((r - l) >> 1);
            if (validCapacity(weights, days, mid)) r = mid;
            else l = mid + 1;
        }
        return l;
    }
};

// example hard - leetcode 410 分割数组的最大值
// 把非负整数数组分割成 k 个连续子数组，要求 max(acc(each subarray)) 最小
// 核心思路 - 其实和 D 天装载货物是一个模型，days 变成了 k，装载能力变成了数组和上限
// 重点是要找到‘答案’所依赖的变量，在此处，acc(each subarray) <= limit 中的 limit 越大越好
// 就像船只的容量越大越好一样
class SplitArray {
    static int validLimit(const vector<int> &nums, int k, int limit) {
        int split = 1;
        int curSum = 0;
        for (int x : nums) {
            if (curSum + x > limit) {
                split++;
                if (split > k) return false;
                curSum = x;
            } else { curSum += x; }
        }
        return true;
    }

public :
    static int splitArray(const vector<int> &nums, int k) {
        int max = *ranges::max_element(nums);
        int l = max, r = accumulate(nums.begin(), nums.end(), 0);
        while (l < r) {
            int mid = l + ((r - l) >> 1);
            if (validLimit(nums, k, mid)) r = mid;
            else l = mid + 1;
        }
        return l;
    }
};

// example leetcode 475 - 房屋供暖器 求供暖器集体半径的最小值，使得所有房屋都能有供暖
class Heat {
    static bool validRadius(const vector<int> &houses, const vector<int> &heaters, int radius) {
        int j = 0, m = static_cast<int>(heaters.size());
        for (int house : houses) {
            while (j < m and heaters[j] + radius < house) ++j;       // 供暖器太左，往右走
            if (j == m or heaters[j] - radius > house) return false; // 这个房屋没供暖器能用
        }
        return true;
    }

public:
    static int findRadius(vector<int> &houses, vector<int> &heaters) {
        // 关键优化 先排序，大幅降低 check 函数的复杂度
        ranges::sort(houses);
        ranges::sort(heaters);
        int l = 0;
        int r = max(abs(houses.back() - heaters.front()), abs(heaters.back() - houses.front()));
        while (l < r) {
            int mid = l + ((r - l) >> 1);
            if (validRadius(houses, heaters, mid)) r = mid;
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
