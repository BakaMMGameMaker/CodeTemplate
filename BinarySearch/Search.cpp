#include <bits/stdc++.h>
using namespace std;

// 二分查找基础
// 二分查找最好用闭区间，不要给自己没事找事
// 条件判断是小于等于，不是小于，因为 l==r 也构成一个区间，只不过只有一个数字
int search(const vector<int> &nums, int target) {
    int l = 0, r = static_cast<int>(nums.size()) - 1;
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        if (nums[mid] == target) return mid;
        if (nums[mid] < target) l = mid + 1;
        else r = mid - 1; // 不是 r = mid
    }
    return -1;
}
