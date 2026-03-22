#include <vector>
#include <algorithm>
using namespace std;

// 下一个排列
// 以 1 3 5 4 2 为例，下一格排列是 1 4 2 3 5
// 本质就是找字典序比自己大的排列中，字典序最小的那一个
// 1 - 找到进行调换的两个数，分别记作 a 和 b
// a 是后缀最大非递增序列的前驱，即 ‘5 4 2’ 的前驱 ‘3’
// 理由是后缀最大非递增序列已经无法通过调换数字产生字典序更大的排列
// 而且我们要让整个排列增加的量最少，所以不能轻易调换前面的数字，如 ‘1’
// b 是后缀最大递增序列中从右往左数的第一个比 a 大的数字
// 理由是只有选择这样的 b，与 a 进行调换才能让字典序上升，且幅度最小，所以 b 为 ‘4’
// 2 - 反转刚才的后缀最大非递增序列，即把 ‘5 3 2’ 变成 ‘2 3 5’
// 理由是让这一部分的字典序变为他能达到的最小值

void nextpermulation(vector<int> &nums) {
    int n = nums.size();
    int i = n - 2;

    while (i >= 0 and nums[i] >= nums[i + 1]) i--; // 找到 ‘3’

    if (i >= 0) {
        int j = n - 1;
        while (j >= 0 and nums[j] <= nums[i]) j--; // 找到 ‘4’
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + 1, nums.end());
}
