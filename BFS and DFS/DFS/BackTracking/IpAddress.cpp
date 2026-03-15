#include <string>
#include <vector>
using namespace std;

// 把数字字符串转换为合法的 ip 地址
vector<string> ipaddress(const string &s) {
    if (s.size() < 4 or s.size() > 12) return {};
    vector<string> res;
    vector<string> path;

    auto pathtoip = [&] {
        return path[0] + "." + path[1] + "." + path[2] + "." + path[3];
    };

    auto dfs = [&](auto &&self, int start, int part) {
        if (part == 4) {
            if (start == s.size()) res.push_back(pathtoip());
            return;
        }
        // 剪枝
        int remaincharacters = s.size() - start;
        int remainparts = 4 - part;
        if (remaincharacters < remainparts or remaincharacters > remainparts * 3) return;

        // 往右处理至多三个数字
        int num = 0;
        for (int i = start; i < s.size() and i < start + 3; i++) {
            num = num * 10 + (s[i] - '0');
            if (num > 255) break;
            if (i > start and s[start] == '0') break; // "0" 合法但是 "0x" 非法
            path.push_back(s.substr(start, i - start + 1));
            self(self, i + 1, part + 1);
            path.pop_back();
        }
    };

    dfs(dfs, 0, 0);
    return res;
}
