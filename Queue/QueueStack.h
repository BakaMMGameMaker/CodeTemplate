#pragma once
#include <queue>
#include <stdexcept>

// 队列实现栈
// 思路 - push 正常入队，再把前面元素依次移动到队尾，让队首永远是栈顶

template<typename T>
class QueueStack {
    std::queue<T> q;

public:
    QueueStack() = default;

    void push(const T &x) {
        q.push(x);
        int n = q.size();
        for (int i = 0; i < n - 1; ++i) {
            q.push(q.front());
            q.pop();
        }
    }

    void pop() {
        if (q.empty()) throw std::out_of_range("stack is empty");
        q.pop();
    }

    T top() {
        if (q.empty()) throw std::out_of_range("stack is empty");
        return q.front();
    }

    bool empty() const { return q.empty(); }

    size_t size() const { return q.size(); }
};

// 方案 2 - 让 pop 变慢
// 思路 - pop 的时候把 q1 除了刚进来的元素都搬到 q2 中
// 弹完后 q1 与 q2 交换，确保数据永远存储于 q1 中
template<typename T>
class QueueStackSlowPop {
    std::queue<T> q1, q2;

public:
    void push(const T &x) { q1.push(x); }

    void pop() {
        if (q1.empty()) throw std::out_of_range("stack is empty");
        // 保留 q1 中的最后一个元素，因为它是刚进入容器中的
        while (q1.size() > 1) {
            q2.push(q1.front());
            q1.pop();
        }
        q1.pop();
        std::swap(q1, q2);
    }

    T top() {
        if (q1.empty()) throw std::out_of_range("stack is empty");
        while (q1.size() > 1) {
            q2.push(q1.front());
            q1.pop();
        }
        T x = q1.front();
        q2.push(q1.front());
        q1.pop();
        std::swap(q1, q2);
        return x;
    }

    bool empty() const { return q1.empty(); }

    size_t size() const { return q1.size(); }
};
