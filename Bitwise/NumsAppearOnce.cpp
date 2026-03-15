#include <vector>
using namespace std;

// 找到数组中只出现了一次的两个数字, 其它数字都出现了两次

// 使用位运算一次遍历解决
// 假设出现的数字是 a 和 b, 由于 k ^ k = 0, 所以整个数组异或的结果 x = a ^ b
// 由于 a b 不等, 所以 x 至少有一位是 1, 取出最右边的 1, 把数组中所有数字再次分为两组
// 这一位上为 1 的数字为一组, 其它的到另一组, 这样成对出现的数字必然在一组, 同时 a 和 b 不会在一组
// 最后两组分别异或, 结果就是答案
vector<int> singleNumber(const vector<int> &nums) {
    int xorSum = 0;
    for (int num : nums) xorSum ^= num;
    int lowBit = xorSum & (-xorSum);
    int a = 0, b = 0;

    for (int x : nums) {
        if (x & lowBit) a ^= x;
        else b ^= x;
    }
    return {a, b};
}
