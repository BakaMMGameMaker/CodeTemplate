#include <ranges>
#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

// 删除非法链条
class RemoveInvalidChains {
public:
    struct value {
        bool isvalid;
        string sonkey;
    };

    // state = 0 => 未访问
    // 1 => 位于当前递归栈中
    // 2 => 非法链
    // 3 => 合法链
    bool dfs(const string &key, unordered_map<string, value> &mp, unordered_map<string, int> &state) {
        if (not mp.contains(key)) return true; // 节点不存在 => 节点非法
        if (state[key] == 1) return true;      // 已经在递归栈中，说明形成环，节点非法
        if (state[key] == 2) return true;      // 已经非法了
        if (state[key] == 3) return false;     // 合法链条

        state[key] = 1; // 标记位于当前递归栈中

        // 当前节点就不合法，直接标记非法并返回
        if (not mp[key].isvalid) {
            state[key] = 2; // 标记非法
            return true;
        }

        // 当前节点合法，如果当前节点有子节点，那么往下递归看这个链条是否合法
        bool bad = false;
        if (not mp[key].sonkey.empty()) bad = dfs(mp[key].sonkey, mp, state);

        // 标记节点的最终状态
        state[key] = bad ? 2 : 3;
        return bad; // 返回节点的是否非法
    }

    void removeinvalidchains(unordered_map<string, value> &mp) {
        unordered_map<string, int> state;

        for (const auto &key : mp | views::keys) {
            if (state.contains(key)) continue; // key 已经访问过了
            dfs(key, mp, state);               // 标记这条链上所有节点的合法性
        }

        vector<string> todelete;
        for (const auto &key : mp | views::keys) {
            if (state[key] == 2) todelete.push_back(key);
        }

        for (const string &key : todelete) mp.erase(key);
    }
};
