#include <vector>
using namespace std;

// 环形打家劫舍

int robrange(vector<int> &nums, int l, int r) {
    int pre2 = 0, pre1 = 0;
    for (int i = l; i <= r; ++i) {
        int cur = max(pre1, pre2 + nums[i]);
        pre2 = pre1;
        pre1 = cur;
    }
    return pre1;
}

int rob(vector<int> &nums) {
    int n = nums.size();
    if (n == 1) return nums[0];
    if (n == 2) return max(nums[0], nums[1]);
    return max(robrange(nums, 0, n - 2), robrange(nums, 1, n - 1));
}
