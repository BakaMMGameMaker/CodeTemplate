#include <bits/stdc++.h>
using namespace std;

// 比较版本号
int compareVersion(const string &version1, const string &version2) {
    int i = 0, j = 0;
    int n = static_cast<int>(version1.size()), m = static_cast<int>(version2.size());
    while (i < n or j < m) { // 因为空位置被当作 0 处理, 不需要两者都同时满足
        uint64_t x = 0, y = 0;
        while (i < n and version1[i] != '.') {
            x = x * 10 + (version1[i] - '0');
            ++i;
        }
        while (j < m and version2[j] != '.') {
            y = y * 10 + (version2[j] - '0');
            ++j;
        }
        if (x > y) return 1;
        if (x < y) return -1;

        ++i; // 跳过分隔符
        ++j;
    }
    return 0;
}
