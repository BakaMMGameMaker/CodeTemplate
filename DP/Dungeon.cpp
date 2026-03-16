#include <vector>
#include <cstdint>
using namespace std;

// 地下城解救公主
// 左上角走到右下角, 生命值变化量等于格中值, 如果中途 <= 0 立马死亡
// 求走到右下角所需的最小初始生命值

// 这题的核心难点是要学会倒着思考, 既然正着走我们很难判断走到这一个格子究竟需要多少初始生命值
// 因为有些格子扣血, 但有些格子加血
// 所以我们反过来进行 dp, 让骑士从 1 生命值开始往回走, 并维护到达某个格子时至少需要多少血量, 这样就能确保骑士全程存活

// 遍历时, 假设右边的格子需要 3 滴血, 但是下面的要 5 滴, 显然右边的路更好走, 因此我们取 min
int minhp(vector<vector<int> > &dungeon) {
    int m = dungeon.size(), n = dungeon.back().size();

    vector dp(m + 1, vector(n + 1, INT32_MAX));
    dp[m][n - 1] = 1;
    dp[m - 1][n] = 1;
}
