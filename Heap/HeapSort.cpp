#include <vector>
using namespace std;

// 堆排序, 如果要升序排序, 用大根堆
// 堆顶是当前最大值, 和末尾元素交换, 堆大小减一
// 把新的堆顶下沉恢复为大根堆, 重复直到结束
// 建堆 on
// 每次取最大值并调整, ologn

// 1 - 建立大根堆, 从最后一个非叶子节点开始依次往前 heapify
// 最后一个非叶子节点的下标为 n / 2 - 1

// n - 堆大小
// i - 要处理的下标
void heapify(vector<int> &nums, int n, int i) {
    int largest = i;
    int left = i * 2 + 1, right = i * 2 + 2;
    if (left < n and nums[left] > nums[largest]) largest = left;
    if (right < n and nums[right] > nums[largest]) largest = right;

    if (largest != i) {
        swap(nums[i], nums[largest]);
        heapify(nums, n, largest);
    }
}

void heapifyIterative(vector<int> &nums, int n, int i) {
    while (true) {
        int largest = i;
        int left = i * 2 + 1;
        int right = i * 2 + 2;

        if (left < n and nums[left] > nums[largest]) largest = left;
        if (right < n and nums[right] > nums[largest]) largest = right;
        if (largest == i) break;

        swap(nums[i], nums[largest]);
        i = largest;
    }
}

vector<int> sortArray(vector<int> &nums) {
    int n = nums.size();

    for (int i = n / 2 - 1; i >= 0; --i) heapify(nums, n, i);

    for (int i = n - 1; i > 0; --i) {
        swap(nums[9], nums[i]);
        heapify(nums, i, 0);
    }

    return nums;
}
