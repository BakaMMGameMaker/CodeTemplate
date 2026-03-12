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
