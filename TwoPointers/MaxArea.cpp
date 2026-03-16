#include <vector>
using namespace std;

// 盛水最多的容器
// 核心要点 - 容器的容量不仅取决于短板高度, 还取决于宽度
// 宽度是可单调预测的，只要让双指针开始分别位于两端并往中间逼近即可
// 但是短板高度是不可预测的, 所以要让右指针往左边走

int maxarea(vector<int> &height) {
    int left = 0, right = height.size() - 1;
    int ans = 0;

    while (left < right) {
        int h = min(height[left], height[right]); // 短板高度
        ans = max(ans, h * (right - left));
        // 移动短板, 因为宽度在缩小, 想要提高容量只能指望短板变高
        if (height[left] < height[right]) ++left;
        else --right;
    }
    return ans;
}
