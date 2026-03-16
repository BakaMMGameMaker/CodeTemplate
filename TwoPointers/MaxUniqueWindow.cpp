#include <vector>
#include <unordered_map>
using namespace std;

// 最长无重复子数组
// 可以用 map 记录每个元素上一次出现的位置, 这样更新 left 的时候可以直接移动到上一次出现位置之后

int maxlength(vector<int> &arr) {
    unordered_map<int, int> last;
    int left = 0, ans = 0;
    for (int right = 0; right < arr.size(); ++right) {
        if (last.contains(arr[right]) and last[arr[right]] >= left) left = last[arr[right]] + 1;
        last[arr[right]] = right;
        ans = max(ans, right - left + 1);
    }
    return ans;
}
