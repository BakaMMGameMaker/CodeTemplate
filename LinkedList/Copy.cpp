#include <bits/stdc++.h>
using namespace std;

// 随机指针链表复制
struct Node {
    int val;
    Node *next;
    Node *random;

    explicit Node(const int _val) : val(_val), next(nullptr), random(nullptr) {
    }
};

// 哈希表写法
Node *copyRandomList(Node *head) {
    if (!head) return nullptr;

    unordered_map<Node *, Node *> mp; // 旧节点 -> 新节点
    Node *cur = head;
    // 创建所有新节点
    while (cur) {
        mp[cur] = new Node(cur->val);
        cur = cur->next;
    }

    cur = head;
    // 连接所有新节点
    while (cur) {
        mp[cur]->next = mp[cur->next];
        mp[cur]->random = mp[cur->random];
        cur = cur->next;
    }

    return mp[head];
}

// 原地穿插写法 - 省空间
Node *copyRandomListII(Node *head) {
    if (!head) return nullptr;

    // 1->2->3->4 => 1->1->2->2->3->3->4->4，每一对的第二个节点为新节点
    Node *cur = head;
    while (cur) {
        Node *copy = new Node(cur->val);
        copy->next = cur->next;
        cur->next = copy;
        cur = copy->next;
    }

    cur = head;
    while (cur) {
        // 1->copy1->...->random->copyRandom
        // 让 copy1 的 random 指向 copyRandom
        if (cur->random) {
            cur->next->random = cur->random->next;
        }
        cur = cur->next->next; // 直接跳两步
    }
    // 分离链表
    // 1->1->2->2->3->3->4->4 => 1->2->3->4, 1->2->3->4
    cur = head;
    Node *newHead = head->next;
    while (cur) {
        Node *copy = cur->next;
        cur->next = copy->next; // cur.next 不会为 nullptr
        if (copy->next) {
            copy->next = copy->next->next;
        }
        cur = cur->next;
    }
    return newHead;
}
