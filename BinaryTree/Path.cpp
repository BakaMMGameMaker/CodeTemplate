#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 路径类问题
// - 是否必须从根节点出发
// - 是否必须到叶子节点结束
// - 路径上需要记载什么信息

// 路经总和 - 是否存在根到叶子的路径是的和为 target
bool hasPathSum(const TreeNode* root, int target) {
    if (not root) return false;
    // 遇到叶子节点
    if (not root->left and not root->right) return target == root->val;
    return hasPathSum(root->left, target - root->val) or hasPathSum(root->right, target - root->val);
}

// 非递归写法 1 - DFS
// 栈存当前节点与从根走到当前节点的路径和
// 如果弹出一个叶子节点且和为 target, 返回 true
// 否则压栈左右孩子并更新路径和
bool hasPathSumDFS(const TreeNode* root, int target) {
    if (not root) return false;

    vector st{pair{root, root->val}};

    while (!st.empty()) {
        auto [node, sum] = st.back();
        st.pop_back();
        if (not node->left and not node->right and sum == target) return true;

        if (node->right) st.emplace_back(node->right, sum + node->right->val);
        if (node->left) st.emplace_back(node->left, sum + node->left->val);
    }
    return false;
}

// BFS - 思路没什么区别
bool hasPathSumBFS(const TreeNode* root, int target) {
    if (not root) return false;

    deque q{pair{root, root->val}};
    while (not q.empty()) {
        auto [node, sum] = q.front();
        q.pop_front();

        if (not node->left and not node->right and sum == target) return true;
        if (node->left) q.emplace_back(node->left, sum + node->left->val);
        if (node->right) q.emplace_back(node->right, sum + node->right->val);
    }
    return false;
}

// 路径总和 II - 找出所有根到叶子且路径和为 target 的路径
// 变化 1 - 时刻维护当前路径 path
// 变化 2 - 找到合法路径的时候需要把 path 记录在答案中

// 递归思路
// 把当前节点加入路径, 更新剩余目标值
// 递归左右子树, 返回上一层之前, 把当前节点从路径中移除
vector<vector<int> > pathSum(const TreeNode* root, int target) {
    if (not root) return {};
    vector<vector<int> > ans;
    vector<int> path;

    function<void(const TreeNode*, int)> dfs = [&](const TreeNode* node, int remainSum) {
        if (not node) return;
        path.push_back(node->val);
        if (not node->left and not node->right) {
            // 遇到了叶子节点, 如果构成合法路径, 加入答案
            // 无论有无合法路径, 都不再往下递归了
            if (remainSum == node->val) ans.push_back(path);
            path.pop_back();
            return;
        }

        dfs(node->left, remainSum - node->val);
        dfs(node->right, remainSum - node->val);

        path.pop_back(); // 恢复现场
    };

    dfs(root, target);
    return ans;
}

// 路径总和 III - 不要求从根开始, 不要求到叶子节点结束, 路径必须一直向下
// 朴素 DFS
// - 枚举每个点作为起点 - 从这个点往下 DFS 统计以它开头的合法路径数量
int pathSumIII(const TreeNode* root, int target) {
    if (not root) return 0;

    function<int(const TreeNode*, int)> countFrom = [&](const TreeNode* node, int remain) {
        if (not node) return 0;
        int res = 0;
        if (node->val == remain) ++res; // 别着急停止, 谁知道后面会不会正负抵消

        res += countFrom(node->left, remain - node->val);
        res += countFrom(node->right, remain - node->val);

        return res;
    };

    return countFrom(root, target) + pathSumIII(root->left, target) + pathSumIII(root->right, target);
}

// 前缀和 + 哈希表优化
// 每个节点记录从根节点到当前的和
// 如果 Root->A 有和 a, 而 A 的子节点 B 到底下的 C 路径和恰好为到 C 时的总和为 target + a, 说明 B 到 C 的总和为 target
int pathSumIIIOptimized(const TreeNode* root, int target) {
    unordered_map prefixCount{pair{0, 1}}; // 前缀和 0 出现了 1 次, 以便统计一根节点为起点的路径

    function<int(const TreeNode*, int)> dfs = [&](
        const TreeNode* node, int curSumFromRoot) {
        if (not node) return 0;

        curSumFromRoot += node->val;

        int res = 0;

        res += prefixCount[curSumFromRoot - target];

        ++prefixCount[curSumFromRoot];
        res += dfs(node->left, curSumFromRoot);
        res += dfs(node->right, curSumFromRoot);

        // 回溯, 从子树返回父节点时减少 curSumFromRoot 记录
        --prefixCount[curSumFromRoot];

        return res;
    };
    return dfs(root, 0);
}

// 所有路径 - 根节点到所有叶子节点的路径

// 解法 1 - 递归
// 思路 往下走就加连接符号, 遇到叶子节点就记录答案
vector<string> binaryTreePaths(const TreeNode* root) {
    if (not root) return {};
    vector<string> ans;

    function<void(const TreeNode*, string)> dfs = [&](const TreeNode* node, string path) {
        if (not node) return;

        if (not path.empty()) path += "->";
        path += to_string(node->val);

        if (not node->left and not node->right) {
            // 当前节点为叶子
            ans.push_back(path);
            return;
        }

        dfs(node->left, path);
        dfs(node->right, path);
    };

    dfs(root, "");
    return ans;
}

// path 用数组记录, 遇到叶子再拼接, 减少反复拷贝字符串的开销
vector<string> binaryTreePathsBackTrack(const TreeNode* root) {
    if (not root) return {};
    vector<int> path;
    vector<string> ans;

    auto buildPath = [&] {
        if (path.empty()) return string();
        string s = to_string(path[0]);
        for (int i = 1; i < path.size(); ++i) {
            s += "->";
            s += to_string(path[i]);
        }
        return s;
    };

    function<void(const TreeNode*)> dfs = [&](const TreeNode* node) {
        if (not node) return;
        path.push_back(node->val);
        if (not node->left and not node->right) {
            ans.emplace_back(buildPath());
        } else {
            dfs(node->left);
            dfs(node->right);
        }
        path.pop_back();
    };

    dfs(root);
    return ans;
}

// 非递归 DFS
// 栈中存储当前节点以及到当前节点位置的路径, 如果遇到了叶子节点就加入答案
vector<string> binaryTreePathsDFSIterative(const TreeNode* root) {
    if (not root) return {};
    vector<string> ans;
    vector st{pair{root, to_string(root->val)}};
    while (!st.empty()) {
        auto [node, path] = st.back();
        st.pop_back();

        if (not node->left and not node->right) {
            ans.push_back(path);
            continue;
        }

        if (node->right) st.emplace_back(node->right, path + "->" + to_string(node->right->val));
        if (node->left) st.emplace_back(node->left, path + "->" + to_string(node->left->val));
    }
    return ans;
}

// 非递归队列解法
vector<string> binaryTreePathBFSIterative(const TreeNode* root) {
    if (not root) return {};

    vector<string> ans;
    deque q{pair{root, to_string(root->val)}};

    while (not q.empty()) {
        auto [node, path] = q.front();
        q.pop_front();
        if (not node->left and not node->right) {
            ans.push_back(path);
            continue;
        }
        if (node->left) q.emplace_back(node->left, path + "->" + to_string(node->left->val));
        if (node->right) q.emplace_back(node->right, path + "->" + to_string(node->right->val));
    }
    return ans;
}
