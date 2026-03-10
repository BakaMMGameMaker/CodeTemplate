#pragma once

struct ListNode {
    int val;
    ListNode *next;

    explicit ListNode(const int x = 0) : val(x), next(nullptr) {
    }

    ListNode(const int x, ListNode *next) : val(x), next(next) {
    }
};
