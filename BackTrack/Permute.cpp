#include<vector>
using namespace std;

// 没有重复项数字的全排列

// 思路 - 维护 path, 每次把还没有用过的数字选一个放进去, 直到长度等于 size

vector<vector<int> > permute(const vector<int> &nums) {
    vector<vector<int> > ans;
    vector<int> path;
    vector<char> used(nums.size(), false);

    auto dfs = [&](auto &&self) {
        if (path.size() == nums.size()) {
            ans.push_back(path);
            return;
        }
        // 因为所有数字都要用上, 所以没有不 use 就直接往下递归的分支
        for (size_t i = 0; i < nums.size(); ++i) {
            if (used[i]) continue;
            used[i] = 1;
            path.push_back(nums[i]);
            self(self);
            path.pop_back();
            used[i] = 0;
        }
    };

    dfs(dfs);
    return ans;
}
