#include <bits/stdc++.h>
using namespace std;

// 最小的 K 个数字

// 做法一 大根堆
vector<int> leastK(const vector<int> &arr, int k) {
    if (k <= 0) return {};
    if (k >= static_cast<int>(arr.size())) return arr;

    priority_queue<int> pq;

    for (int i = 0; i < k; ++i) pq.push(arr[i]);

    for (int i = k; i < static_cast<int>(arr.size()); ++i) {
        if (arr[i] < pq.top()) {
            pq.pop();
            pq.push(arr[i]);
        }
    }

    vector<int> res;
    while (!pq.empty()) {
        res.push_back(pq.top());
        pq.pop();
    }
    return res;
}

// 快速选择
vector<int> quickSelectLeastK(vector<int> &arr, int k) {
    if (k <= 0) return {};
    if (k >= arr.size()) return arr;

    auto partition = [&](size_t left, size_t right) {
        size_t mid = left + ((right - left) >> 1);
        swap(arr[left], arr[mid]); // 技巧 - 仿佛一开始就是选择 left 位置为基准值一样
        int pivot = arr[left];
        size_t i = left, j = right;
        while (i < j) {
            // 注意必须先 --j 再 ++i, 顺序不能反过来
            // 因为 left 位置其实是一个坑位, 如果先 ++i, 会导致坑位被遗忘
            // 比如 22332 => 32332, 开局直接 ++i, 就会导致结果错误
            while (i < j and arr[j] >= pivot) --j;
            while (i < j and arr[i] <= pivot) ++i;
            // 运行到这里时, 有 arr[j] < pivot and arr[i] > pivot
            // 交换他们万事大吉
            if (i < j) swap(arr[i], arr[j]);
        }
        swap(arr[left], arr[i]);
        return i;
    };

    auto quickSelect = [&](auto &&self, size_t left, size_t right) {
        if (left >= right) return;
        size_t pivotIndex = partition(left, right); // 分好左右并获取 pivot 的下标

        if (pivotIndex == k - 1) return;
        if (pivotIndex > k - 1) self(self, left, pivotIndex - 1);
        else self(self, pivotIndex + 1, right);
    };

    quickSelect(quickSelect, 0, arr.size() - 1);
    return {arr.begin(), arr.begin() + k};
}

// 寻找第 k 大
// 核心思路 - 我们希望最终 pivotIndex = target 的时候, 右边有 k-1 个比 pivot 大的数字
// 由此可知, target = n - k
int findKth(vector<int> &nums, int k) {
    size_t n = nums.size();
    size_t t = n - k;
    auto partition = [&](size_t left, size_t right) {
        size_t mid = left + ((right - left) >> 1);
        swap(nums[left], nums[mid]);
        int pivot = nums[left];
        size_t i = left, j = right;
        while (i < j) {
            while (i < j and nums[j] >= pivot) --j;
            while (i < j and nums[i] <= pivot) ++i;
            if (i < j) swap(nums[i], nums[j]);
        }
        swap(nums[left], nums[i]);
        return i;
    };

    auto quickSelect = [&](auto &&self, size_t left, size_t right) {
        // 此处等于即可, 因为永远有 left <= target <= right
        if (left == right) return nums[left]; // left = right 的时候, 也等于 target, 所以直接返回
        size_t pivotIndex = partition(left, right);
        if (pivotIndex == t) return nums[pivotIndex];
        if (pivotIndex > t) return self(self, left, pivotIndex - 1);
        return self(self, pivotIndex + 1, right);
    };

    return quickSelect(quickSelect, 0, n - 1);
}
