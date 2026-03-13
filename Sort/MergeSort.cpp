#include <bits/stdc++.h>
using namespace std;

// 归并排序模板

// 链表排序
class SortList {
    struct ListNode {
        int val = 0;
        ListNode *next = nullptr;

        ListNode() = default;
    };

    static ListNode *mergeList(ListNode *h1, ListNode *h2) {
        ListNode dm;
        ListNode *tail = &dm;
        while (h1 and h2) {
            if (h1->val <= h2->val) {
                tail->next = h1;
                h1 = h1->next;
            } else {
                tail->next = h2;
                h2 = h2->next;
            }
            tail = tail->next;
        }
        tail->next = h1 ? h1 : h2;
        return dm.next;
    }

public:
    static ListNode *mergeSort(ListNode *head) {
        if (not head or not head->next) return head;
        ListNode *fast = head->next; // fast 多走一步，防止链长 2 时断不开
        ListNode *slow = head;
        while (fast and fast->next) {
            fast = fast->next->next;
            slow = slow->next;
        }
        ListNode *mid = slow->next;
        slow->next = nullptr;
        return mergeList(mergeSort(head), mergeSort(mid));
    }
};

// 统计逆序对的数目
class CountInversePair {
    // temp 用于存储排序的结果，并最终写回 nums 中
    // left 为处理区间起点，right 为处理区间中点
    // 返回 - 该区间逆序对的数目
    int mergeSort(vector<int> &nums, vector<int> &temp, int left, int right) {
        if (left >= right) return 0;

        int mid = left + ((right - left) >> 1);
        // 记录逆序对的数目
        int count = mergeSort(nums, temp, left, mid);
        count += mergeSort(nums, temp, mid + 1, right);

        // 现在 left...mid, mid+1...right 各自有序
        // 第一件事就是把排序结果写入 temp 并最终写回 nums
        // 第二件事就是顺便统计逆序对的数目
        // 对于逆序对的数目，如果遍历过程发现 nums[l] > nums[r]
        // 那么证明 nums[l...mid] > nums[r]，这样就构成了 mid - l + 1 个逆序对

        int l = left, r = mid + 1;
        int k = left; // temp 指针，我们要写入 left...right 的部分
        while (l <= mid and r <= right) {
            if (nums[l] <= nums[r]) {
                // 没有逆序对出现
                temp[k++] = nums[l++];
            } else {
                // 出现 mid - l + 1 个逆序对
                count += mid - l + 1;
                temp[k++] = nums[j++];
            }
        }

        // 处理剩余元素
        while (l <= mid) temp[k++] = nums[l++];
        while (r <= right) temp[k++] = nums[r++];
        // 排序结果写回 nums
        for (int p = left; p <= right; ++p) nums[p] = temp[p];

        // 返回结果
        return count;
    }

public:
    int InversePair(vector<int> &nums) {
        if (nums.empty()) return 0;
        vector<int> temp(nums.size());
        return mergeSort(nums, temp, 0, static_cast<int>(nums.size()) - 1);
    }
};
