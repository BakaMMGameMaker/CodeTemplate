#include "ListNode.h"
#include <bits/stdc++.h>
using namespace std;

// 按 x 分隔链表，小的放前面，大的放后面，保持相对顺序不变
// 思路，创建两个链表，最后合并在一起
ListNode *Partition(ListNode *head, int x) {
    ListNode smallDm, bigDm;
    ListNode *small = &smallDm;
    ListNode *big = &bigDm;
    while (head) {
        if (head->val < x) {
            small->next = head;
            small = small->next;
        } else {
            big->next = head;
            big = big->next;
        }
        head = head->next;
    }

    // 最终 big 部分的尾节点可能还连着 small 中的节点，要注意断连
    big->next = nullptr;
    small->next = bigDm.next;

    return smallDm.next;
}
