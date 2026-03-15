#include <unordered_map>
#include <vector>
using namespace std;

// 数组中出现次数超过一半的数字

// 哈希表解法 count[x] >= (nums.size() + 1) / 2
int moreThanHalf(const vector<int> &numbers) {
    unordered_map<int, int> count;
    for (int x : numbers) {
        if (++count[x] >= (numbers.size() + 1) / 2) return x;
    }
    return -1;
}

// 投票法 - 两两不相同的数字抵消, 剩下的就是超过一半的数字
int majorityElement(const vector<int> &nums) {
    int candidate = 0;
    int count = 0;

    for (int num : nums) {
        if (count == 0) {
            candidate = num;
            count = 1;
        } else if (num == candidate) {
            ++count;
        } else {
            --count;
        }
    }
    return candidate;
}
