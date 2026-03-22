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
    int capacity = 0; // 缓存容量
    int minFreq = 0;  // 当前最小频率 - 用于容量满的时候确定踢掉谁
    // 三个哈希表
    // 1 - 某个 key 的值和频率是多少
    // 2 - 频率对应哪个缓存链表
    // 3 - 某个 key 在他的缓存链中的迭代器是哪个
    std::unordered_map<int, std::pair<int, int> > keyToValFreq;  // key => {value, freq}
    std::unordered_map<int, std::list<int> > freqToCacheList;    // freq => 包含当前频次的节点的 list
    std::unordered_map<int, std::list<int>::iterator> keyToIter; // key => 目标节点在自己列表中的迭代器

    // 增加一个 key 的频率，在 get 和 put 已有节点时都要调用
    // 1 - 把 key 从旧链中删掉
    // 2 - 如果旧链空了，删除旧链。如果旧链对应的刚好还是最小频率，别忘了增加最小频率
    // 3 - 加入到新链的头部
    void increaseFreq(int key) {
        auto [value, freq] = keyToValFreq[key];
        freqToCacheList[freq].erase(keyToIter[key]);
        if (freqToCacheList[freq].empty()) {
            freqToCacheList.erase(freq);
            if (minFreq == freq) ++minFreq;
        }
        freq++;
        keyToValFreq[key] = {value, freq};
        freqToCacheList[freq].push_front(key);
        keyToIter[key] = freqToCacheList[freq].begin();
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

        // 设置已有值
        if (keyToValFreq.count(key)) {
            keyToValFreq[key].first = value;
            increaseFreq(key);
            return;
        }

        // 缓存满了
        // 最小频率对应缓存链删除末尾节点
        // 如果对应缓存链空了，删除链条
        // 没必要关心最小频率，因为到这里肯定是插入新值，最后最小频率肯定是 1
        if (keyToValFreq.size() == capacity) {
            int keyToRemove = freqToCacheList[minFreq].back();
            freqToCacheList[minFreq].pop_back();
            if (freqToCacheList[minFreq].empty()) freqToCacheList.erase(minFreq);
            keyToValFreq.erase(keyToRemove);
            keyToIter.erase(keyToRemove);
        }

        keyToValFreq[key] = {value, 1};
        freqToCacheList[1].push_front(key);
        keyToIter[key] = freqToCacheList[1].begin();
        minFreq = 1;
    }
};

