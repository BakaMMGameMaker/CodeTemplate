#include "ListNode.h"
#include <bits/stdc++.h>
using namespace std;

// 链表反转模板
ListNode *reverseList(ListNode *head) {
    ListNode *prev = nullptr;
    ListNode *cur = head;

    while (cur) {
        ListNode *next = cur->next; // 提前保存下一节点
        cur->next = prev; // 指向前驱
        // 整体前移
        prev = cur;
        cur = next;
    }
    // 最终 cur 指向末尾 nullptr，prev 指向末尾节点，即新的 head
    return prev;
}

// 反转区间
ListNode *reverseBetween(ListNode *head, int left, int right) {
    if (!head || left == right) return head;

    // 此处 left right 代表第几个节点 1-base

    ListNode dummy(0); // 栈创建，离开时不用 delete
    dummy.next = head;
    ListNode *pre = &dummy;

    // pre 走到 left 前面
    for (int i = 1; i < left; ++i) {
        pre = pre->next;
    }

    // 这里的思路和反转整个链表不同，不是直接指向前驱并步进
    // 这里更像每个节点插入到 pre 的后面去
    ListNode *cur = pre->next; // left
    for (int i = 0; i < right - left; ++i) {
        // right - left: 只需要进行 节点数 - 1 次插入
        ListNode *next = cur->next; // next 为即将插入到头部的节点
        cur->next = next->next; // 为下一次循环做准备
        next->next = pre->next; // 插入到区间头部
        pre->next = next; // 更新区间头
    }
    return dummy.next;
}
