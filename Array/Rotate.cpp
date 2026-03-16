#include <vector>
#include <algorithm>
using namespace std;

// 旋转数组

// 使用额外数组
void anotherarray(vector<int> &nums, int k) {
    int n = nums.size();
    k %= n;
    vector<int> tmp(n);
    for (int i = 0; i < n; ++i) tmp[(i + k) % n] = nums[i];
    nums = tmp;
}

// 三次翻转 - 原地做法
// 把数组的后 k 个数字和前面的数字分开
// 整体翻转, 随后翻转前 k 个, 最后翻转剩下的
void reverse3(vector<int> &nums, int k) {
    int n = nums.size();
    k %= n;
    if (k == 0) return;
    ranges::reverse(nums);
    reverse(nums.begin(), nums.begin() + k);
    reverse(nums.begin() + k, nums.end());
}

// 最少移动次数, 走环
void rotate(vector<int> &nums, int k) {
    int n = nums.size();
    if (n <= 1) return;
    k %= n;
    if (k == 0) return;
    int count = 0;
    for (int start = 0; count < n; ++start) { // 可以通过数学证明不会重复搬运, 但是过于复杂, 不再赘述
        int cur = start;                      // 当前要处理的数字原本的下标
        int prev = nums[start];               // 当前要处理的数字的值
        do {
            int next = (cur + k) % n; // 这个数字应该去哪里
            int temp = nums[next];    // 这个地方原本的数字
            nums[next] = prev;        // 放进去
            prev = temp;              // 要处理的数字变成了被挤出来的数字
            cur = next;               // 要处理的数字原本的位置
            ++count;                  // 处理好了一个数字
        } while (cur != start);       // 在形成闭环之前都不停下
    }
}
