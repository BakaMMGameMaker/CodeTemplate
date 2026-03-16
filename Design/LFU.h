#pragma once
#include <list>
#include <unordered_map>

// LFU 不是淘汰刚才进去的元素, 不然有点神人了
// LFU - 淘汰访问次数最少的元素
// 每个节点需要记录 key value 和 frequency
class LFUCache {
    struct Node {
        int key;
        int value;
        int freq;
        Node *prev = nullptr;
        Node *next = nullptr;
        Node(int k, int v) : key(k), value(v), freq(1) {}
    };

    struct DoublyLinkedList {
        Node *head = nullptr;
        Node *tail = nullptr;
        int size;

        DoublyLinkedList() {
            head = new Node(-1, -1);
            tail = new Node(-1, -1);
            head->next = tail;
            tail->prev = head;
            size = 0;
        }

        void addToHead(Node *node) {
            node->next = head->next;
            node->prev = head->prev;
            head->next->prev = node;
            head->next = node;
            size++;
        }

        void remove(Node *node) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
            size--;
        }

        Node *removeLast() {
            if (size == 0) return nullptr;
            Node *node = tail->prev;
            remove(node);
            return node;
        }

        bool empty() const { return size == 0; }

        ~DoublyLinkedList() {
            delete head;
            delete tail;
        }
    };

    int capacity = 0;
    int minFreq = 0;
    std::unordered_map<int, Node *> keyToNode;
    std::unordered_map<int, DoublyLinkedList *> freqToList;

    void increaseFreq(Node *node) {
        int oldFreq = node->freq;
        freqToList[oldFreq]->remove(node);
        if (oldFreq == minFreq and freqToList[oldFreq]->empty()) minFreq++;
        node->freq++;
        if (not freqToList.count(node->freq)) freqToList[node->freq] = new DoublyLinkedList();
        freqToList[node->freq]->addToHead(node);
    }

public:
    explicit LFUCache(int _capacity) : capacity(_capacity) {}

    int get(int key) {
        if (!keyToNode.count(key)) return -1;
        Node *node = keyToNode[key];
        increaseFreq(node);
        return node->value;
    }

    void put(int key, int value) {
        if (capacity == 0) return;

        if (keyToNode.count(key)) {
            Node *node = keyToNode[key];
            node->value = value;
            increaseFreq(node);
            return;
        }
        if (keyToNode.size() == capacity) {
            Node *removed = freqToList[minFreq]->removeLast(); // 不仅使用频率最低, 还是那一批里最早入队的
            keyToNode.erase(removed->key);
            delete removed;
        }

        Node *newNode = new Node(key, value);
        minFreq = 1;
        if (!freqToList.count(1)) freqToList[1] = new DoublyLinkedList();
        freqToList[1]->addToHead(newNode);
        keyToNode[key] = newNode;
    }

    ~LFUCache() {
        for (auto &[freq, listPtr] : freqToList) {
            Node *cur = listPtr->head->next;
            while (cur != listPtr->tail) {
                Node *nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            delete listPtr;
        }
    }
};

class LFUCache_STL {
    int capacity = 0;
    int minFreq = 0;
    std::unordered_map<int, std::pair<int, int> > keyToValFreq;  // key => {value, freq}
    std::unordered_map<int, std::list<int> > freqToKeys;         // freq => 包含当前频次的节点的 list
    std::unordered_map<int, std::list<int>::iterator> keyToIter; // key => 目标节点在自己列表中的迭代器

    void increaseFreq(int key) {
        auto [value, freq] = keyToValFreq[key];
        freqToKeys[freq].erase(keyToIter[key]);
        if (freqToKeys[freq].empty()) {
            freqToKeys.erase(freq);
            if (minFreq == freq) ++minFreq;
        }
        freq++;
        keyToValFreq[key] = {value, freq};
        freqToKeys[freq].push_front(key);
        keyToIter[key] = freqToKeys[freq].begin();
    }

public:
    explicit LFUCache_STL(int _capacity) : capacity(_capacity) {}

    int get(int key) {
        if (!keyToValFreq.count(key)) return -1;
        int value = keyToValFreq[key].first;
        increaseFreq(key);
        return value;
    }

    void put(int key, int value) {
        if (capacity == 0) return;

        if (keyToValFreq.count(key)) {
            keyToValFreq[key].first = value;
            increaseFreq(key);
            return;
        }
        if (keyToValFreq.size() == capacity) {
            int keyToRemove = freqToKeys[minFreq].back();
            freqToKeys[minFreq].pop_back();
            if (freqToKeys[minFreq].empty()) freqToKeys.erase(minFreq);
            keyToValFreq.erase(keyToRemove);
            keyToIter.erase(keyToRemove);
        }

        keyToValFreq[key] = {value, 1};
        freqToKeys[1].push_front(key);
        keyToIter[key] = freqToKeys[1].begin();
        minFreq = 1;
    }
};

