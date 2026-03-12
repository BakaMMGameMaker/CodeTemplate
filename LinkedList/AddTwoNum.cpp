#include <bits/stdc++.h>
#include "ListNode.h"
using namespace std;

// 两数相加模板
// 数字倒序存储于链表中，左对齐

ListNode *AddTwoNumbers(const ListNode *l1, const ListNode *l2) {
    ListNode dm;
    ListNode *tail = &dm;
    int carry = 0;

    while (l1 or l2 or carry) {
        int sum = carry;
        if (l1) {
            sum += l1->val;
            l1 = l1->next;
        }
        if (l2) {
            sum += l2->val;
            l2 = l2->next;
        }
        tail->next = new ListNode(sum % 10);
        tail = tail->next;
        carry = sum / 10;
    }
    return dm.next;
}

// 两个数相加，但是这次数字从左往右排列，且右对齐
// 思路一 反转两个链表，然后正常进行两个数相加的第一个版本，最后再反转所得结果
// 思路二 利用两个栈，从右往左构建节点
stack<int> toStack(const ListNode *head) {
    stack<int> res;
    while (head) {
        res.push(head->val);
        head = head->next;
    }
    return res;
}

ListNode *AddTwoNumbersII(const ListNode *head1, const ListNode *head2) {
    ListNode *head = nullptr; // 最终结果的头节点
    stack st1 = toStack(head1);
    stack st2 = toStack(head2);

    int carry = 0;
    while (!st1.empty() or !st2.empty() or carry) {
        int sum = carry;
        if (!st1.empty()) {
            sum += st1.top();
            st1.pop();
        }
        if (!st2.empty()) {
            sum += st2.top();
            st2.pop();
        }
        auto newHead = new ListNode(sum % 10);
        newHead->next = head;
        head = newHead;
        carry = sum / 10;
    }
    return head;
}
