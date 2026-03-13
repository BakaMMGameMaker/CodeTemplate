#include <bits/stdc++.h>
using namespace std;

// 旋转数组的二分查找
// 依然查找具体值，依然闭区间和小于等于
// 思路 根据 left 与 mid 的大小关系来判断 [left...mid] 和 [mid...right] 哪一个有序
// 哪个有序就去哪边搜索
// 再根据 target 是否位于当前有序区间，来决定 left 和 right 更新的方式
int search(const vector<int> &nums, int target) {
    int l = 0, r = static_cast<int>(nums.size()) - 1;
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        int x = nums[mid];
        if (x == target) return mid;

        if (nums[l] <= x) {
            // l...mid 有序
            // 注意只有一边是带等号，因为 x == target 已经在上面判断过了
            if (nums[l] <= target and target < x) r = mid - 1; // target 在 l...mid-1 区间内
            else l = mid + 1;
        } else {
            // mid...r 有序
            if (target > x and target <= nums[r]) l = mid + 1; // target 在 mid+1...r 区间内
            else r = mid - 1;
        }
    }
    return -1;
}

// example 1 - 旋转排序数组中的最小值
// 思路 - 最小值的右手边一定为有序区间，包括最小值自己
// 所以我们只需要看 mid...r 的区间
// 如果不是有序区间，说明 mid 太小，更新 l，由于 mid 位置不可能为最小值位，所以更新为 mid + 1
// 如果是有序区间，说明结果位一定 <= mid，但不能排除 mid 是答案本身，所以 r 更新为 mid
// 由于 r 更新为 mid 而非 mid - 1，所以循环条件是严格小于而不是小于等于
int findMin(const vector<int> &nums) {
    int l = 0, r = static_cast<int>(nums.size() - 1);
    while (l < r) { // 如果带等号会陷入死循环，因为 r 只更新为 mid 而非 mid - 1
        int mid = l + ((r - l) >> 1);
        if (nums[mid] > nums[r]) l = mid + 1; // 说明 mid...r 不单增，最小值肯定在右边
        else r = mid;                         // mid 而非 mid - 1 的原因是不能排除 mid 位置为最小值位置的可能性
    }
    return nums[l];
    // 如果要找最大值，就返回 nums[(l - 1 + n) % n]
}

// example 2 - 含有重复值的旋转数组的最小值
// 核心 - 当 nums[mid] == nums[r] 的时候，不能断定 mid...r 有序，因为中间可能会凹下去
int findMinII(const vector<int> &nums) {
    int l = 0, r = static_cast<int>(nums.size() - 1);
    while (l < r) {
        int mid = l + ((r - l) >> 1);
        if (nums[mid] == nums[r]) r--;
        else if (nums[mid] > nums[r]) l = mid + 1;
        else r = mid;
    }
    return l;
}
