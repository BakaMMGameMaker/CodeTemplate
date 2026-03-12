#include <bits/stdc++.h>
using namespace std;

// 二分峰值模板

// leetcode 162 - 寻找峰值
// 返回任意一索引，满足其左右两侧元素小于自己
// -1 和 n 位置为地核

// 这道题与其说是缩小索引可能的区间，不如说是每次砍掉一半搜索区间
// 即便被砍掉的部分可能存在结果也不管，因为剩余的区间一定存在答案
// 因为两侧为地核，所以单增会去到 n-1 位置，单减会去到 0 位置
// 如果不单调，那就更说明原数组存在峰值

// 如果 mid 到 mid + 1 是上升的，砍掉左半边不影响答案
// 因为如果 mid+1...n-1 有起伏，那就说明有答案，如果接着上升，那么答案在末尾
// 所以无论如何都可以放心地只搜索右边
int findPeakElement(const vector<int> &nums) {
    int l = 0, r = static_cast<int>(nums.size()) - 1;
    while (l < r) {
        int mid = l + ((r - l) >> 1);
        if (nums[mid] < nums[mid + 1]) l = mid + 1;
        else r = mid;
    }
    return l;
}
