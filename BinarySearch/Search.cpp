#include <bits/stdc++.h>
using namespace std;

// 二分查找基础
// 区间究竟是开区间还是闭区间，取决于答案可能落在哪里
// while 条件为 < 还是 <= 主要取决于 r 的更新方式
// 如果有充足证据证明 mid 位置不可能为结果，那么 r = mid - 1，否则 r = mid
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

// 搜索二维矩阵
// 每行首元素 > 上一行末尾元素
// 本质还是一个一维有序数组
bool searchMatrix(const vector<vector<int> > &matrix, int target) {
    int m = static_cast<int>(matrix.size()), n = static_cast<int>(matrix.back().size());
    int l = 0, r = m * n - 1;
    while (l <= r) {
        int mid = l + ((r - l) >> 1);
        int x = matrix[mid / n][mid % n];
        if (x == target) return true;
        if (x < target) l = mid + 1;
        else r = mid - 1;
    }
    return false;
}
