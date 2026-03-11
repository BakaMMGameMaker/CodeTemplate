#pragma once
#include <stack>

// 双栈实现队列
// in 负责入队，out 负责出队
// push 时，进入 in
// pop / peek 时，若 out 不空，直接从 out 拿；若 out 空，in 中元素进入 out 再从 out 拿

class StackQueue {
    std::stack<int> in, out;

    void moveInToOut() {
        if (!out.empty()) return;
        while (!in.empty()) {
            out.push(in.top());
            in.pop();
        }
    }

public :
    StackQueue() = default;

    void push(const int x) { in.push(x); }

    int pop() {
        moveInToOut();
        const int x = out.top();
        out.pop();
        return x;
    }

    int peek() {
        moveInToOut();
        return out.top();
    }

    bool empty() const { return in.empty() && out.empty(); }
};
