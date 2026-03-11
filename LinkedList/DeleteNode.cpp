#include "ListNode.h"
#include <bits/stdc++.h>
using namespace std;

// 链表删除节点模板

// 删除所有值为 target 的节点
ListNode *removeElements(ListNode *head, const int val) {
    ListNode dummy;
    dummy.next = head;
    ListNode *cur = &dummy;

    while (cur->next) {
        if (cur->next->val == val) {
            const ListNode *del = cur->next;
            cur->next = del->next;
            delete del;
            // 不要前进，因为新的 next 又要再判断
        } else {
            cur = cur->next;
        }
    }

    return dummy.next;
}

// 删除排序链表中的重复元素
ListNode *deleteDuplicates(ListNode *head) {
    // 不需要 dummy，因为头节点不可能被删除
    ListNode *cur = head;

    while (cur && cur->next) {
        if (cur->val == cur->next->val) {
            const ListNode *del = cur->next;
            cur->next = del->next;
            delete del;
        } else {
            cur = cur->next;
        }
    }
    return head;
}

// 删除所有发生重复的元素
ListNode *deleteDuplicatesII(ListNode *head) {
    ListNode dummy;
    dummy.next = head;
    ListNode *pre = &dummy; // 重复区间的前驱节点

    while (pre->next && pre->next->next) {
        if (pre->next->val == pre->next->next->val) {
            // 发现重复区间，准备删除整个区间
            const int x = pre->next->val;
            while (pre->next && pre->next->val == x) {
                const ListNode *del = pre->next;
                pre->next = del->next;
                delete del;
            }
        } else {
            pre = pre->next;
        }
    }

    return dummy.next;
}
