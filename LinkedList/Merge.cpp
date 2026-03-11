#include "ListNode.h"
#include <bits/stdc++.h>
using namespace std;

// 链表合并模板

// 合并两个有序链表
ListNode *mergeTwoLists(ListNode *list1, ListNode *list2) {
    ListNode dm; // 新链表 dm
    ListNode *tail = &dm; // 新链表当前结尾

    while (list1 && list2) {
        if (list1->val <= list2->val) {
            tail->next = list1;
            list1 = list1->next;
        } else {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    // 拼接残留的链表
    tail->next = list1 ? list1 : list2;
    return dm.next;
}

// 递归版本
ListNode *mergeTwoListsRecursive(ListNode *l1, ListNode *l2) {
    // 返回后面的部分处理完之后的头节点
    if (!l1)return l2;
    if (!l2)return l1;

    if (l1->val <= l2->val) {
        l1->next = mergeTwoListsRecursive(l1->next, l2);
        return l1;
    } else {
        l2->next = mergeTwoListsRecursive(l1, l2->next);
        return l2;
    }
}

// 合并 K 个有序链表 - 优先队列找最小节点
struct cmp {
    bool operator()(const ListNode *a, const ListNode *b) const {
        // 注意堆的比较器要和排序时用的比较器反着理解
        // 排序时，想要让小的靠前，就要 return a < b
        // 堆中如果想要小的在顶部，就要 return a > b
        return a->val > b->val;
    }
};

ListNode *mergeKLists(const vector<ListNode *> &lists) {
    priority_queue<ListNode *, vector<ListNode *>, cmp> pq;
    for (auto node: lists) {
        if (node) pq.push(node); // 加入每个链表的头节点
    }

    ListNode dm;
    ListNode *tail = &dm;
    while (!pq.empty()) {
        ListNode *node = pq.top();
        pq.pop();

        tail->next = node;
        tail = tail->next;
        if (node->next) pq.push(node->next);
    }

    return dm.next;
}
