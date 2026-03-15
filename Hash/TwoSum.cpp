#include <unordered_map>
#include <vector>
using namespace std;

// 哈希表两数之和
// 遍历到 x 时, 表中出现过 target - x 就找到了答案

vector<size_t> twoSum(const vector<int> &numbers, int target) {
    unordered_map<int, size_t> mp;
    for (size_t i = 0; i < numbers.size(); ++i) {
        int need = target - numbers[i];
        if (mp.contains(need)) return {mp[need] + 1, i + 1};
        mp[numbers[i]] = i;
    }
    return {};
}
