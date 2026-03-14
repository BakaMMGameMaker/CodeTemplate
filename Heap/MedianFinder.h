#pragma once
#include <functional>
#include <vector>
#include <queue>

// 数据流中的中位数
// 大根堆 left + 小根堆 right
// left 维护较小的数字, right 维护较大的数字
// left.size == right.size or right.size + 1
// 插入数字时, 先塞到合适的堆中, 再调整平衡
// 获取中位数时, 根据 left.size == right.size 还是 right.size + 1 来决定返回值
class MedianFinder {
    std::priority_queue<int> l;
    std::priority_queue<int, std::vector<int>, std::greater<> > r;

public:
    MedianFinder() = default;

    void add(int num) {
        // 加入到合适的堆, 不是无脑加左
        // 这是因为加完后的新节点并不一定在堆顶, 但是调整平衡的时候动的是堆顶
        if (l.empty() or num <= l.top()) l.push(num);
        else r.push(num);

        if (l.size() > r.size() + 1) {
            // 左边太多, 堆顶去右边
            r.push(l.top());
            l.pop();
        } else if (r.size() > l.size()) {
            // 右边太多, 堆顶去左边
            l.push(r.top());
            r.pop();
        }
    }

    [[nodiscard]] double median() const {
        if (l.size() != r.size()) return l.top(); // l.size == r.size + 1
        return (l.top() + r.top()) / 2.0;
    }
};
