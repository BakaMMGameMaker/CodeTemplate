#include <algorithm>
#include <vector>
using namespace std;

// 合并区间

// 思路 - 把区间按照左端点从小到大排序
// 遍历每个区间, 如果结果数组当前为空, 或者说当前区间的左端点大于结果数组的最后一个区间的右端点, 那么就直接加入
// 否则意味着可以合并, 直接把结果数组中的最后一个区间的右端点更新为两个区间的右端点中的较大值
vector<vector<int> > mergeintervals(vector<vector<int> > &intervals) {
    ranges::sort(intervals.begin(), intervals.end());
    vector<vector<int> > res;
    for (const auto &interval : intervals) {
        if (res.empty() or interval.front() > res.back().back()) res.push_back(interval);
        else res.back().back() = max(res.back().back(), interval.back());
    }
    return res;
}

// 解法 2 - 如果所有区间的数值都在某个范围内 0 <= val <= 1e5
// 思路 - 直接记录每个位置有多少区间开始和结束, 对每一个 [l, r], 只需要 diff[l] += 1, diff[r+1] -= 1
// 最后对 diff 做前缀和就能得到 cover 情况, 对于连续的 cover[i] > 0 的位置, 合并成一个区间, 遇到 0 则断开
vector<vector<int> > mergeintervals2(vector<vector<int> > &intervals) {
    if (intervals.empty()) return {};
    int maximum = 0;
    for (const auto &it : intervals) maximum = max(maximum, it.back());

    vector diff(maximum + 2, 0);

    for (const auto &it : intervals) {
        int l = it.front(), r = it.back();
        diff[l]++;
        diff[r + 1]--;
    }
    vector<vector<int> > res;
    int cover = 0;
    int start = -1;

    for (int i = 0; i < maximum + 1; ++i) {
        int prev = cover;
        cover += diff[i];
        if (prev == 0 and cover > 0) start = i;
        if (prev > 0 and cover == 0) res.push_back({start, i - 1});
    }
    return res;
}
