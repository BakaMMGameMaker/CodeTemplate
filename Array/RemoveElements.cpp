#include <vector>
using namespace std;

// 删除等于 val 的元素

// 新开数组
int removeElement(vector<int> &nums, int val) {
    vector<int> tmp;
    for (int x : nums) {
        if (x != val) {
            tmp.push_back(x);
        }
    }

    for (int i = 0; i < tmp.size(); i++) {
        nums[i] = tmp[i];
    }

    return tmp.size();
}

// 双指针
// 左指针找到要删除的位置，右指针找保留的元素
// 如果左指针指向 val，那么把右指针不指向 val 的元素交换过来
int removeElementTwoPointers(vector<int> &nums, int val) {
    int left = 0;
    int right = nums.size();

    while (left < right) {
        if (nums[left] == val) {
            nums[left] = nums[right - 1];
            right--;
            // left 不急着走，因为可能交换一个 val 过来
        } else {
            left++;
        }
    }

    return left;
}

// 快慢指针
// 快指针固定速度，慢指针指向当前要拷贝的位置
int removeElementFastSlowPointers(vector<int> &nums, int val) {
    int slow = 0;
    for (int fast = 0; fast < nums.size(); fast++) {
        if (nums[fast] != val) {
            nums[slow] = nums[fast];
            slow++;
        }
    }
    return slow;
}
