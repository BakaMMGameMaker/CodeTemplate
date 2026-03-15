#include <vector>
using namespace std;

// 缺失的第一个整数 - 原地哈希

// 把 x 放到 x - 1 位置上, 处理完毕后, 若 nums[x] != x + 1, 答案就是 x + 1
// 如果 nums 中每个都满足, 那么答案就是 n + 1
int firstMissingPositive(vector<int> &nums) {
    int n = static_cast<int>(nums.size());

    for (int i = 0; i < n; ++i) {
        // 注意是 while 循环, 因为换过来的数字可能还是错位的
        // 前两条是为了第三条服务, 因为 0 <= nums[i] - 1 <= n - 1
        while (nums[i] >= 1 and nums[i] <= n and nums[nums[i] - 1] != nums[i]) { // 必须有第三条, 不然死循环
            swap(nums[i], nums[nums[i] - 1]);
        }
    }

    for (int i = 0; i < n; ++i) if (nums[i] != i + 1) return i + 1;

    return n + 1;
}
