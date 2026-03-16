#include <vector>
using namespace std;

// 螺旋矩阵

// 螺旋遍历
// 核心是维护四个边界, 碰到就收缩
// 最好用闭区间, 没事不要折磨自己
vector<int> traversal(vector<vector<int> > &matrix) {
    vector<int> ans;
    if (matrix.empty() or matrix[0].empty()) return ans;
    int top = 0, bottom = matrix.size() - 1, left = 0, right = matrix[0].size() - 1;
    while (top <= bottom and left <= right) { // 每轮循环走一圈
        for (int j = left; j <= right; ++j) ans.push_back(matrix[top][j]);
        ++top;

        for (int i = top; i <= bottom; ++i) ans.push_back(matrix[i][right]);
        --right;

        // 注意后面两个要额外判断, 而且不要搭配错
        // 如果你感觉 if 里的条件和 for 的重复了, 那就是搭配错了
        if (top <= bottom) {
            for (int j = right; j >= left; --j) ans.push_back(matrix[bottom][j]);
            --bottom;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; --i) ans.push_back(matrix[i][left]);
            ++left;
        }
    }
    return ans;
}

// 生成 n*n 矩阵
vector<vector<int> > generate(int n) {
    vector ans(n, vector(n, 0));
    int top = 0, left = 0;
    int bottom = n - 1, right = n - 1;
    int num = 1;
    while (top <= bottom and left <= right) {
        for (int j = left; j <= right; ++j) ans[top][j] = num++;
        ++top;

        for (int i = top; i <= bottom; ++i) ans[i][right] = num++;
        --right;
        if (top <= bottom) {
            for (int j = right; j >= left; --j) ans[bottom][j] = num++;
            --bottom;
        }

        if (left <= right) {
            for (int i = bottom; i >= top; --i) ans[i][left] = num++;
            ++left;
        }
    }
    return ans;
}
