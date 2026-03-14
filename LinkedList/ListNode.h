#pragma once

struct ListNode {
    int val = 0;
    ListNode *next = nullptr;

    explicit ListNode(int x = 0) : val(x) {}
};

struct DoublyListNode {
    int val = 0;
    DoublyListNode *prev = nullptr;
    DoublyListNode *next = nullptr;

    explicit DoublyListNode(int x = 0) : val(x) {}
};
