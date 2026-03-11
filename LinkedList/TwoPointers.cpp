#include "ListNode.h"
#include <bits/stdc++.h>
using namespace std;

// 链表双指针模板

// 找中点
// 注意初始化都为 head，fast 不需要提前走一步
const ListNode *middleNode(const ListNode *head) {
    const ListNode *slow = head;
    const ListNode *fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    return slow;
}

// 判断是否有环
bool hasCycle(const ListNode *head) {
    const ListNode *slow = head;
    const ListNode *fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

// 找环入口
const ListNode *detectCycle(const ListNode *head) {
    const ListNode *slow = head;
    const ListNode *fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) {
            // 有环，从相遇点以同样速度前进，新相遇点为入口
            const ListNode *p = head;
            while (p != slow) {
                p = p->next;
                slow = slow->next;
            }
            return p;
        }
    }

    return nullptr;
}

// 倒数第 k 个节点
const ListNode *findKthFromEnd(const ListNode *head, const int k) {
    const ListNode *fast = head;
    const ListNode *slow = head;

    // 快节点先走 k 步
    for (int i = 0; i < k; ++i) {
        fast = fast->next;
    }

    while (fast) {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}

// 删除倒数第 N 个节点
ListNode *removeNthFromEnd(ListNode *head, int n) {
    ListNode dummy(0);
    dummy.next = head;

    const ListNode *fast = &dummy;
    ListNode *slow = &dummy;

    // 因为多了一个 dummy 节点，所以要多走 1 步
    for (int i = 0; i < n + 1; ++i) {
        fast = fast->next;
    }

    // 依旧以同样速度走，最终 slow 会走到待删除节点的前驱节点
    // 这是因为在找倒数第 N 个节点的时候，slow 从 head 开始
    // 但是现在 slow 从 dummy 开始，所以最终会走到前驱
    while (fast) {
        fast = fast->next;
        slow = slow->next;
    }

    const ListNode *del = slow->next;
    slow->next = del->next;
    delete del;

    return dummy.next;
}

// 找两个链表的相交节点
// 思路，A 走完到 B，B 走完到 A
// 如果两个指针相等，要不一起走到了结尾 (lenA+lenB)，要不发生了相遇
const ListNode *IntersectionNode(const ListNode *A, const ListNode *B) {
    const ListNode *pa = A;
    const ListNode *pb = B;
    while (pa != pb) {
        pa = (pa == nullptr) ? B : pa->next;
        pb = (pb == nullptr) ? A : pb->next;
    }
    return pa; // nullptr or IntersectionNode
}

// 判断回文链表
// 思路：找中点 反转后半段 两段进行比较
bool isPalindrome(ListNode *head) {
    if (!head || !head->next) return true;

    ListNode *slow = head;
    ListNode *fast = head;

    // 这样写条件可以在偶数个节点时，让 slow 成为靠前的那个节点
    while (fast->next && fast->next->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    ListNode *prev = nullptr;
    ListNode *cur = slow->next; // 准备对后半段进行反转
    while (cur) {
        ListNode *next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    const ListNode *p1 = head;
    const ListNode *p2 = prev;
    while (p2) {
        if (p1->val != p2->val) return false;
        p1 = p1->next;
        p2 = p2->next;
    }
    return true;
}
