#pragma once
#include <optional>
using namespace std;

// 数组实现循环队列
// 思路 - 定长数组存元素
// front = 队头位置，rear = 队尾的下一个位置
// 入队 - 元素放到 data[rear]，随后 rear 前进
// 出队 - 从 data[front] 拿出元素，随后 front 前进
// 队头元素 data[front] 队尾元素 data[rear-1] Size=rear-front
// 空满判断 - 空：front=rear 满：rear 前进一步=front，实际只能存储 capacity-1 个元素

class CircularQueue {
private:
    int *data;
    int front;
    int rear;
    int capacity;

public:
    explicit CircularQueue(int _capacity)
        : data(new int[_capacity + 1]), front(0), rear(0), capacity(_capacity + 1s) {}

    ~CircularQueue() {
        delete[] data;
    }

    bool Empty() const {
        return front == rear;
    }

    bool Full() const {
        return (rear + 1) % capacity == front;
    }

    bool Push(int value) {
        if (Full()) {
            return false;
        }
        data[rear] = value;
        rear = (rear + 1) % capacity;
        return true;
    }

    bool Pop() {
        if (Empty()) {
            return false;
        }
        front = (front + 1) % capacity;
        return true;
    }

    optional<int> Front() const {
        if (Empty()) {
            return nullopt;
        }
        return data[front];
    }

    // 因为入队是放到 data[rear]，所以队尾其实是 rear 的前一个位置
    optional<int> Rear() const {
        if (Empty()) {
            return nullopt;
        }
        return data[(rear - 1 + capacity) % capacity];
    }

    int Size() const {
        return (rear - front + capacity) % capacity;
    }
};
