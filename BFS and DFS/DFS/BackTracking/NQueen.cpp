#include <string>
#include <vector>
using namespace std;

vector<vector<string> > NQueen(int n) {
    vector<vector<string> > ans;
    vector board(n, string(n, '.'));

    vector col(n, 0);           // 某一列的皇后情况
    vector diag1(2 * n - 1, 0); // 主对角线的皇后情况 一共 2n - 1 条主对角线
    vector diag2(2 * n - 1, 0); // 副对角线的皇后情况 一共 2n - 1 条副对角线
    // ^^^ = 1 即该线上已经有皇后

    auto dfs = [&](auto &&self, int row) {
        if (row == n) { // 已经排完了 n 行
            ans.push_back(board);
            return;
        }
        for (int c = 0; c < n; ++c) { // 列方向上遍历
            int d1 = row - c + n - 1; // 所处的某的主对角线的编号
            int d2 = row + c;         // 所处的某条副对角线的编号
            if (col[c] or diag1[d1] or diag2[d2]) continue;
            board[row][c] = 'Q';
            col[c] = diag1[d1] = diag2[d2] = 1;

            self(self, row + 1); // 摆下一行

            // backtracking
            board[row][c] = '.';
            col[c] = diag1[d1] = diag2[d2] = 0;
        };
        // ^^^ 已经做到了一行最多只摆一个
    };

    dfs(dfs, 0); // 从第零行开始摆
    return ans;
}
