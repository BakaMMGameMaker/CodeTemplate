#include "ListNode.h"
#include <bits/stdc++.h>
using namespace std;

// 交换/重排类模板

// 两两交换
ListNode *swapPairs(ListNode *head) {
    ListNode dm(0);
    dm.next = head;
    ListNode *cur = &dm;

    while (cur->next && cur->next->next) {
        // cur 1 2 3 -> cur 2 1 3
        ListNode *first = cur->next;
        ListNode *second = cur->next->next;

        first->next = second->next;
        second->next = first;
        cur->next = second;

        cur = first; // 跳过这个 pair，不要赋值成 second
    }

    return dm.next;
}

// 重排链表 l0->l1->...->ln-1->ln => l0->ln->l1->ln-1->l2->ln-2...
// 思路 找中点，反转后半段链表，随后交叉合并
void reorderList(ListNode *head) {
    if (!head || !head->next) return;

    // 1. 定位中点
    ListNode *fast = head;
    ListNode *slow = head;
    while (fast && fast->next) {
        fast = fast->next->next;
        slow = slow->next;
    }

    // 反转后半段
    ListNode *second = slow->next; // 后半段的起点，注意反转不包括中点
    slow->next = nullptr;          // 把前后两段隔开

    // 现在相当于手上有一个链表头 second，我们要独立地对这段链表进行反转，参考 Reverse
    ListNode *prev = nullptr; // 当前处理节点的前驱节点
    while (second) {
        ListNode *next = second->next;
        second->next = prev;
        // 往前步进
        prev = second;
        second = next;
    }
    second = prev; // 让 second 变成反转过后的头节点

    // 现在手上有两个独立的链表，一个为以 head 为起点的前半段（包括中点）
    // 另一个为以 second 为起点的反转过后的后半段
    // 对两端链表进行交替合并即可
    ListNode *first = head;
    while (second) {
        ListNode *tmp1 = first->next;
        ListNode *tmp2 = second->next;

        // 每一轮循环创建两个连接
        first->next = second;
        second->next = tmp1;
        // 不要着急连接 tmp2

        first = tmp1;
        second = tmp2;
    }
}

// 奇偶链表重排
// 下标奇数位节点在前，偶在后，保持各自相对顺序
ListNode *oddEvenList(ListNode *head) {
    if (!head || !head->next) return head;
    ListNode *odd = head;
    ListNode *even = head->next;
    ListNode *evenHead = even;

    // 注意条件是 even
    while (even and even->next) {
        odd->next = even->next;
        odd = odd->next;
        even->next = odd->next;
        even = even->next;
    }
    odd->next = evenHead;
    return head;
}
