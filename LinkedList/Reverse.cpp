#include "ListNode.h"
#include <bits/stdc++.h>
using namespace std;

// 链表反转模板

// 思路，俩俩节点连接反向，随后同时步进
ListNode *reverseList(ListNode *head) {
    ListNode *prev = nullptr;
    ListNode *cur = head;

    while (cur) {
        ListNode *next = cur->next; // 提前保存下一节点
        cur->next = prev;           // 指向前驱
        // 整体前移
        prev = cur;
        cur = next;
    }
    // 最终 cur 指向末尾 nullptr，prev 指向末尾节点，即新的 head
    return prev;
}

// 反转区间
// 思路：找到待反转区间的前驱节点，随后把区间内的每个节点依次头插到这个节点之后
ListNode *reverseBetween(ListNode *head, const int left, const int right) {
    if (!head || left == right) return head;

    // 此处 left right 代表第几个节点 1-base

    ListNode dummy(0); // 栈创建，离开时不用 delete
    dummy.next = head;
    ListNode *pre = &dummy;

    // pre 走到 left 前面
    for (int i = 1; i < left; ++i) { pre = pre->next; }

    // 这里的思路和反转整个链表不同，不是直接指向前驱并步进
    // 这里更像每个节点插入到 pre 的后面去
    ListNode *cur = pre->next; // left
    for (int i = 0; i < right - left; ++i) {
        // right - left: 只需要进行 节点数 - 1 次插入
        ListNode *next = cur->next; // next 为即将插入到头部的节点
        cur->next = next->next;     // 为下一次循环做准备
        next->next = pre->next;     // 插入到区间头部
        pre->next = next;           // 更新区间头
    }
    return dummy.next;
}

// 反转前 N 个节点
ListNode *successor = nullptr; // 记录前 n 个节点反转之后，后面剩下的节点是谁，这样反转完才能接上

ListNode *reverseN(ListNode *head, int n) {
    if (n == 1) {
        successor = head->next;
        return head;
    }

    ListNode *newHead = reverseN(head->next, n - 1); // 1 2 3 4 5 -> 1 4 3 2 5
    head->next->next = head;                         // 让 2 指向 1，因为 1 即将去到 2 和 5 中间
    head->next = successor;                          // 反转 2 3 4 后，successor 是 5，让 1 接到 5 上
    return newHead;
}

// K 个一组反转链表
ListNode *reverseKGroup(ListNode *head, int k) {
    ListNode dm;
    dm.next = head;
    ListNode *groupPrev = &dm;

    while (true) {
        // 从 prev 走 k 步到达当前组的末尾
        int step = k;
        ListNode *kth = groupPrev;
        while (kth && step--) kth = kth->next;
        if (!kth) break;

        ListNode *groupNext = kth->next;

        ListNode *cur = groupPrev->next; // 当前组的头节点
        ListNode *prev = groupNext;      // cur 的目标前驱

        // 反转当前这一组
        while (cur != groupNext) {
            ListNode *next = cur->next;
            cur->next = prev;
            prev = cur;
            cur = next;
        }

        ListNode *groupTail = groupPrev->next;
        groupPrev->next = kth;
        groupPrev = groupTail;
        if (!groupPrev) break;
    }
    return dm.next;
}
