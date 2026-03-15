#include <algorithm>
#include <vector>
using namespace std;

// 三数之和 - 找到所有 a + b + c = 0 的三元组

// 思路 - 排序 + 双指针
// 固定第一个数字, 然后在后面的区间使用双指针寻找剩下的两个数字使得三数之和为 0

// 剪枝
// 1 - 第一个数字大于 0 时, 不可能存在合法三元组, 直接返回
// 2 - 重复数字直接跳过, 避免产生重复结果, 同理左右指针

vector<vector<int> > threeSum(vector<int> &nums) {
    vector<vector<int> > ans;
    ranges::sort(nums);
    if (nums.back() < 0) return {};
    int n = static_cast<int>(nums.size());

    for (int i = 0; i < n - 2; ++i) {
        if (nums[i] > 0) break;
        if (i > 0 && nums[i] == nums[i - 1]) continue; // 跳过重复值
        // 这里能安全跳过, 是因为特殊的 000 和 aab, b = -2a 的情况肯定在之前已经考虑过了
        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum < 0) {
                ++left;
            } else if (sum > 0) {
                --right;
            } else {
                ans.push_back({nums[i], nums[left], nums[right]});
                ++left;
                --right;

                // 跳过重复值
                while (left < right and nums[left] == nums[left - 1]) ++left;
                while (left < right and nums[right] == nums[right + 1]) --right;
            }
        }
    }
    return ans;
}

// 要求三数之和为 target, 去掉剪枝, 把判断 0 的位置改为判断 target
vector<vector<int> > threeSum(vector<int> &nums, int target) {
    vector<vector<int> > ans;
    ranges::sort(nums);
    int n = static_cast<int>(nums.size());

    for (int i = 0; i < n - 2; ++i) {
        if (i > 0 && nums[i] == nums[i - 1]) continue; // 跳过重复值
        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];
            if (sum < target) {
                ++left;
            } else if (sum > target) {
                --right;
            } else {
                ans.push_back({nums[i], nums[left], nums[right]});
                ++left;
                --right;

                while (left < right and nums[left] == nums[left - 1]) ++left;
                while (left < right and nums[right] == nums[right + 1]) --right;
            }
        }
    }
    return ans;
}

// 最接近的三数之和 - 从中选择三个数, 让它们的和尽可能接近 target, 并返回这三个数字的和
// 区别 - 不再找恰好等于 target 的方案
// 维护当前最优答案 best, 使用 abs(sum - target) 来更新最优答案
// 这一题去不去重无所谓, 就算重复计算也不会影响答案的正确性
int threeSumClosest(vector<int> &nums, int target) {
    int n = static_cast<int>(nums.size());
    ranges::sort(nums);
    int best = nums[0] + nums[1] + nums[2];

    for (int i = 0; i < n - 2; ++i) {
        int left = i + 1, right = n - 1;
        while (left < right) {
            int sum = nums[i] + nums[left] + nums[right];

            // 更新最优答案
            if (abs(sum - target) < abs(best - target)) best = sum;

            if (sum < target) ++left;
            else if (sum > target) --right;
            else return target; // 不可能存在更优解
        }
    }
    return best;
}

// 通用 nSum, 本质就是外层枚举, 最后 2Sum 双指针
vector<vector<int> > nSum(vector<int> &nums, int n, int start, int target) {
    vector<vector<int> > ans;
    int size = static_cast<int>(nums.size());

    if (n < 2 or size - start < n) return ans; // 如只剩两个数, 凑不出 3Sum
    if (n == 2) {
        // 双指针
        int left = start, right = size - 1;
        while (left < right) {
            int sum = nums[left] + nums[right];
            if (sum < target) {
                ++left;
            } else if (sum > target) {
                --right;
            } else {
                ans.push_back({nums[left], nums[right]});
                ++left;
                --right;

                while (left < right and nums[left] == nums[left - 1]) ++left;
                while (left < right and nums[right] == nums[right + 1]) --right;
            }
        }
    } else {
        for (int i = start; i <= size - n; ++i) {
            if (i > start and nums[i] == nums[i - 1]) continue; // 去重
            vector<vector<int> > sub = nSum(nums, n - 1, i + 1, target - nums[i]);
            for (auto &vec : sub) {
                vec.insert(vec.begin(), nums[i]);
                ans.push_back(vec);
            }
        }
    }
    return ans;
}

vector<vector<int> > fourSum(vector<int> &nums, int target) {
    ranges::sort(nums);
    return nSum(nums, 4, 0, target);
}
