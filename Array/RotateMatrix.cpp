#include <algorithm>
#include <vector>
using namespace std;

// 顺时针旋转矩阵 90°

// 两步走 - 转置, 逐行翻转
void rotatematrixright90(vector<vector<int> > &matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; ++i) {
        // 注意 j 从 i + 1 开始, 否则会有重复交换问题
        // 且对角线元素转置后位置不变, 因此无需操作
        for (int j = i + 1; j < n; ++j) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }

    for (int i = 0; i < n; ++i) ranges::reverse(matrix[i]);
}

// 逆时针旋转 - 区别: 左右翻转变成上下翻转, 所以只需要对 matrix 进行一次 reverse 就行
void rotatematrixleft90(vector<vector<int> > &matrix) {
    int n = matrix.size();
    for (int i = 0; i < n; ++i) {
        // 注意 j 从 i + 1 开始, 否则会有重复交换问题
        // 且对角线元素转置后位置不变, 因此无需操作
        for (int j = i + 1; j < n; ++j) {
            std::swap(matrix[i][j], matrix[j][i]);
        }
    }

    ranges::reverse(matrix);
}

// 变体 mxn 矩阵
// 维度会改变, 所以不能原地实现
// 由于无法原地, 所以干脆直接每个元素放到目标位置就行
vector<vector<int> > rotatemn(vector<vector<int> > &matrix) {
    int m = matrix.size();
    int n = matrix.back().size();

    vector ans(n, vector<int>(m));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            ans[j][m - 1 - i] = matrix[i][j];
            // 如果是逆时针旋转, 就变成
            // ans[n - 1 - j][i] = matrix[i][j];
        }
    }
    return ans;
}
