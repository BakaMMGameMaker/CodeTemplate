#include <bits/stdc++.h>
#include "TreeNode.h"
using namespace std;

// 路径类问题
// - 是否必须从根节点出发
// - 是否必须到叶子节点结束
// - 路径上需要记载什么信息

// 路经总和 - 是否存在根到叶子的路径是的和为 target
bool hasPathSum(const TreeNode *root, int target) {
    if (not root) return false;
    // 遇到叶子节点
    if (not root->left and not root->right) return target == root->val;
    return hasPathSum(root->left, target - root->val) or hasPathSum(root->right, target - root->val);
}

// 非递归写法 1 - DFS
// 栈存当前节点与从根走到当前节点的路径和
// 如果弹出一个叶子节点且和为 target, 返回 true
// 否则压栈左右孩子并更新路径和
bool hasPathSumDFS(const TreeNode *root, int target) {
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
bool hasPathSumBFS(const TreeNode *root, int target) {
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
vector<vector<int> > pathSum(const TreeNode *root, int target) {
    if (not root) return {};
    vector<vector<int> > ans;
    vector<int> path;

    function<void(const TreeNode *, int)> dfs = [&](const TreeNode *node, int remainSum) {
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
