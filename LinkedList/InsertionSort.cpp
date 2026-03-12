#include <bits/stdc++.h>
#include "ListNode.h"
using namespace std;

// 链表的插入排序

ListNode *insertionSort(ListNode *head) {
    // 思路 - prev 作为目标位置的前驱
    // cur 为当前插入的节点
    ListNode dm;
    auto cur = head;
    while (cur) {
        auto nxt = cur->next;
        // 在 dm 中找自己的位置
        auto prev = &dm;
        // 步进条件 - 下一个节点没自己大 严格小于防止无意义多跑
        while (prev->next and prev->next->val < cur->val) { prev = prev->next; }
        cur = prev->next;
        prev->next = cur;
        cur = nxt;
    }
    return dm.next;
}
