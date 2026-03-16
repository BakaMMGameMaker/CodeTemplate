#include <vector>
using namespace std;

// 合并两个有序数组
// 双指针 - 从后往前填充 nums1 (nums1 后面有足够空间)
void merge(vector<int> &nums1, int m, vector<int> &nums2, int n) {
    int i = m - 1, j = n - 1, k = m + n - 1;
    while (i >= 0 and j >= 0) {
        if (nums1[i] > nums2[j]) nums1[k--] = nums1[i--];
        else nums1[k--] = nums2[j--];
    }

    while (j >= 0) nums1[k--] = nums2[j--]; // 只需要处理 j, 因为 i 剩下的本来就在 nums1 里的正确位置
}
