#include <vector>
using namespace std;

// Fisher-Yates 洗牌

int randInt(int i, int j) { return 0; }

void shuffle(vector<int> &v) {
    for (int i = v.size() - 1; i > 0; --i) {
        int j = randInt(0, i);
        swap(v[i], v[j]);
    }
}

// 每种排列出现的概率一致
// i = n - 1 时，每个元素被交换到末尾的概率都是 1/n
// i = n - k 时，每个元素被交换到末尾的概率都是 1/(n-k+1)
// ...
// i = 1 时，每个元素被交换到末尾的概率都是 1/2
// 所以对于特定排列，其出现概率为 1/n * 1/(n-1) * ... * 1/2 = 1/n!
// 而一共有 n! 个排列，所以每一个排列出现的概率都一致
