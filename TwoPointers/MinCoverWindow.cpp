#include <string>
#include <unordered_map>
#include <cstdint>
using namespace std;

// 最小覆盖子串 - 在 s 中找到包含 t 中所有字符的最短连续子串

string minwindow(const string &s, const string &t) {
    unordered_map<char, int> need; // t 里每个字符还需要几个
    for (char c : t) need[c]++;

    int cnt = 0;                       // 当前覆盖了 t 中多少个字符
    int start = 0, maxlen = INT32_MAX; // 最短子串的起始位置和长度
    int l = 0;
    for (int r = 0; r < s.size(); ++r) {
        if (need[s[r]] > 0) cnt++; // 可以用当前字符来补, 多匹配一个字符
        need[s[r]]--;              // 不管 t 需不需要这个字符, 都减去其需求量
        while (cnt == t.size()) {  // 当前 lr 范围可以覆盖 t
            if (r - l + 1 < maxlen) {
                maxlen = r - l + 1;
                start = l;
            }
            need[s[l]]++;              // l 位置字符离开窗口
            if (need[s[l]] > 0) cnt--; // 又欠费了
            l++;
        }
    }
    return maxlen == INT32_MAX ? "" : s.substr(start, maxlen);
}
