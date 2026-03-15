#include <algorithm>
#include <string>
#include<vector>
using namespace std;

// 全排列

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

// 有重复项的全排列
// 区别是, 没有重复项时, 只需要确保每个下标不被重复使用
// 但是有重复数字的时候即便下标不同也会生成重复结果
// 核心思路是先排序, 然后在同一层搜索中跳过重复的选择
// 即 [i] == [i-1] 且 [i-1] 还未被使用时, [i] 应该被跳过
vector<vector<int> > permuteUnique(vector<int> &nums) {
    ranges::sort(nums);
    vector<vector<int> > ans;
    vector<int> path;
    vector<char> used(nums.size());

    auto dfs = [&](auto &&self) {
        if (path.size() >= nums.size()) {
            ans.push_back(path);
            return;
        }
        for (size_t i = 0; i < nums.size(); ++i) {
            if (used[i]) continue;
            if (i > 0 and nums[i] == nums[i - 1] and not used[i - 1]) continue;

            used[i] = true;
            path.push_back(nums[i]);
            self(self);
            path.pop_back();
            used[i] = false;
        }
    };

    dfs(dfs);
    return ans;
}

// 字符串的排列
// 本质就是有重复值的全排列 (具体有没有重复值取决于题目说字符串内有没有重复值, 大概率有)
vector<string> permuteString(string &s) {
    ranges::sort(s);
    vector<string> ans;
    string path;
    vector<char> used(s.size());

    auto dfs = [&](auto &&self) {
        if (path.size() >= s.size()) {
            ans.push_back(path);
            return;
        }
        for (int i = 0; i < s.size(); ++i) {
            if (used[i]) continue;
            if (i > 0 and s[i] == s[i - 1] and not used[i - 1]) continue;
            used[i] = true;
            path.push_back(s[i]);
            self(self);
            path.pop_back();
            used[i] = false;
        }
    };

    dfs(dfs);
    return ans;
}
