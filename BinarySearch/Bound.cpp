#include <bits/stdc++.h>
using namespace std;

// lowerBound and upperBound

// r = n 而非 n - 1 的原因 - 结果位不一定在 0...n-1 范围内，比如所有元素都小于 target 的时候
// lower bound - 找第一个 >= target 的位置
// 更新 r 时究竟是 大于等于 还是 大于，取决于你想不想 r 进入 target 区间
int lowerBound(const vector<int> &nums, int target) {
    int l = 0, r = static_cast<int>(nums.size());
    while (l < r) {
        // 注意不是 mid - 1，而且是 >= - 因为我们要找第一个大于等于 target 的位置
        if (int mid = l + ((r - l) >> 1); nums[mid] >= target) r = mid;
        else l = mid + 1;
    }
    return l; // 如果没有人满足，返回 nums.size
}

// upper bound - 找第一个 > 或者 <= target 的位置，此处提供 <= 版本
// 唯一的区别就是把比较时的大于等于变成严格大于，这样 r 就永远不会进入 target 区间
int upperBoundMinusOne(const vector<int> &nums, int target) {
    int l = 0, r = static_cast<int>(nums.size());
    while (l < r) {
        if (int mid = l + ((r - l) >> 1); nums[mid] > target) r = mid;
        else l = mid + 1;
    }
    return l - 1; // 如果要 >，直接返回 l
}

// example 1 - 查找 target 的 range
// 思路 - lb 找第一个 >= target 位, ub 找第一个 <= target 位, 区间非法则没有区间
// 或 lb 但找 target + 1
vector<int> searchRange(const vector<int> &nums, int target) {
    int left = lowerBound(nums, target);
    int right = upperBoundMinusOne(nums, target);
    if (left > right) return {-1, -1};
    return {left, right};
}

// example 2 - 搜索插入位置
// 本质就是找第一个 >= target 的位置
int searchInsert(const vector<int> &nums, int target) {
    int l = 0, r = static_cast<int>(nums.size());
    while (l < r) {
        int mid = l + ((r - l) >> 1);
        if (int x = nums[mid]; x >= target) r = mid;
        else l = mid + 1;
    }
    return l;
}
