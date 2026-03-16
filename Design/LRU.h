#pragma once
#include <list>
#include <unordered_map>

// 又到了喜闻乐见的设计 LRU 环节

// 版本 1 - 手写 Node
class LRUCache {
    struct Node {
        int key, val; // key 也要存, 不然离队了哈希表不知道删掉谁
        Node *prev = nullptr;
        Node *next = nullptr;
        Node(int _key, int _val) : key(_key), val(_val) {}
    };

    int capacity = 0;
    std::unordered_map<int, Node *> mp;

    // 两个虚拟节点, 会让逻辑清晰简单很多
    Node *head = nullptr;
    Node *tail = nullptr;

    void remove(Node *node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void addToHead(Node *node) const {
        node->next = head->next;
        node->prev = head;
        head->next->prev = node;
        head->next = node;
    }

    void moveToHead(Node *node) {
        remove(node);
        addToHead(node);
    }

    Node *removeTail() {
        Node *node = tail->prev;
        remove(node);
        return node;
    }

public:
    explicit LRUCache(int _capacity)
        : capacity(_capacity), head(new Node(-1, -1)), tail(new Node(-1, -1)) {
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!mp.count(key)) return -1;
        Node *node = mp[key];
        moveToHead(node);
        return node->val;
    }

    void put(int key, int value) {
        if (capacity == 0) return;
        if (mp.count(key)) {
            Node *node = mp[key];
            node->val = value;
            moveToHead(node);
        } else {
            Node *node = new Node(key, value);
            mp[key] = node;
            addToHead(node);

            if (mp.size() > capacity) {
                Node *removed = removeTail();
                mp.erase(removed->key);
                delete removed;
            }
        }
    }

    ~LRUCache() {
        Node *cur = head;
        while (cur) {
            Node *nxt = cur->next;
            delete cur;
            cur = nxt;
        }
    }
};

// 使用 STL list
class LRUCache_STL {
    int capacity = 0;
    std::list<std::pair<int, int> > cache;
    std::unordered_map<int, std::list<std::pair<int, int> >::iterator> mp;

public:
    explicit LRUCache_STL(int _capacity) : capacity(_capacity) {}

    int get(int key) {
        if (!mp.count(key)) return -1;
        auto it = mp[key];
        int value = it->second;
        cache.splice(cache.begin(), cache, it);
        mp[key] = cache.begin();
        return value;
    }

    void put(int key, int value) {
        if (capacity == 0) return;

        if (mp.count(key)) {
            auto it = mp[key];
            it->second = value;
            cache.splice(cache.begin(), cache, it);
            mp[key] = cache.begin(); // 看心情写这一行, splice 不会改变迭代器
            return;
        }

        if (cache.size() == capacity) {
            mp.erase(cache.back().first);
            cache.pop_back();
        }

        cache.push_front({key, value});
        mp[key] = cache.begin();
    }
};
