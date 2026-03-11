#pragma once
#include <stack>
#include <stdexcept>

// 最小栈 - push pop top getMin 都要做到 O(1)

template<typename T>
class MinStack {
    // 单栈版本其实就是把 {当前元素, 当前最小值} 作为一个 pair 存到栈中

    // 优化：仅在遇到可以作为当前最小值的元素时，才压栈
    // 出栈时，如果出栈的元素是当前的最小值，那么 mn 也弹出一个元素
    // 注意不是新的更小值，而是 <= 当前最小值的元素
    std::stack<T> st; // 正常存取所有元素
    std::stack<T> mn; // 存取当前的最小值

public:
    MinStack() = default;

    void push(const T &val) {
        st.push(val);
        if (mn.empty() || val <= mn.top()) mn.push(val);
    }

    void pop() {
        if (st.empty()) return;
        if (st.top() == mn.top()) mn.pop();
        st.pop();
    }

    T top() const {
        if (st.empty()) throw std::runtime_error("stack is empty");
        return st.top();
    }

    T min() const {
        if (mn.empty()) throw std::runtime_error("stack is empty");
        return mn.top();
    }

    bool empty() const { return st.empty(); }

    size_t size() { return st.size(); }
};

// 同理 我们只需要三个栈就可以在 MinStack 基础上实现 O(1) GetMax
template<typename T>
class MinMaxStack {
    std::stack<int> st;
    std::stack<int> mn;
    std::stack<int> mx;

public :
    void push(const T &val) {
        st.push(val);
        if (mn.empty() || val <= mn.top()) mn.push(val);
        if (mx.empty() || val >= mx.top()) mx.push(val);
    }

    void pop() {
        if (st.empty()) return;
        if (st.top() == mn.top()) mn.pop();
        if (st.top() == mx.top()) mx.pop();
        st.pop();
    }

    T top() {
        if (st.empty()) throw std::runtime_error("stack is empty");
        return st.top();
    }

    T min() {
        if (mn.empty()) throw std::runtime_error("stack is empty");
        return mn.top();
    }

    T max() {
        if (mx.empty()) throw std::runtime_error("stack is empty");
        return mx.top();
    }

    bool empty() const { return st.empty(); }

    size_t size() const { return st.size(); }
};
