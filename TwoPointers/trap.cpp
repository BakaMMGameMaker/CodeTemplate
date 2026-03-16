#include <vector>
#include <stack>
#include <queue>
using namespace std;

// 接雨水三种解法

// 前后缀最大值
int prepostsum(vector<int> &height) {
    int n = height.size();
    if (n == 0) return 0;
    vector<int> leftmax(n), rightmax(n);

    leftmax[0] = height[0];
    for (int i = 1; i < n; ++i) leftmax[i] = max(leftmax[i - 1], height[i]);

    rightmax[n - 1] = height[n - 1];
    for (int i = n - 2; i >= 0; i--) rightmax[i] = max(rightmax[i + 1], height[i]);

    // 计算每个位置的盛水量, 不必担心左右边界
    // 以左边界为例, rightmax >= 左边界, leftmax = 左边界, 所以不会出现负数
    int ans = 0;
    for (int i = 0; i < n; ++i) ans += min(leftmax[i], rightmax[i]) - height[i];

    return ans;
}

// 双指针 (最优解法)
// left 从左往右 right 从右往左
// 同时维护左侧目前最高和右侧目前最高
// 当前位置 (left/right) 能装多少水, 只取决于两个最高高度中的较小值
int twopointer(vector<int> &height) {
    int n = height.size();
    int left = 0, right = n - 1;
    int leftmax = 0, rightmax = 0;
    int ans = 0;
    while (left < right) {
        leftmax = max(leftmax, height[left]);
        rightmax = max(rightmax, height[right]);
        if (leftmax < rightmax) {
            ans += leftmax - height[left];
            left++; // left 位置没有任何可能可以装更多的水
        } else {
            ans += rightmax - height[right];
            right--; // right 位置没有任何可能可以装更多的水
        }
    }
    return ans;
}

// 单调栈 - 批量分层灌水, 理解难度较高, 写对难度也较高
// 如果当前高度小于等于栈顶高度, 直接入栈
// 否则代表当前位置可以作为一个右边界, 因为栈内存在头顶还没有水的柱子
// 弹出栈顶, 视作槽底, 新的栈顶就是这个槽的左边界, 计算这一层可以装的水
// 不用担心这个柱子上面可能可以放更多的水, 在处理更高柱子的时候自然会往上面加剩下的水
int monotonicstack(vector<int> &height) {
    int n = height.size();
    stack<int> st;
    int ans = 0;

    for (int i = 0; i < n; ++i) {
        while (not st.empty() and height[i] > height[st.top()]) {
            int bottom = st.top();
            st.pop();
            if (st.empty()) break; // 水流左边去了
            int left = st.top();
            int wid = i - left - 1;
            int h = min(height[left], height[i]) - height[bottom];
            ans += wid * h;
        }
        st.push(i);
    }
    return ans;
}

// 二维网格接雨水
// 思路 - 从边往内部扩散
// 边界格子无法存水, 先把边界格子放进最小堆, 每次取出当前最低的围墙, 看其邻居
// 如果邻居更低, 代表其能存水随后把邻居加入到堆里
int trap2d(vector<vector<int> > &height) {
    int m = height.size();
    if (m == 0) return 0;
    int n = height.back().size();
    if (m < 3 or n < 3) return 0;
    vector visited(m, vector(n, 0));

    using T = tuple<int, int, int>;              // height, x, y
    priority_queue<T, vector<T>, greater<> > pq; // 小根堆的原因: 由于连通性, 格子储水量取决于连通区域最矮柱

    for (int i = 0; i < m; ++i) {
        pq.emplace(height[i][0], i, 0);
        pq.emplace(height[i][n - 1], i, n - 1);
        visited[i][0] = 1;
        visited[i][n - 1] = 1;
    }

    for (int j = 1; j < n - 1; ++j) {
        pq.emplace(height[0][j], 0, j);
        pq.emplace(height[m - 1][j], m - 1, j);
        visited[0][j] = 1;
        visited[m - 1][j] = true;
    }

    int ans = 0;
    static int dirs[5] = {-1, 0, 1, 0, -1};

    while (not pq.empty()) {
        auto [h,x,y] = pq.top();
        pq.pop();
        for (int k = 0; k < 4; ++k) {
            int nx = x + dirs[k], ny = y + dirs[k + 1];
            if (nx < 0 or nx >= m or ny < 0 or ny >= n or visited[nx][ny]) continue;

            visited[nx][ny] = true;
            if (height[nx][ny] < h) ans += h - height[nx][ny];
            // 注意存储的是更大值
            // 因为连通性, 如 321, 对于 1 来说, 应该往 3 这堵墙看齐
            pq.emplace(max(height[nx][ny], h), nx, ny);
        }
    }
    return ans;
}
