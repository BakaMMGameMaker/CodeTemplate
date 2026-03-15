#include <vector>
#include <unordered_set>
using namespace std;

// 找到数组中只出现了一次的两个数字, 其它数字都出现了两次

// 哈希思路 - 用 set, 不在就加进去, 在就删掉, 留下的就是答案
vector<int> singleNumber(const vector<int> &nums) {
    unordered_set<int> st;
    for (int num : nums) {
        if (st.contains(num)) st.erase(num);
        else st.emplace(num);
    }
    return {st.begin(), st.end()};
}
