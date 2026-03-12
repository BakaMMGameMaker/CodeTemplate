#include <bits/stdc++.h>
#include "ListNode.h"
using namespace std;

// 旋转链表模板

// 思路 - 成环 找新尾巴 断连
ListNode *rotateRight(ListNode *head, int k) {
    if (!head || !head->next || k == 0) return head;
    // 计算节点总数
    int n = 1;
    ListNode *tail = head;
    while (tail->next) {
        tail = tail->next;
        ++n;
    }

    k %= n;
    if (k == 0) return head;

    // 先成环
    tail->next = head;

    // 直接找新结尾
    ListNode *newTail = head;
    for (int i = 0; i < n - k - 1; ++i) { newTail = newTail->next; }

    ListNode *newHead = newTail->next;
    newTail->next = nullptr;
    return newHead;
}
