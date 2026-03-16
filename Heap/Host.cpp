#include <queue>
#include <algorithm>
#include <vector>
using namespace std;

// 主持人调度问题
// 一个主持人同一时刻只能主持一场活动, 且需要全程在场
// 思路 - 把所有活动按照开始时间排序, 用一个小根堆维护当前已经分配出去的主持人最早什么时候空闲
// 对于每个活动, 如果堆顶结束时间 <= 当前活动开始时间, 说明有主持人空了出来, 可以复用
// 最终堆的大小就是某一时刻同时进行的活动的最大数量, 等于最少需要的主持人的数量, 因为只有能复用的时候才弹堆顶

int host(vector<vector<int> > &startend) {
    ranges::sort(startend);
    priority_queue<int, vector<int>, greater<> > pq;

    for (const auto &interval : startend) {
        int start = interval.front(), end = interval.back();
        if (not pq.empty() and pq.top() <= start) pq.pop();
        pq.push(end);
    }
    return pq.size();
}
